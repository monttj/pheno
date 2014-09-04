#!/usr/bin/env python
# Author: Tae.Jeong.Kim@cern.ch
# Filename: analysis.py
# How to run: ./copy.py status mc ElEl
import os
import re
import sys
import time
import commands

dir="/home/cms/Prod_13TeV/Gridpacks/"

sample="TTsemilep_madspin_2"

cmd="lcg-ls --verbose -D  srmv2 --nobdii srm://sbgse1.in2p3.fr:8446/srm/managerv2\?SFN=/dpm/in2p3.fr"+dir+sample

fileNames = commands.getoutput(cmd).split("\n")

dest="/cms/home/tjkim/fcnc/sample/"+sample

os.system("mkdir "+dest)

for fileName in fileNames:
  if fileName.endswith("root") > 0:
    print "copying..." + fileName
    files = fileName.split("/")
    destfile = files[len(files)-1]
    os.system("lcg-cp -b -D srmv2 --vo cms  srm://sbgse1.in2p3.fr:8446/srm/managerv2\?SFN="+fileName+" "+dest+"/"+destfile)
