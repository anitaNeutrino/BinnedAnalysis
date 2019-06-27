#ifndef _BINNED_ANITA_EVENT_SUMMARY_H_
#define _BINNED_ANITA_EVENT_SUMMARY_H_

#include AnitaEventSummary.h

/**
  Inheriting classes from AnitaEventSummary, calls in the Binned analysis code should switch from calling AnitaEventsummary and WaveformInfo to the Binned versions.
 */

class BinnedAnitaEventSummary : public AnitaEventSummary {
 public:

  /**
    Adding new signal to noise ratio (SNR) modes for BA code.
   */
  class BinnedWaveformInfo : public WaveformInfo  {

   public:

    Double_t snr1;  //        0.5 * peak2peak / (rms of 1/5 of waveform starting 3/4 of the way through)
    Double_t snr2;  //        0.5 * peak2peak / (rms of 10 secs at beginning of nonzero portion of wf)
    Double_t snr3;  //        average value from hilbert peak-5sec to peak + 10 sec / (average of 10 secs at beginning of nonzero portion of hilbert env)
    
  };
  
  // Waveform info for input waveforms - added by sammy --------------------------
  WaveformInfo inputWfRaw[AnitaPol::kNotAPol][NUM_SEAVEYS];
  WaveformInfo inputWfFiltered[AnitaPol::kNotAPol][NUM_SEAVEYS];
  // -----------------------------------------------------------------------------

};

#endif
