#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Sun Oct 20 06:58:56 CST 2019                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                                                                      #
# Input paramete:                                                      #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

function make_hvbias_map_sh {
    for secID in `(seq $secMin $secMax)`
    do
	crateToModuleMapName=$crateToModuleMap"_sec_"$secID".short"
	ari_sec_id=$secID
	biashvTypeLog="$dataHome/mappingInfo/biashv.type.short.info"
	moduleNameIDMap="$dataHome/mappingInfo/moduleNameIDMap.info"
	$dataHome/make_hvbias_map $crateToModuleMapName $ari_sec_id $biashvTypeLog $moduleNameIDMap | tee $crateToModuleMapName'.sort'
    done
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " -d  : default "
    echo " -p2 : parameters"
}

dataHome='/home/usr/hvala/ari_hv_monitor/'
#dataHome='/home/burmist/home2/ari_hv_monitor/'
daq_slc_Home='/home/usr/hvala/daq_slc/'
crateToModuleMap="$dataHome/mappingInfo/crateToModuleMap.info"

secMin=1
secMax=6

if [ $# -eq 0 ]
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
	make_hvbias_map_sh
    elif [ "$1" = "-p2" ]; then
        echo "p2"
    else
        printHelp
    fi
fi
