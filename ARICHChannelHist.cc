/***********************************************************************
* Copyright(C) 2017 - LBS - (Single person developer.)                 *
* Thu Oct 18 13:36:31 JST 2018                                         *
* Autor: Leonid Burmistrov, Luka Santelj                               *
***********************************************************************/

//my
#include <ARICHChannelHist.h>

//root
#include <TVector2.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TMath.h>
#include <TLine.h>
#include <TCanvas.h>

//c, c++
#include <math.h>
#include <iostream>
#include <algorithm>

using namespace std;

ARICHChannelHist::ARICHChannelHist(const char* name, const char* title, int type) : TH2Poly()
{

  m_histName = name;
  m_histTitle = title;
  m_type = type;
  
  SetName(name);
  SetTitle(title);
  
  const std::vector<unsigned>& moduleIDs = std::vector<unsigned>();
  
  m_hapd2binMap.assign(420, 0);

  // positions of HAPDs and channel mapping (avoid using DB classes...)
  double rs[7] = {57.35, 65.81, 74.37, 82.868, 91.305, 99.794, 108.185};
  unsigned nhapds[7] = {42, 48, 54, 60, 66, 72, 78};
  unsigned chmap[144] = {88, 86, 96, 87, 75, 72, 97, 108, 73, 74, 98, 109, 84, 85, 120, 110, 76, 77, 132, 121, 136, 124, 99, 133, 125, 113, 122, 111, 101, 137, 134, 123, 89, 112, 100, 135, 52, 28, 3, 40, 41, 5, 15, 2, 17, 29, 27, 14, 4, 16, 1, 26, 53, 65, 0, 13, 48, 49, 39, 12, 61, 62, 25, 38, 63, 60, 24, 37, 64, 50, 51, 36, 91, 115, 140, 103, 102, 138, 128, 141, 126, 114, 116, 129, 139, 127, 142, 117, 90, 78, 143, 130, 95, 94, 104, 131, 82, 81, 118, 105, 80, 83, 119, 106, 79, 93, 92, 107, 55, 57, 47, 56, 68, 71, 46, 35, 70, 69, 45, 34, 59, 58, 23, 33, 67, 66, 11, 22, 7, 19, 44, 10, 18, 30, 21, 32, 42, 6, 9, 20, 54, 31, 43, 8};
  double chns[12] = { -2.88, -2.37, -1.86, -1.35, -0.84, -0.33, 0.33, 0.84, 1.35, 1.86, 2.37, 2.88};

  float size = 0.5 / 2. - 0.01;
  if (type == 1) size = 7.0 / 2. - 0.5;
  if (m_type == 2) size = 3.3 / 2.;

  float X[5], Y[5], globX[5], globY[5];
  X[0] = -size;  Y[0] = -size;
  X[1] =  size;  Y[1] = -size;
  X[2] =  size;  Y[2] =  size;
  X[3] = -size;  Y[3] =  size;
  X[4] = -size;  Y[4] = -size;

  int nhapd = 1;
  unsigned iring = 0;
  unsigned ihapd = 0;

  std::vector<unsigned> ids;
  if (moduleIDs.size() > 0) ids = moduleIDs;
  else {
    for (int hapdID = 1; hapdID < 421; hapdID++) {
      ids.push_back(hapdID);
    }
  }

  // HAPD bins
  if (type == 1) {
    for (int hapdID = 1; hapdID < 421; hapdID++) {
      //for (unsigned hapdID : ids) {
      //m_hapd2binMap[hapdID - 1] = nhapd;
      //nhapd++;
      float r = rs[iring];
      float dphi = 2.*M_PI / nhapds[iring];
      float fi = dphi / 2. + ihapd * dphi;
      TVector2 centerPos(r * cos(fi), r * sin(fi));
      for (int i = 0; i < 5; i++) {
        float rotX = X[i] * cos(fi) - Y[i] * sin(fi);
        float rotY = X[i] * sin(fi) + Y[i] * cos(fi);
        globX[i] = rotX + centerPos.X();
        globY[i] = rotY + centerPos.Y();
      }
      if (std::find(ids.begin(), ids.end(), hapdID) != ids.end()) {
        m_hapd2binMap[hapdID - 1] = nhapd;
        nhapd++;
        TGraph* mybox = new TGraph(5, globX, globY);
        mybox->SetName((to_string(hapdID)).c_str());
        AddBin(mybox);
      }
      ihapd++;
      if (ihapd == nhapds[iring]) { iring++; ihapd = 0;}
    }

  } else if (type == 0) {
    for (int hapdID = 1; hapdID < 421; hapdID++) {
      //for (unsigned hapdID : ids) {
      //  m_hapd2binMap[hapdID - 1] = nhapd;
      //  nhapd++;
      float dphi = 2.*M_PI / nhapds[iring];
      float fi = dphi / 2. + ihapd * dphi;
      float r = rs[iring];
      for (int chID = 0; chID < 144; chID++) {

        unsigned chX = chmap[chID] % 12;
        unsigned chY = chmap[chID] / 12;
        TVector2 hapdPos(r * cos(fi), r * sin(fi));
        TVector2 locPos(chns[chX], chns[chY]);
        TVector2 centerPos = hapdPos + locPos.Rotate(fi);

        for (int i = 0; i < 5; i++) {
          float rotX = X[i] * cos(fi) - Y[i] * sin(fi);
          float rotY = X[i] * sin(fi) + Y[i] * cos(fi);
          globX[i] = rotX + centerPos.X();
          globY[i] = rotY + centerPos.Y();
        }
        if (std::find(ids.begin(), ids.end(), hapdID) != ids.end()) {
          m_hapd2binMap[hapdID - 1] = nhapd;
          if (chID == 143) nhapd++;
          TGraph* mybox = new TGraph(5, globX, globY);
          mybox->SetName((to_string(hapdID)).c_str());
          AddBin(mybox);
        }
      }
      ihapd++;
      if (ihapd == nhapds[iring]) { iring++; ihapd = 0;}
    }
  } else if (m_type == 2) {
    size += 0.2;
    for (int hapdID = 1; hapdID < 421; hapdID++) {
      float dphi = 2.*M_PI / nhapds[iring];
      float fi = dphi / 2. + ihapd * dphi;
      float r = rs[iring];
      TVector2 hapdPos(r * cos(fi), r * sin(fi));
      for (int chipID = 0; chipID < 4; chipID++) {
        TVector2 locPos(-size + (chipID / 2)*size * 2, size - (chipID % 2)*size * 2);
        TVector2 centerPos = hapdPos + locPos.Rotate(fi);

        for (int i = 0; i < 5; i++) {
          float rotX = X[i] * cos(fi) - Y[i] * sin(fi);
          float rotY = X[i] * sin(fi) + Y[i] * cos(fi);
          globX[i] = rotX + centerPos.X();
          globY[i] = rotY + centerPos.Y();
        }

        if (std::find(ids.begin(), ids.end(), hapdID) != ids.end()) {
          m_hapd2binMap[hapdID - 1] = nhapd;
          if (chipID == 3) nhapd++;
          TGraph* mybox = new TGraph(5, globX, globY);
          mybox->SetName((to_string(hapdID)).c_str());
          AddBin(mybox);
        }
      }
      ihapd++;
      if (ihapd == nhapds[iring]) { iring++; ihapd = 0;}
    }
  }
  else {
    std::cout << " type  = "<<type<< std::endl;
    std::cout << " m_type = "<<m_type<< std::endl;
    std::cout << "Invalid histogram type! use 0 for channel bins or 1 for HAPD bins" << std::endl;
  }
  SetOption("colz");

}

