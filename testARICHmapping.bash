#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Tue Apr  9 16:10:48 JST 2019                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                                                                      #
# Input paramete:                                                      #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

function testARICHmapping_Dict_bash {
    
    make libarihvmonitorDict.so;

    root -l -e 'gROOT->ProcessLine(".L libarihvmonitorDict.cxx"); gROOT->ProcessLine(".L libarihvmonitorDict.so"); gROOT->ProcessLine("testARICHmapping()");'
    
}

function testARICHmapping_bash {
    
    make testARICHmapping_main;

    ./testARICHmapping_main
    
}

testARICHmapping_Dict_bash

#testARICHmapping_bash
