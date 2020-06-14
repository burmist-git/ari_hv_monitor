#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Mon Oct 21 18:39:32 JST 2019                                         #
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

function convert_hvbias_params_sh {
    inFNameFull=$1
    #inFNameFull=/home/usr/hvala/ari_hv_monitor/data/21.10.2019/sec_1_21.10.2019-13.51.01-1571633461.vmon
    #inFNameFull=/home/usr/hvala/ari_hv_monitor/data/21.10.2019/sec_6_21.10.2019-20.36.06-1571657766.cmon
    inFNameDir=`(basename $(dirname $inFNameFull))`
    inFName=`(basename $inFNameFull)`
    inFNameShort=""
    #echo $inFName
    secID=${inFName: 4:1}
    #echo $secID
    #echo $unixTime
    parArr=(
        vmon
        cmon
        mask
        vlimit
        climit
        vdemand
    )
    lengthArr=(
        4
        4
        4
        6
        6
        7
    )
    nn=${#parArr[@]}
    let nn=nn-1
    #echo "nn = $nn"
    parsufix=""
    for i in `seq 0 $nn`;
    do
	ww=${lengthArr[$i]}
	parSufix=${parArr[$i]}
	sufix="${inFName: -$ww}"
	if [ "$sufix" = "$parSufix" ] ; then
	    parsufix=$parSufix
	    inFNameShort="${inFName:0:${#inFName}-$ww-1}"
	    #echo $inFNameShort
	    unixTime="${inFNameShort: -10}"
	fi
    done
    #
    dateMeas=`date --date @$unixTime | sed 's/ /-/g'`
    #echo "secID    $secID"
    #echo "unixTime $unixTime"
    #echo "dateMeas $dateMeas"
    inCrateToModuleMap=$crateToModuleMap"_sec_"$secID".short.sort"
    #
    parlist=`more $inFNameFull | grep crate | grep slot | grep channel |grep $parsufix | sed -e 's/\[/ /g'| sed -e 's/\]/ /g' | awk '{print $2" "$4" "$6" "$9}' | xargs`
    #
    mkdir -p $outRoot/$inFNameDir
    outRootFile=$outRoot/$inFNameDir/$inFName".root"
    $dataHome/convert_hvbias_params_main $secID $unixTime $dateMeas $inCrateToModuleMap $parsufix $outRootFile $parlist
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " -d    : default (crate ID)"
    echo "       : infile"
    echo " -p2   : p2"
}

source /home/usr/hvala/ari_hv_monitor/runRoot.sh
dataHome='/home/usr/hvala/ari_hv_monitor/'
#dataHome='/home/burmist/home2/ari_hv_monitor/'
outData=$dataHome'/data/'
outRoot=$dataHome'/data_root/'
crateToModuleMap="$dataHome/mappingInfo/crateToModuleMap.info"

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
	    wc_l=`(du -hs $2 | sed 's/4.0K/0/g' | awk '$1 !=0 {print "source convert_hvbias_params.sh -d "$2}' | wc -l)`
	    if [ "$wc_l" = "1" ]; then
		convert_hvbias_params_sh $2
	    else
		echo "File is not complete"
		du -hs $2
	    fi
	else
            printHelp
	fi
    elif [ "$1" = "-p2" ]; then
            printHelp
    else
        printHelp
    fi
fi
