

#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TROOT.h" 
#include "TPaletteAxis.h" 
#include <iostream>
#include "ListLabel.h"

using namespace MA5;
//CARO: Directory
TString rootdi="/Users/tjkim/Work/pheno/fcnc/sourceFiles/V10"; // previous : "rootfiles_ok1";


void WhichBestSignificance(TH1F* HistoSignal, TH1F* HistoBackground, float & sig, float & cut);

void DoContourPlots(){

//CARO: Samples
 std::vector< TString > samplelist;

 samplelist.push_back( "SingleTop_W_madspin");
 samplelist.push_back( "SingleTop_s_madspin");
 samplelist.push_back( "SingleTop_t_madspin");

 samplelist.push_back("TTsemilep_madspin_1");
 samplelist.push_back("TTsemilep_madspin_2");
 samplelist.push_back("TTdilep_madspin");

 samplelist.push_back("AA-1Jet_sm-no_masses");
 samplelist.push_back("AA-2Jets_sm-no_masses");
 samplelist.push_back("AA-3Jets_sm-no_masses");

 //samplelist.push_back("WToLNu-0Jet_sm-no_masses");
 samplelist.push_back("WToLNu-1Jet_sm-no_masses");
 samplelist.push_back("WToLNu-2Jets_sm-no_masses");
 samplelist.push_back("WToLNu-3Jets_sm-no_masses");
 samplelist.push_back("WToLNu-4Jets_sm-no_masses");

 samplelist.push_back("ZToLL10-50-1Jet_sm-no_masses"); 
 samplelist.push_back("ZToLL10-50-2Jets_sm-no_masses");
 samplelist.push_back("ZToLL10-50-3Jets_sm-no_masses");
 samplelist.push_back("ZToLL10-50-4Jets_sm-no_masses");
 //samplelist.push_back("ZToLL50-0Jet_sm-no_masses");
 samplelist.push_back("ZToLL50-1Jet_sm-no_masses");
 samplelist.push_back("ZToLL50-2Jets_sm-no_masses");
 samplelist.push_back("ZToLL50-3Jets_sm-no_masses");
 samplelist.push_back("ZToLL50-4Jets_sm-no_masses");

 samplelist.push_back("TTtoLHtoGammaGamma-kappa-hct");
 samplelist.push_back("TTtoLHtoGammaGamma-kappa-hut");
 
// Get Histograms 

 TH1F* Hsig_gut_mvagut;
 TH1F* Hsig_gct_mvagut;
 TH1F* Hsig_gut_mvagct;
 TH1F* Hsig_gct_mvagct;
 TH1F* Hback_mvagut;
 TH1F* Hback_mvagct;

 ListLabel mylist;
 mylist.Initialize();

 int countmap1=0;
 int countmap2=0;
 for(unsigned int i= 0;  i<samplelist.size(); i++){

  // CARO : CHECK THE LINK
  // mvagut
     TFile * tmp1 = new TFile( (rootdi+"/"+samplelist[i]+".root").Data() );
     tmp1->cd();
     TString name1 = "LeptonicS5/diphotonM";
     TH1F * histo_tmp1 =(TH1F*)tmp1->Get( name1.Data());

     if ((mylist.GetName(samplelist[i].Data())).find("Signal") == std::string::npos) {
        // backgrounds :
        if (countmap1==0) Hback_mvagut = histo_tmp1;
        else Hback_mvagut->Add(histo_tmp1);
        countmap1++;
     }
     else {
        // CARO : CHECK THE SIGNAL NAME
        if ((mylist.GetName(samplelist[i].Data())).find("hut") !=std::string::npos) {
           // gut :
           Hsig_gut_mvagut=histo_tmp1;
        }
        else {
           // gct :
           Hsig_gct_mvagut=histo_tmp1;
        }
     }
  // CARO : CHECK THE LINK
  // mvagct
     TFile * tmp2 = new TFile( (rootdi+"/"+samplelist[i]+".root").Data() );
     tmp2->cd();
     TString name2 = "LeptonicS5/diphotonM";
     TH1F * histo_tmp2 =(TH1F*)tmp2->Get( name2.Data());
     if ((mylist.GetName(samplelist[i].Data())).find("Signal") == std::string::npos) {
        // backgrounds :
        if (countmap2==0) Hback_mvagct = histo_tmp2;
        else Hback_mvagct->Add(histo_tmp2);
        countmap2++;
     }
     else {
        // CARO : CHECK THE SIGNAL NAME
        if ((mylist.GetName(samplelist[i].Data())).find("hut") !=std::string::npos) {
           // gut :
           Hsig_gut_mvagct=histo_tmp2;
        }
        else {
           // gct :
           Hsig_gct_mvagct=histo_tmp2;
        }
     }
 }



 // CARO : CHECK THE BR LIMIT
 float BR_limit_hut = 0.56/100;   
 float BR_limit_hct = 0.56/100;

   cout << " BR_hut_limit " << BR_limit_hut << endl;
   cout << " BR_hct_limit " << BR_limit_hct << endl;



// Best Significance

// gut
 float sig=-1.;
 float cut=-2.;
 cout << "test= " << Hsig_gut_mvagut->Integral() << endl;

 WhichBestSignificance(Hsig_gut_mvagut, Hback_mvagut, sig, cut) ;
// gct
 sig=-1.;
 cut=-2.;
 WhichBestSignificance(Hsig_gct_mvagct, Hback_mvagct, sig, cut) ;

 int nloop1=100;
 int nloop2=100;
 float brmax1= 0.02;
 float brmax2= 0.02;

 TH2F* h2_bdtgut_sig;
 TH2F* h2_bdtgut_cut;
 TH2F* h2_bdtgct_sig;
 TH2F* h2_bdtgct_cut;
 TH2F* h2_best_sig;
 TH2F* h2_best_bdt;
 h2_bdtgut_sig = new TH2F("h2_bdtgut_sig","h2_bdtgut_sig",nloop1-1,0., brmax1, nloop2-1, 0., brmax2 );
 h2_bdtgut_cut = new TH2F("h2_bdtgut_cut","h2_bdtgut_cut",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );
 h2_bdtgct_sig = new TH2F("h2_bdtgct_sig","h2_bdtgct_sig",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );
 h2_bdtgct_cut = new TH2F("h2_bdtgct_cut","h2_bdtgct_cut",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );
 h2_best_sig = new TH2F("h2_best_sig","h2_best_sig",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );
 h2_best_bdt = new TH2F("h2_best_bdt","h2_best_bdt",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );



 // play with scaling :)
 TH1F* h1_bdtgut_sig;
 h1_bdtgut_sig = new TH1F("h1_bdtgut_sig","h1_bdtgut_sig",5*nloop1,0., brmax1);
 float sigforsig5=10000; float brgut_sig5=-1;
 float sigforsig3=10000; float brgut_sig3=-1;
 float sigforsig2=10000; float brgut_sig2=-1;
 for (int k1=0; k1<5*nloop1; k1++) {
    double scaling = 1;
    double br_loop = (k1+0.5)*brmax1/(5*nloop1-5);
    scaling= br_loop/(BR_limit_hut);
    TH1F* newscale = (TH1F*) Hsig_gut_mvagut->Clone();
    newscale->Scale(scaling);
    sig=-1;
    cut=-2;
    WhichBestSignificance(newscale, Hback_mvagut, sig, cut) ;
    h1_bdtgut_sig->SetBinContent(k1+1,sig);
    if (5<sig && sig<sigforsig5) {
       sigforsig5=sig;
       brgut_sig5=br_loop;
    }
    if (3<sig && sig<sigforsig3) {
       sigforsig3=sig;
       brgut_sig3=br_loop;
    }
    if (2<sig && sig<sigforsig2) {
       sigforsig2=sig;
       brgut_sig2=br_loop;
    }
 }


// gct
 float sigcforsig5=10000; float brgct_sig5=-1;
 float sigcforsig3=10000; float brgct_sig3=-1;
 float sigcforsig2=10000; float brgct_sig2=-1;
 for (int k2=0; k2<5*nloop2; k2++) {
    double scaling = 1;
    double br_loop = (k2+0.5)*brmax2/(5*nloop2-5);

    scaling= br_loop/(BR_limit_hct);
    TH1F* newscale = (TH1F*) Hsig_gct_mvagct->Clone();
    newscale->Scale(scaling);
    sig=-1;
    cut=-2;
    WhichBestSignificance(newscale, Hback_mvagct, sig, cut) ;
    if (5<sig && sig<sigcforsig5) {
       sigcforsig5=sig;
       brgct_sig5=br_loop;
    }
    if (3<sig && sig<sigcforsig3) {
       sigcforsig3=sig;
       brgct_sig3=br_loop;
    }
    if (2<sig && sig<sigcforsig2) {
       sigcforsig2=sig;
       brgct_sig2=br_loop;
    }
 }


 // mix gut+gct
 for (int k1=0; k1<nloop1-1; k1++) {
    // gut bdt
    double scaling =1;
    double br_loop = (k1+0.5)*brmax1/(nloop1-1);
    scaling= br_loop/(BR_limit_hut);
    TH1F* newscale1 = (TH1F*) Hsig_gut_mvagut->Clone();
    newscale1->Scale(scaling);
    for (int k2=0; k2<nloop2-1; k2++) {
       double scaling2 = 1;
       double br_loop2 = (k2+0.5)*brmax2/(nloop2-1);
       scaling2= br_loop2/(BR_limit_hct);
       TH1F* newscale2  = (TH1F*) Hsig_gct_mvagut->Clone();
       newscale2->Scale(scaling2);

       TH1F* new_sig_gut = (TH1F*) newscale1->Clone();
       new_sig_gut->Add(newscale2);

       sig=-1;
       cut=-2;
       WhichBestSignificance(new_sig_gut, Hback_mvagut, sig, cut) ;
       h2_bdtgut_sig->SetBinContent(k1+1,k2+1,sig);
       h2_bdtgut_cut->SetBinContent(k1+1,k2+1,cut);
       newscale2->Delete();
       new_sig_gut->Delete();
    }
    newscale1->Delete();
 }

 for (int k1=0; k1<nloop1-1; k1++) {
    // gct bdt
    double scaling = 1;
    double br_loop = (k1+0.5)*brmax1/(nloop1-1);
    scaling= br_loop/(BR_limit_hut);
    TH1F* newscale1 = (TH1F*) Hsig_gut_mvagct->Clone();
    newscale1->Scale(scaling);
    for (int k2=0; k2<nloop2-1; k2++) {
       double scaling2 = 1;
       double br_loop2 = (k2+0.5)*brmax2/(nloop2-1);
       scaling2= br_loop2/(BR_limit_hct);
       TH1F* newscale2  = (TH1F*) Hsig_gct_mvagct->Clone();
       newscale2->Scale(scaling2);

       TH1F* new_sig_gct = (TH1F*) newscale1->Clone();
       new_sig_gct->Add(newscale2);

       sig=-1;
       cut=-2;
       WhichBestSignificance(new_sig_gct, Hback_mvagct, sig, cut) ;
       h2_bdtgct_sig->SetBinContent(k1+1,k2+1,sig);
       h2_bdtgct_cut->SetBinContent(k1+1,k2+1,cut);
       if (sig< h2_bdtgut_sig->GetBinContent(k1+1,k2+1)) {
          h2_best_sig->SetBinContent(k1+1,k2+1,h2_bdtgut_sig->GetBinContent(k1+1,k2+1));
          h2_best_bdt->SetBinContent(k1+1,k2+1,1.);
       }
       else {
          h2_best_sig->SetBinContent(k1+1,k2+1,sig);
          h2_best_bdt->SetBinContent(k1+1,k2+1,2.);
       }
       newscale2->Delete();
       new_sig_gct->Delete();
    }
    newscale1->Delete();
 }


 gStyle->SetOptStat(0);
 gStyle->SetOptTitle(0);



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





 TCanvas *c2 = new TCanvas("c2","c2", 1000, 800);
 c2->cd();
 h2_best_sig->SetMaximum(10.);
 h2_best_sig->Draw("colz");
 // CARO : CHECK THE X AND Y TITLES
 h2_best_sig->GetYaxis()->SetTitle("BR_{hct}");
 h2_best_sig->GetXaxis()->SetTitle("BR_{hut}");
 

// contours
 Double_t sigcontours[2];
 sigcontours[0]=3;
 sigcontours[1]=5;
 TH2F* hcontour = (TH2F*) h2_best_sig->Clone();
 hcontour->SetContour(2, sigcontours);
 hcontour->Draw("same" "cont1");
 hcontour->Draw("cont" "list");
 c2->Update();

// adding extremities on the plot
 TObjArray *contoursaccess = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
 TList *list3 = (TList*)contoursaccess->At(0);
 TGraph *gr3 = (TGraph*)list3->First();

 Double_t ax[100],ay[100];
 for (int i=0; i<100; i++) { ax[i]=0; ay[i]=0;}

 int n=gr3->GetN(); //get ploted array dimention
 if (n>98) { 
   n=98;
   cout << " warning : not enought points for the contour " << endl;
 }

 ax[0]=0;
 ax[0]= brgut_sig3;
 ay[0]=0.;
 for(Int_t i=0; i<n; i++) {
  gr3->GetPoint(i,ax[i+1],ay[i+1]);
 }
 ax[n+1]=0;
 ay[n+1]=0;
 ay[n+1]=brgct_sig3;
 TGraph* newgr3 = new TGraph(n+2,ax,ay);


 newgr3->SetLineWidth(2);
 newgr3->SetLineStyle(1);
 newgr3->SetLineColor(10);
 newgr3->Draw("same");

 TList *list5 = (TList*)contoursaccess->At(1);
 TGraph *gr5 = (TGraph*)list5->First();
 for (int i=0; i<100; i++) { ax[i]=0; ay[i]=0;}
 ax[0]= brgut_sig5;
 ay[0]=0.;
 for(Int_t i=0; i<n; i++) {
  gr5->GetPoint(i,ax[i+1],ay[i+1]);
 }
 ax[n+1]=0;
 ay[n+1]= brgct_sig5;
 TGraph* newgr5 = new TGraph(n+2,ax,ay);
 

 newgr5->SetLineWidth(2);
 newgr5->SetLineStyle(1);
 newgr5->SetLineColor(10);

 h2_best_sig->Draw("colz");
 newgr5->SetLineStyle(2);
 newgr3->Draw("same");
 newgr5->Draw("same");

 gPad->Update();
 TPaletteAxis *palette = (TPaletteAxis*) h2_best_sig->GetListOfFunctions()->FindObject("palette");
 if (palette) { 
   cout << " palette existe " << endl;
   palette->SetY1NDC(0.2);
   gPad->Modified();
   gPad->Update();
 }



  // CARO : SAVE
  c2->SaveAs("plots/best_sig.eps");
  c2->SaveAs("plots/best_sig.pdf");
  c2->SaveAs("plots/best_sig.gif");
  c2->SaveAs("plots/best_sig.C");

}


void WhichBestSignificance(TH1F* HistoSignal, TH1F* HistoBackground, float & sig, float & cut) {
 double binwidth = (HistoSignal->GetXaxis()->GetXmax() - HistoSignal->GetXaxis()->GetXmin())/HistoSignal->GetNbinsX();

/*
 for(int i=1; i<HistoSignal->GetNbinsX()+1; i++){

    double s = HistoSignal->Integral(i , HistoSignal->GetNbinsX()+1);
    double b = HistoBackground->Integral(i,HistoSignal->GetNbinsX()+1);
    double significance_tmp = 0;
    if ( (s+b) > 0) {
      significance_tmp = s/pow(s+b, 0.5);
    }
    if (sig< significance_tmp) {
      sig = significance_tmp;
      cut = HistoSignal->GetXaxis()->GetXmin() + binwidth*(i-1);
    }
 }
*/

 double s = HistoSignal->Integral(1, HistoSignal->GetNbinsX()+1);
 double b = HistoBackground->Integral(1,HistoSignal->GetNbinsX()+1);
 double significance_tmp = s/pow(s+b, 0.5);
 sig = significance_tmp;
 cut = HistoSignal->GetXaxis()->GetXmin() + binwidth*(0);

}


