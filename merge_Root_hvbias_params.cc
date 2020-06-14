/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Fri Oct 25 11:13:17 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "libarihvmonitor.h"
#include "hvBiasRootFileStr.h"

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
#include <time.h>

//root
#include <TVector2.h>
#include <TPolyLine.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TText.h>
#include <TMath.h>
#include <TH2F.h>
#include <TH1D.h>
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

//1 climit 1571633519 sec_1_21.10.2019-13.51.59-1571633519.climit.root
//1 climit 1571634121 sec_1_21.10.2019-14.02.01-1571634121.climit.root
//1 climit 1571634714 sec_1_21.10.2019-14.11.54-1571634714.climit.root
std::vector<hvBiasRootFileStr> read_hvBiasRootFileList(TString infileName) {
  ////////////////
  Int_t secID;
  string measType;
  Int_t ut;
  string fname;
  ////////////////
  std::vector<hvBiasRootFileStr> hvBiasRootFileStr_v;
  ifstream infile (infileName.Data());
  if( infile.is_open() ) {
    while( infile >> secID >> measType >> ut >> fname ) {
      hvBiasRootFileStr vhBRfileStr;
      vhBRfileStr.secID = secID;
      vhBRfileStr.measType = measType;
      vhBRfileStr.ut = ut;
      vhBRfileStr.fname = fname;
      hvBiasRootFileStr_v.push_back(vhBRfileStr);
    }
  }
  return hvBiasRootFileStr_v;
}

