void plot(){

  gROOT->ProcessLine(".L ./tdrstyle.C");
  setTDRStyle();

  TFile * AA = new TFile("sourceFiles/V10/AA.root");
//  TFile * AA = new TFile("sourceFiles/V10/AA-0Jet_sm-no_masses.root");
//  TFile * AA_1Jet = new TFile("sourceFiles/V10/AA-1Jet_sm-no_masses.root");
//  TFile * AA_2Jets = new TFile("sourceFiles/V10/AA-2Jets_sm-no_masses.root");
//  TFile * AA_3Jets = new TFile("sourceFiles/V10/AA-3Jets_sm-no_masses.root");
  TFile * TTdilep_madspin = new TFile("sourceFiles/V10/TTdilep_madspin.root");
  TFile * TTsemilep_madspin = new TFile("sourceFiles/V10/TTsemilep_madspin.root");
  TFile * SingleTop = new TFile("sourceFiles/V10/SingleTop.root");
  TFile * ZToLL = new TFile("sourceFiles/V10/ZToLL.root");
//  TFile * TTsemilep_HToAA_madspin_1 = new TFile("sourceFiles/V10/TTdilep_HToAA_madspin.root");
//  TFile * TTsemilep_HToAA_madspin_2 = new TFile("sourceFiles/V10/TTdilep_HToAA_madspin.root");
//  TFile * ZToLL50_1Jet = new TFile("sourceFiles/V10/ZToLL50-1Jet_sm-no_masses.root");
//  TFile * ZToLL50_2Jet = new TFile("sourceFiles/V10/ZToLL50-2Jets_sm-no_masses.root");
//  TFile * ZToLL50_3Jet = new TFile("sourceFiles/V10/ZToLL50-3Jets_sm-no_masses.root");
//  TFile * ZToLL50_4Jet = new TFile("sourceFiles/V10/ZToLL50-4Jets_sm-no_masses.root");
//  TFile * WToLNu_0Jet = new TFile("sourceFiles/V10/WToLNu-0Jet_sm-no_masses.root");
  TFile * WToLNu = new TFile("sourceFiles/V10/WToLNu.root");
  //TFile * WToLNu_1Jet = new TFile("sourceFiles/V10/WToLNu-1Jet_sm-no_masses.root");
  //TFile * WToLNu_2Jets = new TFile("sourceFiles/V10/WToLNu-2Jets_sm-no_masses.root");
  //TFile * WToLNu_3Jets = new TFile("sourceFiles/V10/WToLNu-3Jets_sm-no_masses.root");
  //TFile * WToLNu_4Jets = new TFile("sourceFiles/V10/WToLNu-4Jets_sm-no_masses.root");
  TFile * TT_Kappa_Hct_HtoAA = new TFile("sourceFiles/V10/TT_Kappa_Hct_HtoAA.root");
  //TFile * TT_Kappa_Hct_HtoAA = new TFile("sourceFiles/V10/TTtoLHtoGammaGamma-kappa-hct.root");
  //TFile * TT_Kappa_Hct_HtoAA = new TFile("sourceFiles/V10/TTtoLHtoGammaGamma-kappa-hut.root");
  //TFile * HToAA = new TFile("old/HToAA.root");
  //TFile * WHToAA = new TFile("old/WHToAA.root");
  //TFile * ZHToAA = new TFile("old/ZHToAA.root");

  const int ni = 10; // number of selections
  const int nj = 10; // number of histograms

  TH1 * h_AA[ni][nj];
//  TH1 * h_AA_1Jet[ni][nj];
//  TH1 * h_AA_2Jets[ni][nj];
//  TH1 * h_AA_3Jets[ni][nj];
  TH1 * h_TTdilep_madspin[ni][nj];
  TH1 * h_TTsemilep_madspin[ni][nj];
  TH1 * h_SingleTop[ni][nj];
  TH1 * h_ZToLL[ni][nj];
 // TH1 * h_TTsemilep_HToAA_madspin_1[ni][nj];
 // TH1 * h_TTsemilep_HToAA_madspin_2[ni][nj];
//  TH1 * h_ZToLL50_1Jet[ni][nj];
//  TH1 * h_ZToLL50_2Jet[ni][nj];
//  TH1 * h_ZToLL50_3Jet[ni][nj];
//  TH1 * h_ZToLL50_4Jet[ni][nj];
//  TH1 * h_WToLNu_0Jet[ni][nj];
  TH1 * h_WToLNu[ni][nj];
//  TH1 * h_WToLNu_1Jet[ni][nj];
//  TH1 * h_WToLNu_2Jets[ni][nj];
//  TH1 * h_WToLNu_3Jets[ni][nj];
//  TH1 * h_WToLNu_4Jets[ni][nj];
  TH1 * h_TT_Kappa_Hct_HtoAA[ni][nj];
  //TH1 * h_HToAA[ni][nj];
  //TH1 * h_WHToAA[ni][nj];
  //TH1 * h_ZHToAA[ni][nj];

  TCanvas *c[ni][nj];
  THStack *s[ni][nj];

  cout << "Plotting...." << endl;

  for(int i=0; i < 6; i++){

    TString step = "";
    if( i == 0 ) step = "S0";
    if( i == 1 ) step = "S1";
    if( i == 2 ) step = "LeptonicS2";
    if( i == 3 ) step = "LeptonicS3";
    if( i == 4 ) step = "LeptonicS4";
    if( i == 5 ) step = "LeptonicS5";
    if( i == 6 ) step = "HadronicS2";
    if( i == 7 ) step = "HadronicS3";
    if( i == 8 ) step = "HadronicS4";
    if( i == 9 ) step = "HadronicS5";
 
    for(int j=0; j < nj; j ++){
      TString title = "";
      TString xtitle = "";
      TString ytitle = "";
      double ymax = 0;
      double ymin = 0;
      bool log = false;
      if( j == 0 ) {
        title = "nphotons";
        xtitle = "Photon multiplicity";
        ytitle = "Events";
     //   if(i == 1) ymax = 1000000; 
        
      }
      if( j == 1 ) {
        title = "diphotonM";
        xtitle = "di-photon mass (GeV)";
        ytitle = "Events";
        if( i== 0) ymax = 100000;
        if( i== 1) ymax = 100000;
        if( i== 2) ymax = 10000;
        if( i== 3) ymax = 100;
        if( i== 4) ymax = 30;
        if( i== 5) ymax = 15;
        if( i== 6) ymax = 10;
        log = true;
        if(log) { ymin = 0.1 ; }
      }

      if( j == 2 ) {
        title = "njets";
        xtitle = "Jet multiplicity";
        ytitle = "Events";
        ymax = 14000;
      }
      if( j == 3) {
        title = "nbjetsCSVT";
        xtitle = "b jet multiplicity";
        ytitle = "Events";
        log=true;
      }
      if( j == 4 ) {
        title = "nleptons";
        xtitle = "Lepton multiplicity";
        ytitle = "Events";
      }
      if( j == 5 ) {
        title = "photon1iso";
        xtitle = "1st photon relative isolation";
        ytitle = "Events";
        log = true;
      }
      if( j == 6 ) {
        title = "photon2iso";
        xtitle = "2nd photon relative isolation";
        ytitle = "Events";
        log = true;
      }
      if( j == 7 ) {
        title = "photon1pt";
        xtitle = "1st photon p_{T} (GeV)";
        ytitle = "Events";
        log = true;
      }
      if( j == 8 ) {
        title = "photon2pt";
        xtitle = "2nd photon p_{T} (GeV)";
        ytitle = "Events";
        log = true;
      }
      if( j == 9 ){
        title = "mtop";
        xtitle = "m_{j #gamma#gamma} (GeV)";
        ytitle = "Events";
        if( i < 4 ){
          log = true;
          ymin = 0.0001;
          if( i == 0 ) ymax = 500000;
          if( i == 1 ) ymax = 50000;
          if( i == 2 ) ymax = 10000;
          if( i == 2 ) ymax = 10000;
        } else { 
          log = true;
          ymin = 0.0001;
          if( i == 3 ) ymax = 1000;
          if( i > 3 ) ymax = 24.0;
        }
      }
      if( j == 10 ){
        title = "drjetlep";
        xtitle = "#Delta R between jet and lepton";
        ytitle = "Events";
        log = true;
        if( i== 4) ymax = 500;
        if( i== 5) {
          ymax = 300;
          ymin = 0.1;
        }
        if( i== 6) ymax = 300;
      }
      if( j == 11 ){
        title = "drjetpho";
        xtitle = "#Delta R between jet and photon";
        ytitle = "Events";
        log = true;
        if( i== 4) ymax = 500;
        if( i== 5) {
          ymax = 300;
          ymin = 0.1;
        }
        if( i== 6) ymax = 300;
      }


      h_AA[i][j] = (TH1 * ) AA->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_AA_1Jet[i][j] = (TH1 * ) AA_1Jet->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_AA_2Jets[i][j] = (TH1 * ) AA_2Jets->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_AA_3Jets[i][j] = (TH1 * ) AA_3Jets->Get(Form("%s/%s", step.Data(), title.Data() ));
      h_TTdilep_madspin[i][j] = (TH1 * ) TTdilep_madspin->Get(Form("%s/%s", step.Data(), title.Data() ));
      h_TTsemilep_madspin[i][j] = (TH1 * ) TTsemilep_madspin->Get(Form("%s/%s", step.Data(), title.Data() ));
      h_SingleTop[i][j] = (TH1 * ) SingleTop->Get(Form("%s/%s", step.Data(), title.Data() ));
      h_ZToLL[i][j] = (TH1 * ) ZToLL->Get(Form("%s/%s", step.Data(), title.Data() ));
      h_WToLNu[i][j] = (TH1 * ) WToLNu->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_WToLNu_0Jet[i][j] = (TH1 * ) WToLNu_0Jet->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_WToLNu_1Jet[i][j] = (TH1 * ) WToLNu_1Jet->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_WToLNu_2Jets[i][j] = (TH1 * ) WToLNu_2Jets->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_WToLNu_3Jets[i][j] = (TH1 * ) WToLNu_3Jets->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_WToLNu_4Jets[i][j] = (TH1 * ) WToLNu_4Jets->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_TTsemilep_HToAA_madspin_1[i][j] = (TH1 * ) TTsemilep_HToAA_madspin_1->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_TTsemilep_HToAA_madspin_2[i][j] = (TH1 * ) TTsemilep_HToAA_madspin_2->Get(Form("%s/%s", step.Data(), title.Data() ));
      //h_HToAA[i][j] = (TH1 * ) HToAA->Get(Form("%s/%s_HToAA", step.Data(), title.Data() ));
      //h_WHToAA[i][j] = (TH1 * ) WHToAA->Get(Form("%s/%s_WHToAA", step.Data(), title.Data() ));
      //h_ZHToAA[i][j] = (TH1 * ) ZHToAA->Get(Form("%s/%s_ZHToAA", step.Data(), title.Data() ));
      h_TT_Kappa_Hct_HtoAA[i][j] = (TH1 * ) TT_Kappa_Hct_HtoAA->Get(Form("%s/%s", step.Data(), title.Data() ));

      // Adding histograms for each process with jets //
      //h_WToLNu_0Jet[i][j]->Add(h_WToLNu_1Jet[i][j]);
      //h_WToLNu_1Jet[i][j]->Add(h_WToLNu_2Jets[i][j]);
      //h_WToLNu_1Jet[i][j]->Add(h_WToLNu_3Jets[i][j]);
      //h_WToLNu_1Jet[i][j]->Add(h_WToLNu_4Jets[i][j]);
      //h_AA[i][j]->Add(h_AA_1Jet[i][j]);
      //h_AA_1Jet[i][j]->Add(h_AA_2Jets[i][j]);
      //h_AA_1Jet[i][j]->Add(h_AA_3Jets[i][j]);

 
    //  h_TTsemilep_HToAA_madspin_1[i][j]->Add(h_TTsemilep_HToAA_madspin_2[i][j]);
    //  h_ZToLL50_1Jet[i][j]->Add(h_ZToLL50_2Jet[i][j]);
    //  h_ZToLL50_1Jet[i][j]->Add(h_ZToLL50_3Jet[i][j]);
    //  h_ZToLL50_1Jet[i][j]->Add(h_ZToLL50_4Jet[i][j]);

      // Scale for ttbar //
      //double kf = 831.1/674.1; 
      double kf = 1.0; 
      h_TTdilep_madspin[i][j]->Scale(kf);
      h_TTsemilep_madspin[i][j]->Scale(kf);
      h_TT_Kappa_Hct_HtoAA[i][j]->Scale(kf);


      // Rebin for top mass //
      if( j == 9 ){
        //cout << "rebin histogram for mtop" << endl;
        h_AA[i][j]->Rebin(4);
        h_TTdilep_madspin[i][j]->Rebin(4);
        h_TTsemilep_madspin[i][j]->Rebin(4);
        h_SingleTop[i][j]->Rebin(4);
        h_ZToLL[i][j]->Rebin(4);
        h_WToLNu[i][j]->Rebin(4);
        h_TT_Kappa_Hct_HtoAA[i][j]->Rebin(4);
      }
      
      c[i][j] = new TCanvas(Form("%s_%s",step.Data(), title.Data()),Form("%s_%s",step.Data(), title.Data()), 1);
      s[i][j] = new THStack(Form("%s_%s",step.Data(), title.Data()),Form("%s_%s",step.Data(), title.Data()));

      if(log) c[i][j]->SetLogy();

      //s[i][j]->Add(h_AA_1Jet[i][j]);

      TH1F * h_Backgrounds;

      if( i >= 3 && i <=5 ) { 
        h_Backgrounds = (TH1F*) h_TTsemilep_madspin[2][j]->Clone("h_Backgrounds");
        TH1F * tmp_h_AA = (TH1F*) h_AA[2][j]->Clone("tmp_h_AA");
        TH1F * tmp_TTdilep_madspin = (TH1F*) h_TTdilep_madspin[2][j]->Clone("tmp_TTdilep_madspin");
        TH1F * tmp_WToLNu = (TH1F*) h_WToLNu[2][j]->Clone("tmp_WToLNu");
        TH1F * tmp_ZToLL = (TH1F*) h_ZToLL[2][j]->Clone("tmp_ZToLL");
        TH1F * tmp_SingleTop = (TH1F*) h_SingleTop[2][j]->Clone("tmp_SingleTop");
        h_Backgrounds->Add(tmp_TTdilep_madspin);
        h_Backgrounds->Add(tmp_h_AA);
        h_Backgrounds->Add(tmp_SingleTop);
        h_Backgrounds->Add(tmp_ZToLL);
        h_Backgrounds->Add(tmp_WToLNu);
        double num = h_AA[i][j]->Integral()+h_TTdilep_madspin[i][j]->Integral()+h_ZToLL[i][j]->Integral()+h_SingleTop[i][j]->Integral()+h_TTsemilep_madspin[i][j]->Integral()+h_WToLNu[i][j]->Integral() ;
        double den = h_Backgrounds->Integral();
        double scale = num/den;
        h_Backgrounds->Scale(scale);
        s[i][j]->Add(h_Backgrounds); 
      } else { 
        h_Backgrounds = (TH1F*) h_TTsemilep_madspin[i][j]->Clone("h_Backgrounds");
        h_Backgrounds->Add( h_TTdilep_madspin[i][j]  );
        h_Backgrounds->Add( h_AA[i][j] );
        h_Backgrounds->Add( h_SingleTop[i][j] );
        h_Backgrounds->Add( h_ZToLL[i][j] );
        h_Backgrounds->Add( h_WToLNu[i][j] );
       
        //s[i][j]->Add(h_Backgrounds);
        s[i][j]->Add(h_AA[i][j]);
        s[i][j]->Add(h_SingleTop[i][j]);
        s[i][j]->Add(h_TTsemilep_madspin[i][j]);
        s[i][j]->Add(h_TTdilep_madspin[i][j]);
        s[i][j]->Add(h_WToLNu[i][j]);
        s[i][j]->Add(h_ZToLL[i][j]);

      }
   //  s[i][j]->Add(h_TTsemilep_HToAA_madspin_1[i][j]);
   //  s[i][j]->Add(h_ZToLL50_1Jet[i][j]);
   //  s[i][j]->Add(h_HToAA[i][j]);
   //  s[i][j]->Add(h_WHToAA[i][j]);
   //  s[i][j]->Add(h_ZHToAA[i][j]);
   
   //Set Color   
      h_Backgrounds->SetFillColor(kBlue);
      h_AA[i][j]->SetFillColor(6);
      h_SingleTop[i][j]->SetFillColor(8);
      h_ZToLL[i][j]->SetFillColor(kBlue);
      h_WToLNu[i][j]->SetFillColor(7);
      h_TTdilep_madspin[i][j]->SetFillColor(kRed);
      h_TTsemilep_madspin[i][j]->SetFillColor(kRed+2);

      s[i][j]->Draw();

     //Signal
      TH1F * tmp_sig = (TH1F*) h_TT_Kappa_Hct_HtoAA[i][j]->Clone("tmp_sig");
      if( i <= 2){
        tmp_sig->Scale(100);
        tmp_sig->Draw("same");
        tmp_sig->SetLineWidth(3);        
      }else{
        h_TT_Kappa_Hct_HtoAA[i][j]->Draw("same");
        h_TT_Kappa_Hct_HtoAA[i][j]->SetLineWidth(3);
      }
      //h_TT_Kappa_Hct_HtoAA[i][j]->Scale(0.00228); // Higgs to gamma gamma at 125 GeV = 2.28 x 10^-3 
      //h_TT_Kappa_Hct_HtoAA[i][j]->Scale(100); // Higgs to gamma gamma at 125 GeV = 2.28 x 10^-3 
      s[i][j]->SetTitle( title.Data() );
      s[i][j]->GetYaxis()->SetTitle( ytitle.Data() );
      s[i][j]->GetXaxis()->SetTitle( xtitle.Data() );
      if( ymax != 0 ) s[i][j]->SetMaximum(ymax);

      TLegend * l;

      if( i >= 3 ){
        l = new TLegend(0.60,0.77,0.87,0.92);
        l->AddEntry(h_Backgrounds, "Backgrounds","F");
      }else{
        l = new TLegend(0.65,0.57,0.87,0.92);
        l->AddEntry(h_ZToLL[i][j], "Z #rightarrow ll","F");
        l->AddEntry(h_WToLNu[i][j], "W+jets","F");
        l->AddEntry(h_TTdilep_madspin[i][j], "t#bar{t} Dilep.","F");
        l->AddEntry(h_TTsemilep_madspin[i][j], "t#bar{t} Semilep.","F");
        l->AddEntry(h_SingleTop[i][j], "Single Top","F");
        l->AddEntry(h_AA[i][j], "AA","F");
       //l->AddEntry(h_TTsemilep_HToAA_madspin_1[i][j], "t#bar{t}H Semilep.","F");
       //l->AddEntry(h_HToAA[i][j], "H#rightarrow#gamma #gamma","F");
       //l->AddEntry(h_WHToAA[i][j], "WH#rightarrow#gamma #gamma","F");
       //l->AddEntry(h_ZHToAA[i][j], "ZH#rightarrow#gamma #gamma","F");
      }
      if( i <= 3){
        l->AddEntry(tmp_sig, "tcH#rightarrow#gamma #gamma","L");
      } else {
        l->AddEntry(h_TT_Kappa_Hct_HtoAA[i][j], "tcH#rightarrow#gamma #gamma","L");
      }
 
      l->SetTextSize(0.04);
      l->SetFillColor(0);
      l->SetLineColor(0);
      l->Draw();
   
      if( j == 0 ){  
        double total_bkg = h_AA[i][j]->Integral() 
                           +h_TTdilep_madspin[i][j]->Integral() 
                           +h_TTsemilep_madspin[i][j]->Integral() 
                           +h_WToLNu[i][j]->Integral()
                           +h_SingleTop[i][j]->Integral()
                           +h_ZToLL[i][j]->Integral()
                           //+h_TTsemilep_HToAA_madspin_1[i][j]->Integral() 
                           //+h_ZToLL50_1Jet[i][j]->Integral() 
                           //+h_HToAA[i][j]->Integral()
                           //+h_WHToAA[i][j]->Integral()
                           //+h_ZHToAA[i][j]->Integral()
                           ;
        double sig = h_TT_Kappa_Hct_HtoAA[i][j]->Integral();
        cout << "=== Cut flow Selection : " << step << " ======== " << endl;
        cout << "AA = " <<   h_AA[i][j]->Integral() << endl;
        cout << "Dilepton = " <<   h_TTdilep_madspin[i][j]->Integral() << endl;
        cout << "Semilepton = " << h_TTsemilep_madspin[i][j]->Integral() << endl;
        cout << "SingleTop = " << h_SingleTop[i][j]->Integral() << endl;
        cout << "ZToLL = " << h_ZToLL[i][j]->Integral() << endl;
        cout << "W+jets = " << h_WToLNu[i][j]->Integral() << endl;
        //cout << "ttH Semilepton = " << h_TTsemilep_HToAA_madspin_1[i][j]->Integral() << endl;
        //cout << "Z+jets = " << h_ZToLL50_1Jet[i][j]->Integral() << endl;
        //cout << "HToAA = "  << h_HToAA[i][j]->Integral() << endl;
        //cout << "WHToAA = " << h_WHToAA[i][j]->Integral() << endl;  
        //cout << "ZHToAA = " << h_ZHToAA[i][j]->Integral() << endl;  
        cout << "TT_Kappa_Hct_HtoAA = " << h_TT_Kappa_Hct_HtoAA[i][j]->Integral() << endl;
        double eff = h_TT_Kappa_Hct_HtoAA[i][j]->Integral() /  h_TT_Kappa_Hct_HtoAA[0][j]->Integral() ;
        cout << "Signal Efficiency (%) = " << eff << endl ;  
        cout << "S/sqrt(S+B)= " << (sig)/sqrt(total_bkg+sig) << endl;
      }

      c[i][j]->Print(Form("plots/V10/%s_%s.png", title.Data(), step.Data()));
      c[i][j]->Print(Form("plots/V10/%s_%s.pdf", title.Data(), step.Data()));
    }
  }
}
