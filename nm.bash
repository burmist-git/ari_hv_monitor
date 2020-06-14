#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2017 - LBS - (Single person developer.)                 #
# Thu Oct 26 16:24:03 JST 2017                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                  This script prints list the functions in a shared   #
#                  library.                                            #
#                                                                      #
# Input paramete:                                                      #
#                  NON.                                                #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

function nm_bash {

    liblist=$1
    #liblist="./obj/*.so"
    #iblist="/home/leonid/home2/KEK/releases/head_22.04.2018/lib/Linux_x86_64/opt/libarich*"
    #liblist="/home/leonid/home2/KEK/releases/head_22.04.2018/lib/Linux_x86_64/opt/libarich_calibration.so"
    #liblist="/home/leonid/home2/KEK/releases/head_22.04.2018/lib/Linux_x86_64/opt/libarich_dataobjects.so"
    #liblist="/home/leonid/home2/KEK/releases/head_22.04.2018/lib/Linux_x86_64/opt/libarich.so"    
    
    for filename in $liblist ; do
	#nm -CD --defined-only $filename | grep -H --label= $filename 'test'
	#nm -CD --defined-only $filename | grep 'test::'
	#ff=$(nm -CD --defined-only $filename)
	echo " "
	echo "----> $filename"
	nm -CD --defined-only $filename | awk '{if($2=="T"){{print }}}' | grep -v _fini | grep -v _init | nl
	echo " "
    done
    
}

nm_bash "./obj/*.so"
nm_bash "*.so"
