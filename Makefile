########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Thu Oct 17 22:36:49 CEST 2019                                        #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# Script description:                                                  #
#                                                                      #
# Input paramete: NON                                                  #
#                                                                      #
# This software is provided "as is" without any warranty.              #
########################################################################

ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

# add xml files
ROOTLIBS += -lMLP -lXMLIO

OUTLIB = /home/usr/hvala/ari_hv_monitor/obj/
#OUTLIB = /home/burmist/home2/ari_hv_monitor/obj/
CXX  = g++
CXX += -I./     

CXXFLAGS  = -g -Wall -fPIC -Wno-deprecated
CXXFLAGS += $(ROOTCFLAGS)
CXXFLAGS += $(ROOTLIBS)
CXXFLAGS += $(ROOTGLIBS)
CXXFLAGSDICT = -g -Wall -fPIC -Wno-deprecated

PATHTOSHAREDLIB=$(OUTLIB)

#----------------------------------------------------#

all: makedir convert_hvbias_params_main merge_Root_hvbias_params_main plotsSector_main plotsMergedData_main

makedir:
	mkdir -p $(OUTLIB);
	mkdir -p tmp;

printmakeinfo:
	$(info CXX          = "$(CXX)")
	$(info CXXFLAGS     = "$(CXXFLAGS)")
	$(info CXXFLAGSDICT = "$(CXXFLAGSDICT)")

make_hvbias_map: make_hvbias_map.C
	$(CXX) -DMAKE_HVBIAS_MAP -o make_hvbias_map make_hvbias_map.C $(CXXFLAGS)

convert_hvbias_params_main: convert_hvbias_params_main.cc libarihvmonitor.so
	$(CXX) -DCONVERT_HVBIAS_PARAMS_MAIN -o $@ $< $(OUTLIB)*.so $(CXXFLAGS)

merge_Root_hvbias_params_main: merge_Root_hvbias_params_main.cc libarihvmonitor.so
	$(CXX) -DMERGE_ROOT_HVBIAS_PARAMS_MAIN -o $@ $< $(OUTLIB)*.so $(CXXFLAGS)

testARICHChannelHist_main: testARICHChannelHist_main.cc libarihvmonitor.so
	$(CXX) -DTESTARICHCHANNELHIST_MAIN -o $@ $< $(OUTLIB)*.so $(CXXFLAGS)

testARICHSectorHist_main: testARICHSectorHist_main.cc libarihvmonitor.so
	$(CXX) -DTESTARICHSECTORHIST_MAIN -o $@ $< $(OUTLIB)*.so $(CXXFLAGS)

testARICHmapping_main: testARICHmapping_main.cc libarihvmonitor.so
	$(CXX) -DTESTARICHMAPPING_MAIN -o $@ $< $(OUTLIB)*.so $(CXXFLAGS)

plotsSector_main: plotsSector_main.cc libarihvmonitor.so
	$(CXX) -DPLOTSSECTOR_MAIN -o $@ $< $(OUTLIB)*.so $(CXXFLAGS)

plotsMergedData_main: plotsMergedData_main.cc libarihvmonitor.so
	$(CXX) -DPLOTSMERGEDDATA_MAIN -o $@ $< $(OUTLIB)*.so $(CXXFLAGS)

obj/plots.o: plots.C
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/plotsSector.o: plotsSector.cc
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/plotsMergedData.o: plotsMergedData.cc
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/merge_Root_hvbias_params.o: merge_Root_hvbias_params.cc
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/convert_hvbias_params.o: convert_hvbias_params.cc
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/ARICHChannelHist.o: ARICHChannelHist.cc ARICHChannelHist.h
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/ARICHMergerHist.o: ARICHMergerHist.cc ARICHMergerHist.h
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/ARICHSectorHist.o: ARICHSectorHist.cc ARICHSectorHist.h
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/ARICHmappingCopperMergerFeb.o: ARICHmappingCopperMergerFeb.cc ARICHmappingCopperMergerFeb.h
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/testARICHChannelHist.o: testARICHChannelHist.cc
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/testARICHSectorHist.o: testARICHSectorHist.cc
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/testARICHmapping.o: testARICHmapping.cc
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

libarihvmonitor.so: obj/convert_hvbias_params.o obj/merge_Root_hvbias_params.o obj/ARICHChannelHist.o obj/testARICHChannelHist.o obj/ARICHmappingCopperMergerFeb.o obj/ARICHSectorHist.o obj/testARICHSectorHist.o obj/ARICHMergerHist.o obj/testARICHmapping.o obj/plots.o obj/plotsSector.o obj/plotsMergedData.o
	$(CXX) -shared -o $(PATHTOSHAREDLIB)$@ $^ $(ROOTLIBS) $(ROOTGLIBS)

libarihvmonitorDict.cxx: libarihvmonitor.h LinkDef.h
	rootcling -f $@ -c $(CXXFLAGSDICT) -p $^

libarihvmonitorDict.so: libarihvmonitorDict.cxx ARICHChannelHist.cc ARICHSectorHist.cc ARICHmappingCopperMergerFeb.cc testARICHChannelHist.cc testARICHSectorHist.cc testARICHmapping.cc convert_hvbias_params.cc merge_Root_hvbias_params.cc plots.C plotsSector.cc plotsMergedData.cc
	g++ -shared -o $@ $(CXXFLAGS) -I. -I$(ROOTSYS)/include $^

clean:
	rm -f *~
	rm -f .*~
	rm -rf $(OUTLIB)
	rm -f make_hvbias_map
	rm -f plotsSector_main
	rm -f convert_hvbias_params_main
	rm -f merge_Root_hvbias_params_main
	rm -f testARICHChannelHist_main
	rm -f testARICHSectorHist_main
	rm -f testARICHmapping_main
	rm -f libarihvmonitorDict.cxx
	rm -f libarihvmonitorDict.so
	rm -f libarihvmonitorDict_rdict.pcm
