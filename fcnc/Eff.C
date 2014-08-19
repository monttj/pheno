#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include <iostream>

void plot(TH1 * h, const TString & xtitle,  const TString & ytitle, const TString & name, bool stat=false, bool logy = false){

  TCanvas * c = new TCanvas(Form("%s",name.Data()),Form("%s",name.Data()) ,1);
  if(logy) c->SetLogy();
  h->Draw();
  h->SetTitle("");
  h->SetStats(0);
  h->GetXaxis()->SetTitle(Form("%s", xtitle.Data()));
  h->GetYaxis()->SetTitle(Form("%s", ytitle.Data()));
  h->SetLineWidth(2);

  c->Print(Form("%s.pdf",name.Data()));
  c->Print(Form("%s.eps",name.Data()));
}


void Eff(){

  TFile * f_p100= new TFile("hist_h2aa_100p.root");
  TFile * f = new TFile("hist_h2aa.root");

  TH1 * h_nEvents_p100 = (TH1F*) f_p100->Get("nEvents");
  TH1 * h_nEvents = (TH1F*) f->Get("nEvents");

  TH1 * h_p100_pt1 = (TH1F*) f_p100->Get("photon1_pt");
  TH1 * h_p100_pt2 = (TH1F*) f_p100->Get("photon2_pt");
  TH1 * h_pt1 = (TH1F*) f->Get("photon1_pt");
  TH1 * h_pt2 = (TH1F*) f->Get("photon2_pt");
  TH1 * h_dimass = (TH1F*) f->Get("diphoton_mass");

  TH1 * h_p100_pt = (TH1F*) f_p100->Get("photon_pt_wide");
  TH1 * h_pt = (TH1F*) f->Get("photon_pt_wide");
  TH1 * h_p100_eta = (TH1F*) f_p100->Get("photon_eta");
  TH1 * h_eta = (TH1F*) f->Get("photon_eta");
  
  TH1 * h_p100_pt_nocut = (TH1F*) f_p100->Get("photon_pt_nocut");
  TH1 * h_pt_nocut = (TH1F*) f->Get("photon_pt_nocut");

  TH1 * h_eff_pt = (TH1F*) h_pt->Clone("h_eff_pt");
  h_eff_pt->Divide(h_p100_pt);

  TH1 * h_eff_eta = (TH1F*) h_eta->Clone("h_eff_eta");
  h_eff_eta->Divide(h_p100_eta);

  TH1 * h_eff_pt1 = (TH1F*) h_pt1->Clone("h_eff_pt1");
  h_eff_pt1->Divide(h_p100_pt1);

  TH1 * h_eff_pt2 = (TH1F*) h_pt2->Clone("h_eff_pt2");
  h_eff_pt2->Divide(h_p100_pt2);


  
  std::cout << "total =  " << h_nEvents->Integral() << std::endl;
  std::cout << "one photon > 20 GeV with 100% assumption   :  "  << h_nEvents_p100->GetBinContent(2) << std::endl; 
  std::cout << "one photon > 20 GeV with delphes_CMS       :  "  << h_nEvents->GetBinContent(2) << std::endl;
  std::cout << "diphoton                                   :  "  <<  h_dimass->Integral() << std::endl;
  std::cout << "event selection efficiency with one photon above 20 GeV --->  " << h_nEvents->GetBinContent(2)/h_nEvents->GetBinContent(1) << std::endl;
  std::cout << "event selection efficiency with two photons above 20 GeV --->  " << h_dimass->Integral()/h_nEvents->GetBinContent(1) << std::endl;

  plot( h_eff_pt, "Photon P_{T}", "Efficiency", "eff_pt");
  plot( h_eff_pt1, "Photon P_{T}", "Efficiency", "eff_pt1");
  plot( h_eff_pt2, "Photon P_{T}", "Efficiency", "eff_pt2");
  plot( h_eff_eta, "Photon #eta", "Efficiency", "eff_eta");

  plot( h_p100_pt_nocut, "Photon P_{T}", "Entries", "entries_pt_100p", true, true);
  plot( h_pt_nocut, "Photon P_{T}", "Entries", "entries_pt", true, true);

  
}
