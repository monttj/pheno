#! /usr/bin/env python 

from ROOT import gROOT, TFile, TCanvas, TF1, TGraph, TPaveText, TLatex, TH1F, TLegend, TLine, TColor
from array import array
import ROOT

import re

files = []

f15 = open("Numtab_WF15_LOTopDecay_v2.m","r")
f5 = open("Numtab_WF5_LOTopDecay_v2.m","r")
f3 = open("Numtab_WF3_LOTopDecay_v2.m","r")
f2 = open("Numtab_WF2_LOTopDecay_v2.m","r")

files.append( f2 )
files.append( f3 )
files.append( f5 )
files.append( f15 )

gr2 = TGraph()
gr3 = TGraph()
gr5 = TGraph()
gr15 = TGraph()

mymass = []

c= TCanvas("c","c",1)
nfile = 0

for f in files:

  total = ""
  for line in f:
    total += line

  weights = re.split("{{", total)

  i = 0
  massidx = 0

  myenergy = [ [] ]
  myvalue = [ [] ]
  mymass = []

  mass = []
  energy = []
  value = [] 

  for weight in weights:
    if i == 0:
      i = i+1
      continue
 
    tmp = re.sub('{','',weight) 
    tmp1 = re.sub('}','',tmp) 
    tmp2 = re.sub('\n','',tmp1)
    tmp3 = re.sub(' ','',tmp2)
    tmp4 = re.sub('\*\^','1e',tmp3)
    out = tmp4.split(",") 

    print tmp
    print " " 
    print tmp1
    print " " 
    print tmp2
    print " " 
    print tmp4
    print " "
    print out
    
    mass.append( float(out[0]) )
    energy.append( float(out[1]) )
    value.append( float(out[2]) ) 

    if i == 1:
      mymass.append( float(out[0]) )  
    if i > 1 and mass[len(mass)-2] != mass[len(mass)-1]: 
      mymass.append( mass[len(mass)-1] )
      myenergy.append( [] )
      myvalue.append( [] )
      massidx = massidx + 1
      i= 1

    if i > 1800 :
      continue
    else:
      myenergy[massidx].append( float(out[1]) )
      myvalue[massidx].append( float(out[2]) )  

    i = i + 1

#print myenergy[0]
#print myvalue[0]

  energylen =  len(myenergy)
  masslen =  len(mymass)
  if energylen != masslen: 
    print "WRONG!!!!!!!!!!!!!!!!!!!!!"

  if nfile == 0:
    num = 2
  elif nfile == 1:
    num = 3
  elif nfile == 2:
    num = 5
  else:
    num = 15

  outname = "weightfunc"+str(num) + ".root"
  out = TFile( outname, "recreate")
  

  for j in range(0,massidx+1): 

    x = array('f',myenergy[j])
    y = array('f',myvalue[j])
    grname = str(mymass[j])
    gr = TGraph(len(x), x, y)
    gr.SetName(grname)
    gr.Write()

    if nfile == 0:
      if mymass[j] == 173:
        gr2 = TGraph(len(x), x, y)
        gr2.Draw("AC");
        gr2.SetLineColor(ROOT.kGreen)
        gr2.SetMaximum(0.3);
        gr2.SetMinimum(-1.0);
        gr2.GetXaxis().SetTitle("Energy (GeV)")
    elif nfile == 1:
      if mymass[j] == 173:
        gr3 = TGraph(len(x), x, y)
        gr3.Draw("C");
        gr3.SetLineColor(ROOT.kBlue)
    elif nfile == 2:
      if mymass[j] == 173:
        gr5 = TGraph(len(x), x, y)
        gr5.Draw("C");
        gr5.SetLineColor(ROOT.kRed)
    else:
      if mymass[j] == 173:
        gr15 = TGraph(len(x), x, y)
        gr15.Draw("C");
        gr15.SetLineColor(ROOT.kOrange+1)
  
  nfile = nfile + 1

line = TLine(0,0,147.5,0);
line.SetLineWidth(1)
line.Draw()

l = TLegend(0.65,0.2,0.85,0.5)
l.SetHeader("m_{t} = 173 GeV")
l.AddEntry(gr2,"n = 2","L")
l.AddEntry(gr3,"n = 3","L")
l.AddEntry(gr5,"n = 5","L")
l.AddEntry(gr15,"n = 15","L")
l.SetTextSize(0.05);
l.SetLineColor(0);
l.SetFillColor(0);
l.Draw()

c.Print("weightfunc.pdf")
