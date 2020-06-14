/***********************************************************************
* Copyright(C) 2017 - LBS - (Single person developer.)                 *
* Mon Apr 23 00:04:26 JST 2018                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "ARICHMergerHist.h"
#include "libarihvmonitor.h"
#include "mergerStruct.h"

//c, c++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <math.h>
#include <vector>

//root
#include <TVector2.h>
#include <TPolyLine.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TText.h>
#include <TMath.h>
#include <TH2F.h>
#include <TLegend.h>
#include <TCrown.h>
#include <TArc.h>
#include <TROOT.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TPad.h>
#include <TString.h>
#include <TFile.h>
#include <TAxis.h>
#include <TVector2.h>
#include <TImage.h>

using namespace std;

ARICHMergerHist::ARICHMergerHist(const char* name, const char* title, TString fnameInfo) : TH2Poly()
{
  m_histName = name;
  m_histTitle = title;
  SetName(m_histName.Data());
  SetTitle(m_histTitle.Data());
  m_fnameInfo = fnameInfo;
  SetInitialParametersByDefault();
  SetUpVerticesMap();

  if(m_verboseLevel>0)
    dumpVerticesMap();

  setUpARICHMergerHist();  
}

ARICHMergerHist::ARICHMergerHist(const char* name, const char* title) : TH2Poly()
{
  m_histName = name;
  m_histTitle = title;
  SetName(m_histName.Data());
  SetTitle(m_histTitle.Data());
  m_fnameInfo = "./merger_positions.dat";
  SetInitialParametersByDefault();
  SetUpVerticesMap();

  if(m_verboseLevel>0)
    dumpVerticesMap();

  setUpARICHMergerHist();  
}

ARICHMergerHist::~ARICHMergerHist(){
}

void ARICHMergerHist::setUpARICHMergerHist(){
  unsigned int n;
  Double_t* x; 
  Double_t* y;

  Double_t xold;
  Double_t yold;
  Double_t xnew;
  Double_t ynew;
  Double_t r;
  Double_t phi;

  for (unsigned i=0; i<m_merStrV.size(); i++){
    //cout <<i<< ' ' <<m_merStrV[i].center_radius_mm<<endl;
    r = m_merStrV[i].center_radius_mm;
    phi = m_merStrV[i].center_rotation_deg;
    n = m_verticesMap.size();  
    x = new Double_t [n];
    y = new Double_t [n];
    TGraph gr;
    for (unsigned j=0; j < n; j++){
      //cout<<'  '<<m_verticesMap[j].X()<<'  '<<m_verticesMap[j].Y()<<endl;
      xold = m_verticesMap[j].X();
      yold = m_verticesMap[j].Y();
      makeRotation( xold, yold, xnew, ynew, r, phi);
      makeRotation( xold, yold, xnew, ynew, r, phi);
      x[j] = xnew;
      y[j] = ynew;
      gr.SetPoint(gr.GetN(),xnew,ynew);
    }
    AddBin(n,x,y);
    m_verticesGraphV.push_back(gr);
    int mergerSlotNumber = m_merStrV[i].MergerSN + (m_merStrV[i].Sector - 1)*12.0;
    m_mergerSlotNumber.push_back(mergerSlotNumber);
    //cout<<"GetNcells() "<<GetNcells()<<endl;
    //SetBinContent(GetNcells(),m_merStrV[i].Sector);
    //SetBinContent(GetNcells(),mergerSlotNumber);
    //SetBinContent(GetNcells(),GetNcells());
    delete x;
    delete y;
  }
}

void ARICHMergerHist::makeRotation( Double_t xold, Double_t yold, Double_t &xnew, Double_t &ynew, Double_t r, Double_t phi){
  TVector2 v(xold+r,yold);
  TVector2 vrot;
  vrot = v.Rotate(phi/180.0*TMath::Pi());
  xnew = vrot.X();
  ynew = vrot.Y();
}

Int_t ARICHMergerHist::GetBinIDFromMergerSlotNumber( Int_t mergerSlotNumber){
  for (unsigned i=0; i<m_mergerSlotNumber.size(); i++)
    if(m_mergerSlotNumber[i] == mergerSlotNumber)
      return i+1;
  return -999;
}

Int_t ARICHMergerHist::GetMergerSlotNumberFromBinID( Int_t binID){
  unsigned int ubinID = binID;
  if(ubinID>=1 && ubinID<=m_mergerSlotNumber.size())
    return m_mergerSlotNumber[ubinID-1];
  return -999;
}

void ARICHMergerHist::DrawHisto( TString opt = "ZCOLOR text same", TString pdfOutFileName = "./out.pdf", TString frameTitle = "", TString lineDrawOpt = "drawLine", Double_t ztMin = 0, Double_t ztMax = 100){
  gStyle->SetPalette(kRainBow);
  //TImage *img = TImage::Open("./merger.png");
  //img->SetConstRatio(kFALSE);
  TCanvas *c1 = new TCanvas("c1","c1",1000,1000);
  c1->SetTitle(m_histTitle.Data());
  c1->SetName(m_histName.Data());
  c1->SetRightMargin(0.12);
  c1->SetLeftMargin(0.12);
  c1->SetTopMargin(0.1);
  c1->SetBottomMargin(0.15);
  ///////////////////////////////////////////////////////////////
  //Double_t frameXmin = -1200;
  //Double_t frameXmax =  1200;
  //Double_t frameYmin = -1200;
  //Double_t frameYmax =  1200;

  //Double_t frameXmin = -1140;
  //Double_t frameXmax =  1140;
  //Double_t frameYmin = -1140;
  //Double_t frameYmax =  1140;

  Double_t frameXmin = -1140;
  Double_t frameXmax =  1140;
  Double_t frameYmin = -1140;
  Double_t frameYmax =  1140;

  /*
  Double_t frameXmin =  120;
  Double_t frameXmax =  1140;
  Double_t frameYmin = -1000;
  Double_t frameYmax =  20;
  */
  
  TH2F *frame = new TH2F( "h2", "h2", 40, frameXmin, frameXmax,40, frameYmin, frameYmax);
  frame->GetXaxis()->SetTitle("x (mm)");
  frame->GetYaxis()->SetTitle("y (mm)");
  frame->GetXaxis()->CenterTitle();
  frame->GetYaxis()->CenterTitle();
  frame->GetYaxis()->SetTitleOffset(1.5);
  frame->SetStats(kFALSE);
  frame->SetTitle(frameTitle.Data());
  frame->Draw();
  //img->Draw("same");
  ///////////////////////////////////////////////////////////////
  SetStats(kFALSE);
  GetXaxis()->SetTitle("x (mm)");
  GetYaxis()->SetTitle("y (mm)");
  GetXaxis()->CenterTitle();
  GetYaxis()->CenterTitle();
  GetYaxis()->SetTitleOffset(1.5);
  //SetLineStyle(1);
  //SetLineWidth(3.0);
  GetZaxis()->SetRangeUser(ztMin, ztMax);
  Draw(opt.Data());  

  if(lineDrawOpt == "drawLine"){
    cout<<"lineDrawOpt = "<<lineDrawOpt<<endl;
    //TLine *ln_0 = new TLine( frameXmin, 0, frameXmax, 0);
    //TLine *ln_1 = new TLine( frameYmin/TMath::Tan(60.0*TMath::Pi()/180.0), frameYmin, frameYmax/TMath::Tan(60.0*TMath::Pi()/180.0), frameYmax);
    //TLine *ln_2 = new TLine( frameYmin/TMath::Tan(120.0*TMath::Pi()/180.0), frameYmin, frameYmax/TMath::Tan(120.0*TMath::Pi()/180.0), frameYmax);
    //ln_0->SetLineWidth(3.0);
    //ln_1->SetLineWidth(3.0);
    //ln_2->SetLineWidth(3.0);
    //ln_0->Draw("same");
    //ln_1->Draw("same");
    //ln_2->Draw("same");
    
    /////////////////////////////////
    vector<TGraph*> m_additionalGraphV;
    //../arich/data/ARICH-Detectors.xml
    //<outerRadius desc="Outer radius of support plate" unit="mm">1132</outerRadius>
    //<innerRadius desc="Inner radius of support plate" unit="mm">435</innerRadius>    
    double envelopeR_min = 435;
    double envelopeR_max = 1132;
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
      m_additionalGraphV.at(i)->SetLineWidth(3);
      m_additionalGraphV.at(i)->SetLineColor(kBlack);
      m_additionalGraphV.at(i)->Draw("same");
    }
    /////////////////////////////////
    
    TVector2 vmin_0(envelopeR_min,0);
    TVector2 vmax_0(envelopeR_max,0);
    TVector2 vminrot_0;
    TVector2 vmaxrot_0;
    
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
    ln_0->SetLineWidth(3.0);
    ln_1->SetLineWidth(3.0);
    ln_2->SetLineWidth(3.0);
    ln_3->SetLineWidth(3.0);
    ln_4->SetLineWidth(3.0);
    ln_5->SetLineWidth(3.0);
    ln_0->Draw("same");
    ln_1->Draw("same");
    ln_2->Draw("same");
    ln_3->Draw("same");
    ln_4->Draw("same");
    ln_5->Draw("same");
  }

  /*
  for (unsigned i=0; i<m_verticesGraphV.size(); i++){
    m_verticesGraphV[i].SetLineWidth(3);
    m_verticesGraphV[i].SetLineColor(kRed);
    m_verticesGraphV[i].Draw("same");
  }
  */
  
  c1->Modified();
  c1->Update();
  if( pdfOutFileName.Length() > 0)
    c1->SaveAs(pdfOutFileName.Data());
  
}

