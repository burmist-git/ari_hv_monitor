#pragma once

//c, c++
#include <iostream>
#include <iomanip>
#include <vector>

//root
#include <TROOT.h>

//1 KA0430 crate 1 slot  0 channel 42 bias-a
//1 KA0430 crate 1 slot  0 channel 40 bias-b
//1 KA0430 crate 1 slot  0 channel 38 bias-c
// ...
struct crateToModuleMapStr {
  Int_t id;
  TString serial;
  Int_t crate;
  Int_t slot;
  Int_t channel;
  TString type;
  crateToModuleMapStr () {
    id = -999;
    serial="";
    crate=-999;
    slot=-999;
    channel=-999;
    type="";
  }
  void print(){
    std::cout<<std::setw(4)<<id
	<<std::setw(7)<<serial
	<<std::setw(6)<<"crate"
      	<<std::setw(2)<<crate
	<<std::setw(5)<<"slot"
	<<std::setw(3)<<slot
	<<std::setw(8)<<"channel"
	<<std::setw(3)<<channel
      	<<std::setw(7)<<type
	<<std::endl;
  }
};
