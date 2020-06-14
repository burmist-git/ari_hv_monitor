#pragma once

#include <iostream>
#include <vector>
#include "ARICHsectorStr.h"

struct ARICHmappingStr {
  std::vector<ARICHsectorStr> sector;
  void printInfo(){
    for (unsigned i = 0; i<sector.size(); i++)
      sector[i].printInfo();
  }
};
