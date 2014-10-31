#!/usr/bin/env python
# Author: Tae.Jeong.Kim@cern.ch
# Filename: analysis.py
# How to run: ./copy.py status mc ElEl
import os
import re
import sys
import time
import commands

#voms-proxy-init -voms cms
dirBrussels="/pnfs/iihe/cms/store/user/dhondt/FCN_13TeV_IIHE_IPHC/Gridpacks/"

dir="/home/cms/Prod_13TeV/Gridpacks/"

samples=["TTsemilep_madspin_1"]
#samples = ["ZToLL50-0Jet_sm-no_masses"]
#samples = ["HToAA"]
#samples = ["TTdilep_madspin"]
#samples = ["WToLNu-2Jets_sm-no_masses"]
#samples = ["ZToLL50-2Jets_sm-no_masses"]
#samples = ["AA"]
#samples = ["WHToAA"]
#samples += ["ZHToAA"]

for sample in samples:

  cmd="lcg-ls --verbose -D  srmv2 --nobdii srm://sbgse1.in2p3.fr:8446/srm/managerv2\?SFN=/dpm/in2p3.fr"+dir+sample
#cmd = "lcg-ls --verbose -D srmv2 --nobdii srm://maite.iihe.ac.be:8443/srm/managerv2\?SFN="
  fileNames = commands.getoutput(cmd).split("\n")

  dest="/cms/home/tjkim/fcnc/sample/"+sample

  os.system("mkdir "+dest)

  for fileName in fileNames:
    print fileName
    if fileName.endswith("root") > 0:
      print "copying..." + fileName
      files = fileName.split("/")
      destfile = files[len(files)-1]
      os.system("lcg-cp -b -D srmv2 --vo cms  srm://sbgse1.in2p3.fr:8446/srm/managerv2\?SFN="+fileName+" "+dest+"/"+destfile)
