/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Thu Oct 31 17:11:31 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "libarihvmonitor.h"

#ifdef PLOTSSECTOR_MAIN
# ifndef __CINT__
int main(int argc, char *argv[]){
 if(argc == 7){
    TString inRootFile = argv[1];
    TString outPdfFile = argv[2];
    Int_t secID = atoi( argv[3]);
    Double_t vmin = atof( argv[4]);
    Double_t vmax = atof( argv[5]);
    TString hapdTemplateMappingPath = argv[6];
    plotsSector( inRootFile, outPdfFile, secID, vmin, vmax, hapdTemplateMappingPath);
 }
  return 0;
}
# endif //ifndef __CINT__
#endif //ifdef PLOTSSECTOR_MAIN
