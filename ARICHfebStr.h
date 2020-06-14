#pragma once

#include <iostream>

struct ARICHfebStr {
  int iRing;
  int iPos;
  int localID;
  int globalID;
  ARICHfebStr(){
    iRing = -999;
    iPos = -999;
    localID = -999;
    globalID = -999;
  }
  ~ARICHfebStr(){;}
  void printInfo(){
    std::cout<<"                     FEBLOCALID  "<<localID<<std::endl;
    std::cout<<"                     FEBGLOBALID "<<globalID<<std::endl;
    std::cout<<"                                iR "<<iRing<<std::endl
	     <<"                                iP "<<iPos<<std::endl;
  }
};
