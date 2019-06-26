#include "BinnedAnalysisConfig.h"


// added sammy  -----------------------------------------------
static const char * normalizations[] = {"None", "Magnitude", "Overlap magnitude", "Overlap-weighted magnitude"}; 
//-------------------------------------------------------------

#ifdef ENABLE_LIBCONFIG
#include "libconfig.h++"
#define LOOKUP(X) cfg.lookupValue(#X,X) 

// This function appears necessary, but defining it here may be unneccessary since it should be in Analysisconfig.cc already.  Keeping for now but commenting it out.
// void lookupEnum(libconfig::Config * cfg, const char * key, int * val, int N, const char ** allowed) 
// {
//   if (!cfg->exists(key)) return; 

//   libconfig::Setting & set = cfg->lookup(key); 

//   for (int i = 0; i < N; i++)
//   {
//     if (!strcasecmp(set, allowed[i]))
//     {
//       *val = i; 
//       return; 
//     }
//   }

//   fprintf(stderr,"Config Parse Error on line %d: %s must be one of: \n\t", set.getSourceLine(), key); 


//   for (int i = 0; i < N; i++)
//   {
//     fprintf(stderr,"%s ", allowed[i]); 
//   }

//   fprintf(stderr, "\n But %s was given.", (const char * ) set); 

// }


void UCorrelator::AnalysisConfig::loadFromFile(const char * config_file) 
{

  libconfig::Config cfg; 
  cfg.setAutoConvert(true); 
  cfg.readFile(config_file); 

  // added sammy
  lookupEnum(&cfg, "normalization_option", normalization_option, sizeof(normalizations)/sizeof(char *), normalizations); 

}


// added sammy ------------------------------------------------------------------
const char * UCorrelator::BinnedAnalysisConfig::getNormalizationString(NormalizationOption_t opt) 
{
  return normalizations[opt]; 
}
// ------------------------------------------------------------------------------

