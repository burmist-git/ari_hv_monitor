#pragma once

#include <iostream>
#include <vector>
#include "ARICHfebStr.h"

struct ARICHmergerStr {
  int localID;
  int globalID;
  ARICHmergerStr(){
    localID = -999;
    globalID = -999;
  }
  std::vector<ARICHfebStr> feb;
  void printInfo(){
    std::cout<<"       MERGERLOCALID "<<localID<<std::endl;
    std::cout<<"       MERGERGLOBALID "<<globalID<<std::endl;
    for (unsigned i = 0; i<feb.size(); i++)
      feb[i].printInfo();
  }
};
