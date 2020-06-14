/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Sun Oct 27 19:34:15 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

{
  gROOT->ProcessLine(".L libarihvmonitorDict.cxx");
  gROOT->ProcessLine(".L libarihvmonitorDict.so");
  plots();    
}

