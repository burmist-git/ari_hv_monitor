#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2018 - LBS - (Single person developer.)                 #
# Thu Oct 17 15:50:28 CEST 2019                                        #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                                                                      #
# Input paramete:                                                      #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

#crate   : 1 -  7 
#slot    : 0 - 14 
#channel : 0 - 47 
#crate[1].slot[0].channel[0].channel     : int get 
#crate[1].slot[0].channel[0].climit      : float get set 
#crate[1].slot[0].channel[0].cmon        : float get 
#crate[1].slot[0].channel[0].mask        : text get set 
#crate[1].slot[0].channel[0].modid       : int get 
#crate[1].slot[0].channel[0].pdown       : int get set 
#crate[1].slot[0].channel[0].pon         : int get set 
#crate[1].slot[0].channel[0].rampdown    : float get set 
#crate[1].slot[0].channel[0].rampup      : float get set 
#crate[1].slot[0].channel[0].sector      : int get 
#crate[1].slot[0].channel[0].serial      : text get 
#crate[1].slot[0].channel[0].slot        : int get 
#crate[1].slot[0].channel[0].state       : text get 
#crate[1].slot[0].channel[0].switch      : text get set 
#crate[1].slot[0].channel[0].trip        : float get set 
#crate[1].slot[0].channel[0].type        : text get 
#crate[1].slot[0].channel[0].vdemand     : float get set 
#crate[1].slot[0].channel[0].vlimit      : float get set 
#crate[1].slot[0].channel[0].vmon        : float get 

function getHVBiasInfo {
    cd $daq_slc_Home;
    source setenv;
    cd $dataHome;
    crateID=$1
    parameterName=$2
    echo "crateID       = $crateID"
    echo "parameterName = $parameterName"
    #
    #nsmvget ARICH_HV crate[$crateID].slot[0].channel[0].$parameterName
    cmd="nsmvget ARICH_HV "
    pvList=""
    for slotID in `(seq $slotMin $slotMax)`
    do
	for channelID in `(seq $channelMin $channelMax)`
	do
	    pvList=$pvList" crate[$crateID].slot[$slotID].channel[$channelID].$parameterName"
	done
    done
    nsmvget ARICH_HV $pvList -c $daq_slc_Home/data/hvcontrol/arich.conf
}

function getScriptForHVBiasInfoForAllSectors { 
    #vmon
    #cmon
    #mask
    #vlimit
    #climit
    #vdemand
    scriptName=""
    sleepSecond=5
    for secID in `(seq $secMin $secMax)`
    do
	crateToModuleMapName=$crateToModuleMap"_sec_"$secID".short.sort"
        #
	scriptName_vmon=$dataHome/"get_hvbias_info_cron_vmon_sec_$secID.sh"
	scriptName_cmon=$dataHome/"get_hvbias_info_cron_cmon_sec_$secID.sh"
	scriptName_mask=$dataHome/"get_hvbias_info_cron_mask_sec_$secID.sh"
	scriptName_vlimit=$dataHome/"get_hvbias_info_cron_vlimit_sec_$secID.sh"
	scriptName_climit=$dataHome/"get_hvbias_info_cron_climit_sec_$secID.sh"
	scriptName_vdemand=$dataHome/"get_hvbias_info_cron_vdemand_sec_$secID.sh"
        #
	rm -rf $scriptName_vmon
	rm -rf $scriptName_cmon
	rm -rf $scriptName_mask
	rm -rf $scriptName_vlimit
	rm -rf $scriptName_climit
	rm -rf $scriptName_vdemand
	#
	listpar_vmon=`(more $crateToModuleMapName | grep -v hv | awk '{print "crate["$4"].slot["$6"].channel["$8"].vmon"}' | xargs)`
	listpar_cmon=`(more $crateToModuleMapName | grep -v hv | awk '{print "crate["$4"].slot["$6"].channel["$8"].cmon"}' | xargs)`
	listpar_mask=`(more $crateToModuleMapName | grep -v hv | awk '{print "crate["$4"].slot["$6"].channel["$8"].mask"}' | xargs)`
	listpar_vlimit=`(more $crateToModuleMapName | grep -v hv | awk '{print "crate["$4"].slot["$6"].channel["$8"].vlimit"}' | xargs)`
	listpar_climit=`(more $crateToModuleMapName | grep -v hv | awk '{print "crate["$4"].slot["$6"].channel["$8"].climit"}' | xargs)`
	listpar_vdemand=`(more $crateToModuleMapName | grep -v hv | awk '{print "crate["$4"].slot["$6"].channel["$8"].vdemand"}' | xargs)`
        #
	echo "$scriptName_vmon"
	echo "cd $daq_slc_Home; source setenv; cd $dataHome; " | tee -a $scriptName_vmon
	echo " " | tee -a $scriptName_vmon
	echo "nsmvget ARICH_HV $listpar_vmon -c $daq_slc_Home/data/hvcontrol/arich.conf" | tee -a $scriptName_vmon
	echo "sleep $sleepSecond" | tee -a $scriptName_vmon
	chmod u+x $scriptName_vmon
        #
	echo "$scriptName_cmon"
	echo "cd $daq_slc_Home; source setenv; cd $dataHome; " | tee -a $scriptName_cmon
	echo " " | tee -a $scriptName_cmon
	echo "nsmvget ARICH_HV $listpar_cmon -c $daq_slc_Home/data/hvcontrol/arich.conf" | tee -a $scriptName_cmon
	echo "sleep $sleepSecond" | tee -a $scriptName_cmon
	chmod u+x $scriptName_cmon
        #
	echo "$scriptName_mask"
	echo "cd $daq_slc_Home; source setenv; cd $dataHome; " | tee -a $scriptName_mask
	echo " " | tee -a $scriptName_mask
	echo "nsmvget ARICH_HV $listpar_mask -c $daq_slc_Home/data/hvcontrol/arich.conf" | tee -a $scriptName_mask
	echo "sleep $sleepSecond" | tee -a $scriptName_mask
	chmod u+x $scriptName_mask
        #
	echo "$scriptName_vlimit"
	echo "cd $daq_slc_Home; source setenv; cd $dataHome; " | tee -a $scriptName_vlimit
	echo " " | tee -a $scriptName_vlimit
	echo "nsmvget ARICH_HV $listpar_vlimit -c $daq_slc_Home/data/hvcontrol/arich.conf" | tee -a $scriptName_vlimit
	echo "sleep $sleepSecond" | tee -a $scriptName_vlimit
	chmod u+x $scriptName_vlimit
        #
	echo "$scriptName_climit"
	echo "cd $daq_slc_Home; source setenv; cd $dataHome; " | tee -a $scriptName_climit
	echo " " | tee -a $scriptName_climit
	echo "nsmvget ARICH_HV $listpar_climit -c $daq_slc_Home/data/hvcontrol/arich.conf" | tee -a $scriptName_climit
	echo "sleep $sleepSecond" | tee -a $scriptName_climit
	chmod u+x $scriptName_climit
        #
	echo "$scriptName_vdemand"
	echo "cd $daq_slc_Home; source setenv; cd $dataHome; " | tee -a $scriptName_vdemand
	echo " " | tee -a $scriptName_vdemand
	echo "nsmvget ARICH_HV $listpar_vdemand -c $daq_slc_Home/data/hvcontrol/arich.conf" | tee -a $scriptName_vdemand
	echo "sleep $sleepSecond" | tee -a $scriptName_vdemand
	chmod u+x $scriptName_vdemand
    done 
}

