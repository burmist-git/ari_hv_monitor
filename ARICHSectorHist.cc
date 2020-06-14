/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Tue Feb 26 23:10:44 CET 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include <ARICHSectorHist.h>

//c, c++
#include <stdio.h>
//#include <assert.h>
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

using namespace std;

ARICHSectorHist::ARICHSectorHist(const char* name, const char* title) : TH2Poly()
{

  m_histName = name;
  m_histTitle = title;
  SetName(m_histName.Data());
  SetTitle(m_histTitle.Data());
  SetInitialParametersByDefault();
  SetUpVerticesMap();

  if (m_verboseLevel > 0) {
    std::cout << "m_SectorRmin                 "<< m_SectorRmin << std::endl
	      << "m_SectorRmax                 "<< m_SectorRmax<<std::endl
	      << "m_SectorRcenter              "<< m_SectorRcenter<<std::endl
	      << "m_SectorDeltaPhiCenter       "<< m_SectorDeltaPhiCenter<<std::endl
	      << "m_SectorAriGapDeltaPhiCenter "<< m_SectorAriGapDeltaPhiCenter<<std::endl
	      << "m_verboseLevel               "<< m_verboseLevel<<std::endl
	      << "m_nCircularPoints            "<< m_nCircularPoints<<std::endl
	      << "m_SectorGap                  "<< m_SectorGap<<std::endl;
    dumpVerticesMap();
  }

  unsigned int n;
  double* x;
  double* y;

  double xold;
  double yold;
  double xnew;
  double ynew;
  double phi;

  //Add "poly bins" to the histogram
  //Loop over rings with different radiuses
  unsigned int iR = 0;
  for (auto& m : m_verticesMap) {
    //Loop aerogel tiles (bins) within one ring.
    double phi0 = m_SectorDeltaPhiCenter / 2.0;
    double deltaPhi = m_SectorDeltaPhiCenter + m_SectorAriGapDeltaPhiCenter;
    for (Int_t j = 0; j <  m_nSectors; j++) {
      phi = phi0 + deltaPhi * j;
      n = m.second.size();
      x = new double [n];
      y = new double [n];
      //Loop over polygonal points which defines bins.
      for (unsigned int i = 0; i < n; i++) {
        xold = m.second[i].X();
        yold = m.second[i].Y();
        makeRotation(xold, yold, xnew, ynew, phi);
        x[i] = xnew;
        y[i] = ynew;
      }
      AddBin(n, x, y);
      delete []x;
      delete []y;
    }
    iR++;
  }// for (auto& m: m_verticesMap) {

}

void ARICHSectorHist::makeRotation(double xold, double yold, double& xnew, double& ynew, double phi){
  TVector2 v(xold, yold);
  TVector2 vrot;
  vrot = v.Rotate(phi);
  xnew = vrot.X();
  ynew = vrot.Y();
}

void ARICHSectorHist::DrawHisto(TString opt = "ZCOLOT text same", TString outDirName = "./"){
  
  TCanvas* c1 = new TCanvas("c1", "c1", 1000, 1000);
  c1->SetTitle(m_histTitle.Data());
  c1->SetName(m_histName.Data());
  c1->SetRightMargin(0.17);
  c1->SetLeftMargin(0.12);
  c1->SetTopMargin(0.15);
  c1->SetBottomMargin(0.15);

  TH2F* frame = new TH2F("h2", "h2", 40, -1200, 1200, 40, -1200, 1200);
  //TH2F *frame = new TH2F("h2","h2",40, 400, 650, 40, -50, 200);
  frame->GetXaxis()->SetTitle("x (mm)");
  frame->GetYaxis()->SetTitle("y (mm)");
  frame->GetXaxis()->CenterTitle();
  frame->GetYaxis()->CenterTitle();
  frame->GetYaxis()->SetTitleOffset(1.5);
  frame->SetStats(kFALSE);
  frame->SetTitle("");
  frame->Draw();

  Draw(opt.Data());
  c1->Modified();
  c1->Update();
  if (outDirName.Length() > 0) {
    std::cout << "outDirName.Length() " << outDirName.Length() << std::endl;
    TString outnamePDF = outDirName; outnamePDF += m_histName; outnamePDF += ".pdf";
    TString outnameEPS = outDirName; outnameEPS += m_histName; outnameEPS += ".eps";
    c1->SaveAs(outnamePDF.Data());
    c1->SaveAs(outnameEPS.Data());
  }
}

void ARICHSectorHist::dumpVerticesMap(){
  unsigned int i;
  for (auto& m : m_verticesMap) {
    std::cout << " --> Aerogel ring : " << m.first << '\n';
    for (i = 0; i < m.second.size(); i++) {
      std::cout << "                    " << setw(15) << m.second[i].X() << setw(15) << m.second[i].Y() << std::endl;
    }
  }
}

void ARICHSectorHist::SetUpVerticesMap(){
  double dPhi = -999.0;
  std::vector<TVector2> vecTvec;
  double lmin = 2 * TMath::Pi() * m_SectorRmin / m_nSectors - m_SectorGap;
  double lmax = 2 * TMath::Pi() * m_SectorRmax / m_nSectors - m_SectorGap;
  double phimin = lmin / m_SectorRmin;
  double phimax = lmax / m_SectorRmax;
  double x1 = m_SectorRmin * TMath::Cos(phimin / 2.0);
  double y1 = m_SectorRmin * TMath::Sin(phimin / 2.0);
  TVector2 v1(x1, y1);
  vecTvec.push_back(v1);
  double x2 = m_SectorRmax * TMath::Cos(phimax / 2.0);
  double y2 = m_SectorRmax * TMath::Sin(phimax / 2.0);
  TVector2 v2(x2, y2);
  vecTvec.push_back(v2);
  //Add circular points from outer radious (clockwise added)
  if (m_nCircularPoints > 0) {
    dPhi = phimax / (m_nCircularPoints + 1);
    for (int j = 0; j < m_nCircularPoints; j++) {
      TVector2 v;
      v.SetMagPhi(m_SectorRmax, (phimax / 2.0 - dPhi * (j + 1)));
      vecTvec.push_back(v);
    }
  }  
  double x3 =  x2;
  double y3 = -y2;
  TVector2 v3(x3, y3);
  vecTvec.push_back(v3);
  double x4 =  x1;
  double y4 = -y1;
  TVector2 v4(x4, y4);
  vecTvec.push_back(v4);
  //Add circular points inner radious
  if (m_nCircularPoints > 0) {
    dPhi = phimin / (m_nCircularPoints + 1);
    for (int j = 0; j < m_nCircularPoints; j++) {
      TVector2 v;
      v.SetMagPhi(m_SectorRmin, (-phimax / 2.0 + dPhi * (j + 1)));
      vecTvec.push_back(v);
    }
  }  
  vecTvec.push_back(v1);
  m_verticesMap[0] = vecTvec;  
}

void ARICHSectorHist::SetInitialParametersByDefault() {
  //Verbose level
  m_verboseLevel = 0;
  //Distance between sectors
  m_SectorGap = 20;
  //Number of circular points
  m_nCircularPoints = 100;
  //Minimum radius of sector
  m_SectorRmin = 441.0;
  //Maximum radius of sector
  m_SectorRmax = 1117.0;
  m_SectorRcenter = (m_SectorRmax + m_SectorRmin)/2.0;
  double l = 2 * TMath::Pi() * m_SectorRcenter / m_nSectors - m_SectorGap;
  double phi = l / m_SectorRcenter;
  m_SectorDeltaPhiCenter = phi;
  m_SectorAriGapDeltaPhiCenter = m_SectorGap / m_SectorRcenter;
}
