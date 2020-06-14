//my
#include "libarihvmonitor.h"
#include "ARICHmappingCopperMergerFeb.h"
#include "ARICHChannelHist.h"

Int_t testARICHmapping(){

  std::cout<<"testARICHChannelHist"<<std::endl;

  TString hapdTemplateMappingPath = "hapdTemplateMapping.dat";
  ARICHmappingCopperMergerFeb *arichmap = new ARICHmappingCopperMergerFeb(hapdTemplateMappingPath);
  
  //------------------------------------------
  Int_t copperID_test = 4008;
  Int_t mergerLocalID_test = 0;
  Int_t febLocalID_test = 4;
  Int_t febID_test = arichmap->getFebIDFromCopperBoardIDAndMergerLocalIDAndFebLocalID( copperID_test, mergerLocalID_test, febLocalID_test);
  std::cout<<"arichmap->GetnCopperPerSector()  "<<arichmap->GetnCopperPerSector()<<endl
	   <<"arichmap->GetnMergersPerCopper() "<<arichmap->GetnMergersPerCopper()<<endl
	   <<"arichmap->GetnHAPD()             "<<arichmap->GetnHAPD()<<endl
	   <<"febID_test                       "<<febID_test<<endl;
  //------------------------------------------

  //------------------------------------------
  Int_t sec_i, cop_i, mer_i, feb_i;
  Int_t febID = 343;
  arichmap->findPositionInarichmappingFormGlobalFebID(febID, sec_i, cop_i, mer_i, feb_i);
  std::cout<<"febID "<<febID<<endl
	   <<"sec_i "<<sec_i<<endl
	   <<"cop_i "<<cop_i<<endl
	   <<"mer_i "<<mer_i<<endl
	   <<"feb_i "<<feb_i<<endl;
  //------------------------------------------

  //------------------------------------------
  Int_t copperID_test2 = 4008;
  Int_t sectorID_test2;
  sectorID_test2 = arichmap->getSectorIDFromCopperBoardID(copperID_test2);
  std::cout<<"copperID_test2 "<<copperID_test2<<endl
	   <<"sectorID_test2 "<<sectorID_test2<<endl;
  //------------------------------------------

  //------------------------------------------
  Int_t mergerID_test3 = 11;
  Int_t nHAPDperMerger = arichmap->GetnHAPDPerMerger(mergerID_test3);
  std::cout<<"mergerID_test3 "<<mergerID_test3<<endl
	   <<"nHAPDperMerger "<<nHAPDperMerger<<endl;
  //------------------------------------------

  //------------------------------------------
  Int_t copperID_test4 = 4008;
  Int_t mergerLocalID_test4 = 0;
  Int_t mergerID_test4 = arichmap->getMergerIDFromCopperBoardIDAndMergerLocalID( copperID_test4, mergerLocalID_test4);
  std::cout<<"copperID_test4      "<<copperID_test4<<endl
	   <<"mergerLocalID_test4 "<<mergerLocalID_test4<<endl
	   <<"mergerID_test4      "<<mergerID_test4<<endl;
  //------------------------------------------

  //------------------------------------------
  for(unsigned int i = 0; i<6;i++){
    std::cout<<"arichmapping.sector[i].globalID "<<arichmap->arichmapping.sector[i].globalID<<std::endl
	     <<"arichmapping.sector[i].localID  "<<arichmap->arichmapping.sector[i].localID<<std::endl;
  }
  //------------------------------------------

  //------------------------------------------
  Int_t feb_globalID;
  Int_t secIDmy = 1;
  sec_i = secIDmy - 1;
  for( cop_i = 0 ; cop_i < arichmap->GetnCopperPerSector(); cop_i++){
    for( mer_i = 0 ; mer_i < arichmap->GetnMergersPerCopper(); mer_i++){
      for( feb_i = 0 ; feb_i < arichmap->GetnHAPDPerMerger() ; feb_i++){
	feb_globalID = arichmap->arichmapping.sector[sec_i].copper[cop_i].merger[mer_i].feb[feb_i].globalID;
	std::cout<<"feb_globalID "<<feb_globalID<<std::endl;
      }
    }
  }
  //------------------------------------------

  //------------------------------------------
  Int_t febGlobalIDmy = 1;
  char chipLetterMy = 'd';
  std::cout<<arichmap->getGlobalBiasChipID( febGlobalIDmy, chipLetterMy)<<std::endl;
  //------------------------------------------

  //------------------------------------------
  febGlobalIDmy = 149;
  chipLetterMy = 'c';
  std::cout<<arichmap->getGlobalBiasChipID( febGlobalIDmy, chipLetterMy)<<std::endl;
  //------------------------------------------

  //------------------------------------------
  febGlobalIDmy = 355;
  chipLetterMy = 'd';
  std::cout<<arichmap->getGlobalBiasChipID( febGlobalIDmy, chipLetterMy)<<std::endl;
  //------------------------------------------
  
  return 0;

}
