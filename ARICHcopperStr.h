#pragma once

#include <iostream>
#include <vector>
#include "ARICHmergerStr.h"

struct ARICHcopperStr {
  int localID;
  int globalID;
  ARICHcopperStr(){
    localID = -999;
    globalID = -999;
  }
  std::vector<ARICHmergerStr> merger;
  void printInfo(){
    std::cout<<"COPPERLOCALID  "<<localID<<std::endl;
    std::cout<<"COPPERGLOBALID "<<globalID<<std::endl;
    for (unsigned i = 0; i<merger.size(); i++)
      merger[i].printInfo();
  }
};
