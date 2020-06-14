/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Mon Oct 21 15:29:30 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "libarihvmonitor.h"
#include "crateToModuleMapStr.h"

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

std::vector<crateToModuleMapStr> read_crateToModuleMap(TString crateToModuleMapFile) {
  ////////////////
  string motserial;
  string mottype;
  string mot;
  Int_t crate;
  Int_t slot;
  Int_t channel;
  Int_t id;
  TString type;
  TString serial;
  ////////////////
  std::vector<crateToModuleMapStr> crate2ModMap_v;
  ifstream infile (crateToModuleMapFile.Data());
  if( infile.is_open() ) {
    while( infile>> id >> motserial >> mot >> crate >> mot >> slot >> mot >> channel >> mottype) {
      crateToModuleMapStr mapS;
      serial = motserial;
      mapS.id = id;
      mapS.crate = crate;
      mapS.slot = slot;
      mapS.channel = channel;
      mapS.serial = serial;
      mapS.type = mottype;
      crate2ModMap_v.push_back(mapS);
    }
  }
  return crate2ModMap_v;
}

void convert_hvbias_params(Int_t secID,
			   Int_t unixTime,
			   TString dateMeas,
			   TString crateToModuleMap, 
			   TString parsufix,
			   TString outRootFile,
			   const std::vector<int>& crate_v,
			   const std::vector<int>& slot_v,
			   const std::vector<int>& channel_v,
			   const std::vector<float>& val_v) {
  if(crate_v.size() != slot_v.size() || crate_v.size() != channel_v.size() || crate_v.size() != val_v.size()){
    std::cout<<"crate_v.size()   = "<<crate_v.size()<<endl
	     <<"slot_v.size()    = "<<slot_v.size()<<endl
	     <<"channel_v.size() = "<<channel_v.size()<<endl
	     <<"val_v.size()     = "<<val_v.size()<<endl;
    assert(0);
  }
  //
  int ii_hvBiasPar = -999;
  for( int k = 0; k< ARIconst::nhvBiasPar; k++)
    if(ARIconst::hvBiasParArr[k] == parsufix)
      ii_hvBiasPar = k;
  if(ii_hvBiasPar<0)
    assert(0);
  //
  std::vector<crateToModuleMapStr> c2mMap_v = read_crateToModuleMap(crateToModuleMap);
  //for(unsigned int i = 0; i < c2mMap_v.size(); i++)
  //c2mMap_v.at(i).print();
  //
  //1 KA0430 crate 1 slot  0 channel 42 bias-a
  //1 KA0430 crate 1 slot  0 channel 40 bias-b
  //1 KA0430 crate 1 slot  0 channel 38 bias-c
  //1 KA0430 crate 1 slot  0 channel 36 bias-d
  //1 KA0430 crate 1 slot  0 channel 34  guard
  //
  std::vector<TString> hv_type;
  hv_type.push_back("bias-a"); //0
  hv_type.push_back("bias-b"); //1
  hv_type.push_back("bias-c"); //2
  hv_type.push_back("bias-d"); //3
  hv_type.push_back("guard");  //4
  //
  TString histTitle;
  //histTitle = "chip_a"; histTitle += " "; histTitle += parsufix; histTitle += " "; histTitle += dateMeas;
  //ARICHChannelHist *hp_chip_a = new ARICHChannelHist("hp_chip_a",histTitle.Data(),2);
  //histTitle = "chip_b"; histTitle += " "; histTitle += parsufix; histTitle += " "; histTitle += dateMeas;
  //ARICHChannelHist *hp_chip_b = new ARICHChannelHist("hp_chip_b",histTitle.Data(),2);
  //histTitle = "chip_c"; histTitle += " "; histTitle += parsufix; histTitle += " "; histTitle += dateMeas;
  //ARICHChannelHist *hp_chip_c = new ARICHChannelHist("hp_chip_c",histTitle.Data(),2);
  //histTitle = "chip_d"; histTitle += " "; histTitle += parsufix; histTitle += " "; histTitle += dateMeas;
  //ARICHChannelHist *hp_chip_d = new ARICHChannelHist("hp_chip_d",histTitle.Data(),2);
  histTitle = "chips"; histTitle += " "; histTitle += parsufix; histTitle += " "; histTitle += dateMeas;
  ARICHChannelHist *hp_chips= new ARICHChannelHist("hp_chips",histTitle.Data(),2);
  histTitle = "guard"; histTitle += " "; histTitle += parsufix; histTitle += " "; histTitle += dateMeas;
  ARICHChannelHist *hp_guard = new ARICHChannelHist("hp_guard",histTitle.Data(),1);
  //
  TH1D *h1_chip_a = new TH1D("h1_chip_a","chip_a",ARIconst::hvBiasPar_bin[ii_hvBiasPar], ARIconst::hvBiasPar_min[ii_hvBiasPar], ARIconst::hvBiasPar_max[ii_hvBiasPar]);
  TH1D *h1_chip_b = new TH1D("h1_chip_b","chip_b",ARIconst::hvBiasPar_bin[ii_hvBiasPar], ARIconst::hvBiasPar_min[ii_hvBiasPar], ARIconst::hvBiasPar_max[ii_hvBiasPar]);
  TH1D *h1_chip_c = new TH1D("h1_chip_c","chip_c",ARIconst::hvBiasPar_bin[ii_hvBiasPar], ARIconst::hvBiasPar_min[ii_hvBiasPar], ARIconst::hvBiasPar_max[ii_hvBiasPar]);
  TH1D *h1_chip_d = new TH1D("h1_chip_d","chip_d",ARIconst::hvBiasPar_bin[ii_hvBiasPar], ARIconst::hvBiasPar_min[ii_hvBiasPar], ARIconst::hvBiasPar_max[ii_hvBiasPar]);
  TH1D *h1_guard = new TH1D("h1_guard","guard",ARIconst::hvBiasPar_bin[ii_hvBiasPar], ARIconst::hvBiasPar_min[ii_hvBiasPar], ARIconst::hvBiasPar_max[ii_hvBiasPar]);
  //
  for(unsigned int i = 0; i < crate_v.size(); i++){
    //std::cout<<std::setw(3)<<crate_v.at(i)<<std::setw(4)<<slot_v.at(i)<<std::setw(4)<<channel_v.at(i)<<std::setw(10)<<val_v.at(i)<<std::endl;
    for(unsigned int j = 0; j < c2mMap_v.size(); j++){
      for(unsigned int k = 0; k < hv_type.size(); k++){
	if(crate_v.at(i) == c2mMap_v.at(j).crate && 
	   slot_v.at(i) == c2mMap_v.at(j).slot && 
	   channel_v.at(i) == c2mMap_v.at(j).channel &&
	   c2mMap_v.at(j).type == hv_type.at(k)){
	  //chip_a
	  if(c2mMap_v.at(j).type == hv_type.at(0)){
	    h1_chip_a->Fill(val_v.at(i));
	    hp_chips->setBinContent(c2mMap_v.at(j).id,0,val_v.at(i));
	    //hp_chips->setBinContent(c2mMap_v.at(j).id,0,1.0);
	    //cout<<"c2mMap_v.at(j).type "<<c2mMap_v.at(j).type<<endl;
	  }
	  //chip_b
	  if(c2mMap_v.at(j).type == hv_type.at(1)){
	    h1_chip_b->Fill(val_v.at(i));
	    hp_chips->setBinContent(c2mMap_v.at(j).id,1,val_v.at(i));
	  }
	  //chip_c
	  if(c2mMap_v.at(j).type == hv_type.at(2)){
	    h1_chip_c->Fill(val_v.at(i));
	    hp_chips->setBinContent(c2mMap_v.at(j).id,3,val_v.at(i));
	  }
	  //chip_d
	  if(c2mMap_v.at(j).type == hv_type.at(3)){
	    h1_chip_d->Fill(val_v.at(i));
	    hp_chips->setBinContent(c2mMap_v.at(j).id,2,val_v.at(i));
	  }
	  //guard
	  if(c2mMap_v.at(j).type == hv_type.at(4)){
	    h1_guard->Fill(val_v.at(i));
	    hp_guard->setBinContent(c2mMap_v.at(j).id,val_v.at(i));
	  }
	}
      }
    }
  }
  //
  TFile* rootFile = new TFile(outRootFile.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<outRootFile.Data()<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<outRootFile.Data()<<endl;
  cout<<endl<<endl;
  //
  hp_chips->Write();
  hp_guard->Write();
  //
  h1_chip_a->Write();
  h1_chip_b->Write();
  h1_chip_c->Write();
  h1_chip_d->Write();
  h1_guard->Write();
  //
  rootFile->Close();
}
