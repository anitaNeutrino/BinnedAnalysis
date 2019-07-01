
#include "BinnedBedmapReader.h"

using namespace std;

void BinnedBedmapReader::EaNoToLonLat(Double_t easting, Double_t northing, Double_t& lon, Double_t& lat) {
  //Takes as input the indicies from a BEDMAP data set, and turns them into latitude and longitude coordinates.  Information on which data set (surface data, ice depth, water depth) is necessary, in the form of coordinates of a corner of the map.  Code by Stephen Hoover.

  Double_t isometric_lat=0;
  //Double_t easting = xLowerLeft+(cellSize*(e_coord+0.5)); //Add offset of 0.5 to get coordinates of middle of cell instead of edges.
  //Double_t northing = -1*(yLowerLeft+(cellSize*(n_coord+0.5)));

  //first set longitude

  if (northing!=0)
    lon = atan(easting/northing);
  else
    lon = 90*TMath::DegToRad();


  if (easting > 0 && lon < 0) //adjust sign of longitude
    lon += TMath::Pi();
  else if (easting < 0 && lon > 0)
    lon -= TMath::Pi();
  else if (easting == 0 && northing < 0)
    lon += TMath::Pi();

  //now find latitude

  if (easting != 0)
    bedmap_R = TMath::Abs(easting/sin(lon));
  else if (easting == 0 && northing != 0)
    bedmap_R = TMath::Abs(northing);
  else {
    lat = 0; //at the pole, set lat=0 degrees
    lon = lon*TMath::RadToDeg();
    return;
  } //else

  isometric_lat = (TMath::Pi()/2) - 2*atan(bedmap_R/(scale_factor*bedmap_c_0));

  lat = isometric_lat + bedmap_a_bar*sin(2*isometric_lat) + bedmap_b_bar*sin(4*isometric_lat) + bedmap_c_bar*sin(6*isometric_lat) + bedmap_d_bar*sin(8*isometric_lat);

  lon = lon * TMath::RadToDeg();  //convert to degrees
  lat =  -lat*TMath::RadToDeg(); //convert to degrees, with -90 degrees at the south pole
  return;
} //method ENtoLonLat

void BinnedBedmapReader::LonLattoEaNo(Double_t lon, Double_t lat, double& ea, double& no) {
// same as LonLattoEN, but returns easting/northing instead of indices

  ea=0;
  no=0;

  Double_t lon_rad = lon * TMath::DegToRad(); //convert to radians
  Double_t lat_rad = -lat * TMath::DegToRad();

  bedmap_R = scale_factor*bedmap_c_0 * pow(( (1 + eccentricity*sin(lat_rad)) / (1 - eccentricity*sin(lat_rad)) ),eccentricity/2) * tan((TMath::Pi()/4) - lat_rad/2);

  ea = bedmap_R * sin(lon_rad);
  no = bedmap_R * cos(lon_rad);

  return;
} //method LonLattoEaNo

void BinnedBedmapReader::ENtoEaNo(Int_t e_coord, Int_t n_coord, Double_t& es, Double_t& no) {
  es = xLowerLeft_surface + double(cellSize) * e_coord;
  no = -(yLowerLeft_surface + double(cellSize) * n_coord);
}