void ARICHMergerHist::dumpVerticesMap(){
  for (unsigned i=0; i<m_verticesMap.size(); i++)
    cout << ' ' <<m_verticesMap[i].X() << ' ' <<m_verticesMap[i].Y()<<endl;
}

void ARICHMergerHist::SetUpVerticesMap(){
  //Starting from the left bottom corner and continue clockwise 
  TVector2 v1( -m_Board_size_w/2.0, -m_Board_size_l/2.0);
  TVector2 v2( -m_Board_size_w/2.0,  m_Board_size_l/2.0);
  TVector2 v3(  m_Board_size_w/2.0,  m_Board_size_l/2.0);
  TVector2 v4(  m_Board_size_w/2.0, -m_Board_size_l/2.0);
  TVector2 v5( -m_Board_size_w/2.0, -m_Board_size_l/2.0);
  m_verticesMap.push_back(v1);
  m_verticesMap.push_back(v2);
  m_verticesMap.push_back(v3);
  m_verticesMap.push_back(v4);
  m_verticesMap.push_back(v5);
}

void ARICHMergerHist::SetInitialParametersByDefault(){
  float size_l;
  float size_w;
  int nMerger;
  m_merStrV = read_merger_positions( m_fnameInfo, size_l, size_w, nMerger);
  m_Board_size_l = size_l;
  m_Board_size_w = size_w;
  m_verboseLevel = 0;
}

