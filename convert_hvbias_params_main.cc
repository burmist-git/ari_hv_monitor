/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Mon Oct 21 19:47:02 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "libarihvmonitor.h"

#ifdef CONVERT_HVBIAS_PARAMS_MAIN
# ifndef __CINT__
int main(int argc, char *argv[]){
  if(argc>7){
    Int_t secID = atoi(argv[1]);
    Int_t unixTime = atoi(argv[2]);
    TString dateMeas = argv[3];
    TString inCrateToModuleMap = argv[4];
    TString parsufix = argv[5];
    TString outRootFile = argv[6];
    TString maskFlag;
    cout<<"secID              "<<secID<<endl
	<<"unixTime           "<<unixTime<<endl
	<<"dateMeas           "<<dateMeas<<endl
	<<"inCrateToModuleMap "<<inCrateToModuleMap<<endl
	<<"parsufix           "<<parsufix<<endl
	<<"outRootFile        "<<outRootFile<<endl
	<<endl;
    std::vector<int> crate_v;
    std::vector<int> slot_v;
    std::vector<int> channel_v;
    std::vector<float> val_v;
    for(int i = 7;i<argc;i++){
      crate_v.push_back(atoi(argv[i]));
      i++;
      slot_v.push_back(atoi(argv[i]));
      i++;
      channel_v.push_back(atoi(argv[i]));
      i++;
      if(parsufix != "mask"){
	val_v.push_back(atof(argv[i]));
      }
      else{
	maskFlag = argv[i];
	if(maskFlag == "no")
	  val_v.push_back(0.0);
	if(maskFlag == "mask")
	  val_v.push_back(1.0);
      }
    }
    convert_hvbias_params( secID,
			   unixTime, 
			   dateMeas, 
			   inCrateToModuleMap,
			   parsufix,
			   outRootFile,
			   crate_v,
			   slot_v,
			   channel_v,
			   val_v);
  }
  return 0;
}
# endif //ifndef __CINT__
#endif //ifdef CONVERT_HVBIAS_PARAMS_MAIN
