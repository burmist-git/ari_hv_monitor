#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Tue Oct 22 10:33:45 JST 2019                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                  Program used to compress raw data.                  #
#                                                                      #
# Input paramete:                                                      #
#                 NON.                                                 #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

LC_TIME=en_US.UTF-8

function compressRawDataFolderAndRemove {
    echo " --> compressRawDataFolderAndRemove"
    floderToCompressAndRemove=$(basename $1)
    tar -zcvf $archData/$floderToCompressAndRemove.tar.gz $outData/$floderToCompressAndRemove
    rm -rf $outData/$floderToCompressAndRemove
}

function unCompressRawDataFolder {
    echo " --> unCompressRawDataFolder"
    tgzToUnCompress=$(basename $1)
    dataDir=`echo $tgzToUnCompress | sed -e 's/.tar.gz//g'`
    echo $tgzToUnCompress
    echo $dataDir
    mkdir -p $outData/$dataDir/
    tar -xf $archData/$tgzToUnCompress --directory $outData/$dataDir/.
    dataPathDir=`find $outData/$dataDir/ -type d | tail -n1`
    mv $dataPathDir/* $outData/$dataDir/.
    dirToDelete=`find $outData/$dataDir/ -type d | head -n2 | tail -n1`
    echo $dirToDelete
    rm -rf $dirToDelete
}

#function deleteDataFromRootpdfsummaryFolderData {
#    echo " --> deleteDataFromRootpdfsummaryFolderData : "
#    find $rootpdfsummaryFolder/* -type f -ctime $maxDaysOld -exec ls -lrt {} \;
#    find $rootpdfsummaryFolder/* -type f -ctime $maxDaysOld -exec rm -rf {} \;
#    find $rootpdfsummaryFolder/* -type f -ctime $maxDaysOld -exec ls -lrt {} \;
#}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -d         : default - clean and compress old raw data"
    echo " [1] (optional) : name of the folder to convert"
    echo " [0] -uc        : un compress raw data"
    echo " [1]            : name of the tar.gz file to unpack"
    echo " [0] -h         : print help"
}

dataHome='/home/usr/hvala/ari_hv_monitor/'
#dataHome='/home/burmist/home2/ari_hv_monitor/'
outData=$dataHome'/data/'
outData_pdf=$dataHome'/data_pdf/'
outData_root=$dataHome'/data_root/'
archData=$dataHome'/archive/'
maxDaysOld=14
echo "$archData"
mkdir -p $archData

if [ $# -eq 0 ] 
then    
    printHelp 
else
    if [ "$1" = "-d" ]; then
	if [ $# -eq 2 ]
        then
	    compressRawDataFolderAndRemove $2
        elif [ $# -eq 1 ]; then
	    dirlist=`find $outData/* -type d -ctime +$maxDaysOld | xargs`
	    for folderToProcess in $dirlist
	    do	    
		echo "folderToProcess = $folderToProcess"
		compressRawDataFolderAndRemove $folderToProcess
	    done
	    dirlist_pdf=`find $outData_pdf/* -type d -ctime +$maxDaysOld | xargs`
	    for folderToProcess_pdf in $dirlist_pdf
	    do	    
		echo "folderToProcess_pdf = $folderToProcess_pdf"
		rm -rf $folderToProcess_pdf
	    done
	    dirlist_root=`find $outData_root/* -type d -ctime +$maxDaysOld | xargs`
	    for folderToProcess_root in $dirlist_root
	    do	    
		echo "folderToProcess_root = $folderToProcess_root"
		rm -rf $folderToProcess_root
	    done
        else
            printHelp
        fi
    elif [ "$1" = "-uc" ]; then
	if [ $# -eq 2 ]
        then
	    unCompressRawDataFolder $2
        else
            printHelp
        fi
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi    
fi
