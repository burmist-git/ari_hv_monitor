/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Tue Feb 26 23:10:44 CET 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/
#pragma once

//root
#include <TObject.h>
#include <TH2Poly.h>
#include <TGraph.h>
#include <TVector2.h>

//c, c++
#include <string>
#include <vector>
#include <map>

class ARICHSectorHist: public TH2Poly {
 public:

  ARICHSectorHist() {};
  ~ARICHSectorHist() {};
  ARICHSectorHist(const char* name, const char* title);
  void DrawHisto(TString opt, TString outDirName);

 protected:

  void SetInitialParametersByDefault();
  void SetUpVerticesMap();
  void dumpVerticesMap();
  void makeRotation(double xold, double yold, double& xnew, double& ynew, double phi);

  const int m_nSectors = 6;
  double m_SectorRmin;
  double m_SectorRmax;
  double m_SectorRcenter;
  double m_SectorDeltaPhiCenter;
  double m_SectorAriGapDeltaPhiCenter;
  Int_t m_verboseLevel;
  Int_t m_nCircularPoints;
  double m_SectorGap;

  std::map<Int_t, std::vector<TVector2>> m_verticesMap;

  TString m_histName;
  TString m_histTitle;

};
