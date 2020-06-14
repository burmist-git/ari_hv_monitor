#ifndef ARIconst_h
#define ARIconst_h

#include <TString.h>

namespace ARIconst{

  //ARICH number of sectors
  const int nSectors = 6;

  //ARICH number of chips per HAPD
  const int nHAPDchips = 4;

  //ARICH hv-bias histos info 
  const int nhvBiasPar = 6;
  const TString hvBiasParArr[nhvBiasPar] = { "vmon", "cmon", "mask", "vlimit", "climit", "vdemand"};
  const double hvBiasPar_min[nhvBiasPar] = {  -10.0,  -30.0,   -2.0,    -10.0,    -30.0,     -10.0};
  const double hvBiasPar_max[nhvBiasPar] = {  400.0,   30.0,    2.0,    400.0,     30.0,     400.0};
  const int    hvBiasPar_bin[nhvBiasPar] = {    100,    100,    100,      100,      100,       100};
}

#endif
