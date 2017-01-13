#! /usr/bin/env python 

import sys

from ROOT import gROOT, TFile, TCanvas, TF1, TGraph, TPaveText, TLatex, TH1F, TLegend, TLine, TColor, TIter, gStyle
from ROOT import TGaxis
from array import array
import ROOT

gROOT.SetStyle("Plain")
gStyle.SetOptStat(1110)
gStyle.SetOptFit(1)
gStyle.SetStatW(0.25)
gStyle.SetStatH(0.15)

gStyle.SetCanvasDefH(150)
gStyle.SetCanvasDefW(150)

gStyle.SetAxisColor(1, "XYZ")
gStyle.SetStripDecimals(1)
gStyle.SetTickLength(0.03, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetPadTickX(1)  
gStyle.SetPadTickY(1)

gStyle.SetPadTopMargin(0.1)
gStyle.SetPadBottomMargin(0.13)
gStyle.SetPadLeftMargin(0.15)
gStyle.SetPadRightMargin(0.10)

gStyle.SetTitleColor(1, "XYZ")
gStyle.SetTitleFont(42, "XYZ")
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetTitleXOffset(0.9)
gStyle.SetTitleYOffset(1.6)

gStyle.SetLabelColor(1, "XYZ")
gStyle.SetLabelFont(42, "XYZ")
gStyle.SetLabelOffset(0.007, "XYZ")
gStyle.SetLabelSize(0.06, "XYZ")


nlo = 0 

def GetIntegral( energy_lepton_unfolded, gr, integral ):
  tmp_integral = 0
  for nbin in range(1,nbins+1):
     energy_bincenter = energy_lepton_unfolded.GetBinCenter(nbin)
     energy_binconten = energy_lepton_unfolded.GetBinContent(nbin)
     weight_value = gr.Eval(energy_bincenter)
     tmp_integral = tmp_integral + weight_value*energy_binconten
  integral.append(tmp_integral)

import re

mass = ""

if len(sys.argv) == 1:
  mass = "173"
else:
  mass = sys.argv[1]

histfile = TFile("hist_LO_final_unfolded_mod.root","read")

histogram_name = "h_unfold_"+mass
energy_lepton_unfolded = histfile.Get( histogram_name )

nevents = energy_lepton_unfolded.Integral()
energy_lepton_unfolded.Scale(1.0/float(nevents))
nbins = energy_lepton_unfolded.GetNbinsX()

files = []

f15 = TFile("weightfunctions2/weightfunc15.root","read")
f5 = TFile("weightfunctions2/weightfunc5.root","read")
f3 = TFile("weightfunctions2/weightfunc3.root","read")
f2 = TFile("weightfunctions2/weightfunc2.root","read")

files.append( f2 )
files.append( f3 )
files.append( f5 )
files.append( f15 )

integral2 = []
integral3 = []
integral5 = []
integral15 = []

mymass2 = []
mymass3 = []
mymass5 = []
mymass15 = []

nfile = 0
for f in files:
  
  dirlist =  f.GetListOfKeys()
  iter = dirlist.MakeIterator()
  key = iter.Next()
  while key :
    cl = gROOT.GetClass( key.GetClassName())
    if cl.InheritsFrom("TGraph") == 0: 
      continue
    gr = key.ReadObj();
    if nfile == 0:
      mymass2.append(float(gr.GetName()))
      GetIntegral( energy_lepton_unfolded, gr, integral2)
    elif nfile == 1:
      mymass3.append(float(gr.GetName()))
      GetIntegral( energy_lepton_unfolded, gr, integral3) 
    elif nfile == 2:
      mymass5.append(float(gr.GetName()))
      GetIntegral( energy_lepton_unfolded, gr, integral5)
    elif nfile == 3:
      mymass15.append(float(gr.GetName()))
      GetIntegral( energy_lepton_unfolded, gr, integral15)
    key = iter.Next()
  nfile = nfile + 1

wlepton = TCanvas("wlepton","wlepton",1)

final2 = TGraph()
final3 = TGraph()
final5 = TGraph()
final15 = TGraph()

final2_inv = TGraph()
final3_inv = TGraph()
final5_inv = TGraph()
final15_inv = TGraph()

nintegral = len(integral2)
nmymass = len(mymass2)

for i in range(0,nmymass):
  final2.SetPoint(i, mymass2[i], integral2[i])
  final3.SetPoint(i, mymass3[i], integral3[i])
  final5.SetPoint(i, mymass5[i], integral5[i])
  final15.SetPoint(i, mymass15[i], integral15[i])

  final2_inv.SetPoint(i, integral2[i], mymass2[i])
  final3_inv.SetPoint(i, integral3[i], mymass3[i])
  final5_inv.SetPoint(i, integral5[i], mymass5[i])
  final15_inv.SetPoint(i, integral15[i], mymass15[i])

final2.GetXaxis().SetTitle("m_{t} (GeV)")
final15.GetXaxis().SetTitle("m_{t} (GeV)")
final2.SetLineColor(ROOT.kGreen)
final3.SetLineColor(ROOT.kBlue)
final5.SetLineColor(ROOT.kRed)
final15.SetLineColor(ROOT.kOrange+1)
final2.SetLineWidth(2)
final3.SetLineWidth(2)
final5.SetLineWidth(2)
final15.SetLineWidth(2)

final2.Draw("AC")
final3.Draw("C")
final5.Draw("C")
final15.Draw("C")

reco_mass_n2 = final2_inv.Eval(0)
reco_mass_n3 = final3_inv.Eval(0)
reco_mass_n5 = final5_inv.Eval(0)
reco_mass_n15 = final15_inv.Eval(0)

print "reco mass for n=2 :", reco_mass_n2
print "reco mass for n=3 :", reco_mass_n3
print "reco mass for n=5 :", reco_mass_n5
print "reco mass for n=15 :", reco_mass_n15

linex = TLine(145,0,205,0);
linex.SetLineWidth(1)
linex.Draw()

liney = TLine(173,-0.00825,173,0.00725);
if mass == "167" :
  liney = TLine(173,-0.0096,173,0.0059);

if nlo :
  liney = TLine(173,-0.01025,173,0.0055);

liney.SetLineWidth(1)
liney.SetLineStyle(2)
liney.Draw()

l = TLegend(0.6,0.6,0.8,0.8)
if mass == "167" :
  l = TLegend(0.6,0.62,0.8,0.82)

if nlo :
  l = TLegend(0.6,0.65,0.8,0.85)
l.AddEntry(final2,"n = 2","L")
l.AddEntry(final3,"n = 3","L")
l.AddEntry(final5,"n = 5","L")
l.AddEntry(final15,"n = 15","L")
l.SetTextSize(0.05);
l.SetLineColor(0);
l.SetFillColor(0);
l.Draw()

if nlo :
  wlepton.Print("integralmass_NLO_lepton_"+mass+".pdf")
else :
  wlepton.Print("integralmass_LO_lepton_"+mass+".pdf")
