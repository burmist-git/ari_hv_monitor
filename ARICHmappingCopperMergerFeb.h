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

#include "ARICHfebStr.h"
#include "ARICHcopperStr.h"
#include "ARICHmergerStr.h"
#include "ARICHsectorStr.h"
#include "ARICHmappingStr.h"

struct ARICHfebStr;
struct ARICHmergerStr;
struct ARICHcopperStr;
struct ARICHsectorStr;
struct ARICHmappingStr;


class ARICHmappingCopperMergerFeb  {
  
 public:
  
  ARICHmappingCopperMergerFeb(TString hapdTemplateMappingFileName);
  ~ARICHmappingCopperMergerFeb();
  
  void dumpMapping();
  void printMappingConstants();
  Int_t getSectorIDFromCopperBoardID( Int_t copperBoardID);
  Int_t getCopperBoardLocalIDFromCopperBoardID( Int_t copperBoardID);
  Int_t getMergerIDFromCopperBoardIDAndMergerLocalID( Int_t copperBoardID, Int_t mergerLocalID);
  Int_t getFebIDFromCopperBoardIDAndMergerLocalIDAndFebLocalID( Int_t copperBoardID, Int_t mergerLocalID, Int_t febLocalID);
  bool checkCopperBoardID(Int_t copperBoardID);
  bool checkMergerLocalID(Int_t mergerLocalID);
  bool checkFebLocalID(Int_t febLocalID);
  
  const Int_t GetnSectors(){return nSectors;};
  const Int_t GetnCopperPerSector(){return nCopperPerSector;};
  const Int_t GetnMergersPerCopper(){return nMergersPerCopper;};
  const Int_t GetnHAPDPerMerger(){return nHAPDPerMerger;};
  const Int_t GetnHAPDPerMerger(Int_t mergerID);
  const Int_t GetnHAPDPerCopper(){return nHAPDPerCopper;};
  const Int_t GetnCopper(){return nCopper;};
  const Int_t GetnMergersPerSector(){return nMergersPerSector;};
  const Int_t GetnMergers(){return nMergers;};
  const Int_t GetnFebs(){return nFebs;};
  const Int_t GetnHAPDrings(){return nHAPDrings;};
  const Int_t GetnHAPDperSector(){return nHAPDperSector;};
  const Int_t GetnChipsPerHAPD(){return nChipsPerHAPD;};
  const Int_t GetnHAPD(){return nHAPD;};
  
  Int_t getHAPD_globalID_from_sec_iR_iP(Int_t secID, Int_t iR, Int_t iP);
  void findPositionInarichmappingFormSecIDiRingiPosition(Int_t secID, Int_t iR, Int_t iP, Int_t &sec_i, Int_t &cop_i, Int_t &mer_i, Int_t &feb_i);
  void findPositionInarichmappingFormGlobalFebID(Int_t febID, Int_t &sec_i, Int_t &cop_i, Int_t &mer_i, Int_t &feb_i);
  void findPositionInarichmappingFormGlobalMergerID(Int_t mergerID, Int_t &sec_i, Int_t &cop_i, Int_t &mer_i);

  Int_t getGlobalBiasChipID(Int_t febGlobalID, char chipLetter);
  Int_t getGlobalBiasChipID( Int_t febGlobalID, Int_t chipID);
  
  ARICHsectorStr sectorTemplate;
  ARICHmappingStr arichmapping;
  
 private:
  
  static const Int_t nSectors = 6;
  static const Int_t nCopperPerSector = 3;
  static const Int_t nMergersPerCopper = 4;
  static const Int_t nHAPDPerMerger = 6;
  static const Int_t nHAPDPerCopper = nMergersPerCopper*nHAPDPerMerger;
  static const Int_t nCopper = nSectors*nCopperPerSector;
  static const Int_t nMergersPerSector = nCopperPerSector*nMergersPerCopper;
  static const Int_t nMergers = nSectors*nCopperPerSector*nMergersPerCopper;
  static const Int_t nFebs = nSectors*nCopperPerSector*nMergersPerCopper*nHAPDPerMerger - 2*nSectors;
  static const Int_t nHAPDrings = 7;
  static const Int_t nHAPDperSector = 70;
  static const Int_t nChipsPerHAPD = 4;
  static const Int_t nHAPD = nHAPDperSector * nSectors;
  
  void initialisationOfTheMappingArr();
  void initialisationOfTheHAPDpositionTemplate(TString hapdTemplateMappingFileName);
  void initialisationOfTheFullArichMapping();
  Int_t gSectorID_Arr[nSectors];
  Int_t gCoperID_Arr[nSectors][nCopperPerSector];
  Int_t gMergerID_Arr[nSectors][nCopperPerSector][nMergersPerCopper];
  Int_t gHAPDID_Arr[nSectors][nCopperPerSector][nMergersPerCopper][nHAPDPerMerger];
  Int_t gMergerIDPerSector_Arr[nMergersPerSector];
  Int_t nFebPerMerger_Arr[nSectors][nCopperPerSector][nMergersPerCopper];
  
 public:
  
  const Int_t gHAPDpositionInRingPerSector_Arr[nHAPDrings];
  Int_t nHAPDInRingInARICH_Arr[nHAPDrings];
  
};