function getHVBiasInfoPerSector {
    sectorID=$1
    dataOutPerDay=$(date +%d.%m.20%y)
    mkdir -p $outData/$dataOutPerDay
    #
    #vmon
    #cmon
    #mask
    #vlimit
    #climit
    #vdemand
    #
    parArr=(
	vmon
	cmon
	mask
	vlimit
	climit
	vdemand
    )
    nn=${#parArr[@]}
    let nn=nn-1
    echo "nn = $nn"
    for i in `seq 0 $nn`;
    do
	outDataFile=$outData/$dataOutPerDay/"sec_"$sectorID"_"$(date +%d.%m.20%y-%H.%M.%S-%s.${parArr[$i]})
	$dataHome/"get_hvbias_info_cron_"${parArr[$i]}"_sec_"$sectorID".sh" | tee $outDataFile
	source $dataHome/convert_hvbias_params.sh -d $outDataFile
	inRootFile=`(basename $outDataFile)`'.root'
	source $dataHome/plotsSector.sh -d $outRoot/$dataOutPerDay/$inRootFile
    done

}

function makeCrateToModuleMap {
    sectorID=$1
    file_map=$crateToModuleMap"_sec_$sectorID"
    rm -rf $file_map
    rm -rf $file_map'.short'
    moduleIDlist=`(awk -v secID=$sectorID -f $dataHome/moduleID.awk $mappingInfoModule)`
    for moduleID in $moduleIDlist
    do
	#echo $moduleID
	hapdName=`(more mappingInfo/sensorMap.info | grep hapd | sed -e 's/,//g' | awk -v moduleID=$moduleID -f $dataHome/sensorMap.awk)`
	biasHVlist=`(grep $hapdName $mappingInfoSerial | sed -e 's/.serial//g' | awk '{print $1}' | xargs)`
	nch=`(echo $biasHVlist | wc -w)`
	for biashv_channel in $biasHVlist
	do
	    #echo "$moduleID $hapdName $biashv_channel $nch" | tee -a $file_map
	    echo "$moduleID $hapdName $biashv_channel" | tee -a $file_map
	done
    done
    more $file_map | sed -e 's/\[/ /g' | sed -e 's/\]./ /g' | sed 's/\]//g' | tee -a $file_map'.short'
    #awk -v crateID=1 -v slotID=0 -v channelID=0 -f biashv_type.awk mappingInfo/biashv.type.short.info
}

function getModuleNameIDPerSector {
    sectorID=$1
    moduleIDlist=`(awk -v secID=$sectorID -f $dataHome/moduleID.awk $mappingInfoModule)`
    for moduleID in $moduleIDlist
    do
	hapdName=`(more mappingInfo/sensorMap.info | grep hapd | sed -e 's/,//g' | awk -v moduleID=$moduleID -f $dataHome/sensorMap.awk)`
	echo "$sectorID $moduleID $hapdName" | tee -a $moduleNameIDMap
    done
}

function stripSerialInfo {
    rm -rf $mappingInfoSerialShort
    more $mappingInfoSerial | grep crate | grep slot | grep channel | sed -e 's/crate\[/ /g' | sed -e 's/\].slot\[/ /g' | sed -e 's/\].channel\[/ /g' | sed -e 's/\].serial/ /g' | tee -a $mappingInfoSerialShort
}

function stripTypeInfo {
    rm -rf $mappingInfoTypeShort
    more $mappingInfoType | grep crate | grep slot | grep channel | sed -e 's/crate\[/ /g' | sed -e 's/\].slot\[/ /g' | sed -e 's/\].channel\[/ /g' | sed -e 's/\].type/ /g' | tee -a $mappingInfoTypeShort
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " -c    : default (crate ID)"
    echo "       : crate ID (crate : 1 -  7)"
    echo "       : parameter name (channel climit cmon mask modid pdown pon rampdown rampup sector serial slot state switch trip type vdemand vlimit vmon)"
    echo " -a    : all crates"
    echo "       : parameter name (channel climit cmon mask modid pdown pon rampdown rampup sector serial slot state switch trip type vdemand vlimit vmon)"
    echo " -m    : get mapping files"
    echo " -secI : get HV information for a given sector"
    echo "       : sector ID (sector : 1 - 6)"
    echo " -secS : get script for getting HV information for all sectors"
    echo " -secN : get module name for a given sector"
    echo "       : sector ID (sector : 1 - 6)"
    echo " -secM : make crate to module map and strip it"
    echo " -strS : strip serial info"
    echo " -strT : strip type info"
}

dataHome='/home/usr/hvala/ari_hv_monitor/'
#dataHome='/home/burmist/home2/ari_hv_monitor/'
outData=$dataHome'/data/'
outRoot=$dataHome'/data_root/'
daq_slc_Home='/home/usr/hvala/daq_slc/'
mappingInfoType="$dataHome/mappingInfo/biashv.type.info"
mappingInfoSerial="$dataHome/mappingInfo/biashv.serial.info"
mappingInfoTypeShort="$dataHome/mappingInfo/biashv.type.short.info"
mappingInfoSerialShort="$dataHome/mappingInfo/biashv.serial.short.info"
mappingInfoSensor="$dataHome/mappingInfo/sensorMap.info"
mappingInfoModule="$dataHome/mappingInfo/moduleID.info"
crateToModuleMap="$dataHome/mappingInfo/crateToModuleMap.info"
moduleNameIDMap="$dataHome/mappingInfo/moduleNameIDMap.info"

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
    if [ "$1" = "-c" ]; then
	if [ $# -eq 3 ] 
	then
	    crateID=$2
	    parameterName=$3
	    getHVBiasInfo $crateID $parameterName
	else
            printHelp
	fi
    elif [ "$1" = "-a" ]; then
	if [ $# -eq 2 ] 
	then
	    parameterName=$2
	    for crateID in `(seq $crateMin $crateMax)`
	    do
		getHVBiasInfo $crateID $parameterName
	    done
	else
            printHelp
	fi
    elif [ "$1" = "-m" ]; then
	rm -rf $mappingInfoType
	rm -rf $mappingInfoSerial
	parameterName1=type
	parameterName2=serial
	for crateID in `(seq $crateMin $crateMax)`
	do
	    getHVBiasInfo $crateID $parameterName1 | tee -a $mappingInfoType
	    getHVBiasInfo $crateID $parameterName2 | tee -a $mappingInfoSerial
	done
    elif [ "$1" = "-secI" ]; then
	if [ $# -eq 2 ] 
	then
	    secID=$2
	    getHVBiasInfoPerSector $secID
	else
            printHelp
	fi
    elif [ "$1" = "-secS" ]; then
	if [ $# -eq 1 ]
	then
	    getScriptForHVBiasInfoForAllSectors
	else
            printHelp
	fi
    elif [ "$1" = "-secN" ]; then
	if [ $# -eq 2 ] 
	then
	    secID=$2
	    getModuleNameIDPerSector $secID
	else
	    rm -rf $moduleNameIDMap
	    for secID in `(seq $secMin $secMax)`
	    do
		getModuleNameIDPerSector $secID
	    done
	fi
    elif [ "$1" = "-secM" ]; then
	#makeCrateToModuleMap 1
	for secID in `(seq $secMin $secMax)`
	do
	    makeCrateToModuleMap $secID
	done
    elif [ "$1" = "-strS" ]; then
	stripSerialInfo
    elif [ "$1" = "-strT" ]; then
	stripTypeInfo
    else
        printHelp
    fi
fi
