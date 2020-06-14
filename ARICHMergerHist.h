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

struct mergerStruct;

class ARICHMergerHist: public TH2Poly {
 public:
  
  ARICHMergerHist(const char* name, const char* title);
  ARICHMergerHist(const char* name, const char* title, TString fnameInfo);
  ~ARICHMergerHist();
  void DrawHisto( TString opt, TString pdfOutFileName, TString frameTitle, TString lineDrawOpt, Double_t ztMin, Double_t ztMax);  
  Int_t GetBinIDFromMergerSlotNumber( Int_t mergerSlotNumber);
  Int_t GetMergerSlotNumberFromBinID( Int_t binID);

 private:

  void setUpARICHMergerHist();
  
  void SetInitialParametersByDefault();

  void SetUpVerticesMap();

  void dumpVerticesMap();

  void makeRotation( Double_t xold, Double_t yold, Double_t &xnew, Double_t &ynew, Double_t r, Double_t phi);

  std::vector<mergerStruct> read_merger_positions( TString fnameInfo, float &size_l, float &size_w, int &nMerger);
  
  //Vector of tructures which contains merger positions.
  std::vector<mergerStruct> m_merStrV;
  //Vector mergerSlotNumber m_mergerSlotNumber[binID] = mergerSlotNumber.
  std::vector<int> m_mergerSlotNumber;
  //Length of the merger board
  float m_Board_size_l;
  //Width of the merger board
  float m_Board_size_w;
  //Verbose level
  Int_t m_verboseLevel;
  //Meger vertices map
  std::vector<TVector2> m_verticesMap;
  //Meger vertices map
  std::vector<TGraph> m_verticesGraphV;

  TString m_histName;
  TString m_histTitle;
  TString m_fnameInfo; //merger_positions.dat

};
