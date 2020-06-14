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

Int_t plotsSector( TString file_s1 = "data_root/30.10.2019/sec_1_30.10.2019-08.51.01-1572393061.vmon.root", TString outPdfFile = "c1.pdf", Int_t secID = 0, Double_t vmin = 0.0, Double_t vmax = 400.0, TString hapdTemplateMappingPath = "hapdTemplateMapping.dat"){
  //
  TString objectName01 = "hp_chips";
  TString objectName02 = "hp_guard";
  TString objectName03 = "h1_chip_a";
  TString objectName04 = "h1_chip_b";
  TString objectName05 = "h1_chip_c";
  TString objectName06 = "h1_chip_d";
  TString objectName07 = "h1_guard";
  //Double_t vmon;
  //Double_t dvmon;
  Double_t vmonmin = vmin;
  Double_t vmonmax = vmax;
  //vmon = 0.0;
  //dvmon = 1.0;
  //vmon = 325.0;
  //dvmon = 30.0;
  //vmon = 175.0;
  //dvmon = 0.03;
  //vmonmin = vmon - dvmon;
  //vmonmax = vmon + dvmon;
  //vmonmin = 0.0;
  //vmonmax = 400.0;
  //vmonmin = 50;
  //vmonmax = 100;
  // 
  TFile *f_s1 = new TFile(file_s1.Data());
  //
  TH2Poly *hp_chips = (TH2Poly*)f_s1->Get(objectName01.Data());
  TH2Poly *hp_guard = (TH2Poly*)f_s1->Get(objectName02.Data());
  //
  TH1D *h1_a = (TH1D*)f_s1->Get(objectName03.Data());
  TH1D *h1_b = (TH1D*)f_s1->Get(objectName04.Data());
  TH1D *h1_c = (TH1D*)f_s1->Get(objectName05.Data());
  TH1D *h1_d = (TH1D*)f_s1->Get(objectName06.Data());
  TH1D *h1_g = (TH1D*)f_s1->Get(objectName07.Data());
  //
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
  hp_chips_my->DrawHisto( "same ZCOLOR lcol", "", hp_chips->GetTitle(), "drawLine", vmonmin, vmonmax, c1, secID);
  c1->cd(2);
  c1->GetPad(2)->SetGrid();
  hp_guard_my->DrawHisto( "same ZCOLOR lcol", "", hp_guard->GetTitle(), "drawLine", vmonmin, vmonmax, c1, secID);
  //
  c1->cd(4);
  c1->GetPad(4)->SetGrid();
  //gStyle->SetOptStat(kFALSE);
  h1_a->SetTitle("Chips");
  h1_a->SetMinimum(0.0);
  h1_a->SetMaximum(25.0);
  //
  h1_a->SetLineColor(kBlack);
  h1_b->SetLineColor(kRed);
  h1_c->SetLineColor(kBlue);
  h1_d->SetLineColor(kGreen+2);
  //
  h1_a->SetLineWidth(2.0);
  h1_b->SetLineWidth(2.0);
  h1_c->SetLineWidth(2.0);
  h1_d->SetLineWidth(2.0);
  //  
  h1_a->Draw();
  h1_b->Draw("sames");
  h1_c->Draw("sames");
  h1_d->Draw("sames");
  //
  c1->cd(5);
  c1->GetPad(5)->SetGrid();
  //gStyle->SetOptStat(kFALSE);
  h1_g->SetTitle("guard");
  h1_g->SetLineWidth(2.0);
  h1_g->Draw();
  //
  //
  //
  ARICHmappingCopperMergerFeb *arichmap = new ARICHmappingCopperMergerFeb(hapdTemplateMappingPath);  
  Int_t sec_i, cop_i, mer_i, feb_i;
  //
  c1->cd(3);
  //
  ARICHChannelHist *h1_ARICHChannelHistFeb = new ARICHChannelHist( "h1_ARICHChannelHistFeb", "Feb ID reminder", 1);
  for(unsigned int i = 1; i<=420;i++){
    Int_t febID = i;
    arichmap->findPositionInarichmappingFormGlobalFebID(febID, sec_i, cop_i, mer_i, feb_i);
    if(sec_i == (secID-1))
      h1_ARICHChannelHistFeb->SetBinContent(i, i);
  }
  h1_ARICHChannelHistFeb->DrawHisto("same ZCOLOR text lcol", "", h1_ARICHChannelHistFeb->GetTitle(), "drawLine",0.0,420, c1, secID);
  //  
  c1->cd(6);
  //
  ARICHChannelHist *h1_ARICHChannelHistChip = new ARICHChannelHist("h1_ARICHChannelHistChip","Chip ID reminder",2);
  for(unsigned int i = 1; i<=420;i++){
    for(unsigned int j = 0; j<4;j++){
      Int_t febID = i;
      arichmap->findPositionInarichmappingFormGlobalFebID(febID, sec_i, cop_i, mer_i, feb_i);
      if(sec_i == (secID-1)){
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
  h1_ARICHChannelHistChip->DrawHisto("same ZCOLOR text lcol", "", h1_ARICHChannelHistChip->GetTitle(), "drawLine",0.5,4.5,c1, secID);
  //TLegend *leg1 = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  //leg1->AddEntry(h1_a, "Chip A", "apl");
  //leg1->AddEntry(h1_b, "Chip B", "apl");
  //leg1->AddEntry(h1_c, "Chip C", "apl");
  //leg1->AddEntry(h1_d, "Chip D", "apl");
  //leg1->Draw();
  //
  c1->SetName("c1");
  c1->SetTitle("c1");
  //c1->SaveAs("c1.root");
  c1->SaveAs(outPdfFile.Data());
  
  /*
  ARICHChannelHist *hp_sum = new ARICHChannelHist("hp_sum",title01.Data(),2);
  hp_sum->SetMaximum(vmonmax);
  hp_sum->SetMinimum(vmonmin);
  Double_t sum = 0;
  for( unsigned int i = 1; i <= (unsigned int)hp_sum->GetNcells(); i++){
    sum = 
      hp_s1->GetBinContent(i) +
      hp_s2->GetBinContent(i) +
      hp_s3->GetBinContent(i) +
      hp_s4->GetBinContent(i) +
      hp_s5->GetBinContent(i) +
      hp_s6->GetBinContent(i);
    hp_sum->SetBinContent(i,sum);
  }
  //hp_sum->Draw("ZCOLOR lcol");
  //void DrawHisto( TString opt, TString pdfOutFileName, TString frameTitle, TString lineDrawOpt, Double_t ztMin, Double_t ztMax);
  hp_sum->DrawHisto( "same ZCOLOR lcol", "hp_sum.pdf", title01.Data(), "drawLine", vmonmin, vmonmax, c1);
  */

  /*   
  h1_feb_t1->GetXaxis()->SetTitle("Temperature, ^{o}C");
  //mg->GetYaxis()->SetTitle("Power, W");
  h1_feb_t1->SetTitle(mgTitle.Data());
  h1_feb_t1->GetXaxis()->SetRangeUser(tmin,tmax);
  h1_feb_t1->SetLineColor(kBlack);
  h1_feb_t2->SetLineColor(kRed);
  h1_feb_t1->SetLineWidth(3.0);
  h1_feb_t2->SetLineWidth(3.0);
  h1_feb_t1->SetMaximum(20000);
  h1_feb_t1->Draw();
  h1_feb_t2->Draw("sames");
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(h1_feb_t1, "t1", "apl");
  leg->AddEntry(h1_feb_t2, "t2", "apl");
  leg->Draw();

  TCanvas *c2 = new TCanvas("c2",fileN_m.Data(),20,20,600,600);
  h1_merger_t_fpga->GetXaxis()->SetTitle("Temperature, ^{o}C");
  //mg->GetYaxis()->SetTitle("Power, W");
  h1_merger_t_fpga->SetTitle(mgTitle.Data());
  h1_merger_t_fpga->GetXaxis()->SetRangeUser(tmin,tmax);
  h1_merger_t_fpga->SetLineColor(kBlack);
  h1_merger_t_board->SetLineColor(kRed);
  h1_merger_t_fpga->SetLineWidth(3.0);
  h1_merger_t_board->SetLineWidth(3.0);
  h1_merger_t_fpga->SetMaximum(4000);
  h1_merger_t_fpga->Draw();
  h1_merger_t_board->Draw("sames");

  TLegend *leg2 = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg2->AddEntry(h1_merger_t_fpga, "fpga", "apl");
  leg2->AddEntry(h1_merger_t_board, "board", "apl");
  leg2->Draw();
  */

  return 0;
}
