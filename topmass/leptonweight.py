#! /usr/bin/env python 

import sys

from ROOT import gROOT, TFile, TCanvas, TF1, TGraph, TPaveText, TLatex, TH1F, TLegend, TLine, TColor, TIter, gStyle
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
#gStyle.SetStripDecimals(kTRUE)
gStyle.SetTickLength(0.03, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetPadTickX(1)  
gStyle.SetPadTickY(1)

gStyle.SetPadTopMargin(0.1)
gStyle.SetPadBottomMargin(0.13)
gStyle.SetPadLeftMargin(0.17)
gStyle.SetPadRightMargin(0.09)

gStyle.SetTitleColor(1, "XYZ")
gStyle.SetTitleFont(42, "XYZ")
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetTitleXOffset(0.9)
gStyle.SetTitleYOffset(1.9)

gStyle.SetLabelColor(1, "XYZ")
gStyle.SetLabelFont(42, "XYZ")
gStyle.SetLabelOffset(0.007, "XYZ")
gStyle.SetLabelSize(0.04, "XYZ")


nlo = 0 

def GetIntegral( energy_muon, gr, integral ):
  tmp_integral = 0
  for nbin in range(1,nbins+1):
     energy_bincenter = energy_muon.GetBinCenter(nbin)
     energy_binconten = energy_muon.GetBinContent(nbin)
     weight_value = gr.Eval(energy_bincenter)
     tmp_integral = tmp_integral + weight_value*energy_binconten
  integral.append(tmp_integral)

import re

renbins = 10 

mass = ""

if len(sys.argv) == 1:
  mass = "173"
else:
  mass = sys.argv[1]

#histfile = TFile("hist_LO_reco_v5_20GeV.root","read")
histfile = TFile("hist_LO_"+mass+".root","read")
#histfile = TFile("hist_LO_res_60.root","read")

objectname = "lepton"
gen_histogram_name = "h_"+objectname+"_energy"
#reco_histogram_name = "h_"+objectname+"_energy_reco_final" 
reco_histogram_name = "h_"+objectname+"_energy_reco" 
energy_muon = histfile.Get( reco_histogram_name )
energy_muon_gen = histfile.Get( gen_histogram_name )
energy_muon.Rebin( renbins )
energy_muon_gen.Rebin( renbins )

#c_reco_muon = TCanvas("c_reco_muon","c_reco_muon",1)
energy_muon_reco = energy_muon.Clone("energy_muon_reco") 
#energy_muon_reco.Draw()
energy_muon_reco.SetFillColor(2)
energy_muon_reco.SetLineColor(2)
energy_muon_reco.GetXaxis().SetTitle("Energy (GeV)")
#c_reco_muon.Print("energy_reco_"+objectname+".pdf")

renbins_acc = renbins 
#resfile = TFile("hist_LO_"+mass+".root","read")
#resfile = TFile("hist_LO_res_v5_20GeV.root","read")
resfile = TFile("hist_LO_res_60.root","read")
#resfile = TFile("hist_LO_172.root","read")
res_gen = resfile.Get( gen_histogram_name )
res_reco = resfile.Get( reco_histogram_name )
#if mass == "173":
#  res_gen.Scale(1.0)
#  res_reco.Scale(1.0)
#else:
res_gen.Scale(0.5)
res_reco.Scale(0.5)
res_gen_origin = res_gen.Clone("res_gen_origin")
res_gen_origin.Rebin( renbins )
res_gen.Rebin( renbins_acc )
res_reco.Rebin( renbins_acc )

h_acc = res_gen.Clone("h_acc")

tmpn = h_acc.GetNbinsX()

for i in range(0, h_acc.GetNbinsX()):
  y_gen = res_gen.GetBinContent(i+1)
  y_reco = res_reco.GetBinContent(i+1)
  if i+1 <= 200/renbins_acc:
    h_acc.SetBinContent(i+1, 0)
  else:
    acc = 0
    if y_reco > 0:
      acc = y_reco / y_gen
    else:
      print "zero reco"
    h_acc.SetBinContent(i+1, acc)

#f5 = TF1("f5","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x+[6]*x*x*x*x*x*x",20,500);
#h_acc.Fit("f5","R")

for i in range(0,energy_muon.GetNbinsX()):
  y = energy_muon.GetBinContent(i+1)
  y_gen = res_gen_origin.GetBinContent(i+1)
  if i+1 <= 200/renbins:
    energy_muon.SetBinContent(i+1, y_gen)
  else:
    val = energy_muon.GetBinLowEdge(i+1)+energy_muon.GetBinWidth(i+1)/2.0
    #corr = 1.0/h_acc.GetBinContent(i+1)
    corr = 1.0/h_acc.Interpolate(val)
    #corr = 1.0/f5.Eval(val)
    energy_muon.SetBinContent(i+1, y*corr)


c_acceptance = TCanvas("c_acceptance","c_acceptance",1)
h_acc.Draw()
h_acc.GetXaxis().SetTitle("Energy (GeV)")
h_acc.SetTitle("Acceptance (reco/gen)")
c_acceptance.Print("acceptance_"+objectname+".pdf")

c_energy = TCanvas("c_energy","c_energy",1)
energy_muon.Draw()
energy_muon.SetStats(0)
#energy_muon.SetMarkerStyle(20)
#energy_muon.SetMarkerSize(0.5)
energy_muon.SetFillColor(4)
energy_muon.SetFillStyle(3004)
energy_muon.GetXaxis().SetTitle("Energy (GeV)")
#energy_muon.GetYaxis().SetTitle("Entries")
energy_muon.SetTitle("")
energy_muon_reco.Draw("same")

l = TLegend(0.6,0.65,0.8,0.8)
l.AddEntry(energy_muon,"Unfolded","F")
l.AddEntry(energy_muon_reco,"Reco.","F")
l.SetTextSize(0.05);
l.SetLineColor(0);
l.SetFillColor(0);
l.Draw()

if nlo :
  c_energy.Print("energy_NLO_"+objectname+"_"+mass+".pdf")
else :
  c_energy.Print("energy_LO_"+objectname+"_"+mass+".pdf")

nevents = energy_muon.Integral()
energy_muon.Scale(1.0/float(nevents))
nbins = energy_muon.GetNbinsX()

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
      GetIntegral( energy_muon, gr, integral2)
    elif nfile == 1:
      mymass3.append(float(gr.GetName()))
      GetIntegral( energy_muon, gr, integral3) 
    elif nfile == 2:
      mymass5.append(float(gr.GetName()))
      GetIntegral( energy_muon, gr, integral5)
    elif nfile == 3:
      mymass15.append(float(gr.GetName()))
      GetIntegral( energy_muon, gr, integral15)
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
  wlepton.Print("integralmass_NLO_"+objectname+"_"+mass+".pdf")
else :
  wlepton.Print("integralmass_LO_"+objectname+"_"+mass+".pdf")
