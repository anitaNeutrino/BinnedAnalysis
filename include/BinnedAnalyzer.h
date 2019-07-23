#ifndef UCORRELATOR_BINNED_ANALYZER_H
#define UCORRELATOR_BINNED_ANALYZER_H

#include Analyzer.h
#include BinnedCorrelator.h
#include BinnedAnitaEventSummary.h

// This class inherits the properties of AnalysisConfig from AnalysisConfig.h but adds options for normalization calls inside the Binned Analysis code.
// Any calls in the BA code AnalysisConfig should point instead to Normalization.
class BinnedAnalyzer : public Analyzer{

	private:
		BinnedCorrelator bcorr;
  
}

#endif
