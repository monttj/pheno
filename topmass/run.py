#! /usr/bin/env python
import os, sys, imp, re, pprint, string

fileList = [
"LO_167_sayaka_1",
"LO_167_sayaka_2",
"LO_167_sayaka_3",
"LO_170_sayaka_1",
"LO_170_sayaka_2",
"LO_170_sayaka_3",
"LO_173_sayaka_1",
"LO_173_sayaka_2",
"LO_173_sayaka_3",
"LO_173_sayaka_4",
"LO_173_sayaka_5",
"LO_173_sayaka_6",
"LO_176_sayaka_1",
"LO_176_sayaka_2",
"LO_176_sayaka_3",
"LO_179_sayaka_1",
"LO_179_sayaka_2",
"LO_179_sayaka_3",
]

for files in fileList:
  script = """#!/bin/bash 

/share/apps/root/bin/root -b -l -q '/home/tjkim/work/pheno/topmass/LeptonEnergy.C("%s")'
  """ %files


  scriptFileName = 'batchScript.sh'
  scriptFile = open(scriptFileName,'w')
  scriptFile.write(script)

  #print script

  os.system("qsub batchScript.sh")

os.system("qsub batchScript.sh") #somehow we need this line


