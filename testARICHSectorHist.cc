/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Wed Feb 27 15:49:12 CET 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "libarihvmonitor.h"
#include "ARICHSectorHist.h"

Int_t testARICHSectorHist(){

  std::cout<<"testARICHSectorHist"<<std::endl;
  
  ARICHSectorHist *h1_ARICHSectorHist = new ARICHSectorHist("h1_ARICHSectorHist","ARICHSectorHist");

  for(unsigned int i = 1; i<=6;i++)
    h1_ARICHSectorHist->SetBinContent(i,i);

  //h1_ARICHSectorHist->Draw("ZCOLOR TEXT");
  //h1_ARICHSectorHist->SaveAs("h1_ARICHSectorHist.root");
  h1_ARICHSectorHist->DrawHisto("ZCOLOT text same", "./");
  
  return 0;
  
}
