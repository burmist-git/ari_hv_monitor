#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Wed Oct 23 22:01:47 JST 2019                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                                                                      #
# Input paramete:                                                      #
#                  NON.                                                #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

LC_TIME=en_US.UTF-8
LANG=en_US.UTF-8

function make_bash {

    make clean;
    makelog="Makefile.log"
    rm -rf $makelog
    nthreads=`nproc --all`
    make -j$nthreads -k 2>&1 | tee $makelog
    #make -k 2>&1 | tee $makelog

    nErrors=`grep -i error $makelog | grep -v Total | nl | wc -l`     
    nWarnings=`grep -i warning $makelog | grep -v Total | nl | wc -l`

    echo "-----"
    echo " "
    echo " "
    
    echo "Total number of errors   = $nErrors"
    echo "Total number of warnings = $nWarnings"
    echo "Output log file with compilation information : $makelog"

    echo "Total number of errors   = $nErrors" >> $makelog
    echo "Total number of warnings = $nWarnings" >> $makelog

    echo " "
    echo " "
    echo "-----"

}

source /home/usr/hvala/ari_hv_monitor/runRoot.sh

make_bash
