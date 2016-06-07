
#include "TString.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "THStack.h"
#include "ListLabel_caro.h"

#include <iostream>

using namespace MA5;

bool Caro=true;

void PlotStack(TString plotname, std::vector< TString > samplelist, std::vector< int >     colortype, float scale_signal1, float scale_signal2,
TString xaxisLabel, bool setlogy){
  
  
   // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0); 
  gStyle->SetCanvasDefH(600);
  gStyle->SetCanvasDefW(600);
  gStyle->SetCanvasDefX(0); 
  gStyle->SetCanvasDefY(0);
  
  
  // For the Pad:
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0); 
  gStyle->SetPadGridX(0); 
  gStyle->SetPadGridY(0); 
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
  
  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
  
  // For the histo:
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(1);
  
  gStyle->SetEndErrorSize(2);
  
  //For the fit/function:
  gStyle->SetOptFit(1011);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(2);
  gStyle->SetFuncStyle(1);
  gStyle->SetFuncWidth(1);
  
  //For the date:
  gStyle->SetOptDate(0);
  
  // For the statistics box:
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(0); 
  gStyle->SetStatColor(0); 
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.04);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatW(0.15);
  
  // Margins:
  gStyle->SetPadTopMargin(0.07);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.03);

  
  // For the Global title:
  
  gStyle->SetOptTitle(0);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);
  
  // For the axis titles:
  
  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.045, "XYZ");
  gStyle->SetTitleXOffset(1.0);
  gStyle->SetTitleYOffset(1.5);
  
  // For the axis labels:
  
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  
  // For the axis:
  
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(1); 
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);  
  gStyle->SetPadTickY(1);
  
  // Change for log plots:
  gStyle->SetOptLogx(0);
  gStyle->SetOptLogy(0);
  gStyle->SetOptLogz(0);
  
  // Postscript options:
  gStyle->SetPaperSize(20.,20.);
  
  
  TCanvas *c1 = new TCanvas("c1","c1", 1000, 800);
  c1->SetLogy(setlogy);
  c1->cd();
  
  std::vector<TH1F *> histo;  
  
  
  std::map < std::string, TH1F * > histo_map;  
  ListLabel mylist;
  
  mylist.Initialize();
  cout << " samplelist.size() " << samplelist.size() << endl;
  for(unsigned int i= 0;  i<samplelist.size(); i++){
  
     TFile * tmp;
     // CARO : ROOTFILES
     tmp  = new TFile( ("rootfiles/mva_gut/outputmva_"+samplelist[i]+".root").Data() );
     cout << " reading rootfiles/mva_gut/outputmva_" << samplelist[i] << ".root   " ;
     tmp->cd();

     
     
     TString name;
     // CARO : NAME OF SIGNAL
     if (samplelist[i] !="original") name = plotname+"_"+samplelist[i];
     else name = plotname+"_t-1j-Kappa-gct";
     cout << " accessing " << name  << endl;

     
     TH1F * histo_tmp =(TH1F*)tmp->Get( name.Data());

     // CARO : REBIN IF NEEDED
     if (Caro) histo_tmp->Rebin();

//     cout << "  integral "<< histo_tmp->Integral(0,histo_tmp->GetNbinsX()+2) << endl;
     
     histo_tmp->SetLineWidth(2);
     histo_tmp->SetLineColor(1);
     if ((mylist.GetName(samplelist[i].Data())).find("Signal") == std::string::npos) {
           histo_tmp->SetFillColor(colortype[i]);
     }
     else {
            // CARO : SPECIAL LINE STYLE & RENORMALISATION FOR SIGNAL
            histo_tmp->SetLineColor(colortype[i]);
            if ( (mylist.GetName(samplelist[i].Data())).find("gct") != std::string::npos)  {
                 // gct
                 histo_tmp->SetLineStyle(2);
                 histo_tmp->Scale(scale_signal1);
            }
            else if ( (mylist.GetName(samplelist[i].Data())).find("gut") != std::string::npos)  {
                 // gut
                 histo_tmp->SetLineStyle(1);
                 histo_tmp->Scale(scale_signal2);
            }
            histo_tmp->SetMarkerColor(colortype[i]);
     }
     if(histo_map.count(mylist.GetName(samplelist[i].Data()))  == 0 ) histo_map[mylist.GetName(samplelist[i].Data())] = histo_tmp;
     else histo_map[mylist.GetName(samplelist[i].Data())]->Add(histo_tmp);
     
     
  }
  
  THStack* hs= new THStack();
  

  float minhs=-1;
  float maxhs=-1;
   
  typedef std::map< std::string,  TH1F * >::iterator it_type;
  for(it_type iterator = histo_map.begin(); iterator != histo_map.end(); iterator++) {
    if ((iterator->first).find("Signal") == std::string::npos)  hs->Add( iterator->second );
    else {
       if (minhs==-1) {
          minhs=(iterator->second)->GetMinimum()/5.;
          maxhs=(iterator->second)->GetMaximum()*1.2;
       }
       if ((iterator->second)->GetMinimum() <minhs) minhs=(iterator->second)->GetMinimum()/5.;
       if ((iterator->second)->GetMaximum() >maxhs) maxhs=(iterator->second)->GetMaximum()*1.2;
    }

  }
  
  if (minhs< hs->GetMinimum()) {
     if (setlogy && minhs<1) minhs=1;
     hs->SetMinimum( minhs);
  }
  if (maxhs > hs->GetMaximum()) hs->SetMaximum( maxhs*1.2);
  
  hs->Draw("h");


  for(it_type iterator = histo_map.begin(); iterator != histo_map.end(); iterator++) {
    if ((iterator->first).find("Signal") != std::string::npos) (iterator->second)->Draw("same");
  }

  hs->GetXaxis()->SetTitle(xaxisLabel.Data());
  hs->GetYaxis()->SetTitle("Entries");
/*
  hs->GetXaxis()->SetLabelSize(0.045);
  hs->GetXaxis()->SetTitleSize(0.045);
  hs->GetYaxis()->SetLabelSize(0.045);
  hs->GetYaxis()->SetTitleSize(0.045);
*/
  
  
  TLegend* qw = new TLegend(.70,.65,.95,.90);

  qw->SetShadowColor(0);
  qw->SetFillColor(0);
  qw->SetLineColor(0);
  
  for(it_type iterator = histo_map.begin(); iterator != histo_map.end(); iterator++) {
    TString blabla = iterator->first;
    if ((iterator->first).find("Signal") != std::string::npos) qw->AddEntry(  iterator->second ,  blabla.Data(), "l");
    else qw->AddEntry(  iterator->second ,  blabla.Data(), "f");
  }
  qw->Draw();
  

  
   TLatex *   tex = new TLatex(0.98,0.95125,"#sqrt{s}=13 TeV, 100 fb^{-1}");
   tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04875);
   tex->SetLineWidth(2);
   tex->Draw();

  
  if (setlogy) plotname+="_log";
  c1->SaveAs(("plots/"+plotname+".eps").Data());
  c1->SaveAs(("plots/"+plotname+".pdf").Data());
  c1->SaveAs(("plots/"+plotname+".gif").Data());
  c1->SaveAs(("plots/"+plotname+".C").Data());
   
}

