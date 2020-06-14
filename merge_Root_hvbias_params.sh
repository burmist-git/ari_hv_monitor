#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Thu Oct 24 19:31:45 JST 2019                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                                                                      #
# Input paramete:                                                      #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

#1,11,21,31,41,51 * * * * /home/usr/hvala/ari_hv_monitor/get_hvbias_info.sh -secI 1 
#2,12,22,32,42,52 * * * * /home/usr/hvala/ari_hv_monitor/get_hvbias_info.sh -secI 2
#3,13,23,33,43,53 * * * * /home/usr/hvala/ari_hv_monitor/get_hvbias_info.sh -secI 3
#4,14,24,34,44,54 * * * * /home/usr/hvala/ari_hv_monitor/get_hvbias_info.sh -secI 4
#5,15,25,35,45,55 * * * * /home/usr/hvala/ari_hv_monitor/get_hvbias_info.sh -secI 5
#6,16,26,36,46,56 * * * * /home/usr/hvala/ari_hv_monitor/get_hvbias_info.sh -secI 6
#8,18,28,38,48,58 * * * * /home/usr/hvala/ari_hv_monitor/merge_Root_hvbias_params.sh -d

LC_TIME=en_US.UTF-8
LANG=en_US.UTF-8

function marge_Root_hvbias_params_sh {
    inName=$1
    unixTimeMin=$2
    unixTimeMax=$3
    inFNameDir=`(basename $inName)`
    echo $inName
    echo $inFNameDir
    #secID=${inFName: 4:1}
    #echo $secID
    #echo $unixTime
    #parArr=(
    #    vmon
    #)
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
    #lengthArr=(
    #    4
    #)
    nn=${#parArr[@]}
    let nn=nn-1
    #let nn=0
    #echo "nn = $nn"
    parsufix=""
    for i in `seq 0 $nn`;
    do
	parSufix=${parArr[$i]}
	listFileName=$outRoot/$inFNameDir/$parSufix'.list'
	rm -rf $listFileName
	for inRootFile in `ls -lrt $outRoot/$inFNameDir/*.root | grep $parSufix | awk '{print $9}' | grep .root | xargs `;
	do
	    inRootFileShort=`(basename $inRootFile)`
	    secID=`echo $inRootFileShort | sed -e 's/_/ /g' | awk '{print $2}'`
	    unixTime=`echo $inRootFileShort | sed -e 's/-/ /g'| sed -e "s/.$parSufix/ /g" | awk '{print $3}'`
	    if [ $unixTime -ge $unixTimeMin ] ; then
		if [ $unixTime -le $unixTimeMax ] ; then
		    echo "$secID $parSufix $unixTime $inRootFileShort" | tee -a $listFileName
		fi
	    fi
	done
	mkdir -p $outRootPdfSummary/$inFNameDir
	$dataHome/merge_Root_hvbias_params_main $listFileName $outRoot/$inFNameDir/ $outRootPdfSummary/$inFNameDir/ $hapdTemplateMappingPath
	#echo "inFNameDir --> $inFNameDir"
	source $dataHome/plotsMergedData.sh -d $inFNameDir
    done
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " -d    : default (crate ID)"
    echo "       : (optional) directory with root files"
    echo "       : (optional) unix time min"
    echo "       : (optional) unix time max"
    echo " -p2   : p2"
}

source /home/usr/hvala/ari_hv_monitor/runRoot.sh
dataHome='/home/usr/hvala/ari_hv_monitor/'
#dataHome='/home/burmist/home2/ari_hv_monitor/'
outData=$dataHome'/data/'
outRoot=$dataHome'/data_root/'
outRootPdfSummary=$dataHome'/root_pdf_summary/'
crateToModuleMap="$dataHome/mappingInfo/crateToModuleMap.info"
hapdTemplateMappingPath=$dataHome'/hapdTemplateMapping.dat'
timeWindowToAverage_short=600
timeWindowToAverage_long=3600

mkdir -p $outRootPdfSummary

if [ $# -eq 0 ]
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
	if [ $# -eq 4 ] ; then
	    unixTimeMin=$3
	    unixTimeMax=$4
	    marge_Root_hvbias_params_sh $2 $unixTimeMin $unixTimeMax
	elif [ $# -eq 2 ] ; then
	    unixTimeMax=`(date +%s)`
	    let unixTimeMin=$unixTimeMax-$timeWindowToAverage_short
	    #echo "unixTimeMin $unixTimeMin"
	    #echo "unixTimeMax $unixTimeMax"
	    marge_Root_hvbias_params_sh $2 $unixTimeMin $unixTimeMax
	    let unixTimeMin=$unixTimeMax-$timeWindowToAverage_long
	    marge_Root_hvbias_params_sh $2 $unixTimeMin $unixTimeMax
	elif [ $# -eq 1 ] ; then
	    infolder=`(date +%d.%m.20%y)`
	    unixTimeMax=`(date +%s)`
	    let unixTimeMin=$unixTimeMax-$timeWindowToAverage_short
	    #echo "unixTimeMin $unixTimeMin"
	    #echo "unixTimeMax $unixTimeMax"
	    marge_Root_hvbias_params_sh $infolder $unixTimeMin $unixTimeMax
	    let unixTimeMin=$unixTimeMax-$timeWindowToAverage_long
	    marge_Root_hvbias_params_sh $infolder $unixTimeMin $unixTimeMax
	else
            printHelp
	fi
    elif [ "$1" = "-p2" ]; then
        printHelp
    else
        printHelp
    fi
fi

#espeak "I have done"
