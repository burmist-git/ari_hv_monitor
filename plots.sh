#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Tue Oct 22 02:54:59 JST 2019                                         #
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

function plots_sh {
    
    make libarihvmonitorDict.so;

    root -l -e 'gROOT->ProcessLine(".L libarihvmonitorDict.cxx"); gROOT->ProcessLine(".L libarihvmonitorDict.so"); gROOT->ProcessLine("plots()");'
    
}

plots_sh

