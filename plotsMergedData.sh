#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2020 - LBS - (Single person developer.)                 #
# Mon Jan 27 12:54:25 CET 2020                                         #
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

function plotsMergedData_sh {
    inFILE=$1
    #dirName=`(basename $(dirname $inFILE))`
    inFILE_base=`(basename $inFILE)`
    #secID=${inFILE_base:4:1}
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
    outPdf=$inFILE'.pdf'
    echo $outPdf
    if [ ! -f "$outPdf" ]; then
	$dataHome/plotsMergedData_main $inFILE $outPdf $parMin $parMax $hapdTemplateMappingPath
    fi
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " -d    : default (crate ID)"
    echo "       : inRootPdfSummaryFolder"
    echo " -p2   : p2"
}

source /home/usr/hvala/ari_hv_monitor/runRoot.sh
dataHome='/home/usr/hvala/ari_hv_monitor/'
#dataHome='/home/burmist/home2/ari_hv_monitor/'
outRoot=$dataHome'/data_root/'
outPdf=$dataHome'/data_pdf/'
rootPdfSummary=$dataHome'/root_pdf_summary/'
hapdTemplateMappingPath=$dataHome'/hapdTemplateMapping.dat';
mkdir -p $outPdf

if [ $# -eq 0 ]
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
        if [ $# -eq 2 ] ; then
	    inFolder=$2
	    for inFILE in `ls -lrt $rootPdfSummary/$inFolder/*.root | awk '{print $9}' | xargs`;
	    do
		if [ -f "$inFILE" ]; then
		    echo $inFILE
		    plotsMergedData_sh $inFILE
		else
		    echo "ERROR --> file : $inFILE does not exist "
		fi
	    done
        else
            printHelp
        fi
    elif [ "$1" = "-p2" ]; then
            printHelp
    else
        printHelp
    fi
fi
