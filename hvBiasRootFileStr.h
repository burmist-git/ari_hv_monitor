#pragma once

//c, c++
#include <iostream>
#include <iomanip>
#include <vector>

//root
#include <TROOT.h>

//1 climit 1571633519 sec_1_21.10.2019-13.51.59-1571633519.climit.root
//1 climit 1571634121 sec_1_21.10.2019-14.02.01-1571634121.climit.root
//1 climit 1571634714 sec_1_21.10.2019-14.11.54-1571634714.climit.root
// ...
struct hvBiasRootFileStr {
  Int_t secID;
  TString measType;
  Int_t ut;
  TString fname;
  hvBiasRootFileStr () {
    secID = -999;
    measType = "";
    ut = -999;
    fname = "";
  }
  void print(){
    std::cout<<std::setw(3)<<secID
	     <<" "<<measType
	     <<" "<<ut
	     <<" "<<fname
	     <<std::endl;
  }
  //void operator=(const hvBiasRootFileStr& mstr){
  //secID = mstr.secID;
  //measType = mstr.measType;
  //ut = mstr.ut;
  //fname = mstr.fname;
  //}
};
