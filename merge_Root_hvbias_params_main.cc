/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Fri Oct 25 11:38:58 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "libarihvmonitor.h"

#ifdef MERGE_ROOT_HVBIAS_PARAMS_MAIN
# ifndef __CINT__
int main(int argc, char *argv[]){
  if(argc == 5){
    TString inList = argv[1];
    TString inDataDir = argv[2];
    TString outRootFileDir = argv[3];
    TString hapdTemplateMappingPath = argv[4];
    merge_Root_hvbias_params( inList, inDataDir, outRootFileDir, hapdTemplateMappingPath);
  }
  return 0;
}
# endif //ifndef __CINT__
#endif //ifdef MERGE_ROOT_HVBIAS_PARAMS_MAIN
