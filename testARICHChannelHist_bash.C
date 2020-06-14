/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Mon Oct 28 15:53:02 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

{
  gROOT->ProcessLine(".L libarihvmonitorDict.cxx");
  gROOT->ProcessLine(".L libarihvmonitorDict.so");
  testARICHChannelHist();
}