void merge_Root_hvbias_params( TString inList, TString inDataDir, TString outRootFileDir, TString hapdTemplateMappingPath) {
  std::vector<hvBiasRootFileStr> hvBiasRootF_v = read_hvBiasRootFileList(inList);
  //for(unsigned int i = 0; i < hvBiasRootF_v.size(); i++)
  //hvBiasRootF_v.at(i).print();
  bubbleSort(hvBiasRootF_v);
  std::vector<ARICHChannelHist*> arichBiasInfoChipsHistos_v;
  std::vector<ARICHChannelHist*> arichBiasInfoGuardHistos_v;
  std::vector<std::vector<hvBiasRootFileStr>> arichBiasInfo_v;
  //cout<<" "<<endl;
  TH1D *h1_dut = new TH1D("h1_dut","dut",1000,0.0,1000);
  TH1D *h1_secID = new TH1D("h1_secID","secID",8,-0.5,7.5);
  for(unsigned int i = 1; i < hvBiasRootF_v.size(); i++){
    h1_dut->Fill(hvBiasRootF_v.at(i).ut - hvBiasRootF_v.at(i-1).ut);
  }
  for( unsigned int i = 0; i < hvBiasRootF_v.size(); i++){
    h1_secID->Fill(hvBiasRootF_v.at(i).secID);
    //hvBiasRootF_v.at(i).print();
  }
  int sedIDadd[6];
  int n_combineMeasurements = 0;
  for( unsigned int i = 0; i < hvBiasRootF_v.size(); i++){
    std::vector<hvBiasRootFileStr> hvBiasRootFadd_v;
    //nFileToAdd = 0;
    for( unsigned int j = 0; j < 6; j++){
      if( i < hvBiasRootF_v.size()){
	if(j == 0){
	  for(unsigned int k = 0 ; k < 6 ; k++)
	    sedIDadd[k] = 0;
	  hvBiasRootFadd_v.push_back(hvBiasRootF_v.at(i));
	  sedIDadd[hvBiasRootF_v.at(i).secID-1] = 1;
	}
	else if( (hvBiasRootF_v.at(i).ut - hvBiasRootFadd_v.at(0).ut) < 10.0*60.0
		 && sedIDadd[hvBiasRootF_v.at(i).secID-1] != 1 ){
	  sedIDadd[hvBiasRootF_v.at(i).secID-1] = 1;
	  hvBiasRootFadd_v.push_back(hvBiasRootF_v.at(i));
	}
	i++;
      }
    }
    //
    TString hp_sum_chips_name = "hp_sum_chips_name_"; hp_sum_chips_name += n_combineMeasurements;
    TString hp_sum_guard_name = "hp_sum_guard_name_"; hp_sum_guard_name += n_combineMeasurements;
    //
    ARICHChannelHist *hp_sum_chips = new ARICHChannelHist(hp_sum_chips_name.Data(),"",2);
    ARICHChannelHist *hp_sum_chips_norm = new ARICHChannelHist("tmp","",2);
    ARICHChannelHist *hp_sum_chips_aver = new ARICHChannelHist("tmp","",2);
    //
    ARICHChannelHist *hp_sum_guard = new ARICHChannelHist(hp_sum_guard_name.Data(),"",1);
    ARICHChannelHist *hp_sum_guard_norm = new ARICHChannelHist("tmp","",1);
    ARICHChannelHist *hp_sum_guard_aver = new ARICHChannelHist("tmp","",1);
    //
    combineMeasurements( hvBiasRootFadd_v, inDataDir,
			 hp_sum_chips, hp_sum_guard,
			 hp_sum_chips_norm, hp_sum_guard_norm,
			 hp_sum_chips_aver, hp_sum_guard_aver);
    arichBiasInfoChipsHistos_v.push_back(hp_sum_chips);
    arichBiasInfoGuardHistos_v.push_back(hp_sum_guard);
    arichBiasInfo_v.push_back(hvBiasRootFadd_v);
    n_combineMeasurements++;
    //
    delete hp_sum_chips_norm;
    delete hp_sum_chips_aver;
    delete hp_sum_guard_norm;
    delete hp_sum_guard_aver;
    //std::cout<<"--------------------"<<std::endl;
    //for(unsigned int ll = 0 ; ll < hvBiasRootFadd_v.size() ; ll++)
    //hvBiasRootFadd_v.at(ll).print();
    //for(unsigned int ll = 0 ; ll < hvBiasRootFadd_v.size() ; ll++){
    //std::cout<<std::setw(3)<<hvBiasRootFadd_v.at(ll).secID
    //	       <<std::setw(10)<<hvBiasRootFadd_v.at(ll).measType
    //	       <<std::setw(20)<<hvBiasRootFadd_v.at(ll).ut
    //	       <<std::setw(60)<<hvBiasRootFadd_v.at(ll).fname
    //	       <<std::setw(10)<<(hvBiasRootFadd_v.at(ll).ut - hvBiasRootFadd_v.at(0).ut)/60
    //	       <<std::endl;
    //<<std::setw(10)<<hvBiasRootFadd_v.at(ll).ut - hvBiasRootFadd_v.at((ll > 0 ? (ll-1) : 0)).ut
    //}
    //getMeasurements
  }
  //
  //for(unsigned int k = 0; k<arichBiasInfo_v.size();k++){
  //std::cout<<"  ----------------------------------------------------------------"<<std::endl;
  //for(unsigned int l = 0; l<arichBiasInfo_v.at(k).size();l++){
  //  arichBiasInfo_v.at(k).at(l).print();
  //}
  //}
  //
  //Average calculation
  //
  ARICHChannelHist *hp_sum_chips = new ARICHChannelHist("hp_sum_chips","",2);
  ARICHChannelHist *hp_sum_chips_norm = new ARICHChannelHist("hp_sum_chips_norm","",2);
  ARICHChannelHist *hp_sum_chips_aver = new ARICHChannelHist("hp_sum_chips_aver","",2);
  //
  ARICHChannelHist *hp_sum_guard = new ARICHChannelHist("hp_sum_guard","",1);
  ARICHChannelHist *hp_sum_guard_norm = new ARICHChannelHist("hp_sum_guard_norm","",1);
  ARICHChannelHist *hp_sum_guard_aver = new ARICHChannelHist("hp_sum_guard_aver","",1);
  //
  combineMeasurements( hvBiasRootF_v, inDataDir,
		       hp_sum_chips, hp_sum_guard,
		       hp_sum_chips_norm, hp_sum_guard_norm,
		       hp_sum_chips_aver, hp_sum_guard_aver);
  if(arichBiasInfo_v.size()<=0){
    std::cout<<"arichBiasInfo_v.size() "<<arichBiasInfo_v.size()<<std::endl;
    assert(0);
  }
  //
  //Get graphics with variation of the measured parameters with time
  //std::cout<<"ARIconst::nSectors "<<ARIconst::nSectors<<std::endl;
  std::vector<std::vector<TGraph*>> grMin_v;
  std::vector<std::vector<TGraph*>> grMax_v;
  std::vector<std::vector<TGraph*>> grAve_v;
  std::vector<TGraph*> grMin_guard_v;
  std::vector<TGraph*> grMax_guard_v;
  std::vector<TGraph*> grAve_guard_v;
  //
  std::vector<std::vector<TH1D*>> h1Val_v;
  std::vector<TH1D*> h1Val_guard_v;
  //
  for(Int_t i = 0; i<ARIconst::nSectors; i++){
    //
    TGraph *grMin_guard = new TGraph();
    TGraph *grMax_guard = new TGraph();
    TGraph *grAve_guard = new TGraph();
    grMin_guard_v.push_back(grMin_guard);
    grMax_guard_v.push_back(grMax_guard);
    grAve_guard_v.push_back(grAve_guard);
    std::vector<TGraph*> grMin_tmp_v;
    std::vector<TGraph*> grMax_tmp_v;
    std::vector<TGraph*> grAve_tmp_v;
    //
    TH1D *h1_guard = new TH1D();
    h1Val_guard_v.push_back(h1_guard);
    std::vector<TH1D*> h1_tmp_v;
    //
    for(Int_t j = 0; j<ARIconst::nHAPDchips; j++){
      TGraph *grMin = new TGraph();
      TGraph *grMax = new TGraph();
      TGraph *grAve = new TGraph();
      grMin_tmp_v.push_back(grMin);
      grMax_tmp_v.push_back(grMax);
      grAve_tmp_v.push_back(grAve);
      //
      TH1D *h1 = new TH1D();
      h1_tmp_v.push_back(h1);
    }
    grMin_v.push_back(grMin_tmp_v);
    grMax_v.push_back(grMax_tmp_v);
    grAve_v.push_back(grAve_tmp_v);
    //
    h1Val_v.push_back(h1_tmp_v);
  }
  getMinMaxAveTGraphs(hvBiasRootF_v, inDataDir, hapdTemplateMappingPath,
		      grMin_v, grMax_v, grAve_v,
		      grMin_guard_v, grMax_guard_v, grAve_guard_v,
		      h1Val_v, h1Val_guard_v);
  //
  TString outNameShort = arichBiasInfo_v.at(0).at(0).measType; outNameShort += "_";
  TString utStr_first = getLocalTimeStringFromUnixTime(arichBiasInfo_v.at(0).at(0).ut);
  TString utStr_last = getLocalTimeStringFromUnixTime(arichBiasInfo_v.at(arichBiasInfo_v.size()-1).at(arichBiasInfo_v.at(arichBiasInfo_v.size()-1).size()-1).ut);
  outNameShort += utStr_first; outNameShort += "-"; outNameShort += utStr_last;
  outNameShort += ".root";
  TString outRootFile = outRootFileDir; outRootFile += "/"; outRootFile += outNameShort;
  TFile* rootFile = new TFile(outRootFile.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<outRootFile.Data()<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<outRootFile.Data()<<endl;
  cout<<endl<<endl;
  TDirectory* subDall = rootFile->mkdir("all");
  subDall->cd();
  for(unsigned int k = 0; k < arichBiasInfoChipsHistos_v.size(); k++)
    arichBiasInfoChipsHistos_v.at(k)->Write();
  for(unsigned int k = 0; k < arichBiasInfoGuardHistos_v.size(); k++)
    arichBiasInfoGuardHistos_v.at(k)->Write();
  //
  hp_sum_chips->Write();
  hp_sum_chips_norm->Write();
  //
  hp_sum_guard->Write();
  hp_sum_guard_norm->Write();
  //
  for( unsigned int i = 0; i < (unsigned int)ARIconst::nSectors; i++){
    for( unsigned int j = 0; j < (unsigned int)ARIconst::nHAPDchips; j++){
      grMin_v.at(i).at(j)->Write();
      grMax_v.at(i).at(j)->Write();
      grAve_v.at(i).at(j)->Write();
      h1Val_v.at(i).at(j)->Write();
    }
    grMin_guard_v.at(i)->Write();
    grMax_guard_v.at(i)->Write();
    grAve_guard_v.at(i)->Write();
    h1Val_guard_v.at(i)->Write();
  }
  //
  rootFile->cd();
  hp_sum_chips_aver->Write();
  hp_sum_guard_aver->Write();
  //
  h1_dut->Write();
  h1_secID->Write();
  //
  rootFile->Close();
}

void getMinMaxAveFromTH2Poly(TH2Poly *hp, int secID, ARICHmappingCopperMergerFeb *arichmap, std::vector<double> &minVal_v, std::vector<double> &maxVal_v, std::vector<double> &aveVal_v, std::vector<std::vector<double>> &allVal_v){
  Int_t sec_i, cop_i, mer_i, feb_i, chi_i;
  Int_t feb_globalID;
  Int_t febChip_globalID;
  double binVal;
  sec_i = secID - 1;
  std::vector<int> nncount;
  for( chi_i = 0 ; chi_i < arichmap->GetnChipsPerHAPD() ; chi_i++){
    nncount.push_back(0);
    minVal_v.push_back(0);
    maxVal_v.push_back(0);
    aveVal_v.push_back(0);
  }
  //
  std::vector<double> tmpVal_a;
  std::vector<double> tmpVal_b;
  std::vector<double> tmpVal_c;
  std::vector<double> tmpVal_d;
  //  
  for( cop_i = 0 ; cop_i < arichmap->GetnCopperPerSector(); cop_i++){
    for( mer_i = 0 ; mer_i < arichmap->GetnMergersPerCopper(); mer_i++){
      for( feb_i = 0 ; feb_i < arichmap->GetnHAPDPerMerger() ; feb_i++){
	feb_globalID = arichmap->arichmapping.sector[sec_i].copper[cop_i].merger[mer_i].feb[feb_i].globalID;
	if(hp->GetNcells()>500){
	  for( chi_i = 0 ; chi_i < arichmap->GetnChipsPerHAPD() ; chi_i++){
	    febChip_globalID = arichmap->getGlobalBiasChipID(feb_globalID, (chi_i+1));
	    binVal = hp->GetBinContent(febChip_globalID);
	    if(chi_i == 0)
	      tmpVal_a.push_back(binVal);
	    else if(chi_i == 1)
	      tmpVal_b.push_back(binVal);
	    else if(chi_i == 2)
	      tmpVal_c.push_back(binVal);
	    else if(chi_i == 3)
	      tmpVal_d.push_back(binVal);
	    if(nncount.at(chi_i) == 0){
	      minVal_v.at(chi_i) = binVal;
	      maxVal_v.at(chi_i) = binVal;
	      //std::cout<<minVal_v.at(chi_i)<<std::endl;
	    }
	    if(binVal>maxVal_v.at(chi_i))
	      maxVal_v.at(chi_i) = binVal;
	    if(binVal<minVal_v.at(chi_i))
	      minVal_v.at(chi_i) = binVal;
	    aveVal_v.at(chi_i) += binVal;
	    nncount.at(chi_i)++;
	  }
	}
	else{
	  binVal = hp->GetBinContent(feb_globalID);	  
	  tmpVal_a.push_back(binVal);
	  if(nncount.at(0) == 0){
	    minVal_v.at(0) = binVal;
	    maxVal_v.at(0) = binVal;
	  }
	  if(binVal>maxVal_v.at(0))
	    maxVal_v.at(0) = binVal;
	  if(binVal<minVal_v.at(0))
	    minVal_v.at(0) = binVal;
	  aveVal_v.at(0) += binVal;
	  nncount.at(0)++;
	}
      }
    }
  }
  if(hp->GetNcells()>500){
    for( chi_i = 0 ; chi_i < arichmap->GetnChipsPerHAPD() ; chi_i++)
      if(nncount.at(chi_i) > 0)
	aveVal_v.at(chi_i) = aveVal_v.at(chi_i)/nncount.at(chi_i);
      else
	aveVal_v.at(chi_i) = 0.0;
    //for( chi_i = 0 ; chi_i < arichmap->GetnChipsPerHAPD() ; chi_i++)
    //std::cout<<chi_i<<" "<<minVal_v.at(chi_i)<<std::endl;
    allVal_v.push_back(tmpVal_a);
    allVal_v.push_back(tmpVal_b);
    allVal_v.push_back(tmpVal_c);
    allVal_v.push_back(tmpVal_d);
  }
  else{
    if(nncount.at(0) > 0)
      aveVal_v.at(0) = aveVal_v.at(0)/nncount.at(0);
    else
      aveVal_v.at(0) = 0.0;
    allVal_v.push_back(tmpVal_a);
  }
}

void getMinMaxAveTGraphs( std::vector<hvBiasRootFileStr> &mstr_v, TString inDataDir, TString hapdTemplateMappingPath,
			  std::vector<std::vector<TGraph*>> grMin_v, std::vector<std::vector<TGraph*>> grMax_v, std::vector<std::vector<TGraph*>> grAve_v,
			  std::vector<TGraph*> grMin_guard_v, std::vector<TGraph*> grMax_guard_v, std::vector<TGraph*> grAve_guard_v,
			  std::vector<std::vector<TH1D*>> h1Val_v, std::vector<TH1D*>h1Val_guard_v){
  //
  ARICHmappingCopperMergerFeb *arichmap = new ARICHmappingCopperMergerFeb(hapdTemplateMappingPath);
  //
  if(mstr_v.size()<=0){
    std::cout<<"mstr_v.size() "<<mstr_v.size()<<std::endl;
    assert(0);
  }
  std::vector<TFile*> f_v;
  for( unsigned int i = 0; i < mstr_v.size(); i++){
    TString ffname = inDataDir;
    ffname += mstr_v.at(i).fname;
    TFile *rootf = new TFile(ffname.Data());
    f_v.push_back(rootf);
  }
  //
  int ii_hvBiasPar = -999;
  for( int k = 0; k< ARIconst::nhvBiasPar; k++)
    if(ARIconst::hvBiasParArr[k] == mstr_v.at(0).measType)
      ii_hvBiasPar = k;
  if(ii_hvBiasPar<0)
    assert(0);
  //
  TString grName;
  TString grTitle;
  TString h1Name;
  TString h1Title;
  for( unsigned int i = 0; i<(unsigned int)ARIconst::nSectors; i++){
    for( unsigned int j = 0; j<(unsigned int)ARIconst::nHAPDchips; j++){
      //
      grName="grMin_sec_"; grName += i+1; grName += "_chip_"; grName += j+1;
      grMin_v.at(i).at(j)->SetName(grName.Data());
      grName="grMax_sec_"; grName += i+1; grName += "_chip_"; grName += j+1;
      grMax_v.at(i).at(j)->SetName(grName.Data());
      grName="grAve_sec_"; grName += i+1; grName += "_chip_"; grName += j+1;
      grAve_v.at(i).at(j)->SetName(grName.Data());
      //
      grTitle="grMin_sec_"; grTitle += i+1; grTitle += "_chip_"; grTitle += j+1;
      grMin_v.at(i).at(j)->SetTitle(grTitle.Data());
      grTitle="grMax_sec_"; grTitle += i+1; grTitle += "_chip_"; grTitle += j+1;
      grMax_v.at(i).at(j)->SetTitle(grTitle.Data());
      grTitle="grAve_sec_"; grTitle += i+1; grTitle += "_chip_"; grTitle += j+1;
      grAve_v.at(i).at(j)->SetTitle(grTitle.Data());
      //
      h1Name="h1_sec_"; h1Name += i+1; h1Name += "_chip_"; h1Name += j+1;
      h1Val_v.at(i).at(j)->SetName(h1Name.Data());
      //
      h1Title="h1_sec_"; h1Title += i+1; h1Title += "_chip_"; h1Title += j+1;
      h1Val_v.at(i).at(j)->SetTitle(h1Title.Data());
      h1Val_v.at(i).at(j)->SetBins( ARIconst::hvBiasPar_bin[ii_hvBiasPar], ARIconst::hvBiasPar_min[ii_hvBiasPar], ARIconst::hvBiasPar_max[ii_hvBiasPar]);
    }
    //
    grName="grMin_sec_"; grName += i+1;
    grMin_guard_v.at(i)->SetName(grName.Data());
    grName="grMax_sec_"; grName += i+1;
    grMax_guard_v.at(i)->SetName(grName.Data());
    grName="grAve_sec_"; grName += i+1;
    grAve_guard_v.at(i)->SetName(grName.Data());
    //
    grTitle="grMin_sec_"; grTitle += i+1;
    grMin_guard_v.at(i)->SetTitle(grTitle.Data());
    grTitle="grMax_sec_"; grTitle += i+1;
    grMax_guard_v.at(i)->SetTitle(grTitle.Data());
    grTitle="grAve_sec_"; grTitle += i+1;
    grAve_guard_v.at(i)->SetTitle(grTitle.Data());
    //
    h1Name="h1_sec_"; h1Name += i+1;
    h1Title="h1_sec_"; h1Title += i+1;
    h1Val_guard_v.at(i)->SetName(h1Name.Data());
    h1Val_guard_v.at(i)->SetTitle(h1Title.Data());
    h1Val_guard_v.at(i)->SetBins( ARIconst::hvBiasPar_bin[ii_hvBiasPar], ARIconst::hvBiasPar_min[ii_hvBiasPar], ARIconst::hvBiasPar_max[ii_hvBiasPar]);
  }
  //hp_chips
  for( unsigned int i = 0; i < f_v.size(); i++){
    //hp_chips--------------------
    TH2Poly* hp = (TH2Poly*)f_v.at(i)->Get("hp_chips");
    std::vector<double> minVal_v;
    std::vector<double> maxVal_v;
    std::vector<double> aveVal_v;
    std::vector<std::vector<double>> allVal_v;
    getMinMaxAveFromTH2Poly( hp, mstr_v.at(i).secID, arichmap, minVal_v, maxVal_v, aveVal_v, allVal_v);
    //for( unsigned int j = 0 ; j < (unsigned int)arichmap->GetnChipsPerHAPD() ; j++)
    //std::cout<<j<<" "<<minVal_v.at(j)<<std::endl;
    for( unsigned int j = 0 ; j < (unsigned int)arichmap->GetnChipsPerHAPD() ; j++){
      if(minVal_v.size() > 0 && maxVal_v.size() > 0 &&  aveVal_v.size() > 0 &&  allVal_v.size() > 0 ){
	//std::cout<<"minVal_v.at(j) "<<minVal_v.at(j)<<std::endl;
	grMin_v.at((mstr_v.at(i).secID-1)).at(j)->SetPoint(grMin_v.at((mstr_v.at(i).secID-1)).at(j)->GetN(),mstr_v.at(i).ut,minVal_v.at(j));
	grMax_v.at((mstr_v.at(i).secID-1)).at(j)->SetPoint(grMax_v.at((mstr_v.at(i).secID-1)).at(j)->GetN(),mstr_v.at(i).ut,maxVal_v.at(j));
	grAve_v.at((mstr_v.at(i).secID-1)).at(j)->SetPoint(grAve_v.at((mstr_v.at(i).secID-1)).at(j)->GetN(),mstr_v.at(i).ut,aveVal_v.at(j));
	//h1_chip_xx
	for( unsigned int k = 0 ; k < (unsigned int)allVal_v.at(j).size(); k++)
	  h1Val_v.at((mstr_v.at(i).secID-1)).at(j)->Fill(allVal_v.at(j).at(k));
      }
    }
    //hp_guard--------------------
    TH2Poly* hpg = (TH2Poly*)f_v.at(i)->Get("hp_guard");
    std::vector<double> minValg_v;
    std::vector<double> maxValg_v;
    std::vector<double> aveValg_v;
    std::vector<std::vector<double>> allValg_v;
    getMinMaxAveFromTH2Poly( hpg, mstr_v.at(i).secID, arichmap, minValg_v, maxValg_v, aveValg_v, allValg_v);
    if(minValg_v.size() > 0 && maxValg_v.size() > 0 &&  aveValg_v.size() > 0 &&  allValg_v.size() > 0 ){
      grMin_guard_v.at((mstr_v.at(i).secID-1))->SetPoint(grMin_guard_v.at((mstr_v.at(i).secID-1))->GetN(),mstr_v.at(i).ut,minValg_v.at(0));
      grMax_guard_v.at((mstr_v.at(i).secID-1))->SetPoint(grMax_guard_v.at((mstr_v.at(i).secID-1))->GetN(),mstr_v.at(i).ut,maxValg_v.at(0));
      grAve_guard_v.at((mstr_v.at(i).secID-1))->SetPoint(grAve_guard_v.at((mstr_v.at(i).secID-1))->GetN(),mstr_v.at(i).ut,aveValg_v.at(0));
      for( unsigned int k = 0 ; k < (unsigned int)allValg_v.at(0).size(); k++)
	h1Val_guard_v.at((mstr_v.at(i).secID-1))->Fill(allValg_v.at(0).at(k));
    }
    //for( unsigned int j = 1; j <= (unsigned int)hp->GetNcells(); j++){
    //hp_sum_chips->SetBinContent(j,hp_sum_chips->GetBinContent(j) + hp->GetBinContent(j));
    //if(hp->GetBinContent(j) != 0)
    //}
    f_v.at(i)->Close();
  }
  //for( unsigned int i = 0; i < f_v.size(); i++)
}

//KEY: TH2Poly hp_chips;1  chips vlimit Mon-Oct-21-15:45:37-JST-2019
//KEY: TH2Poly hp_guard;1  guard vlimit Mon-Oct-21-15:45:37-JST-2019
//KEY: TH1D    h1_chip_a;1 chip_a
//KEY: TH1D    h1_chip_b;1 chip_b
//KEY: TH1D    h1_chip_c;1 chip_c
//KEY: TH1D    h1_chip_d;1 chip_d
//KEY: TH1D    h1_guard;1  guard
void combineMeasurements(std::vector<hvBiasRootFileStr> &mstr_v,
			 TString inDataDir,
			 ARICHChannelHist *hp_sum_chips, ARICHChannelHist *hp_sum_guard,
			 ARICHChannelHist *hp_sum_chips_norm, ARICHChannelHist *hp_sum_guard_norm,
			 ARICHChannelHist *hp_sum_chips_aver, ARICHChannelHist *hp_sum_guard_aver){
  //
  if(mstr_v.size()<=0){
    std::cout<<"mstr_v.size() "<<mstr_v.size()<<std::endl;
    assert(0);
  }
  std::vector<TFile*> f_v;
  for( unsigned int i = 0; i < mstr_v.size(); i++){
    TString ffname = inDataDir;
    ffname += mstr_v.at(i).fname;
    TFile *rootf = new TFile(ffname.Data());
    f_v.push_back(rootf);
  }
  std::vector<TH2Poly*> th2poly_v;
  TString hp_sum_chips_title = "Chips "; hp_sum_chips_title += mstr_v.at(0).measType;
  hp_sum_chips_title += " "; hp_sum_chips_title += getLocalTimeStringFromUnixTime(mstr_v.at(0).ut);
  hp_sum_chips_title += " "; hp_sum_chips_title += getLocalTimeStringFromUnixTime(mstr_v.at(mstr_v.size()-1).ut);
  TString hp_sum_guard_title = "Guard "; hp_sum_guard_title += mstr_v.at(0).measType;
  hp_sum_guard_title += " "; hp_sum_guard_title += getLocalTimeStringFromUnixTime(mstr_v.at(0).ut);
  hp_sum_guard_title += " "; hp_sum_guard_title += getLocalTimeStringFromUnixTime(mstr_v.at(mstr_v.size()-1).ut);
  //
  hp_sum_chips->SetTitle(hp_sum_chips_title.Data());
  hp_sum_guard->SetTitle(hp_sum_guard_title.Data());  
  //
  TString hp_sum_chips_norm_title = "Chips "; hp_sum_chips_norm_title += mstr_v.at(0).measType; hp_sum_chips_norm_title += " norm";
  hp_sum_chips_norm_title += " "; hp_sum_chips_norm_title += getLocalTimeStringFromUnixTime(mstr_v.at(0).ut);
  hp_sum_chips_norm_title += " "; hp_sum_chips_norm_title += getLocalTimeStringFromUnixTime(mstr_v.at(mstr_v.size()-1).ut);
  TString hp_sum_guard_norm_title = "Guard "; hp_sum_guard_norm_title += mstr_v.at(0).measType; hp_sum_guard_norm_title += " norm";
  hp_sum_guard_norm_title += " "; hp_sum_guard_norm_title += getLocalTimeStringFromUnixTime(mstr_v.at(0).ut);
  hp_sum_guard_norm_title += " "; hp_sum_guard_norm_title += getLocalTimeStringFromUnixTime(mstr_v.at(mstr_v.size()-1).ut);
  //
  hp_sum_chips_norm->SetTitle(hp_sum_chips_norm_title.Data());
  hp_sum_guard_norm->SetTitle(hp_sum_guard_norm_title.Data());  
  //
  TString hp_sum_chips_aver_title = "Chips "; hp_sum_chips_aver_title += mstr_v.at(0).measType; hp_sum_chips_aver_title += " aver";
  hp_sum_chips_aver_title += " "; hp_sum_chips_aver_title += getLocalTimeStringFromUnixTime(mstr_v.at(0).ut);
  hp_sum_chips_aver_title += " "; hp_sum_chips_aver_title += getLocalTimeStringFromUnixTime(mstr_v.at(mstr_v.size()-1).ut);
  TString hp_sum_guard_aver_title = "Guard "; hp_sum_guard_aver_title += mstr_v.at(0).measType; hp_sum_guard_aver_title += " aver";
  hp_sum_guard_aver_title += " "; hp_sum_guard_aver_title += getLocalTimeStringFromUnixTime(mstr_v.at(0).ut);
  hp_sum_guard_aver_title += " "; hp_sum_guard_aver_title += getLocalTimeStringFromUnixTime(mstr_v.at(mstr_v.size()-1).ut);
  //
  hp_sum_chips_aver->SetTitle(hp_sum_chips_aver_title.Data());
  hp_sum_guard_aver->SetTitle(hp_sum_guard_aver_title.Data());  
  //
  for( unsigned int i = 0; i < f_v.size(); i++){
    TH2Poly* hp = (TH2Poly*)f_v.at(i)->Get("hp_chips");
    TH2Poly* hpg = (TH2Poly*)f_v.at(i)->Get("hp_guard");
    //std::cout<<"hp->GetNcells() "<<hp->GetNcells()<<std::endl;
    //assert(0);
    for( unsigned int j = 1; j <= (unsigned int)hp->GetNcells(); j++){
      hp_sum_chips->SetBinContent(j,hp_sum_chips->GetBinContent(j) + hp->GetBinContent(j));
      if(hp->GetBinContent(j) != 0)
	hp_sum_chips_norm->SetBinContent(j,hp_sum_chips_norm->GetBinContent(j) + 1.0);
    }
    for( unsigned int k = 1; k <= (unsigned int)hpg->GetNcells(); k++){
      hp_sum_guard->SetBinContent(k,hp_sum_guard->GetBinContent(k) + hpg->GetBinContent(k));
      if(hpg->GetBinContent(k) != 0)
	hp_sum_guard_norm->SetBinContent(k,hp_sum_guard_norm->GetBinContent(k) + 1.0);
    }
  }
  //Average calculation
  Double_t aver = 0.0;
  Double_t norm = 0.0;
  for( unsigned int j = 1; j <= (unsigned int)hp_sum_chips->GetNcells(); j++){
    aver = hp_sum_chips->GetBinContent(j);
    norm = hp_sum_chips_norm->GetBinContent(j);
    if(norm > 0)
      hp_sum_chips_aver->SetBinContent(j,aver/norm);
    else
      hp_sum_chips_aver->SetBinContent(j,0.0);
  }
  for( unsigned int k = 1; k <= (unsigned int)hp_sum_guard->GetNcells(); k++){
    aver = hp_sum_guard->GetBinContent(k);
    norm = hp_sum_guard_norm->GetBinContent(k);
    if(norm > 0)
      hp_sum_guard_aver->SetBinContent(k,aver/norm);
    else
      hp_sum_guard_aver->SetBinContent(k,0.0);
  }
}

TString getLocalTimeStringFromUnixTime( int ut ){
  time_t t_ut = ut;
  struct tm * timeinfo = localtime(&t_ut);
  //TString timeStr = asctime(timeinfo);
  //return timeStr;
  //printf ("Current local time and date: %s", asctime(timeinfo));
  //std::cout<<ut<<std::endl
  //	   <<timeStr<<std::endl
  //	   <<"timeinfo->tm_year "<<timeinfo->tm_year+1900<<std::endl
  //	   <<"timeinfo->tm_mon  "<<timeinfo->tm_mon+1<<std::endl
  //	   <<"timeinfo->tm_mday "<<timeinfo->tm_mday<<std::endl
  //	   <<"timeinfo->tm_hour "<<timeinfo->tm_hour<<std::endl
  //	   <<"timeinfo->tm_min  "<<timeinfo->tm_min<<std::endl
  //	   <<"timeinfo->tm_sec  "<<timeinfo->tm_sec<<std::endl;
  TString timeStrFormat="";
  int data_t;
  data_t=timeinfo->tm_mday;
  if(data_t<10)
    timeStrFormat += "0";
  timeStrFormat += data_t; timeStrFormat +=".";
  data_t=timeinfo->tm_mon + 1;
  if(data_t<10)
    timeStrFormat += "0";
  timeStrFormat += data_t; timeStrFormat +=".";
  data_t=timeinfo->tm_year; timeStrFormat += data_t+1900; timeStrFormat +="_";
  data_t=timeinfo->tm_hour;
  if(data_t<10)
    timeStrFormat += "0";
  timeStrFormat += data_t;      timeStrFormat +="-";
  data_t=timeinfo->tm_min;
  if(data_t<10)
    timeStrFormat += "0";
  timeStrFormat += data_t;      timeStrFormat +="-";
  data_t=timeinfo->tm_sec;
  if(data_t<10)
    timeStrFormat += "0";
  timeStrFormat += data_t;
  //std::cout<<timeStrFormat<<std::endl;
  return timeStrFormat;
}

void bubbleSort(std::vector<hvBiasRootFileStr> &mstr_v){
  unsigned int n = mstr_v.size();
  unsigned int i, j;
  int flag=1;
  //Now we will sort the array if my flag value is 1 then only the loop will execute
  for(i=0;i<n-1 && flag==1;i++){
    //here after each time of j loop we will re-initialize the flag to 0
    flag=0;
    for(j=0;j<n-i-1;j++){
      //checking if previous value is grater than next one or not
      if(mstr_v.at(j).ut>mstr_v.at(j+1).ut){
	//temp will temporarly store the value of arr[j] then we will swap the values
	hvBiasRootFileStr temp = mstr_v.at(j);
	mstr_v.at(j)=mstr_v.at(j+1);
	mstr_v.at(j+1) = temp;
	//Here if there is a swap then we will make it 1
	flag=1;
      }
    }
  }
}
