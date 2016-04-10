

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
TString rootdi="sourceFiles"; // previous : "rootfiles_ok1";

bool Multijet=true;
bool syst=true;
float xsyst=0.05;
TString format = ".eps"; // .png, .eps

void WhichBestSignificance(TH1F* HistoSignal, TH1F* HistoBackground, float & sig, float & cut);

void DoContourPlots(int inumPlot=0){

 if (syst) {
     cout << "which syst? " << endl;
//     cin >> xsyst;
     cout << "ok, will work with " << xsyst << endl;
 }

// Samples
 std::vector< TString > samplelist;

 samplelist.push_back( "AA-0Jet_sm-no_masses");         
 samplelist.push_back( "AA-1Jet_sm-no_masses");         
 samplelist.push_back( "AA-2Jets_sm-no_masses");         
 samplelist.push_back( "AA-3Jets_sm-no_masses");         

 samplelist.push_back( "SingleTop_W_madspin"); 
 samplelist.push_back( "SingleTop_s_madspin"); 
 samplelist.push_back( "SingleTop_t_madspin"); 
 
 samplelist.push_back("TTsemilep_madspin_1"); 
 samplelist.push_back("TTsemilep_madspin_2"); 
 samplelist.push_back("TTdilep_madspin");     
 
 samplelist.push_back("WToLNu-0Jet_sm-no_masses"); 
 samplelist.push_back("WToLNu-1Jet_sm-no_masses"); 
 samplelist.push_back("WToLNu-2Jets_sm-no_masses"); 
 samplelist.push_back("WToLNu-3Jets_sm-no_masses"); 
 samplelist.push_back("WToLNu-4Jets_sm-no_masses"); 

 samplelist.push_back("ZToLL10-50-1Jet_sm-no_masses"); 
 samplelist.push_back("ZToLL10-50-2Jets_sm-no_masses"); 
 samplelist.push_back("ZToLL10-50-3Jets_sm-no_masses"); 
 samplelist.push_back("ZToLL10-50-4Jets_sm-no_masses"); 

 samplelist.push_back("ZToLL50-1Jet_sm-no_masses"); 
 samplelist.push_back("ZToLL50-2Jets_sm-no_masses"); 
 samplelist.push_back("ZToLL50-3Jets_sm-no_masses"); 
 samplelist.push_back("ZToLL50-4Jets_sm-no_masses"); 

 samplelist.push_back("TTtoLHtoGammaGamma-kappa-hct"); 
 samplelist.push_back("TTtoLHtoGammaGamma-kappa-hut"); 
  
 
 
// Get Histograms 

 TH1F* Hsig_hut_mvahut;
 TH1F* Hsig_hct_mvahut;
 TH1F* Hsig_hut_mvahct;
 TH1F* Hsig_hct_mvahct;
 TH1F* Hback_mvahut;
 TH1F* Hback_mvahct;

 ListLabel mylist;
 mylist.Initialize();

int countmap1=0;
 int countmap2=0;
 for(unsigned int i= 0;  i<samplelist.size(); i++){

  // mvahut
     TFile * tmp1 = new TFile( (rootdi+"/V10/"+samplelist[i]+".root").Data() );
     tmp1->cd();
     TString name1 = "LeptonicS5/diphotonM";
     TH1F * histo_tmp1 =(TH1F*)tmp1->Get( name1.Data());

     if ((mylist.GetName(samplelist[i].Data())).find("Signal") == std::string::npos ) {
        // backgrounds :
        if (countmap1==0) Hback_mvahut = histo_tmp1;
        else Hback_mvahut->Add(histo_tmp1);
        countmap1++;
     }
     else {
        if ((mylist.GetName(samplelist[i].Data())).find("hut") !=std::string::npos) {
           // hut :
           Hsig_hut_mvahut=histo_tmp1;
        }
        else {
           // hct :
           Hsig_hct_mvahut=histo_tmp1;
        }
     }
  // mvahct
     TFile * tmp2 = new TFile( (rootdi+"/V10/"+samplelist[i]+".root").Data() );
     tmp2->cd();
     TString name2 = "LeptonicS5/diphotonM";
     TH1F * histo_tmp2 =(TH1F*)tmp2->Get( name2.Data());

     if ((mylist.GetName(samplelist[i].Data())).find("Signal") == std::string::npos) {
        // backgrounds :
        if (countmap2==0) Hback_mvahct = histo_tmp2;
        else Hback_mvahct->Add(histo_tmp2);
        countmap2++;
     }
     else {
        if ((mylist.GetName(samplelist[i].Data())).find("hut") !=std::string::npos) {
           // hut :
           Hsig_hut_mvahct=histo_tmp2;
        }
        else {
           // hct :
           Hsig_hct_mvahct=histo_tmp2;
        }
     }
 }

/*
//Coupling's info from Lorenzo 
K_limit = sqrt[ ( BR_limit * Gamma_top) / partial width (K=1) ]

In your case, I take K_hut as an example,
BR_limit = 0.0355%
partial_width(k=1) = 3.665e5 GeV  (in couplings["kappa_hut"]=FCNCcoupling(3.665e5,0.0355/100.,true); )
Gamma_top = 1.32158 GeV (found in SignalSingleTop.cpp)
-> K_hut_limit = 3.578e-5 (in units of GeV^-1)

the limit on the BR and the partial width both depend on the coupling, only the total top width does not.
You find all the numbers in the XsectionSignalExtractor.cpp,
where the FCNCcoupling vector has as first argument the partial width evaluated with the cupling is =1,
and as second argument the limit on the BR.
*/

 float Gamma_top= 1.32158 ; // GeV

 float BR_limit_hut = 0.56/100;   
 //float partial_w_hut = 3.665e5; // GeV
 float partial_w_hut = 0.1836; // GeV
 float k_hut_limit = pow( ( BR_limit_hut * Gamma_top)/partial_w_hut, 0.5);

 float BR_limit_hct = 0.56/100;
 //float partial_w_hct = 3.665e5; // GeV
 float partial_w_hct = 0.1836; // GeV
 float k_hct_limit = pow( ( BR_limit_hct * Gamma_top)/partial_w_hct, 0.5);

 if (inumPlot==0) {

   cout << " k_hut_limit " << k_hut_limit << endl;
   cout << " k_hct_limit " << k_hct_limit << endl;
 }
 else if (inumPlot==1) {
   cout << " BR_hut_limit " << BR_limit_hut << endl;
   cout << " BR_hct_limit " << BR_limit_hct << endl;
 }


// Best Significance

// hut
 float sig=-1.;
 float cut=-2.;
 double tmp = Hsig_hut_mvahut->Integral() ;
 cout << "test= " << tmp << endl;
 WhichBestSignificance(Hsig_hut_mvahut, Hback_mvahut, sig, cut) ;
 cout << "hut :  sig " << sig << " for cut " << cut << "     " << Hsig_hut_mvahut->Integral() << " " << Hback_mvahut->Integral() << endl;
// hct
 sig=-1.;
 cut=-2.;
 WhichBestSignificance(Hsig_hct_mvahct, Hback_mvahct, sig, cut) ;
 cout << "hct :  sig " << sig << " for cut " << cut << "     " << Hsig_hct_mvahct->Integral() << " " << Hback_mvahct->Integral() << endl;
 cout << endl;

 float kgmax1=4e-01;
 float kgmax2=4e-01;
 int nloop1=33;
 int nloop2=33;

 float brmax1= 2e-02;
 float brmax2= 2e-02;



 if (syst) {
  if (xsyst>0.06) {
   kgmax1=4e-01;
   kgmax2=4e-01;
   brmax1= 0.02;
   brmax2= 0.02;
  }
  else {
   brmax1= 0.02;
   brmax2= 0.02;
  }

  nloop1=101;
  nloop2=101;
 }

 TH1F* h1_bdthut_sig;
 TH2F* h2_bdthut_sig;
 TH2F* h2_bdthut_cut;
 TH2F* h2_bdthct_sig;
 TH2F* h2_bdthct_cut;
 TH2F* h2_best_sig;
 TH2F* h2_best_bdt;
 if (inumPlot==0) {
  h1_bdthut_sig = new TH1F("h1_bdthut_sig","h1_bdthut_sig",5*nloop1,0., kgmax1);
  h2_bdthut_sig = new TH2F("h2_bdthut_sig","h2_bdthut_sig",nloop1-1,0., kgmax1, nloop2-1, 0., kgmax2);
  h2_bdthut_cut = new TH2F("h2_bdthut_cut","h2_bdthut_cut",nloop1-1,0., kgmax1, nloop2-1, 0., kgmax2);
  h2_bdthct_sig = new TH2F("h2_bdthct_sig","h2_bdthct_sig",nloop1-1,0., kgmax1, nloop2-1, 0., kgmax2);
  h2_bdthct_cut = new TH2F("h2_bdthct_cut","h2_bdthct_cut",nloop1-1,0., kgmax1, nloop2-1, 0., kgmax2);
  h2_best_sig = new TH2F("h2_best_sig","h2_best_sig",nloop1-1,0., kgmax1, nloop2-1, 0., kgmax2);
  h2_best_bdt = new TH2F("h2_best_bdt","h2_best_bdt",nloop1-1,0., kgmax1, nloop2-1, 0., kgmax2);
 }
 else if (inumPlot==1) {
  nloop1=41;
  nloop2=41;
  h1_bdthut_sig = new TH1F("h1_bdthut_sig","h1_bdthut_sig",5*nloop1,0., brmax1);
  h2_bdthut_sig = new TH2F("h2_bdthut_sig","h2_bdthut_sig",nloop1-1,0., brmax1, nloop2-1, 0., brmax2 );
  h2_bdthut_cut = new TH2F("h2_bdthut_cut","h2_bdthut_cut",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );
  h2_bdthct_sig = new TH2F("h2_bdthct_sig","h2_bdthct_sig",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );
  h2_bdthct_cut = new TH2F("h2_bdthct_cut","h2_bdthct_cut",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );
  h2_best_sig = new TH2F("h2_best_sig","h2_best_sig",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );
  h2_best_bdt = new TH2F("h2_best_bdt","h2_best_bdt",nloop1-1,0., brmax1,  nloop2-1, 0., brmax2 );
 }

 // play with scaling :)
 // sigma ~ coupling**2
 // scale = (coupling**2/k_hut_limit**2);
 cout << " --------------- " << endl;
 cout << "       hut       " << endl;
 cout << " --------------- " << endl;
 float khut_sig5=-1; float sigforsig5=10000; float brhut_sig5=-1;
 float khut_sig3=-1; float sigforsig3=10000; float brhut_sig3=-1;
 float khut_sig2=-1; float sigforsig2=10000; float brhut_sig2=-1;
 for (int k1=0; k1<5*nloop1; k1++) {
    double scaling = 1;
    double coupling_loop = (k1+0.5)*kgmax1/(5*nloop1-5);
    double br_loop = (k1+0.5)*brmax1/(5*nloop1-5);
    if (inumPlot==0) scaling= coupling_loop*coupling_loop/(k_hut_limit*k_hut_limit);
    else if (inumPlot==1) scaling= br_loop/(BR_limit_hut);
    TH1F* newscale = (TH1F*) Hsig_hut_mvahut->Clone();
    newscale->Scale(scaling);
    sig=-1;
    cut=-2;
    WhichBestSignificance(newscale, Hback_mvahut, sig, cut) ;
    h1_bdthut_sig->SetBinContent(k1+1,sig);
    if (inumPlot==0) cout << " k = " << coupling_loop << "    sig " << sig << " for cut " << cut << endl;
    else if (inumPlot==1)  cout << " br = " << br_loop << "    sig " << sig << " for cut " << cut << endl;
    if (5<sig && sig<sigforsig5) {
       sigforsig5=sig;
       khut_sig5= coupling_loop;
       brhut_sig5=br_loop;
    }
    if (3<sig && sig<sigforsig3) {
       sigforsig3=sig;
       khut_sig3= coupling_loop;
       brhut_sig3=br_loop;
    }
    if (2<sig && sig<sigforsig2) {
       sigforsig2=sig;
       khut_sig2= coupling_loop;
       brhut_sig2=br_loop;
    }
 }


// hct
 cout << " --------------- " << endl;
 cout << "       hct       " << endl;
 cout << " --------------- " << endl;
 float khct_sig5=-1; float sigcforsig5=10000; float brhct_sig5=-1;
 float khct_sig3=-1; float sigcforsig3=10000; float brhct_sig3=-1;
 float khct_sig2=-1; float sigcforsig2=10000; float brhct_sig2=-1;
 for (int k2=0; k2<5*nloop2; k2++) {
    double scaling = 1;
    double coupling_loop = (k2+0.5)*kgmax2/(5*nloop2-5);
    double br_loop = (k2+0.5)*brmax2/(5*nloop2-5);

    if (inumPlot==0) scaling= coupling_loop*coupling_loop/(k_hct_limit*k_hct_limit);
    else if (inumPlot==1) scaling= br_loop/(BR_limit_hct);
    TH1F* newscale = (TH1F*) Hsig_hct_mvahct->Clone();
    newscale->Scale(scaling);
    sig=-1;
    cut=-2;
    WhichBestSignificance(newscale, Hback_mvahct, sig, cut) ;
    if (inumPlot==0) cout << " k = " << coupling_loop << "    sig " << sig << " for cut " << cut << endl;
    else if (inumPlot==1)  cout << " br = " << br_loop << "    sig " << sig << " for cut " << cut << endl;
    if (5<sig && sig<sigcforsig5) {
       sigcforsig5=sig;
       khct_sig5=coupling_loop;
       brhct_sig5=br_loop;
    }
    if (3<sig && sig<sigcforsig3) {
       sigcforsig3=sig;
       khct_sig3=coupling_loop;
       brhct_sig3=br_loop;
    }
    if (2<sig && sig<sigcforsig2) {
       sigcforsig2=sig;
       khct_sig2=coupling_loop;
       brhct_sig2=br_loop;
    }
 }

 // mix hut+hct

 cout << " --------------- " << endl;
 cout << "   mix hut+hct   " << endl;
 cout << " --------------- " << endl;
 for (int k1=0; k1<nloop1-1; k1++) {
    // hut bdt
    double scaling =1;
    double coupling_loop = (k1+0.5)*kgmax1/(nloop1-1);
    double br_loop = (k1+0.5)*brmax1/(nloop1-1);
    if (inumPlot==0) scaling= coupling_loop*coupling_loop/(k_hut_limit*k_hut_limit);
    else if (inumPlot==1) scaling= br_loop/(BR_limit_hut);
    TH1F* newscale1 = (TH1F*) Hsig_hut_mvahut->Clone();
    newscale1->Scale(scaling);
    for (int k2=0; k2<nloop2-1; k2++) {
       double scaling2 = 1;
       double coupling_loop2 = (k2+0.5)*kgmax2/(nloop2-1);
       double br_loop2 = (k2+0.5)*brmax2/(nloop2-1);
       if (inumPlot==0) scaling2= coupling_loop2*coupling_loop2 /(k_hct_limit*k_hct_limit);
       else if (inumPlot==1) scaling2= br_loop2/(BR_limit_hct);
       TH1F* newscale2  = (TH1F*) Hsig_hct_mvahut->Clone();
       newscale2->Scale(scaling2);

       TH1F* new_sig_hut = (TH1F*) newscale1->Clone();
       new_sig_hut->Add(newscale2);

       sig=-1;
       cut=-2;
       WhichBestSignificance(new_sig_hut, Hback_mvahut, sig, cut) ;
       h2_bdthut_sig->SetBinContent(k1+1,k2+1,sig);
       h2_bdthut_cut->SetBinContent(k1+1,k2+1,cut);
       newscale2->Delete();
       new_sig_hut->Delete();
    }
    newscale1->Delete();
 }

 for (int k1=0; k1<nloop1-1; k1++) {
    // hct bdt
    double scaling = 1;
    double coupling_loop = (k1+0.5)*kgmax1/(nloop1-1);
    double br_loop = (k1+0.5)*brmax1/(nloop1-1);
    if (inumPlot==0) scaling= coupling_loop*coupling_loop/(k_hut_limit*k_hut_limit);
    else if (inumPlot==1) scaling= br_loop/(BR_limit_hut);
    TH1F* newscale1 = (TH1F*) Hsig_hut_mvahct->Clone();
    newscale1->Scale(scaling);
    for (int k2=0; k2<nloop2-1; k2++) {
       double scaling2 = 1;
       double coupling_loop2 = (k2+0.5)*kgmax2/(nloop2-1);
       double br_loop2 = (k2+0.5)*brmax2/(nloop2-1);
       if (inumPlot==0) scaling2= coupling_loop2*coupling_loop2/(k_hct_limit*k_hct_limit);
       else if (inumPlot==1) scaling2= br_loop2/(BR_limit_hct);
       TH1F* newscale2  = (TH1F*) Hsig_hct_mvahct->Clone();
       newscale2->Scale(scaling2);

       TH1F* new_sig_hct = (TH1F*) newscale1->Clone();
       new_sig_hct->Add(newscale2);

       sig=-1;
       cut=-2;
       WhichBestSignificance(new_sig_hct, Hback_mvahct, sig, cut) ;
       h2_bdthct_sig->SetBinContent(k1+1,k2+1,sig);
       h2_bdthct_cut->SetBinContent(k1+1,k2+1,cut);
       if (k1==0 && inumPlot==0) cout << " k2 " <<  coupling_loop2 << " sig " << sig << " k1 " << k1 << " scaling " << scaling << " " << scaling2 << endl;
       if (k1==0 && inumPlot==1) cout << " k2 " <<  br_loop2 << " sig " << sig << " k1 " << k1 << " scaling " << scaling << " " << scaling2 << endl;
       if (sig< h2_bdthut_sig->GetBinContent(k1+1,k2+1)) {
          h2_best_sig->SetBinContent(k1+1,k2+1,h2_bdthut_sig->GetBinContent(k1+1,k2+1));
          h2_best_bdt->SetBinContent(k1+1,k2+1,1.);
       }
       else {
          h2_best_sig->SetBinContent(k1+1,k2+1,sig);
          h2_best_bdt->SetBinContent(k1+1,k2+1,2.);
       }
       newscale2->Delete();
       new_sig_hct->Delete();
    }
    newscale1->Delete();
 }


 gStyle->SetOptStat(0);
 gStyle->SetOptTitle(0);

 TCanvas *c1 = new TCanvas("c1","c1", 1000, 400);
 c1->Divide(2,1);
 c1->cd(1);
 h2_bdthut_sig->Draw("colz");
 c1->cd(2);
 h2_bdthct_sig->Draw("colz");


 TCanvas *c1a = new TCanvas("c1a","c1a", 750, 600);
 c1a->cd();
 h1_bdthut_sig->Draw();
 if (inumPlot==0) {
    h1_bdthut_sig->GetYaxis()->SetTitle("S/#sqrt{S+B}");
    h1_bdthut_sig->GetXaxis()->SetTitle("K_{hut}");
    c1a->Print(("plots/1D_coupling_hut"+format).Data());
 }
 else if (inumPlot==1) { 
    h1_bdthut_sig->GetYaxis()->SetTitle("S/#sqrt{S+B}");
    h1_bdthut_sig->GetXaxis()->SetTitle("BR_{hut}");
    c1a->Print(("plots/1D_br_hut"+format).Data());
 }
 


 TCanvas *c2 = new TCanvas("c2","c2", 750, 600);
 c2->cd();
 h2_best_sig->SetMaximum(10.);
 h2_best_sig->Draw("colz");
 if (inumPlot==0) {
   h2_best_sig->GetYaxis()->SetTitle("K_{hct}");
   h2_best_sig->GetXaxis()->SetTitle("K_{hut}");
 }
 else if (inumPlot==1) {
   h2_best_sig->GetYaxis()->SetTitle("BR_{hct}");
   h2_best_sig->GetXaxis()->SetTitle("BR_{hut}");
 }

 gStyle->SetLabelSize(0.03, "XYZ"); 

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

 cout << "adding extrem" << endl;
 TObjArray *contoursaccess = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
 Int_t ncontours = contoursaccess->GetSize();

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
 if (inumPlot==0) ax[0]=khut_sig3;
 else if (inumPlot==1) ax[0]= brhut_sig3;
 ay[0]=0.;
 for(Int_t i=0; i<n; i++) {
  gr3->GetPoint(i,ax[i+1],ay[i+1]);
 }
 ax[n+1]=0;
 ay[n+1]=0;
 if (inumPlot==0) ay[n+1]=khct_sig3;
 else if (inumPlot==1) ay[n+1]=brhct_sig3;
 TGraph* newgr3 = new TGraph(n+2,ax,ay);

 newgr3->SetLineStyle(1);
 newgr3->SetLineColor(1);
 newgr3->Draw("same");

 TList *list5 = (TList*)contoursaccess->At(1);
 TGraph *gr5 = (TGraph*)list5->First();
 for (int i=0; i<100; i++) { ax[i]=0; ay[i]=0;}
 ax[0]=0.;
 if (inumPlot==0) ax[0]=khut_sig5;
 else if (inumPlot==1) ax[0]= brhut_sig5;
 ay[0]=0.;

 for(Int_t i=0; i<n; i++) {
  gr5->GetPoint(i,ax[i+1],ay[i+1]);
 }

 ax[n+1]=0;
 ay[n+1]=0;
 if (inumPlot==0) ay[n+1]=khct_sig5;
 else if (inumPlot==1) ay[n+1]= brhct_sig5;

 TGraph* newgr5 = new TGraph(n+2,ax,ay);
 
 newgr5->SetLineStyle(1);
 newgr5->SetLineColor(1);

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




if (inumPlot==0) {
cout << " khut_sig2 " << khut_sig2 << endl;
cout << " khut_sig3 " << khut_sig3 << endl;
cout << " khut_sig5 " << khut_sig5 << endl;
cout << " khct_sig2 " << khct_sig2 << endl;
cout << " khct_sig3 " << khct_sig3 << endl;
cout << " khct_sig5 " << khct_sig5 << endl;
} 
else if (inumPlot==1) {
cout << " brhut_sig2 " << brhut_sig2 << endl;
cout << " brhut_sig3 " << brhut_sig3 << endl;
cout << " brhut_sig5 " << brhut_sig5 << endl;
cout << " brhct_sig2 " << brhct_sig2 << endl;
cout << " brhct_sig3 " << brhct_sig3 << endl;
cout << " brhct_sig5 " << brhct_sig5 << endl;
}



 TString inumInfo;
 if (inumPlot==0) inumInfo="coupling_";
 else if (inumPlot==1) inumInfo="bratio_";
 TString name_plot_fin= "plots/"+inumInfo+"best_sig"+format;
 cout << " name_plot_fin " << name_plot_fin << endl;
 c2->Print(name_plot_fin.Data());

 TFile* outputFile = TFile::Open( "best_significance.root", "RECREATE" );
 h2_bdthut_sig->Write();
 h2_bdthut_cut->Write();
 h2_bdthct_sig->Write();
 h2_bdthct_cut->Write();
 h2_best_sig->Write();
 h2_best_bdt->Write();
 outputFile->Close();



}


