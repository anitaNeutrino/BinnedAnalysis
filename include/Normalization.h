#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#include Analysisconfig.h

// This class inherits the properties of AnalysisConfig from AnalysisConfig.h but adds options for normalization calls inside the Binned Analysis code.
// Any calls in the BA code AnalysisConfig should point instead to Normalization.
class Normalization : public AnalysisConfig{

	public:
        enum NormalizationOption_t
      {
        NormalizationNone,          // no normalization
        NormalizationStandard,      // normalize by the magnitudes (pythagorean sums) of the inputs
        NormalizationOverlap,      // normalize by pythagorean sums across the mutually supported regions (overlap normalization)
        NormalizationOverlapWeighted   // normalize by magnitudes, but weighted by the width of overlap (not implemented yet)
      } normalization_option;
      static const char * getNormalizationString(NormalizationOption_t opt);
  
}

#endif
