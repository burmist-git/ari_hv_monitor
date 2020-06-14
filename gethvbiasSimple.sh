#!/bin/bash

cd /home/usr/hvala/daq_slc/; source setenv;
nsmvget ARICH_HV crate[1].slot[0].channel[34].climit -c /home/usr/hvala/daq_slc/data/hvcontrol/arich.conf | tee /home/usr/hvala/ari_hv_monitor/data/$(date +%d.%m.20%y-%H.%M.%S-%s.climit)
