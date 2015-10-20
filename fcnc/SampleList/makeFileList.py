#!/usr/bin/env python
# Author: Tae.Jeong.Kim@cern.ch
import os
import re
import sys
import time
import commands

prefix = "dcap://maite.iihe.ac.be:/"
dir = "/pnfs/iihe/cms/store/user/dhondt/FCN_13TeV_IIHE_IPHC/Gridpacks/"
process = "SingleTop_t_madspin"
outfile = open(process+".txt","w")

files = os.listdir(dir + process) 

for file in files:
  if file.find("matching") != -1:
    continue
  if file.find("root") != -1:
    out = prefix+dir+process+"/"+file
    outfile.write(out+"\n")
    print out 


outfile.close()
