#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2018 - LBS - (Single person developer.)                 #
# Wed Feb 27 15:56:51 CET 2019                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                  This script print the ARICH sector histogram.       #
#                                                                      #
# Input paramete:                                                      #
#                  NON.                                                #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

function testARICHSectorHist_bash {

    make libarihvmonitorDict.so;

    root -l -e 'gROOT->ProcessLine(".L libarihvmonitorDict.cxx"); gROOT->ProcessLine(".L libarihvmonitorDict.so"); gROOT->ProcessLine("testARICHSectorHist()");'
    
}

testARICHSectorHist_bash
