#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2018 - LBS - (Single person developer.)                 #
# Thu Oct 18 19:13:05 JST 2018                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                  This script print the ARICH mapping.                #
#                                                                      #
# Input paramete:                                                      #
#                   NON.                                               #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

function testStandAloneARICHChannelHist_bash {
    
    make libarihvmonitorDict.so;

    root -l -e 'gROOT->ProcessLine(".L libarihvmonitorDict.cxx"); gROOT->ProcessLine(".L libarihvmonitorDict.so"); gROOT->ProcessLine("testARICHChannelHist()");'
    
}

testStandAloneARICHChannelHist_bash
