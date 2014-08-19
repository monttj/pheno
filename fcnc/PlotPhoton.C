#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1.h"

void plot(TH1 * h, const TString & title, const TString & name){

  TCanvas * c = new TCanvas(Form("%s",name.Data()),Form("%s",name.Data()) ,1);
  h->Draw();
  h->SetTitle("");
  //h->SetStats(0);
  h->GetXaxis()->SetTitle(Form("%s", title.Data()));
  h->GetYaxis()->SetTitle("Entries");
  h->SetLineWidth(2);
}

void compare(TH1 *h1, TH1 *h2, const TString & h1_name, const TString & h2_name, const TString & title, const TString & name, double max = 0.22, bool norm=true){

  if(norm){
    h1->Scale(1.0/h1->Integral());
    h2->Scale(1.0/h2->Integral());
  }

  TCanvas * c = new TCanvas(Form("%s",name.Data()),Form("%s",name.Data()) ,1);
  h1->Draw();
  h1->SetTitle("");
  h1->SetStats(0);
  h1->GetXaxis()->SetTitle(Form("%s", title.Data()));
  h1->GetYaxis()->SetTitle("Entries");
  h1->SetMaximum(max);
  h1->SetLineWidth(2);
  h2->SetLineColor(2);
  h2->SetLineWidth(2);
  h2->Draw("same");

  TLegend *l= new TLegend(0.65,0.7,0.8,0.8);
  l->AddEntry(h1,Form("%s",h1_name.Data()),"l");
  l->AddEntry(h2,Form("%s",h2_name.Data()),"l");
  l->SetTextSize(0.06);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

  c->Print(Form("%s.pdf", name.Data()));
  c->Print(Form("%s.eps", name.Data()));

} 

