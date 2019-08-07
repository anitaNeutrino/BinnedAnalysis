#include "FilteredAnitaEvent.h" 
#include "TString.h"
#include "AntennaPositions.h"
#include "DeltaT.h"
#include "TTree.h"
#include "TFile.h"
#include "TrigCache.h"
#include "FFTtools.h"
#include <assert.h>
#include "AnitaGeomTool.h"
#include "Correlator.h"
#include "BinnedCorrelator.h"
#include "AnalysisWaveform.h"
#include "TStopwatch.h" 

#ifdef UCORRELATOR_OPENMP
#include <omp.h>

#define SECTIONS _Pragma("omp parallel sections")
#define SECTION _Pragma("omp section") 

#else 

#define SECTIONS if(true) 
#define SECTION if(true) 


#endif

AnalysisWaveform * UCorrelator::BinnedCorrelator::getCorrelation(int ant1, int ant2) 
{
//  printf("%d %d / %d \n",ant1,ant2, NANTENNAS); 


#ifdef UCORRELATOR_OPENMP
#ifndef FFTTOOLS_COMPILED_WITH_OPENMP
#pragma omp critical (getCorrelation)
  {
#endif
#endif


#ifdef UCORRELATOR_OPENMP
  omp_set_lock(&locks->waveform_locks[ant1]); 
#endif
  if (!padded_waveforms[ant1])
  {
//    printf("Copying and padding %d\n",ant1); 
     padded_waveforms[ant1] = new AnalysisWaveform(*ev->getFilteredGraph(ant1, pol)); 
     rms[ant1] = padded_waveforms[ant1]->even()->GetRMS(2); 
     padded_waveforms[ant1]->padEven(1); 
  }
#ifdef UCORRELATOR_OPENMP
  omp_unset_lock(&locks->waveform_locks[ant1]); 
  omp_set_lock(&locks->waveform_locks[ant2]); 
#endif

  if (!padded_waveforms[ant2])
  {
//    printf("Copying and padding %d\n",ant2); 
      padded_waveforms[ant2] = new AnalysisWaveform(*ev->getFilteredGraph(ant2, pol)); 
//      printf("Computing rms!\n"); 
      rms[ant2] = padded_waveforms[ant2]->even()->GetRMS(2); 
      padded_waveforms[ant2]->padEven(1); 
  }

#ifdef UCORRELATOR_OPENMP
  omp_unset_lock(&locks->waveform_locks[ant2]); 
  omp_set_lock(&locks->correlation_locks[ant1][ant2]); 
#endif

  if (!correlations[ant1][ant2])
    {
#ifdef UCORRELATOR_OPENMP
    omp_set_lock(&locks->waveform_locks[ant1]); 
    omp_set_lock(&locks->waveform_locks[ant2]); 
#endif

    // -------------------------- new normalization code sammy  ---------------------------------------------------------------------
    //  we don't need magnitude-normalized correlation to start with

    if (normalization_option == Normalization::NormalizationNone) 
      {
      correlations[ant1][ant2] = AnalysisWaveform::correlation(padded_waveforms[ant1],padded_waveforms[ant2],pad_factor, 1); 
      }

    else if (normalization_option == Normalization::NormalizationStandard) 
      {
      correlations[ant1][ant2] = AnalysisWaveform::correlation(padded_waveforms[ant1],padded_waveforms[ant2],pad_factor, rms[ant1]*rms[ant2]);     
      }

    else if (normalization_option == Normalization::NormalizationOverlap) 
      {
      correlations[ant1][ant2] = AnalysisWaveform::correlation(padded_waveforms[ant1],padded_waveforms[ant2],pad_factor, 1); 
      TGraphAligned* thisCorr = correlations[ant1][ant2]->updateEven();
    
      // figure out i0 and i1, the starting and ending indices of nonpadded (nonzero) array content
      //  N = padded time domain array size
      float dt = (padded_waveforms[ant1]->even()->GetX()[1]-padded_waveforms[ant1]->even()->GetX()[0])/(float)(pad_factor+1);
      TGraph* wfA = FFTtools::getInterpolatedGraph((TGraph*)padded_waveforms[ant1]->even(), dt);
      TGraph* wfB = FFTtools::getInterpolatedGraph((TGraph*)padded_waveforms[ant2]->even(), dt);

      int ip = -1; 
      int iq = -1;
      int N = wfA->GetN();
      if (wfB->GetN() < N)  N = wfB->GetN();
      for (int k=0; k<N && ip<0; ++k) 
        {
        if (wfA->GetY()[k] !=0 && wfB->GetY()[k] !=0)  
          {
          ip = k;
          }
        }
      for (int k=N-1; k>=0 && iq<0; --k) 
        {
        if (wfA->GetY()[k] !=0 && wfB->GetY()[k] !=0) 
          {
          iq = k;
          }
        }

      // sum-squared accumulators
      double sumA2 = 0;
      double sumB2 = 0;

      // starting with maximum negative time delay with overlap (tau), and iterate through tau=zero
      // add the terms up along the way
      int numTerms = 0;

      for (int tau = ip-iq; tau<=0; ++tau) 
        {
        //int a = iq+tau;
        //int b = ip-tau;
        int a = ip-tau;
        int b = iq+tau;
        double valA = wfA->GetY()[a];
        double valB = wfB->GetY()[b];
        sumA2 += pow(valA,2);
        sumB2 += pow(valB,2);
        ++numTerms;
        double normFac = sqrt(sumA2*sumB2)/(float)N;
        int k = tau + thisCorr->GetN()/2;  // (+1???) TODO check this

        thisCorr->GetY()[k] /= normFac;

        }
      // start from maximum tau positive and iterate back through tau=one, accumulating terms as we go
      sumA2 = 0;
      sumB2 = 0;
      for (int tau=iq-ip; tau>0; --tau)
        {

        //int a = ip+tau;
        //int b = iq-tau;
        int a = iq-tau;
        int b = ip+tau;
        double valA = wfA->GetY()[a];
        double valB = wfB->GetY()[b];

        sumA2 += pow(valA,2);
        sumB2 += pow(valB,2);
        --numTerms;
        double normFac = sqrt(sumA2*sumB2)/(float)N;
        int k = tau + thisCorr->GetN()/2;  // (+1???) 

        thisCorr->GetY()[k] /= normFac;

        }
      const TGraphAligned* thatCorr = correlations[ant1][ant2]->even();

      char fred;     
      wfA->Delete();
      wfB->Delete();
      }            
// -------------------------- end of new normalization code sammy  ---------------------------------------------------------------

#ifdef UCORRELATOR_OPENMP
    omp_unset_lock(&locks->waveform_locks[ant2]); 
    omp_unset_lock(&locks->waveform_locks[ant1]); 
#endif
  }


#ifdef UCORRELATOR_OPENMP
  omp_unset_lock(&locks->correlation_locks[ant1][ant2]); 
#ifndef FFTTOOLS_COMPILED_WITH_OPENMP
  }
#endif
#endif
  return correlations[ant1][ant2]; 
}