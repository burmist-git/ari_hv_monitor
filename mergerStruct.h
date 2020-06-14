#pragma once

#include <TString.h>

struct mergerStruct {
  // ----
  //Board_size      95 120 mm2                      
  //Sector  MergerSN        center_radius_mm        center_rotation_deg     Board_Rotation_deg
  //1       1               584.2                   9                       9
  // ----
  int Sector;  
  int MergerSN;        
  float center_radius_mm;
  float center_rotation_deg;
  float Board_Rotation_deg;
  inline mergerStruct operator=(mergerStruct a) {
    Sector = a.Sector;  
    MergerSN = a.MergerSN;        
    center_radius_mm = a.center_radius_mm;
    center_rotation_deg = a.center_rotation_deg;
    Board_Rotation_deg = a.Board_Rotation_deg;
    return a;
  }
  mergerStruct(){
    Sector = -999;
    MergerSN = -999;
    center_radius_mm = -999.0;
    center_rotation_deg = -999.0;
    Board_Rotation_deg = -999.0;
  }
};