void PlotPhoton(const TString & file = "hist_h2aa"){

  TFile * f = new TFile(Form("%s.root",file.Data()));
  TString h1_name = "h#rightarrow#gamma#gamma";

  TH1 * h_pt = (TH1F*) f->Get("photon_pt");
  TH1 * h_eta = (TH1F*) f->Get("photon_eta");
  TH1 * h_pt_1 = (TH1F*) f->Get("photon1_pt");
  TH1 * h_pt_2 = (TH1F*) f->Get("photon2_pt");
  TH1 * h_mass = (TH1F*) f->Get("diphoton_mass");
  TH1 * h_nPhoton = (TH1F*) f->Get("nPhoton");
  TH1 * h_nEle = (TH1F*) f->Get("nElectron");
  TH1 * h_nMuo = (TH1F*) f->Get("nMuon");
  TH1 * h_nJet = (TH1F*) f->Get("nJet");
  TH1 * h_nbJet = (TH1F*) f->Get("nbJet");

/*
  plot( h_pt, "Photon P_{T} (GeV)", "pt");
  plot( h_eta, "Photon #eta","eta");

  plot( h_pt_1, "Leading Photon P_{T} (GeV)","pt1");
  plot( h_pt_2, "Second Leading Photon P_{T} (GeV)","pt2");
  
  plot( h_mass, "Diphoton mass (GeV)","diphotonmass");

  plot( h_nEle, "Electron Multiplicity", "nele");
  plot( h_nMuo, "Muon Multiplicity","nele"); 
*/

  TFile * f2 = new TFile("hist_tch2aa.root");
  TString h2_name = "tch, h#rightarrow#gamma#gamma";

  TH1 * h2_pt = (TH1F*) f2->Get("photon_pt");
  TH1 * h2_eta = (TH1F*) f2->Get("photon_eta");
  TH1 * h2_pt_1 = (TH1F*) f2->Get("photon1_pt");
  TH1 * h2_pt_2 = (TH1F*) f2->Get("photon2_pt");
  TH1 * h2_mass = (TH1F*) f2->Get("diphoton_mass");
  TH1 * h2_nPhoton = (TH1F*) f2->Get("nPhoton");
  TH1 * h2_nEle = (TH1F*) f2->Get("nElectron");
  TH1 * h2_nMuo = (TH1F*) f2->Get("nMuon");
  TH1 * h2_nJet = (TH1F*) f2->Get("nJet");
  TH1 * h2_nbJet = (TH1F*) f2->Get("nbJet");


  TFile * f3 = new TFile("hist_ttbarLO.root");
  TString h3_name = "ttbar";

  TH1 * h3_pt = (TH1F*) f3->Get("photon_pt");
  TH1 * h3_eta = (TH1F*) f3->Get("photon_eta");
  TH1 * h3_pt_1 = (TH1F*) f3->Get("photon1_pt");
  TH1 * h3_pt_2 = (TH1F*) f3->Get("photon2_pt");
  TH1 * h3_mass = (TH1F*) f3->Get("diphoton_mass");
  TH1 * h3_nPhoton = (TH1F*) f3->Get("nPhoton");
  TH1 * h3_nEle = (TH1F*) f3->Get("nElectron");
  TH1 * h3_nMuo = (TH1F*) f3->Get("nMuon");
  TH1 * h3_nJet = (TH1F*) f3->Get("nJet");
  TH1 * h3_nbJet = (TH1F*) f3->Get("nbJet");

  compare(h_pt,      h2_pt,      h1_name, h2_name, "Photon P_{T} (GeV)",    "photon_pt_h2aa", 0.18) ; 
  compare(h_eta,     h2_eta,     h1_name, h2_name, "Photon #eta",           "photon_eta_h2aa", 0.1) ; 
  compare(h_pt_1,    h2_pt_1,    h1_name, h2_name, "Leading Photon P_{T} (GeV)",    "photon1_pt_h2aa", 0.22) ;
  compare(h_pt_2,    h2_pt_2,    h1_name, h2_name, "Second Leading Photon P_{T} (GeV)",    "photon2_pt_h2aa", 0.22) ;
  compare(h_mass,    h2_mass,    h1_name, h2_name, "Diphoton mass (GeV)",   "diphoton_mass_h2aa", 0.11) ; 
  compare(h_nPhoton, h2_nPhoton, h1_name, h2_name, "Photon Multiplicity",   "photon_multiplicity_h2aa", 1.0) ; 
  compare(h_nEle,    h2_nEle,    h1_name, h2_name, "Electron Multiplicity", "electron_multiplicity_h2aa", 1.0) ; 
  compare(h_nMuo,    h2_nMuo,    h1_name, h2_name, "Muon Multiplicity",     "muon_multiplicity_h2aa", 1.0) ; 
  compare(h_nJet,    h2_nJet,    h1_name, h2_name, "Jet Multiplicity",      "jet_multiplicity_h2aa", 0.5) ; 
  compare(h_nbJet,   h2_nbJet,   h1_name, h2_name, "b Jet Multiplicity",    "b_jet_multiplicity_h2aa", 1.0) ; 
 
  compare(h3_pt,      h2_pt,      h3_name, h2_name, "Photon P_{T} (GeV)",    "photon_pt_ttbar", 0.18) ;
  compare(h3_eta,     h2_eta,     h3_name, h2_name, "Photon #eta",           "photon_eta_ttbar", 0.1) ;
  compare(h3_pt_1,    h2_pt_1,    h3_name, h2_name, "Leading Photon P_{T} (GeV)",    "photon1_pt_ttbar", 0.22) ;
  //compare(h3_pt_2,    h2_pt_2,    h3_name, h2_name, "Second Leading Photon P_{T} (GeV)",    "photon2_pt_ttbar", 0.22) ;
  //compare(h3_mass,    h2_mass,    h3_name, h2_name, "Diphoton mass (GeV)",   "diphoton_mass_ttbar", 0.22) ;
  compare(h3_nPhoton, h2_nPhoton, h3_name, h2_name, "Photon Multiplicity",   "photon_multiplicity_ttbar", 1.0) ;
  compare(h3_nEle,    h2_nEle,    h3_name, h2_name, "Electron Multiplicity", "electron_multiplicity_ttbar", 1.0) ;
  compare(h3_nMuo,    h2_nMuo,    h3_name, h2_name, "Muon Multiplicity",     "muon_multiplicity_ttbar", 1.0) ;
  compare(h3_nJet,    h2_nJet,    h3_name, h2_name, "Jet Multiplicity",      "jet_multiplicity_ttbar", 0.5) ;
  compare(h3_nbJet,   h2_nbJet,   h3_name, h2_name, "b Jet Multiplicity",    "b_jet_multiplicity_ttbar", 1.0) ;


 
}
