#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Wed Oct 23 20:09:22 JST 2019                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                                                                      #
# Input paramete:                                                      #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

LC_TIME=en_US.UTF-8
LANG=en_US.UTF-8

function convert_hvbias_params_by_hand_sh {
    inFolder=$1
    outScript=$2
    du -hs data/$inFolder/* | sed 's/4.0K/0/g' | awk '$1 !=0 {print "source convert_hvbias_params.sh -d /home/usr/hvala/ari_hv_monitor/"$2}' | tee $outScript
    #du -hs data/$inFolder/* | sed 's/4.0K/0/g' | awk '$1 !=0 {print "source convert_hvbias_params.sh -d /home/burmist/home2/ari_hv_monitor/"$2}' | tee $outScript
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " -d  : default (crate ID)"
    echo "     : in raw data folder"
    echo " -p2 : p2"
}

source /home/usr/hvala/ari_hv_monitor/runRoot.sh
dataHome='/home/usr/hvala/ari_hv_monitor/'
#dataHome='/home/burmist/home2/ari_hv_monitor/'
outData=$dataHome'/data/'
outRoot=$dataHome'/data_root/'
crateToModuleMap="$dataHome/mappingInfo/crateToModuleMap.info"
outScript=$dataHome'/convert_hvbias_folder.sh'

crateMin=1
crateMax=7
slotMin=0
slotMax=14
channelMin=0
channelMax=47
secMin=1
secMax=6

if [ $# -eq 0 ]
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
	if [ $# -eq 2 ]
	then
	    rndb=`cat /dev/urandom | tr -cd 'a-f0-9' | head -c 10`
	    outScriptToRun=$outScript$rndb
	    convert_hvbias_params_by_hand_sh $2 $outScriptToRun
	    #echo $2
	    #echo $outScriptToRun
	    chmod u+x $outScriptToRun
	    $outScriptToRun
	else
            printHelp
	fi
    elif [ "$1" = "-p2" ]; then
            printHelp
    else
        printHelp
    fi
fi
