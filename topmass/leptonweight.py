#! /usr/bin/env python 

from ROOT import gROOT, TFile, TCanvas, TF1, TGraph, TPaveText, TLatex, TH1F, TLegend, TLine, TColor, TIter
from array import array
import ROOT

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

renbins = 1 

histfile = TFile("hist_LO_reco_v3.root","read")
objectname = "lepton"
gen_histogram_name = "h_"+objectname+"_energy"
reco_histogram_name = "h_"+objectname+"_energy_reco_final" 
energy_muon = histfile.Get( reco_histogram_name )
#energy_muon = histfile.Get( gen_histogram_name )
energy_muon.Rebin( renbins )

c_reco_muon = TCanvas("c_reco_muon","c_reco_muon",1)
energy_muon_reco = energy_muon.Clone("energy_muon_reco") 
energy_muon_reco.Draw()
energy_muon_reco.GetXaxis().SetTitle("Energy (GeV)")
c_reco_muon.Print("energy_reco_"+objectname+".pdf")

resfile = TFile("hist_LO_res_v3.root","read")
res_gen = resfile.Get( "h_lepton_energy" )
res_reco = resfile.Get( reco_histogram_name )
efficiency = resfile.Get( "h_efficiency_lepton" )
acceptance = resfile.Get( "h_acceptance_lepton" )
res_gen.Rebin( renbins )
res_reco.Rebin( renbins )
efficiency.Rebin( renbins )
acceptance.Rebin( renbins )

h_acc = res_gen.Clone("h_acc")

for i in range(0,energy_muon.GetNbinsX()):
  y_gen = res_gen.GetBinContent(i+1)
  y_reco = res_reco.GetBinContent(i+1)
  if i < 30/renbins:
    h_acc.SetBinContent(i+1, 0)
  else:
    acc = 0
    if y_reco > 0:
      acc = y_reco / y_gen
    else:
      print "zero reco"
    h_acc.SetBinContent(i+1, acc)

f5 = TF1("f5","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x",30,400);
h_acc.Fit("f5","R")

for i in range(0,energy_muon.GetNbinsX()):
  y = energy_muon.GetBinContent(i+1)
  y_gen = res_gen.GetBinContent(i+1)
  if i < 30/renbins:
    energy_muon.SetBinContent(i+1, y_gen)
  else:
    val = h_acc.GetBinLowEdge(i+1)+h_acc.GetBinWidth(i+1)/2.0
    #corr = 1.0/f5.Eval( 2*(i+1)-1 )
    corr = 1.0/h_acc.GetBinContent(i+1)
    energy_muon.SetBinContent(i+1, y*corr)


c_acceptance = TCanvas("c_acceptance","c_acceptance",1)
h_acc.Draw()
h_acc.GetXaxis().SetTitle("Energy (GeV)")
h_acc.SetTitle("Acceptance (reco/gen)")
c_acceptance.Print("acceptance_"+objectname+".pdf")

c_energy = TCanvas("c_energy","c_energy",1)
energy_muon.Draw()
energy_muon.GetXaxis().SetTitle("Energy (GeV)")
energy_muon.SetTitle("Unfolded distribution")
if nlo :
  c_energy.Print("energy_NLO_"+objectname+".pdf")
else :
  c_energy.Print("energy_LO_"+objectname+".pdf")

nevents = energy_muon.Integral()
energy_muon.Scale(1.0/float(nevents))
nbins = energy_muon.GetNbinsX()

files = []

f15 = TFile("weightfunc15.root","read")
f5 = TFile("weightfunc5.root","read")
f3 = TFile("weightfunc3.root","read")
f2 = TFile("weightfunc2.root","read")

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

nintegral = len(integral2)
nmymass = len(mymass2)

for i in range(0,nmymass):
  final2.SetPoint(i, mymass2[i], integral2[i])
  final3.SetPoint(i, mymass3[i], integral3[i])
  final5.SetPoint(i, mymass5[i], integral5[i])
  final15.SetPoint(i, mymass15[i], integral15[i])
  
final2.Draw("AC")
final3.Draw("C")
final5.Draw("C")
final15.Draw("C")

final2.GetXaxis().SetTitle("m_{t} (GeV)")
final2.SetLineColor(ROOT.kGreen)
final3.SetLineColor(ROOT.kBlue)
final5.SetLineColor(ROOT.kRed)
final15.SetLineColor(ROOT.kOrange+1)


linex = TLine(145,0,205,0);
linex.SetLineWidth(1)
linex.Draw()

liney = TLine(173,-0.0085,173,0.00725);
if nlo :
  liney = TLine(173,-0.01025,173,0.0055);
liney.SetLineWidth(1)
liney.SetLineStyle(2)
liney.Draw()

l = TLegend(0.6,0.6,0.8,0.8)
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
  wlepton.Print("integralmass_NLO_"+objectname+".pdf")
else :
  wlepton.Print("integralmass_LO_"+objectname+".pdf")