void WhichBestSignificance(TH1F* HistoSignal, TH1F* HistoBackground, float & sig, float & cut) {

 double binwidth = (HistoSignal->GetXaxis()->GetXmax() - HistoSignal->GetXaxis()->GetXmin())/HistoSignal->GetNbinsX();
 float sig_multijet =0;
 float sig_systmul =0;

 //for(int i=1; i< HistoSignal->GetNbinsX()+1; i++){
    int i = 40;
    int m = 50;
    double s = HistoSignal->Integral(i , m );
    double b = HistoBackground->Integral(i, m );
    //double s = HistoSignal->Integral(i , HistoSignal->GetNbinsX()+1 );
    //double b = HistoBackground->Integral(i,HistoSignal->GetNbinsX()+1 );
    double significance_tmp = 0;
    if ( (s+b) > 0) {
      if(!syst)  significance_tmp = s/pow(s+b, 0.5);
      else significance_tmp = s/pow(s+b + xsyst*xsyst*b*b, 0.5);
    }
    if (sig< significance_tmp) {
      sig = significance_tmp;
      cut = HistoSignal->GetXaxis()->GetXmin() + binwidth*(i-1);
      sig_multijet = s/pow( s+1.5*b, 0.5);
      //sig_systmul = s/pow( s+1.15*b + xsyst*xsyst*(1.15)*(1.15)*b*b, 0.5);
      sig_systmul = s/pow( s+1.5*b + xsyst*xsyst*(1.5)*(1.5)*b*b, 0.5);
    }

 //}

 if (Multijet and !syst) sig = sig_multijet;
 if (Multijet and syst) sig = sig_systmul;

}


