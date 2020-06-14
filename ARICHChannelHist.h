/***********************************************************************
* Copyright(C) 2017 - LBS - (Single person developer.)                 *
* Thu Oct 18 13:36:31 JST 2018                                         *
* Autor: Leonid Burmistrov, Luka Santelj                               *
***********************************************************************/

#pragma once

//root
#include <TObject.h>
#include <TH2Poly.h>
#include <TGraph.h>
#include <TVector2.h>
#include <TGraph.h>
#include <TCanvas.h>

//c, c++
#include <string>

/**
 * ARICH histogram with HAPD plane
 * 3 options for bin segmentation are available
 * type 0 - one bin per HAPD, type 1 - one bin per channel, type 2 - one bin per APD chip
 * HAPD numbering is 1-420 (software numbering), channel is ASIC channel, APD is ASIC channel / 36
 */
class ARICHChannelHist: public TH2Poly {
  
 public:
  
  /**
   * Default constructor
   */
  ARICHChannelHist() {};
  
  /**
   * Constructor with name, title, and type (0 for channel bins (144/HAPD), 1 for HAPD bins (1/HAPD))
   * @param name name
   * @param title title
   * @param type type
   */
  ARICHChannelHist(const char* name, const char* title, int type = 0);

  void DrawHisto( TString opt, TString pdfOutFileName, TString frameTitle, TString lineDrawOpt, Double_t ztMin, Double_t ztMax, TCanvas *c1, Int_t secID);

  /**
   * Add entry to bin corresponding to hapd hapdID and channel chID
   * @param hapdID ID number of HAPD module
   * @param chID channel ID (asic channel)
   * @param weight weight
   */
  void fillBin(unsigned hapdID, unsigned chID, double weight = 1.0);
  
  /**
   * Set content of bin corresponding to hapd hapdID and channel chID
   * @param hapdID ID number of HAPD module
   * @param chID channel ID (asic channel)
   * @param value bin content
   */
  void setBinContent(unsigned hapdID, unsigned chID, double value);
  
  /**
   * Add entry to bin corresponding to hapd hapdID
   * @param hapdID ID number of HAPD module
   */
  void fillBin(unsigned hapdID);
  
  /**
   * Set content of bin corresponding to hapd hapdID
   * @param hapdID ID number of HAPD module
   * @param value bin content
   */
  void setBinContent(unsigned hapdID, double value);

  const TString& getHistName() const { return m_histName; }
  const TString& getHistTitle() const { return m_histTitle; }
  const int getType() const { return m_type; }

  //static ARICHChannelHist* createFrom(ARICHChannelHist* tpoly_chHist);
  
 protected:

  std::vector<unsigned> m_hapd2binMap; /**< map of bins*/

  std::vector<TGraph> m_verticesGraphV;
  
  TString m_histName;
  TString m_histTitle;
  int m_type;
  
};
