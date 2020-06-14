/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Thu Oct 17 22:35:30 CEST 2019                                        *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//c, c++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <math.h>
#include <vector>

//root
#include <TVector2.h>
#include <TPolyLine.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TText.h>
#include <TMath.h>
#include <TH2F.h>
#include <TLegend.h>
#include <TCrown.h>
#include <TArc.h>
#include <TROOT.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TPad.h>
#include <TString.h>
#include <TFile.h>
#include <TAxis.h>
#include <TVector2.h>
#include <TImage.h>

using namespace std;

//1 KA0430 crate 1 slot 0 channel 34
//1 KA0430 crate 1 slot 0 channel 36
//1 KA0430 crate 1 slot 0 channel 38
struct crateToModuleMap_str {
  Int_t id;
  TString serial;
  Int_t crate;
  Int_t slot;
  Int_t channel;
  TString type;
  crateToModuleMap_str () {
    id = -999;
    serial="";
    crate=-999;
    slot=-999;
    channel=-999;
    type="";
  }
  void print(){
    cout<<setw(4)<<id
	<<setw(7)<<serial
	<<setw(6)<<"crate"
      	<<setw(2)<<crate
	<<setw(5)<<"slot"
	<<setw(3)<<slot
	<<setw(8)<<"channel"
	<<setw(3)<<channel
      	<<setw(7)<<type
	<<endl;
  }
};

//1 0 0  : bias-d
//1 0 1  : bias-b
//1 0 2  : bias-c
struct biashvType_str {
  Int_t crate;
  Int_t slot;
  Int_t channel;
  TString type;
  void print(){
    cout<<setw(2)<<crate
	<<setw(3)<<slot
	<<setw(3)<<channel
	<<setw(3)<<":"
	<<setw(7)<<type
	<<endl;
  }
};

//1 1 KA0430
//1 2 ZJ6721
//1 3 KA0793
struct moduleNameIDMap_str {
  Int_t secID;
  Int_t id;
  TString serial;
  void print(){
    cout<<setw(2)<<secID
	<<setw(4)<<id
	<<setw(7)<<serial
	<<endl;
  }
};

