#! /usr/bin/env python 

from ROOT import gROOT, TFile, TCanvas, TF1, TGraph, TPaveText, TLatex, TH1F, TLegend, TLine, TColor
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

renbins = 2 

#histfile = TFile("hist_LO_noleptoncut.root","read")
histfile = TFile("hist_LO_reco_v3.root","read")
#histfile = TFile("hist_NLO.root","read")
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
#resfile = TFile("hist_NLO.root","read")
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
  y = energy_muon.GetBinContent(i+1)
  y_gen = res_gen.GetBinContent(i+1)
  y_reco = res_reco.GetBinContent(i+1)
  y_eff = efficiency.GetBinContent(i+1)
  y_acc = acceptance.GetBinContent(i+1)
  #print i, " " , y_reco
  if i < 30/renbins:
    energy_muon.SetBinContent(i+1, y_gen)
    h_acc.SetBinContent(i+1, 0)
  else:
    acc = 0
    if y_reco > 0 and y > 0:
      corr = y_gen / y_reco
      acc = y_reco / y_gen
      #print "acc= ", acc, " acc from hist= ", y_acc, " ", y_eff
      #energy_muon.SetBinContent(i+1, y*corr)
    else:
      print "zero reco"
      #energy_muon.SetBinContent(i+1, y_gen)
    
    #energy_muon.SetBinContent(i+1, y*1.0/(y_eff*y_acc))
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
    print 2*(i+1)-1
    corr = 1.0/h_acc.GetBinContent(i+1)
    energy_muon.SetBinContent(i+1, y*corr)


c_acceptance = TCanvas("c_acceptance","c_acceptance",1)
h_acc.Draw()
h_acc.GetXaxis().SetTitle("Energy (GeV)")
h_acc.SetTitle("Acceptance (reco/gen)")
c_acceptance.Print("acceptance_"+objectname+".pdf")

#f1 = TF1("f1","[0] + [1] * x + [2] * x * x +[3] * x * x * x", 0, 400)

#energy_muon.Fit("f1","R") 
#response = resfile.Get("res_" + objectname)
#energy_muon.Divide( response )

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

f15 = open("Numtab_WF15_LOTopDecay.m","r")
f5 = open("Numtab_WF5_LOTopDecay.m","r")
f3 = open("Numtab_WF3_LOTopDecay.m","r")
f2 = open("Numtab_WF2_LOTopDecay.m","r")

files.append( f2 )
files.append( f3 )
files.append( f5 )
files.append( f15 )

gr2 = TGraph()
gr3 = TGraph()
gr5 = TGraph()
gr15 = TGraph()

integral2 = []
integral3 = []
integral5 = []
integral15 = []

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

    #print out

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

  for j in range(0,massidx+1): 
    x = array('f',myenergy[j])
    y = array('f',myvalue[j])
    if nfile == 0:
      gr = TGraph(len(x), x, y)
      gr.Write()
      GetIntegral( energy_muon, gr, integral2)
      if mymass[j] == 173:
        gr2 = TGraph(len(x), x, y)
        gr2.Draw("AC");
        gr2.SetLineColor(ROOT.kGreen)
        gr2.SetMaximum(0.3);
        gr2.SetMinimum(-1.0);
        gr2.GetXaxis().SetTitle("Energy (GeV)")
    elif nfile == 1:
      gr = TGraph(len(x), x, y)
      gr.Write()
      GetIntegral( energy_muon, gr, integral3)
      if mymass[j] == 173:
        gr3 = TGraph(len(x), x, y)
        gr3.Draw("C");
        gr3.SetLineColor(ROOT.kBlue)
    elif nfile == 2:
      gr = TGraph(len(x), x, y)
      gr.Write()
      GetIntegral( energy_muon, gr, integral5)
      if mymass[j] == 173:
        gr5 = TGraph(len(x), x, y)
        gr5.Draw("C");
        gr5.SetLineColor(ROOT.kRed)
    else:
      gr = TGraph(len(x), x, y)
      gr.Write()
      GetIntegral( energy_muon, gr, integral15)
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

wlepton = TCanvas("wlepton","wlepton",1)

final2 = TGraph()
final3 = TGraph()
final5 = TGraph()
final15 = TGraph()

nintegral = len(integral2)
nmymass = len(mymass)

for i in range(0,nmymass):
  final2.SetPoint(i, mymass[i], integral2[i])
  final3.SetPoint(i, mymass[i], integral3[i])
  final5.SetPoint(i, mymass[i], integral5[i])
  final15.SetPoint(i, mymass[i], integral15[i])
  
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
