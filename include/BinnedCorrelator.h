#include Correlator.h

//added sammy -------------------
#include "AnalysisConfig.h"
// -------------------------------

/**
  Inheriting classes from Correlator, calls in the Binned analysis code should switch from calling Correlator to the Binned version.
 */

  /** This creates the inteferometric map for an ANITA event */ 
  class BinnedCorrelator : public Correlator {
    public:

      /** Disable the antennas given by the bitmap, but leave all others alone */    // sammy 2016/10/27
      void disallowAntennas(uint64_t disallowed) { disallowed_antennas |= disallowed; }


      /** Enable the antennas given by the bitmap, but leave all others alone */    // sammy 2016/10/27
      void allowAntennas(uint64_t allowed) { disallowed_antennas &= ~allowed; }


			/** Set the normalization      added sammy */
			void setNormalization(AnalysisConfig::NormalizationOption_t n) { normalization_option = n; }
			const char * getNormalizationString() {return AnalysisConfig::getNormalizationString(normalization_option);}
			//----------------------------------------------------------------------

			// added sammy -----------------------------------------------
 			AnalysisConfig::NormalizationOption_t normalization_option;
		  //------------------------------------------------------------


  }; 

