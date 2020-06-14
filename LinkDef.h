// A LinkDef.h file with all the explicit template instances
// that will be needed at link time
#ifdef __CLING__
#pragma link C++ class TObject+;
#pragma link C++ class ARICHChannelHist+;
#pragma link C++ class ARICHmappingCopperMergerFeb+;
#pragma link C++ class ARICHSectorHist+;
#pragma link C++ function read_crateToModuleMap;
#pragma link C++ function convert_hvbias_params;
#pragma link C++ function testARICHChannelHist;
#pragma link C++ function testARICHSectorHist;
#pragma link C++ function testARICHmapping;
#pragma link C++ function plots;
#endif