std::vector<mergerStruct> ARICHMergerHist::read_merger_positions( TString fnameInfo, float &size_l, float &size_w, int &nMerger){
  Int_t verboseLevel = 0;
  if(verboseLevel>0)
    std::cout<<"fnameInfo "<<fnameInfo<<std::endl;
  std::vector<mergerStruct> merStrV;
  //Int_t i = 0;
  // --> READING FILE <--
  int nMergerii = 0;
  int Sector;
  int MergerSN;
  float center_radius_mm;
  float center_rotation_deg;
  float Board_Rotation_deg;
  float Board_size_l = 0.0;
  float Board_size_w = 0.0;
  std::string mot;
  std::ifstream myfile(fnameInfo.Data());
  if(myfile.is_open()){
    myfile>>mot;//Board_size
    myfile>>Board_size_w;
    myfile>>Board_size_l;
    size_w = Board_size_w;
    size_l = Board_size_l;
    myfile>>mot;//mm2
    myfile>>mot>>mot>>mot>>mot>>mot; //Sector  MergerSN        center_radius_mm        center_rotation_deg     Board_Rotation_deg
    if(verboseLevel>1)
      std::cout<<std::setw(10)<<"Sector"
	       <<std::setw(10)<<"MergerSN"
	       <<std::setw(10)<<"center_radius_mm"
	       <<std::setw(10)<<"center_rotation_deg"
	       <<std::setw(10)<<"Board_Rotation_deg"<<std::endl;      
    while(myfile>>Sector>>MergerSN>>center_radius_mm>>center_rotation_deg>>Board_Rotation_deg){
      mergerStruct merStr;
      merStr.Sector = Sector;
      merStr.MergerSN = MergerSN;
      merStr.center_radius_mm = center_radius_mm;
      merStr.center_rotation_deg = center_rotation_deg;
      merStr.Board_Rotation_deg = Board_Rotation_deg;
      merStrV.push_back(merStr);
      nMergerii++;
      if(verboseLevel>1)
	std::cout<<std::setw(25)<<Sector
		 <<std::setw(25)<<MergerSN
		 <<std::setw(25)<<center_radius_mm
		 <<std::setw(25)<<center_rotation_deg
		 <<std::setw(25)<<Board_Rotation_deg<<std::endl;
      //std::cout<<Board_Rotation_deg - center_rotation_deg<<std::endl;
    }
    nMerger = nMergerii;
    myfile.close();
  } 
  else{
    std::cout << "Unable to open file"; 
  }
  if(verboseLevel>0){
    std::cout<<"Merger slot number : "<<std::endl;
    for (unsigned i=0; i<merStrV.size(); i++){
      std::cout<<std::setw(8)<<merStrV[i].MergerSN + 12*(merStrV[i].Sector - 1);
    }
    std::cout<<std::endl;
    std::cout<<"Distance from the center in mm : "<<std::endl;
    for (unsigned i=0; i<merStrV.size(); i++){
      std::cout<<std::setw(8)<<merStrV[i].center_radius_mm;
      /*
	std::cout<<"<mergerSlotID slotID='"
	<<(merStrV[i].MergerSN + 12*(merStrV[i].Sector - 1))<<"'>"
	<<merStrV[i].center_radius_mm<<"</mergerSlotID>"<<std::endl;	
      */
    }
    std::cout<<std::endl;
    std::cout<<"Azimuthal angle of the merger PCB center in polar coordinate system in deg"<<std::endl;
    for (unsigned i=0; i<merStrV.size(); i++){
      std::cout<<std::setw(8)<<merStrV[i].center_rotation_deg;	
      /*
	std::cout<<"<mergerSlotID slotID='"
	<<(merStrV[i].MergerSN + 12*(merStrV[i].Sector - 1))<<"'>"
	<<merStrV[i].center_rotation_deg<<"</mergerSlotID>"<<std::endl;	
      */
    }
    std::cout<<std::endl;
  }
  return merStrV;
}
