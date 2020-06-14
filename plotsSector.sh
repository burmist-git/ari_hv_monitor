#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Fri Nov  1 15:48:46 JST 2019                                         #
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

function plotsSector_sh {
    inFILE=$1
    dirName=`(basename $(dirname $inFILE))`
    inFILE_base=`(basename $inFILE)`
    secID=${inFILE_base:4:1}
    ###############################
    # get measurements ID (parID) #
    parArr=(
        vmon
        cmon
        mask
        vlimit
        climit
        vdemand
    )
    parArrMin=(
        0.0
       -1.0
       -1.0
        0.0
        0.0
        0.0
    )
    parArrMax=(
        400.0
        1.0
        2.0
        400.0
        10.0
        400.0
    )
    nn=${#parArr[@]}
    let nn=nn-1
    #echo "nn = $nn"
    for i in `seq 0 $nn`;
    do
        parArr_wc=`(echo "$inFILE_base" | grep ${parArr[$i]} | wc -l)`
	#echo "$parArr_wc"
	if [ $parArr_wc -eq 1 ] ; then 
	    parID="${parArr[$i]}"
	    parMin="${parArrMin[$i]}"
	    parMax="${parArrMax[$i]}"
	    #echo $parID
	    #echo $parMin
	    #echo $parMax
        fi
    done
    ###############################
    #
    #echo "inFILE         : "$inFILE
    #echo "inFILE_base    : "$inFILE_base
    #echo "secID          : "$secID
    #echo "dirName        : "$dirName
    #echo "plotsSector_sh : "$1
    mkdir -p $outPdf/$dirName
    outPdf=$outPdf/$dirName/`(basename $inFILE)`'.pdf'
    echo $outPdf
    $dataHome/plotsSector_main $inFILE $outPdf $secID $parMin $parMax $hapdTemplateMappingPath
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " -d    : default (crate ID)"
    echo "       : inRootfile"
    echo " -p2   : p2"
}

source /home/usr/hvala/ari_hv_monitor/runRoot.sh
dataHome='/home/usr/hvala/ari_hv_monitor/'
#dataHome='/home/burmist/home2/ari_hv_monitor/'
outRoot=$dataHome'/data_root/'
outPdf=$dataHome'/data_pdf/'
hapdTemplateMappingPath=$dataHome'/hapdTemplateMapping.dat';
mkdir -p $outPdf

if [ $# -eq 0 ]
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
        if [ $# -eq 2 ]
        then
	    inFILE=$2
	    if [ -f "$inFILE" ]; then
		plotsSector_sh $inFILE
	    else
		echo "ERROR --> file : $inFILE does not exist "
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

