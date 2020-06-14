/***********************************************************************
* Copyright(C) 2020 - LBS - (Single person developer.)                 *
* Mon Jan 27 12:52:09 CET 2020                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "libarihvmonitor.h"

#ifdef PLOTSMERGEDDATA_MAIN
# ifndef __CINT__
int main(int argc, char *argv[]){
 if(argc == 6){
    TString inRootFile = argv[1];
    TString outPdfFile = argv[2];
    Double_t vmin = atof( argv[3]);
    Double_t vmax = atof( argv[4]);
    TString hapdTemplateMappingPath = argv[5];
    plotsMergedData( inRootFile, outPdfFile, vmin, vmax, hapdTemplateMappingPath);
 }
  return 0;
}
# endif //ifndef __CINT__
#endif //ifdef PLOTSMERGEDDATA_MAIN