void make_hvbias_map(TString crateToModuleMap = "mappingInfo/crateToModuleMap.info_sec_1.short",
		     Int_t ari_sec_id = 1, 
		     TString biashvTypeLog = "mappingInfo/biashv.type.short.info",
		     TString moduleNameIDMap = "mappingInfo/moduleNameIDMap.info"){
  
  //cout<<"crateToModuleMap "<<crateToModuleMap<<endl
  //  <<"ari_sec_id       "<<ari_sec_id<<endl
  //  <<"biashvTypeLog    "<<biashvTypeLog<<endl
  //  <<"moduleNameIDMap  "<<moduleNameIDMap<<endl;

  string motserial;
  string mottype;
  string mot;
  Int_t crate;
  Int_t slot;
  Int_t channel;
  Int_t id;
  Int_t secID;
  TString type;
  TString serial;
  //
  //biashvSerialLog
  std::vector <crateToModuleMap_str> crateToModuleMap_v;
  std::vector <crateToModuleMap_str> crateToModuleMapFullshafl_v;
  std::vector <crateToModuleMap_str> crateToModuleMapFull_v;
  ifstream crateToModuleMap_file (crateToModuleMap.Data());
  if( crateToModuleMap_file.is_open() ){
    while( crateToModuleMap_file>> id >> motserial >> mot >>crate >> mot >> slot >> mot >> channel ){
      crateToModuleMap_str crateToModuleMapStr;
      serial = motserial;
      crateToModuleMapStr.id = id;
      crateToModuleMapStr.crate = crate;
      crateToModuleMapStr.slot = slot;
      crateToModuleMapStr.channel = channel;
      crateToModuleMapStr.serial = serial;
      crateToModuleMap_v.push_back(crateToModuleMapStr);
    }
  }
  //biashvTypeLog
  std::vector <biashvType_str> biasTypeStr_v;
  ifstream biashvTypeLog_file (biashvTypeLog.Data());
  if(biashvTypeLog_file.is_open()){
    while(biashvTypeLog_file >> crate >> slot >> channel >> mot >> mottype){
      //cout<<crate<<endl;
      biashvType_str biasTypeStr;
      type = mottype;
      biasTypeStr.crate = crate;
      biasTypeStr.slot = slot;
      biasTypeStr.channel = channel;
      biasTypeStr.type = type;
      biasTypeStr_v.push_back(biasTypeStr);
    }
  }
  //moduleNameIDMap
  std::vector <moduleNameIDMap_str> moduleNameIDMapStr_v;
  ifstream moduleNameIDMap_file (moduleNameIDMap.Data());
  if(moduleNameIDMap_file.is_open()){
    while(moduleNameIDMap_file >> secID >> id >> mot){
      //cout<<crate<<endl;
      moduleNameIDMap_str moduleNameIDMapStr;
      moduleNameIDMapStr.secID = secID;
      moduleNameIDMapStr.id = id;
      moduleNameIDMapStr.serial = mot;
      moduleNameIDMapStr_v.push_back(moduleNameIDMapStr);
    }
  }
  crateToModuleMap_file.close();
  biashvTypeLog_file.close();
  moduleNameIDMap_file.close();
  //
  //for(unsigned int i = 0; i < moduleNameIDMapStr_v.size(); i++)
  //moduleNameIDMapStr_v.at(i).print();
  //
  //cout<<crateToModuleMap_v.size()<<endl;
  //for(unsigned int i = 0; i < crateToModuleMap_v.size(); i++)
  //crateToModuleMap_v.at(i).print();
  //
  //cout<<biasTypeStr_v.size()<<endl;
  //for(unsigned int i = 0; i < biasTypeStr_v.size(); i++)
  //biasTypeStr_v.at(i).print();
  //
  for(unsigned int i = 0; i < crateToModuleMap_v.size(); i++){
    for(unsigned int j = 0; j < biasTypeStr_v.size(); j++){
      if(crateToModuleMap_v.at(i).crate == biasTypeStr_v.at(j).crate &&
	 crateToModuleMap_v.at(i).slot == biasTypeStr_v.at(j).slot &&
	 crateToModuleMap_v.at(i).channel == biasTypeStr_v.at(j).channel){
	crateToModuleMap_str crateToModuleMapStr;
	crateToModuleMapStr.id = crateToModuleMap_v.at(i).id;
	crateToModuleMapStr.crate = crateToModuleMap_v.at(i).crate;
	crateToModuleMapStr.slot = crateToModuleMap_v.at(i).slot;
	crateToModuleMapStr.channel = crateToModuleMap_v.at(i).channel;
	crateToModuleMapStr.serial = crateToModuleMap_v.at(i).serial;
	crateToModuleMapStr.type = biasTypeStr_v.at(j).type;
	crateToModuleMapFullshafl_v.push_back(crateToModuleMapStr);	
	//cout<<setw(4)<<crateToModuleMap_v.at(i).id
	//	<<setw(7)<<crateToModuleMap_v.at(i).serial
	//	<<setw(6)<<"crate"
	//	<<setw(2)<<crateToModuleMap_v.at(i).crate
	//	<<setw(5)<<"slot"
	//	<<setw(3)<<crateToModuleMap_v.at(i).slot
	//	<<setw(8)<<"channel"
	//	<<setw(3)<<crateToModuleMap_v.at(i).channel
	//	<<setw(7)<<biasTypeStr_v.at(j).type
	//	<<endl;
      }
    }
  }
  //
  //for(unsigned int i = 0; i < crateToModuleMapFullshafl_v.size(); i++)
  //crateToModuleMapFullshafl_v.at(i).print();
  //
  //
  // 1 KA0430 crate 1 slot  0 channel 34  guard
  // 1 KA0430 crate 1 slot  0 channel 36 bias-d
  // 1 KA0430 crate 1 slot  0 channel 38 bias-c
  // 1 KA0430 crate 1 slot  0 channel 40 bias-b
  // 1 KA0430 crate 1 slot  0 channel 42 bias-a
  //
  // 1   1 KA0430
  // 1   2 ZJ6721
  // 1   3 KA0793
  // 1   4 KA1043
  //
  //bias-a
  //bias-b
  //bias-c
  //bias-d
  //guard
  //hv
  vector <TString> type_v;
  type_v.push_back("bias-a");
  type_v.push_back("bias-b");
  type_v.push_back("bias-c");
  type_v.push_back("bias-d");
  type_v.push_back("guard");
  type_v.push_back("hv");
  //
  for(unsigned int i = 0; i < moduleNameIDMapStr_v.size(); i++){
    if(ari_sec_id == moduleNameIDMapStr_v.at(i).secID){
      //moduleNameIDMapStr_v.at(i).print();
      vector <crateToModuleMap_str> cmmStr_v;
      for(unsigned int k = 0; k < crateToModuleMapFullshafl_v.size(); k++){
	if(moduleNameIDMapStr_v.at(i).id == crateToModuleMapFullshafl_v.at(k).id){
	  crateToModuleMap_str cmmStr;
	  cmmStr.id = crateToModuleMapFullshafl_v.at(k).id;
	  cmmStr.crate = crateToModuleMapFullshafl_v.at(k).crate;
	  cmmStr.slot = crateToModuleMapFullshafl_v.at(k).slot;
	  cmmStr.channel = crateToModuleMapFullshafl_v.at(k).channel;
	  cmmStr.serial = crateToModuleMapFullshafl_v.at(k).serial;
	  cmmStr.type = crateToModuleMapFullshafl_v.at(k).type;
	  cmmStr_v.push_back(cmmStr);	
	}
      }
      //vector <crateToModuleMap_str> cmmStr_v;
      for(unsigned int ii = 0; ii < cmmStr_v.size(); ii++){
	for(unsigned int jj = 0; jj < cmmStr_v.size(); jj++){
	  if(type_v.at(ii) == cmmStr_v.at(jj).type){
	    crateToModuleMap_str cmmStr;
	    cmmStr.id = cmmStr_v.at(jj).id;
	    cmmStr.crate = cmmStr_v.at(jj).crate;
	    cmmStr.slot = cmmStr_v.at(jj).slot;
	    cmmStr.channel = cmmStr_v.at(jj).channel;
	    cmmStr.serial = cmmStr_v.at(jj).serial;
	    cmmStr.type = cmmStr_v.at(jj).type;
	    crateToModuleMapFull_v.push_back(cmmStr);	
	  }
	}
      }
    }  
  }
  for(unsigned int i = 0; i < crateToModuleMapFull_v.size(); i++)
    crateToModuleMapFull_v.at(i).print();
  //
}

#ifdef MAKE_HVBIAS_MAP
int main(int argc, char *argv[]){
    if(argc == 1){
      make_hvbias_map();
    }
    else if(argc == 5){
      TString crateToModuleMap = argv[1];
      Int_t ari_sec_id = atoi(argv[2]);
      TString biashvTypeLog = argv[3];
      TString moduleNameIDMap = argv[4];
      make_hvbias_map( crateToModuleMap, ari_sec_id, biashvTypeLog, moduleNameIDMap);
      //cout<<"ERROR in input argumentswqw : "<<endl;
    }
    else{
      cout<<"ERROR in input arguments : "<<endl;
    }
  return 0;
}
#endif //ifdef MAKE_HVBIAS_MAP
