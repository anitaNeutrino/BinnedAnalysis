#ifndef BINNED_BEDMAPREADER_H
#define BINNED_BEDMAPREADER_H

/**
  Inheriting classes from BedmapReader, calls in the Binned analysis code should switch from calling BedmapReader to the Binned version.
 */


class BinnedBedmapReader : public BedmapReader {

 public:

  // These are used in converting easting and northing coordinates to latitude and longitude.
  // added by sammy ----------------------------------
  void EaNoToLonLat(Double_t easting, Double_t northing, Double_t& lon, Double_t& lat);
  void LonLattoEaNo(Double_t lon, Double_t lat, double& ea, double& no);
  void ENtoEaNo(Int_t e_coord, Int_t n_coord, Double_t& es, Double_t& no);
  // -------------------------------------------------

};

#endif
