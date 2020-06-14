/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Thu Oct 31 17:01:49 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "libarihvmonitor.h"
#include "ARICHChannelHist.h"

//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <time.h>

//root
#include <TLegend.h>

using namespace std;

Int_t plotsMergedData( TString file_s1 = "root_pdf_summary/09.11.2019/vmon_09.11.2019_15-01-01-09.11.2019_15-14-01.root", TString outPdfFile = "c1.pdf", Double_t vmin = 0.0, Double_t vmax = 400.0, TString hapdTemplateMappingPath = "hapdTemplateMapping.dat"){
  //
  TString objectName01 = "hp_sum_chips_aver";
  TString objectName02 = "hp_sum_guard_aver";
  //Double_t vmon;
  //Double_t dvmon;
  Double_t vmonmin = vmin;
  Double_t vmonmax = vmax;
  // 
  TFile *f_s1 = new TFile(file_s1.Data());
  //
  TH2Poly *hp_chips = (TH2Poly*)f_s1->Get(objectName01.Data());
  TH2Poly *hp_guard = (TH2Poly*)f_s1->Get(objectName02.Data());
  //
  ARICHChannelHist *hp_chips_my = new ARICHChannelHist("hp_chips_my",hp_chips->GetTitle(),2);
  ARICHChannelHist *hp_guard_my = new ARICHChannelHist("hp_guard_my",hp_guard->GetTitle(),1);
  //
  for( unsigned int i = 1; i <= (unsigned int)hp_chips->GetNcells(); i++)
    hp_chips_my->SetBinContent(i,hp_chips->GetBinContent(i));
  for( unsigned int i = 1; i <= (unsigned int)hp_guard->GetNcells(); i++)
    hp_guard_my->SetBinContent(i,hp_guard->GetBinContent(i));
  //  
  TCanvas *c1 = new TCanvas("c1",file_s1.Data(),10,10,1500,1000);
  cout<<c1->GetTitle()<<endl;
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  gStyle->SetOptStat(kFALSE);
  //
  c1->Divide(3,2);
  c1->cd(1);
  c1->GetPad(1)->SetGrid();
  hp_chips_my->DrawHisto( "same ZCOLOR lcol", "", hp_chips->GetTitle(), "drawLine", vmonmin, vmonmax, c1, 0);
  c1->cd(2);
  c1->GetPad(2)->SetGrid();
  hp_guard_my->DrawHisto( "same ZCOLOR lcol", "", hp_guard->GetTitle(), "drawLine", vmonmin, vmonmax, c1, 0);
  //
  c1->cd(3);
  //
  ARICHmappingCopperMergerFeb *arichmap = new ARICHmappingCopperMergerFeb(hapdTemplateMappingPath);  
  Int_t sec_i, cop_i, mer_i, feb_i;
  ARICHChannelHist *h1_ARICHChannelHistFeb = new ARICHChannelHist( "h1_ARICHChannelHistFeb", "Feb ID reminder", 1);
  for(unsigned int i = 1; i<=420;i++){
    Int_t febID = i;
    arichmap->findPositionInarichmappingFormGlobalFebID(febID, sec_i, cop_i, mer_i, feb_i);
    h1_ARICHChannelHistFeb->SetBinContent(i, i);
  }
  h1_ARICHChannelHistFeb->DrawHisto("same ZCOLOR text lcol", "", h1_ARICHChannelHistFeb->GetTitle(), "drawLine",0.0,420, c1, 0);
  //
  c1->cd(4);
  //
  TH1D *h1_chips = new TH1D( "h1_chips", "chips", 200, vmin, vmax);
  for( unsigned int j = 1; j <= (unsigned int)hp_chips_my->GetNcells(); j++)
    h1_chips->Fill(hp_chips_my->GetBinContent(j));
  h1_chips->Draw();
  //
  c1->cd(5);
  //
  TH1D *h1_guard = new TH1D( "h1_chips", "chips", 200, vmin, vmax);
  for( unsigned int j = 1; j <= (unsigned int)hp_guard_my->GetNcells(); j++)
    h1_guard->Fill(hp_guard_my->GetBinContent(j));
  h1_guard->Draw();
  //
  c1->cd(6);
  //
  ARICHChannelHist *h1_ARICHChannelHistChip = new ARICHChannelHist("h1_ARICHChannelHistChip","Chip ID reminder",2);
  for(unsigned int i = 1; i<=420;i++){
    for(unsigned int j = 0; j<4;j++){
      Int_t febID = i;
      arichmap->findPositionInarichmappingFormGlobalFebID(febID, sec_i, cop_i, mer_i, feb_i);
      if(sec_i == 0 ){
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
  }
  h1_ARICHChannelHistChip->DrawHisto("same ZCOLOR text lcol", "", h1_ARICHChannelHistChip->GetTitle(), "drawLine",0.5,4.5,c1, 1);
  //
  c1->SetName("c1");
  c1->SetTitle("c1");
  //c1->SaveAs("c1.root");
  c1->SaveAs(outPdfFile.Data());
  //
  return 0;
}