/*
ARICHChannelHist* ARICHChannelHist::createFrom(ARICHChannelHist* tpoly_chHist){
  ARICHChannelHist *new_tpoly_chHist = new ARICHChannelHist(tpoly_chHist->getHistName(),tpoly_chHist->getHistTitle(),tpoly_chHist->getType());
  for(unsigned int i = 1; i<=420;i++)
    new_tpoly_chHist->SetBinContent(i,tpoly_chHist->GetBinContent(i));
  return new_tpoly_chHist;
}
*/

void ARICHChannelHist::DrawHisto( TString opt = "ZCOLOR text same", TString pdfOutFileName = "./out.pdf", TString frameTitle = "", TString lineDrawOpt = "drawLine", Double_t ztMin = 0, Double_t ztMax = 100, TCanvas *c1 = 0, Int_t secID = 0){
  gStyle->SetPalette(kRainBow);
  //gStyle->SetPalette(kBlackBody);
  //gStyle->SetPalette(kBird);
  //gStyle->SetPalette(kBeach);
  //gStyle->SetPalette(kMint);
  //gStyle->SetPalette(kPastel);
  //TCanvas *c1 = new TCanvas("c1","c1",1000,1000);
  SetTitle(frameTitle.Data());
  c1->SetTitle(m_histTitle.Data());
  c1->SetName(m_histName.Data());
  c1->SetRightMargin(0.12);
  c1->SetLeftMargin(0.12);
  c1->SetTopMargin(0.1);
  c1->SetBottomMargin(0.15);
  c1->SetGrid();
  ///////////////////////////////////////////////////////////////
  //Double_t frameXmin = -1200;
  //Double_t frameXmax =  1200;
  //Double_t frameYmin = -1200;
  //Double_t frameYmax =  1200;
  //
  //Double_t frameXmin = -1140;
  //Double_t frameXmax =  1140;
  //Double_t frameYmin = -1140;
  //Double_t frameYmax =  1140;
  //
  //Double_t frameXmin =  12.0;
  //Double_t frameXmax =  114.0;
  //Double_t frameYmin = -100.0;
  //Double_t frameYmax =  2.0;
  //
  Double_t frameXmin = -117.0;
  Double_t frameXmax =  117.0;
  Double_t frameYmin = -117.0;
  Double_t frameYmax =  117.0;
  if(secID != 0){
    if(secID > 0 && secID < 7){
      Double_t sec_center_R = 83.0;
      Double_t theta = 2*TMath::Pi()/6.0;
      Double_t frameL = 2.0*115*TMath::Sin(theta/2.0);
      Double_t sec_center_X = sec_center_R * TMath::Cos(theta*secID - theta/2.0);
      Double_t sec_center_Y = sec_center_R * TMath::Sin(theta*secID - theta/2.0);
      frameXmin = sec_center_X - frameL/2.0;
      frameXmax = sec_center_X + frameL/2.0;
      frameYmin = sec_center_Y - frameL/2.0;
      frameYmax = sec_center_Y + frameL/2.0;
    }
    else{
      std::cout<<"ERROR --> secID < 0 || secID > 6"<<std::endl
	       <<"secID "<<secID<<std::endl;
      assert(0);
    }
  }
  ///////////////////////////////////////////////////////////////
  
  TH2F *frame = new TH2F( "h2", "h2", 40, frameXmin, frameXmax,40, frameYmin, frameYmax);
  frame->GetXaxis()->SetTitle("x (cm)");
  frame->GetYaxis()->SetTitle("y (cm)");
  frame->GetXaxis()->CenterTitle();
  frame->GetYaxis()->CenterTitle();
  frame->GetYaxis()->SetTitleOffset(1.5);
  frame->SetStats(kFALSE);
  frame->SetTitle(frameTitle.Data());
  frame->Draw();
  //img->Draw("same");
  ///////////////////////////////////////////////////////////////
  SetStats(kFALSE);
  GetXaxis()->SetTitle("x (cm)");
  GetYaxis()->SetTitle("y (cm)");
  //GetXaxis()->SetNdivisions(510);
  GetXaxis()->CenterTitle();
  GetYaxis()->CenterTitle();
  GetYaxis()->SetTitleOffset(1.5);
  //std::cout<<"GetXaxis()->GetNdivisions() "<<GetXaxis()->GetNdivisions()<<std::endl;
  //SetLineStyle(1);
  //SetLineWidth(3.0);
  GetZaxis()->SetRangeUser(ztMin, ztMax);
  SetMarkerSize(0.6);
  Draw(opt.Data());
  //
  if(lineDrawOpt == "drawLine"){
    //TLine *ln_0 = new TLine( frameXmin, 0, frameXmax, 0);
    //TLine *ln_1 = new TLine( frameYmin/TMath::Tan(60.0*TMath::Pi()/180.0), frameYmin, frameYmax/TMath::Tan(60.0*TMath::Pi()/180.0), frameYmax);
    //TLine *ln_2 = new TLine( frameYmin/TMath::Tan(120.0*TMath::Pi()/180.0), frameYmin, frameYmax/TMath::Tan(120.0*TMath::Pi()/180.0), frameYmax);
    //ln_0->SetLineWidth(3.0);
    //ln_1->SetLineWidth(3.0);
    //ln_2->SetLineWidth(3.0);
    //ln_0->Draw("same");
    //ln_1->Draw("same");
    //ln_2->Draw("same");
    //    
    /////////////////////////////////
    vector<TGraph*> m_additionalGraphV;
    //../arich/data/ARICH-Detectors.xml
    //<outerRadius desc="Outer radius of support plate" unit="mm">1132</outerRadius>
    //<innerRadius desc="Inner radius of support plate" unit="mm">435</innerRadius>    
    double envelopeR_min = 43.5;
    double envelopeR_max = 113.2;
    const Int_t nEnvelope = 500;
    Double_t xEnvelope_min[nEnvelope];
    Double_t yEnvelope_min[nEnvelope];
    Double_t xEnvelope_max[nEnvelope];
    Double_t yEnvelope_max[nEnvelope];
    for(int ii = 0; ii<nEnvelope;ii++){
      TVector2 vmin(envelopeR_min,0);
      TVector2 vminrot;
      vminrot = vmin.Rotate(360.0/(nEnvelope - 1)*ii*TMath::Pi()/180.0);
      TVector2 vmax(envelopeR_max,0);
      TVector2 vmaxrot;
      vmaxrot = vmax.Rotate(360.0/(nEnvelope - 1)*ii*TMath::Pi()/180.0);
      xEnvelope_min[ii] = vminrot.X();
      yEnvelope_min[ii] = vminrot.Y();
      xEnvelope_max[ii] = vmaxrot.X();
      yEnvelope_max[ii] = vmaxrot.Y();
    }
    TGraph *gr_envelopeR_min = new TGraph(nEnvelope,xEnvelope_min,yEnvelope_min);
    m_additionalGraphV.push_back(gr_envelopeR_min);
    TGraph *gr_envelopeR_max = new TGraph(nEnvelope,xEnvelope_max,yEnvelope_max);
    m_additionalGraphV.push_back(gr_envelopeR_max);  
    for (unsigned i=0; i<m_additionalGraphV.size(); i++){
      m_additionalGraphV.at(i)->SetLineWidth(2);
      m_additionalGraphV.at(i)->SetLineColor(kBlack);
      m_additionalGraphV.at(i)->Draw("same");
    }
    /////////////////////////////////
    //    
    TVector2 vmin_0(envelopeR_min,0);
    TVector2 vmax_0(envelopeR_max,0);
    TVector2 vminrot_0;
    TVector2 vmaxrot_0;
    //    
    // 0
    vminrot_0 = vmin_0.Rotate(60.0*0*TMath::Pi()/180.0);
    vmaxrot_0 = vmax_0.Rotate(60.0*0*TMath::Pi()/180.0);
    TLine *ln_0 = new TLine( vminrot_0.X(), vminrot_0.Y(), vmaxrot_0.X(), vmaxrot_0.Y());
    // 1
    vminrot_0 = vmin_0.Rotate(60.0*1.0*TMath::Pi()/180.0);
    vmaxrot_0 = vmax_0.Rotate(60.0*1.0*TMath::Pi()/180.0);
    TLine *ln_1 = new TLine( vminrot_0.X(), vminrot_0.Y(), vmaxrot_0.X(), vmaxrot_0.Y());
    // 2
    vminrot_0 = vmin_0.Rotate(60.0*2.0*TMath::Pi()/180.0);
    vmaxrot_0 = vmax_0.Rotate(60.0*2.0*TMath::Pi()/180.0);
    TLine *ln_2 = new TLine( vminrot_0.X(), vminrot_0.Y(), vmaxrot_0.X(), vmaxrot_0.Y());
    // 3
    vminrot_0 = vmin_0.Rotate(60.0*3.0*TMath::Pi()/180.0);
    vmaxrot_0 = vmax_0.Rotate(60.0*3.0*TMath::Pi()/180.0);
    TLine *ln_3 = new TLine( vminrot_0.X(), vminrot_0.Y(), vmaxrot_0.X(), vmaxrot_0.Y());
    // 4
    vminrot_0 = vmin_0.Rotate(60.0*4.0*TMath::Pi()/180.0);
    vmaxrot_0 = vmax_0.Rotate(60.0*4.0*TMath::Pi()/180.0);
    TLine *ln_4 = new TLine( vminrot_0.X(), vminrot_0.Y(), vmaxrot_0.X(), vmaxrot_0.Y());
    // 5
    vminrot_0 = vmin_0.Rotate(60.0*5.0*TMath::Pi()/180.0);
    vmaxrot_0 = vmax_0.Rotate(60.0*5.0*TMath::Pi()/180.0);
    TLine *ln_5 = new TLine( vminrot_0.X(), vminrot_0.Y(), vmaxrot_0.X(), vmaxrot_0.Y());
    ln_0->SetLineWidth(2.0);
    ln_1->SetLineWidth(2.0);
    ln_2->SetLineWidth(2.0);
    ln_3->SetLineWidth(2.0);
    ln_4->SetLineWidth(2.0);
    ln_5->SetLineWidth(2.0);
    ln_0->Draw("same");
    ln_1->Draw("same");
    ln_2->Draw("same");
    ln_3->Draw("same");
    ln_4->Draw("same");
    ln_5->Draw("same");
  }
  //
  for (unsigned i=0; i<m_verticesGraphV.size(); i++){
    m_verticesGraphV[i].SetLineWidth(2);
    m_verticesGraphV[i].SetLineColor(kRed);
    m_verticesGraphV[i].Draw("same");
  }
  //
  c1->Modified();
  c1->Update();
  if( pdfOutFileName.Length() )
    c1->SaveAs(pdfOutFileName.Data());  
}

void ARICHChannelHist::fillBin(unsigned hapdID, unsigned chID, double weight){
  unsigned chIndex = 0;
  if (m_type == 0) chIndex = (m_hapd2binMap[hapdID - 1] - 1) * 144 + chID + 1;
  if (m_type == 2) chIndex = (m_hapd2binMap[hapdID - 1] - 1) * 4 + chID + 1;
  SetBinContent(chIndex, GetBinContent(chIndex) + weight);
}

void ARICHChannelHist::setBinContent(unsigned hapdID, unsigned chID, double value){
  unsigned chIndex = 0;
  if (m_type == 0) chIndex = (m_hapd2binMap[hapdID - 1] - 1) * 144 + chID + 1;
  if (m_type == 2) chIndex = (m_hapd2binMap[hapdID - 1] - 1) * 4 + chID + 1;
  SetBinContent(chIndex, value);
}

void ARICHChannelHist::setBinContent(unsigned hapdID, double value){
  SetBinContent(hapdID, value);
}

void ARICHChannelHist::fillBin(unsigned hapdID){
  SetBinContent(hapdID, GetBinContent(hapdID) + 1);
}
