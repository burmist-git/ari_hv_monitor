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

using namespace std;

Int_t plots(){
  //
  TString file_s1;
  TString file_s2;
  TString file_s3;
  TString file_s4;
  TString file_s5;
  TString file_s6;
  //
  TString objectName01;
  TString objectName02;
  //
  TString title01;
  objectName01 = "hp_chips";
  title01 = " (a) vmon 21.10.2019-13.51.01";
  //objectName01 = "hp_guard";
  //title01 = " (guard) vmon 21.10.2019-13.51.01";
  //objectName02 = "";
  //
  file_s1 = "data_root/21.10.2019/sec_1_21.10.2019-13.51.01-1571633461.vmon.root";
  file_s2 = "data_root/21.10.2019/sec_2_21.10.2019-14.12.01-1571634721.vmon.root";
  file_s3 = "data_root/21.10.2019/sec_3_21.10.2019-13.53.01-1571633581.vmon.root";
  file_s4 = "data_root/21.10.2019/sec_4_21.10.2019-14.04.01-1571634241.vmon.root";
  file_s5 = "data_root/21.10.2019/sec_5_21.10.2019-13.55.01-1571633701.vmon.root";
  file_s6 = "data_root/21.10.2019/sec_6_21.10.2019-14.16.01-1571634961.vmon.root";
  /*
  file_s1 = "data_root/21.10.2019/sec_1_21.10.2019-13.51.06-1571633466.cmon.root";
  file_s2 = "data_root/21.10.2019/sec_2_21.10.2019-13.52.07-1571633527.cmon.root";
  file_s3 = "data_root/21.10.2019/sec_3_21.10.2019-13.53.12-1571633592.cmon.root";
  file_s4 = "data_root/21.10.2019/sec_4_21.10.2019-13.54.11-1571633651.cmon.root";
  file_s5 = "data_root/21.10.2019/sec_5_21.10.2019-14.25.06-1571635506.cmon.root";
  file_s6 = "data_root/21.10.2019/sec_6_21.10.2019-13.56.07-1571633767.cmon.root";
  */
  //
  //mgTitle="Total water flux : 4.2 L/min, water @ 20 ^{o}C";
  //mgTitle="Total water flux : 4.4 L/min";
  //mgTitle="Total water flux : 2.4 L/min";
  //mgTitle="Total water flux : 3.2 L/min";
  //mgTitle="Total water flux : 4.1 L/min, water @ 20 ^{o}C";
  //
  //Double_t vmon;
  //Double_t dvmon;
  Double_t vmonmin;
  Double_t vmonmax;
  //vmon = 0.0;
  //dvmon = 1.0;
  //vmon = 325.0;
  //dvmon = 30.0;
  //vmon = 175.0;
  //dvmon = 0.03;
  //vmonmin = vmon - dvmon;
  //vmonmax = vmon + dvmon;
  vmonmin = 0.0;
  vmonmax = 400.0;
  //vmonmin = 50;
  //vmonmax = 100;
  // 
  TFile *f_s1 = new TFile(file_s1.Data());
  TFile *f_s2 = new TFile(file_s2.Data());
  TFile *f_s3 = new TFile(file_s3.Data());
  TFile *f_s4 = new TFile(file_s4.Data());
  TFile *f_s5 = new TFile(file_s5.Data());
  TFile *f_s6 = new TFile(file_s6.Data());

  TH2Poly *hp_s1 = (TH2Poly*)f_s1->Get(objectName01.Data());
  TH2Poly *hp_s2 = (TH2Poly*)f_s2->Get(objectName01.Data());
  TH2Poly *hp_s3 = (TH2Poly*)f_s3->Get(objectName01.Data());
  TH2Poly *hp_s4 = (TH2Poly*)f_s4->Get(objectName01.Data());
  TH2Poly *hp_s5 = (TH2Poly*)f_s5->Get(objectName01.Data());
  TH2Poly *hp_s6 = (TH2Poly*)f_s6->Get(objectName01.Data());

  TCanvas *c1 = new TCanvas("c1",file_s1.Data(),10,10,600,600);
  cout<<c1->GetTitle()<<endl;
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  gStyle->SetOptStat(kFALSE);

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
  hp_sum->DrawHisto( "same ZCOLOR lcol", "hp_sum.pdf", title01.Data(), "drawLine", vmonmin, vmonmax, c1, 0);


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
