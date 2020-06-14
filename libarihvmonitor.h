#pragma once

//my
#include "ARIconst.h"
#include "ARICHChannelHist.h"
#include "ARICHmappingCopperMergerFeb.h"
#include "ARICHMergerHist.h"
#include "ARICHSectorHist.h"
#include "crateToModuleMapStr.h"
#include "hvBiasRootFileStr.h"

//root
#include <TROOT.h>
#include <TStyle.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TString.h>
#include <TFile.h>
#include <TGraph.h>
#include <TAxis.h>
#include "TXMLEngine.h"
#include "TH2Poly.h"
#include "TLine.h"
#include "TObject.h"

//c, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <vector>

using namespace std;

//convert_hvbias_params.cc
std::vector<crateToModuleMapStr> read_crateToModuleMap(TString crateToModuleMapFile);
void convert_hvbias_params(Int_t secID,
			   Int_t unixTime,
			   TString dateMeas,
			   TString crateToModuleMap,
			   TString parsufix,
			   TString outRootFile,
			   const std::vector<int>& crate_v,
			   const std::vector<int>& slot_v,
			   const std::vector<int>& channel_v,
			   const std::vector<float>& val_v);

//merge_Root_hvbias_params.cc
std::vector<hvBiasRootFileStr> read_hvBiasRootFileList(TString infileName);
void merge_Root_hvbias_params( TString inList, TString inDataDir, TString outRootFileDir, TString hapdTemplateMappingPath);
void bubbleSort(std::vector<hvBiasRootFileStr> &mstr_v);
void combineMeasurements(std::vector<hvBiasRootFileStr> &mstr_v,
			 TString inDataDir,
			 ARICHChannelHist *hp_sum_chips, ARICHChannelHist *hp_sum_guard,
			 ARICHChannelHist *hp_sum_chips_norm, ARICHChannelHist *hp_sum_guard_norm,
			 ARICHChannelHist *hp_sum_chips_aver, ARICHChannelHist *hp_sum_guard_aver);
TString getLocalTimeStringFromUnixTime( int ut );
void getMinMaxAveTGraphs( std::vector<hvBiasRootFileStr> &mstr_v, TString inDataDir, TString hapdTemplateMappingPath,
			  std::vector<std::vector<TGraph*>> grMin_v, std::vector<std::vector<TGraph*>> grMax_v, std::vector<std::vector<TGraph*>> grAve_v,
			  std::vector<TGraph*> grMin_guard_v, std::vector<TGraph*> grMax_guard_v, std::vector<TGraph*> grAve_guard_v,
			  std::vector<std::vector<TH1D*>> h1Val_v, std::vector<TH1D*>h1Val_guard_v);

//plotsSector.cc
Int_t plotsSector( TString file_s1, TString outPdfFile, Int_t secID, Double_t vmin, Double_t vmax, TString hapdTemplateMappingPath);

//plotsMergedData.cc
Int_t plotsMergedData( TString file_s1, TString outPdfFile, Double_t vmin, Double_t vmax, TString hapdTemplateMappingPath);

//plots.C
Int_t plots();

//testARICHChannelHist.cc
Int_t testARICHChannelHist();

//testARICHSectorHist.cc
Int_t testARICHSectorHist();

//testARICHmapping.cc
Int_t testARICHmapping();

//ARICHChannelHist.h
class ARICHChannelHist;

//ARICHMergerHist.h
class ARICHMergerHist;

//ARICHmappingCopperMergerFeb.h
class ARICHmappingCopperMergerFeb;

//ARICHSectorHist.h
class ARICHSectorHist;
