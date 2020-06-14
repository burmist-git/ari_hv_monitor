//my
#include "libarihvmonitor.h"
#include "ARICHChannelHist.h"

//rot
#include <TVirtualPad.h>
#include <TPad.h>
#include <TCanvas.h>

Int_t testARICHChannelHist(){
  //
  std::cout<<"testARICHChannelHist"<<std::endl;
  //////////////////
  ARICHChannelHist *h1_ARICHChannelHist = new ARICHChannelHist("h1_ARICHChannelHist","ARICHChannelHist",1);
  for(unsigned int i = 1; i<=420;i++)
    h1_ARICHChannelHist->SetBinContent(i,i);
  TCanvas *c1 = new TCanvas("c1","canva1",10,10,1000,1000);
  //h1_ARICHChannelHist->Draw("ZCOLOR TEXT");
  h1_ARICHChannelHist->DrawHisto("same ZCOLOR TEXT lcol","","","drawLine",0,420,c1,0);
  h1_ARICHChannelHist->SaveAs("h1_ARICHChannelHist.root");
  //
  ARICHChannelHist *h1_ARICHChannelHistChip = new ARICHChannelHist("h1_ARICHChannelHistChip","ARICHChannelHistChip",2);
  for(unsigned int i = 1; i<=420;i++){
    for(unsigned int j = 0; j<4;j++){
      if( j == 2 )
	h1_ARICHChannelHistChip->setBinContent(i, j, 4);
      else if ( j == 3)
	h1_ARICHChannelHistChip->setBinContent(i, j, 3);
      else
	h1_ARICHChannelHistChip->setBinContent(i, j, j+1);
    }
    //for(unsigned int k = 0; k<(j+1);k++)
    //h1_ARICHChannelHistChip->fillBin(i,j);
    //for(unsigned int j = 1; j<=4;j++)
  }
  TCanvas *c2 = new TCanvas("c2","canva2",20,20,1000,1000);
  h1_ARICHChannelHistChip->DrawHisto("same ZCOLOR TEXT lcol","","","drawLine",0.5,4.5,c2,0);
  h1_ARICHChannelHistChip->SaveAs("h1_ARICHChannelHistChip.root");
  //
  ARICHChannelHist *h1_ARICHChannelHistChip_test = new ARICHChannelHist("h1_ARICHChannelHistChip_test","h1_ARICHChannelHistChip_test",2);
  for(unsigned int i = 1; i<=(unsigned int)h1_ARICHChannelHistChip_test->GetNcells();i++)
    h1_ARICHChannelHistChip_test->SetBinContent(i, i);
  TCanvas *c3 = new TCanvas("c3","canva3",30,30,1000,1000);
  h1_ARICHChannelHistChip_test->DrawHisto("same ZCOLOR lcol","","","drawLine",0.0,2000,c3,0);
  //
  //c2->SetGrid();
  //  
  //new TCanvas("c2","canva2",20,20,1000,1000);
  //ARICHChannelHist *h1_ARICHChannelHist_clone = ARICHChannelHist::createFrom(h1_ARICHChannelHist);
  //h1_ARICHChannelHist_clone->Draw("ZCOLOR TEXT");
  //
  return 0;
}
