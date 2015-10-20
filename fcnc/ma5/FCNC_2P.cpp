#include "SampleAnalyzer/User/Analyzer/FCNC_2P.h"
#include "SampleAnalyzer/User/Analyzer/AnalysisHelper.h"
#include "SampleAnalyzer/User/Analyzer/XsectionBkgExtractor.h"
#include "SampleAnalyzer/User/Analyzer/XsectionSignalExtractor.h"

using namespace MA5;
using namespace std;

// -----------------------------------------------------------------------------
// Initialize
// function called one time at the beginning of the analysis
// -----------------------------------------------------------------------------
bool FCNC_2P::Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters)
{
  cout << "BEGIN Initialization" << endl;
  // initialize variables, histos
  outputName_ = cfg.GetInputName();  
  
  nevent = 0;
  
  luminosity = 100000; // lumi in pb
  
  
  std::cout << "output file name is  " << outputName_ << std::endl;
  //----------------------------------------------------
  //output file 
  output = new TFile((outputName_+ ".root").c_str(),"RECREATE");
  //output = new TFile("output.root","RECREATE");
  output->cd();

  //----------------------------------------------------
  //histograms used for cutflow 
  cutFlow         = new TH1F("cutFlow", "cutFlow", 11, -0.5, 10.5);
  cutFlow2         = new TH1F("cutFlow2", "cutFlow2", 11, -0.5, 10.5);

  for(int i = 0; i < 10 ; i++){

    int k = i ;
    TString channel = "";
    if( i <= 5 && i >= 2 ) channel = "Leptonic";
    else if( i > 5 ) {
      channel = "Hadronic";
      k = i - 4; //just to make it readable by making it the same folder name as leptonic channel.
    }

    TDirectory * dir = output->mkdir(Form("%sS%i",channel.Data(),k));
    dir->cd();

    h_photon1pt[i]       = new TH1F("photon1pt", "photon1pt", 100, 0, 100);
    h_photon2pt[i]       = new TH1F("photon2pt", "photon2pt", 100, 0, 100);
    h_photon1iso[i]       = new TH1F("photon1iso", "photon1iso", 100, 0, 1);
    h_photon2iso[i]       = new TH1F("photon2iso", "photon2iso", 100, 0, 1);
    h_diphotonM[i]       = new TH1F("diphotonM", "diphotonM", 100, 80, 180);
    h_nphotons[i]       = new TH1F("nphotons", "nphotons", 5, 0, 5); 

    h_muon1pt[i]       = new TH1F("muon1pt", "muon1pt", 100, 0, 100);
    h_elec1pt[i]       = new TH1F("elec1pt", "elec1pt", 100, 0, 100);
    h_muon1iso[i]       = new TH1F("muon1iso", "muon1iso", 100, 0, 1);
    h_elec1iso[i]       = new TH1F("elec1iso", "elec1iso", 100, 0, 1);
    h_nelectrons[i]       = new TH1F("nelectrons", "nelectrons", 5, 0, 5);
    h_nmuons[i]       = new TH1F("nmuons", "nmuons", 5, 0, 5);
    h_nleptons[i]       = new TH1F("nleptons", "nleptons", 5, 0, 5); 

    h_MET[i]       = new TH1F("MET", "MET", 100, 0, 100);
    h_nbjetsCSVM[i]       = new TH1F("nbjetsCSVM", "nbjets", 5, 0, 5);
    h_nbjetsCSVT[i]       = new TH1F("nbjetsCSVT", "nbjets", 5, 0, 5);
    h_njets[i]       = new TH1F("njets", "njets", 10, 0, 10);
    h_mtop[i]           = new TH1F("mtop","mtop",100, 130, 230);
    h_drjetlep[i]           = new TH1F("drjetlep","drjetlep", 200, 0, 2);
    h_drjetpho[i]           = new TH1F("drjetpho","drjetpho", 200, 0, 2);

    output->cd();
  }
 
  cutFlow->Sumw2();
  cutFlow2->Sumw2();

  //----------------------------------------------------
  //List here the sample names 
  //and assign the corresponding  cross section
  //if(outputName_ == "ttbar_jets_2l"               ) xsec= 27.2;
  xsec = -1;

  // Initialization of cross section table
  //XsectionBkgExtractor bkg_table;
  //if (!bkg_table.Initialize()) return false;
  //if (!bkg_table.CheckTable()) return false;
  //if (bkg_table.DetectAndLoadProcess(cfg.GetInputName()))
  //{
  //  xsec = bkg_table.GetXsection(); // pb
  //}
  //else
  //{
    //if(outputName_ == "TT_Kappa_Hct_HtoAA" ) xsec = 7.6*0.00228; //Random chosen number in order to get some results -> needs to be changed to something plausible
  //  else std::cout << "ERROR: no xsection found for the file " << cfg.GetInputName() << std::endl;
  //}

  // Get cross sections from common area 
  XsectionBkgExtractor bkg_table;
  XsectionSignalExtractor sgn_table;
  if (!bkg_table.Initialize()) return false;
  if (!bkg_table.CheckTable()) return false;
  if (!sgn_table.Initialize()) return false;
  if (!sgn_table.CheckTable()) return false;
  if (sgn_table.DetectAndLoadProcess(cfg.GetInputName()))
  {
        xsec = sgn_table.GetXsection_fixingBRlimit(); //in pb
  }
  else if (bkg_table.DetectAndLoadProcess(cfg.GetInputName()))
  {
        xsec = bkg_table.GetXsection(); //in pb
  }
  else
  {
        if(outputName_ == "TT_Kappa_Hct_HtoAA" ) xsec = 7.6*0.00228; //Random chosen number in order to get some results -> needs to be changed to something plausible
        std::cout << "ERROR: no xsection found for the file " << cfg.GetInputName() << std::endl;
  }
  std::cout << "Great! The xsection is: " << xsec << " [pb]" << std::endl;
  if (xsec<0) return false;
  
  //----------------------------------------------------
  //Check if the sample exist
  std::cout<<std::endl;
  std::cout<<"--------------SECTION-EFFICACE-----------"<<std::endl;
  std::cout<<std::endl;
  std::cout<<"               x-sec : "<<xsec<<" pb"<<std::endl;
  if(xsec == 0) std::cout << "WARNING : nxsec is 0, most probably, samples unknown " << std::endl;
  std::cout<<std::endl;
  std::cout<<"-----------------------------------------"<<std::endl;
  
  
  //----------------------------------------------------
  //Calcul event weight
  //do not forget to devide by nevent
  // in "Finalize" 
  weight = luminosity*xsec;

  cout << "END   Initialization" << endl;
  return true;
}

// -----------------------------------------------------------------------------
// Finalize
// function called one time at the end of the analysis
// -----------------------------------------------------------------------------
void FCNC_2P::Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files)
{
  cout << "BEGIN Finalization" << endl;
  // saving histos
  // saving histos
  float scale = weight/nevent;
  std::cout << "xsec= " << xsec << std::endl;
  std::cout << "nevent= " << nevent << std::endl;
  std::cout << "luminosity= " << luminosity << std::endl;
  std::cout << "scale= " << scale << std::endl; 
  cutFlow->Scale(scale); 
  cutFlow->GetXaxis()->SetBinLabel(1, "no sel");
  cutFlow->GetXaxis()->SetBinLabel(2, "2 photons");
  cutFlow->GetXaxis()->SetBinLabel(3, "1 lepton");
  cutFlow->GetXaxis()->SetBinLabel(4, ">= 2 jets");
  cutFlow->GetXaxis()->SetBinLabel(5, "== 1 btagged jets");
  cutFlow->GetXaxis()->SetBinLabel(6, "mtop");

  cutFlow2->Scale(scale);
  cutFlow2->GetXaxis()->SetBinLabel(1, "no sel");
  cutFlow2->GetXaxis()->SetBinLabel(2, "2 photons");
  cutFlow2->GetXaxis()->SetBinLabel(3, "no lepton");
  cutFlow2->GetXaxis()->SetBinLabel(4, ">= 4 jets");
  cutFlow2->GetXaxis()->SetBinLabel(5, "== 1 btagged jets");
  cutFlow2->GetXaxis()->SetBinLabel(6, "mtop");

  for(int i = 0 ; i < 10; i++){ 
    h_photon1pt[i]->Scale(scale);
    h_photon2pt[i]->Scale(scale);
    h_photon1iso[i]->Scale(scale);
    h_photon2iso[i]->Scale(scale);
    h_diphotonM[i]->Scale(scale);
    h_nphotons[i]->Scale(scale);

    h_muon1pt[i]->Scale(scale);
    h_elec1pt[i]->Scale(scale);
    h_muon1iso[i]->Scale(scale);
    h_elec1iso[i]->Scale(scale);
    h_nelectrons[i]->Scale(scale);
    h_nmuons[i]->Scale(scale);
    h_nleptons[i]->Scale(scale);


    h_MET[i]->Scale(scale);
    h_nbjetsCSVM[i]->Scale(scale);
    h_nbjetsCSVT[i]->Scale(scale);
    h_njets[i]->Scale(scale);
    h_mtop[i]->Scale(scale);
    h_drjetlep[i]->Scale(scale);
    h_drjetpho[i]->Scale(scale);
  }

  output->Write();
  cout << "END   Finalization" << endl;
}

// -----------------------------------------------------------------------------
// Execute
// function called each time one event is read
// -----------------------------------------------------------------------------
bool FCNC_2P::Execute(SampleFormat& sample, const EventFormat& event)
{
  // ***************************************************************************
  // Example of analysis with generated particles
  // Concerned samples : LHE/STDHEP/HEPMC
  // ***************************************************************************

  AnalysisHelper tool; //Initialize the AnalysisHelper, with many very interesting tools
  tool.Initialize(sample,event);

  AnalysisHelper::ConeType cone03 = AnalysisHelper::CONE03;
  //AnalysisHelper::ConeType cone04 = AnalysisHelper::CONE04;

  if (event.rec()!=0)
  {

    nevent = nevent+1;

    cutFlow->Fill(0.);
    cutFlow2->Fill(0.);
  
    std::vector< RecPhotonFormat > selPhotonsColl;
    std::vector<const RecPhotonFormat*> isoPhotonsColl;

    //****************************************
    //************ photon selection **********
    //****************************************
    double threshold = 0.1; 
    for (unsigned int i=0;i<event.rec()->photons().size();i++)
    {
      const RecPhotonFormat& pho = event.rec()->photons()[i];
      //double reliso = tool.CombinedIsolationObservable(&pho, cone03);
      if(pho.pt() > 30 && fabs(pho.eta()) < 2.5 ){
        selPhotonsColl.push_back(pho);
        if( tool.passCombinedIsolation(pho, threshold, cone03) ) isoPhotonsColl.push_back(&pho);
      }
    } 
    //****************************************
    //************ lepton selection **********
    //****************************************
  
    std::vector< RecLeptonFormat > selElecsColl;
    std::vector< RecLeptonFormat > selMuonsColl;
    std::vector<const RecLeptonFormat*> selLeptonsColl;   

    double muon_iso = -1.0;
    double elec_iso = -1.0;
 
    for (unsigned int i=0;i<event.rec()->electrons().size();i++)
    {
      const RecLeptonFormat& elec = event.rec()->electrons()[i];
      double reliso = tool.CombinedIsolationObservable(&elec,cone03);
      elec_iso = reliso ;
      if(elec.pt() > 20 && fabs(elec.eta()) < 2.4 && tool.passCombinedIsolation(elec, threshold, cone03) ){
        selElecsColl.push_back(elec);
        selLeptonsColl.push_back(&elec);
      }
    }
    
    for (unsigned int i=0;i<event.rec()->muons().size();i++)
    {
      const RecLeptonFormat& mu = event.rec()->muons()[i];
      double reliso = tool.CombinedIsolationObservable(&mu, cone03);
      muon_iso = reliso;
      if(mu.pt() > 20 && fabs(mu.eta()) < 2.4 && tool.passCombinedIsolation(mu, threshold, cone03)){
        selMuonsColl.push_back(mu);
        selLeptonsColl.push_back(&mu);
      }
    }
    
     
    //****************************************
    //************* jet selection ************
    //****************************************
  
    std::vector<const RecJetFormat*> jetsColl;
 
    for (unsigned int i=0;i<event.rec()->jets().size();i++)
    {
      const RecJetFormat& jet = event.rec()->jets()[i];
      double eoverh = 1.0/0.15;
      if(jet.pt() > 20 && fabs(jet.eta()) < 2.5 && jet.EEoverHE() < eoverh ){
        jetsColl.push_back(&jet);
      }
    }

    double minDR = 0.5;
    double minPT = 20.0;
 
    std::vector<const RecJetFormat*> NoLeptonJetsColl = tool.JetCleaning(jetsColl, selLeptonsColl, minDR, minPT);  //Clean the jets which are associated to isolated electrons and muons
    std::vector<const RecJetFormat*> cleanJetsColl = tool.JetCleaning(NoLeptonJetsColl, isoPhotonsColl, minDR, minPT);  //Clean the jets which are associated to isolated electrons and muons

    std::vector< RecJetFormat > selJetsColl;
    std::vector< RecJetFormat > selBJetsColl;
    int nbjetsCSVM=0;
    int nbjetsCSVT=0;

    for (unsigned int i=0; i < cleanJetsColl.size();i++)
    {
      //add further cleaning jets with isolated photons
      const RecJetFormat& jet = *cleanJetsColl[i];
      TLorentzVector tmpjet;
      tmpjet.SetPtEtaPhiM( jet.pt(), jet.eta(), jet.phi(), 0 );
      bool overlap = false;
      for(unsigned int j = 0 ; j < isoPhotonsColl.size() ; j++){
        const RecPhotonFormat& tmpph = *isoPhotonsColl[j];
        TLorentzVector tmpisoph;
        tmpisoph.SetPtEtaPhiM( tmpph.pt(), tmpph.eta(), tmpph.phi(), 0);
        double dr = tmpjet.DeltaR(tmpisoph); 
        if( dr < 0.5 ) overlap = true;
      } 

      if( overlap ) continue;

      selJetsColl.push_back(jet);

      //b-tagging
      bool btaggedCSVM = tool.isCSVM(&jet);
      if( btaggedCSVM ) nbjetsCSVM++;
      bool btaggedCSVT = tool.isCSVT(&jet);
      if( btaggedCSVT ) nbjetsCSVT++;
    }  

    //Fill histograms at Step 0
    if( selPhotonsColl.size() > 1){
      h_photon1pt[0]->Fill( selPhotonsColl[0].pt());
      h_photon2pt[0]->Fill( selPhotonsColl[1].pt());
      h_photon1iso[0]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
      h_photon2iso[0]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
    }

    h_muon1iso[0]->Fill( muon_iso );
    h_elec1iso[0]->Fill( elec_iso );

    h_nphotons[0]->Fill( selPhotonsColl.size() );
    h_njets[0]->Fill( selJetsColl.size() );
    h_nbjetsCSVM[0]->Fill( nbjetsCSVM );
    h_nbjetsCSVT[0]->Fill( nbjetsCSVT );
    h_nleptons[0]->Fill( selElecsColl.size() + selMuonsColl.size() );
    h_nmuons[0]->Fill( selMuonsColl.size() );
    h_nelectrons[0]->Fill( selElecsColl.size() );
    h_MET[0]->Fill( event.rec()->MET().pt() );

    
    //****************************************
    //************ events selection **********
    //****************************************
    if( isoPhotonsColl.size() <= 1) return true;

    const RecPhotonFormat& isoPho1 = *isoPhotonsColl[0];
    const RecPhotonFormat& isoPho2 = *isoPhotonsColl[1];
    bool pre = isoPho1.pt() > 60 && isoPho2.pt() > 30;     

    //at least 2 photons
    if( !pre ) return true;

 
    cutFlow->Fill(1.);
    cutFlow2->Fill(1.);
    
    std::string thechannel = "";
    if(     (selElecsColl.size() + selMuonsColl.size()) == 1 )    thechannel = "leptonic";
    else if( (selElecsColl.size() + selMuonsColl.size()) == 0 )     thechannel = "hadronic";


    TLorentzVector ph1, ph2;
    ph1.SetPtEtaPhiM( selPhotonsColl[0].pt(), selPhotonsColl[0].eta(), selPhotonsColl[0].phi(), 0);
    ph2.SetPtEtaPhiM( selPhotonsColl[1].pt(), selPhotonsColl[1].eta(), selPhotonsColl[1].phi(), 0);

    TLorentzVector isoph1, isoph2;
    isoph1.SetPtEtaPhiM( isoPho1.pt(), isoPho1.eta(), isoPho1.phi(), 0);
    isoph2.SetPtEtaPhiM( isoPho2.pt(), isoPho2.eta(), isoPho2.phi(), 0);
    double diphotonM = (isoph1+isoph2).M();

    double topmass = 0;
    double diff = 999;
    double dr_lep = 999;
    double dr_pho = 999;
    for (unsigned int i=0; i < selJetsColl.size() ;i++)
    {
      TLorentzVector tmpjet;
      tmpjet.SetPtEtaPhiM( selJetsColl[i].pt(), selJetsColl[i].eta(), selJetsColl[i].phi(), 0 ); 
      double tmptopmass = (isoph1+isoph2+tmpjet).M();
      double tmpdiff = abs(topmass-173.3);
      if ( tmpdiff < diff ) topmass = tmptopmass;
      double tmpdr1 = tmpjet.DeltaR(isoph1);
      double tmpdr2 = tmpjet.DeltaR(isoph2);
      if( tmpdr1 > tmpdr2 ) { dr_pho = tmpdr2; }
      else { dr_pho = tmpdr1; }
      
      for (unsigned int i=0; i < selLeptonsColl.size();i++)
      {
        const RecLeptonFormat& l = *selLeptonsColl[i];
        TLorentzVector tmpl;
        tmpl.SetPtEtaPhiM( l.pt(), l.eta(), l.phi(), 0 );
        double tmpdr = tmpjet.DeltaR(tmpl);
        if( tmpdr < dr_lep ) { dr_lep = tmpdr ; }
      }
    }

    //Fill histograms at Step 1
    h_photon1iso[1]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
    h_photon2iso[1]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
    h_photon1pt[1]->Fill( isoPho1.pt());
    h_photon2pt[1]->Fill( isoPho2.pt());
    h_diphotonM[1]->Fill( diphotonM );
    h_nphotons[1]->Fill( isoPhotonsColl.size() );
    h_njets[1]->Fill( selJetsColl.size() );
    h_nbjetsCSVM[1]->Fill( nbjetsCSVM );
    h_nbjetsCSVT[1]->Fill( nbjetsCSVT );
    h_nleptons[1]->Fill( selElecsColl.size() + selMuonsColl.size() );
    h_nmuons[1]->Fill( selMuonsColl.size() );
    h_nelectrons[1]->Fill( selElecsColl.size() );
    h_MET[1]->Fill( event.rec()->MET().pt() );
    h_mtop[1]->Fill(topmass);
    h_drjetlep[1]->Fill(dr_lep);
    h_drjetpho[1]->Fill(dr_pho);


    if( thechannel == "leptonic" ) {
      cutFlow->Fill(2.);
 
      //Fill histograms at Step 2 
      h_photon1iso[2]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
      h_photon2iso[2]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
      h_photon1pt[2]->Fill( isoPho1.pt());
      h_photon2pt[2]->Fill( isoPho2.pt());
      h_diphotonM[2]->Fill( diphotonM );
      h_nphotons[2]->Fill( isoPhotonsColl.size() );
      h_njets[2]->Fill( selJetsColl.size() );
      h_nbjetsCSVM[2]->Fill( nbjetsCSVM );
      h_nbjetsCSVT[2]->Fill( nbjetsCSVT);
      h_nleptons[2]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[2]->Fill( selMuonsColl.size() );
      h_nelectrons[2]->Fill( selElecsColl.size() );
      h_MET[2]->Fill( event.rec()->MET().pt() );
      h_mtop[2]->Fill(topmass);
      h_drjetlep[2]->Fill(dr_lep);
      h_drjetpho[2]->Fill(dr_pho);

      if(selElecsColl.size() > 0 ) h_elec1pt[0]->Fill(selElecsColl[0].pt());
      if(selMuonsColl.size() > 0 ) h_muon1pt[0]->Fill(selMuonsColl[0].pt());


      if(selJetsColl.size() < 2)  return true;
      cutFlow->Fill(3.);
   
      //Fill histograms at Step 2 
      h_photon1iso[3]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
      h_photon2iso[3]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
      h_photon1pt[3]->Fill( isoPho1.pt());
      h_photon2pt[3]->Fill( isoPho2.pt());
      h_diphotonM[3]->Fill( diphotonM );
      h_nphotons[3]->Fill( isoPhotonsColl.size() );
      h_njets[3]->Fill( selJetsColl.size() );
      h_nbjetsCSVM[3]->Fill( nbjetsCSVM );
      h_nbjetsCSVT[3]->Fill( nbjetsCSVT );
      h_nleptons[3]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[3]->Fill( selMuonsColl.size() );
      h_nelectrons[3]->Fill( selElecsColl.size() );
      h_MET[3]->Fill( event.rec()->MET().pt() );
      h_mtop[3]->Fill(topmass);
      h_drjetlep[3]->Fill(dr_lep);
      h_drjetpho[3]->Fill(dr_pho);


      //exactly one bjets
      //if(nbjets < 1 || nbjets > 1 ) return true;
      if(nbjetsCSVT < 1 || nbjetsCSVT > 1 ) return true;
      //if(nbjets < 1 ) return true;
      cutFlow->Fill(4.);
    
      //Fill histograms at Step 3 
      h_photon1iso[4]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
      h_photon2iso[4]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
      h_photon1pt[4]->Fill( isoPho1.pt());
      h_photon2pt[4]->Fill( isoPho2.pt());
      h_diphotonM[4]->Fill( diphotonM );
      h_nphotons[4]->Fill( isoPhotonsColl.size() );
      h_njets[4]->Fill( selJetsColl.size() );
      h_nbjetsCSVM[4]->Fill( nbjetsCSVM );
      h_nbjetsCSVT[4]->Fill( nbjetsCSVT );
      h_nleptons[4]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[4]->Fill( selMuonsColl.size() );
      h_nelectrons[4]->Fill( selElecsColl.size() );
      h_MET[4]->Fill( event.rec()->MET().pt() );
      h_mtop[4]->Fill(topmass);
      h_drjetlep[4]->Fill(dr_lep);
      h_drjetpho[4]->Fill(dr_pho);

      //met > 20 GeV
      //if( event.rec()->MET().pt() < 20 ) return true;
      // mtop
      double minmtop = 173.3-15.0;
      double maxmtop = 173.3+15.0;

      if( (topmass > maxmtop) || (topmass < minmtop ) ) return true;
      cutFlow->Fill(5.);
      //Fill histograms at Step 4 
      h_photon1iso[5]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
      h_photon2iso[5]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
      h_photon1pt[5]->Fill( isoPho1.pt());
      h_photon2pt[5]->Fill( isoPho2.pt());
      h_diphotonM[5]->Fill( diphotonM );
      h_nphotons[5]->Fill( isoPhotonsColl.size() );
      h_njets[5]->Fill( selJetsColl.size() );
      h_nbjetsCSVM[5]->Fill( nbjetsCSVM );
      h_nbjetsCSVT[5]->Fill( nbjetsCSVT );
      h_nleptons[5]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[5]->Fill( selMuonsColl.size() );
      h_nelectrons[5]->Fill( selElecsColl.size() );
      h_MET[5]->Fill( event.rec()->MET().pt() ); 
      h_mtop[5]->Fill(topmass);
      h_drjetlep[5]->Fill(dr_lep);
      h_drjetpho[5]->Fill(dr_pho);
    }else if( thechannel == "hadronic" ) {

      cutFlow2->Fill(2.);

      h_photon1iso[6]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
      h_photon2iso[6]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
      h_photon1pt[6]->Fill( isoPho1.pt());
      h_photon2pt[6]->Fill( isoPho2.pt());
      h_diphotonM[6]->Fill( (isoph1+isoph2).M() );
      h_nphotons[6]->Fill( isoPhotonsColl.size() );
      h_njets[6]->Fill( selJetsColl.size() );
      h_nbjetsCSVM[6]->Fill( nbjetsCSVM );
      h_nbjetsCSVT[6]->Fill( nbjetsCSVT );
      h_nleptons[6]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[6]->Fill( selMuonsColl.size() );
      h_nelectrons[6]->Fill( selElecsColl.size() );
      h_MET[6]->Fill( event.rec()->MET().pt() );
      h_mtop[6]->Fill(topmass);
      h_drjetpho[6]->Fill(dr_pho);

      if(selJetsColl.size() < 3)  return true;
      cutFlow2->Fill(3.);

      h_photon1iso[7]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
      h_photon2iso[7]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
      h_photon1pt[7]->Fill( isoPho1.pt());
      h_photon2pt[7]->Fill( isoPho2.pt());
      h_diphotonM[7]->Fill( (isoph1+isoph2).M() );
      h_nphotons[7]->Fill( isoPhotonsColl.size() );
      h_njets[7]->Fill( selJetsColl.size() );
      h_nbjetsCSVM[7]->Fill( nbjetsCSVM );
      h_nbjetsCSVT[7]->Fill( nbjetsCSVT );
      h_nleptons[7]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[7]->Fill( selMuonsColl.size() );
      h_nelectrons[7]->Fill( selElecsColl.size() );
      h_MET[7]->Fill( event.rec()->MET().pt() );
      h_mtop[7]->Fill(topmass);
      h_drjetpho[7]->Fill(dr_pho);

      //exactly one bjets
      //if(nbjets < 1 || nbjets > 1 ) return true;
      if(nbjetsCSVT < 1 || nbjetsCSVT > 1 ) return true;
      cutFlow2->Fill(4.);

      h_photon1iso[8]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
      h_photon2iso[8]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
      h_photon1pt[8]->Fill( isoPho1.pt());
      h_photon2pt[8]->Fill( isoPho2.pt());
      h_diphotonM[8]->Fill( (isoph1+isoph2).M() );
      h_nphotons[8]->Fill( isoPhotonsColl.size() );
      h_njets[8]->Fill( selJetsColl.size() );
      h_nbjetsCSVM[8]->Fill( nbjetsCSVM );
      h_nbjetsCSVT[8]->Fill( nbjetsCSVT );
      h_nleptons[8]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[8]->Fill( selMuonsColl.size() );
      h_nelectrons[8]->Fill( selElecsColl.size() );
      h_MET[8]->Fill( event.rec()->MET().pt() );
      h_mtop[8]->Fill(topmass);
      h_drjetpho[8]->Fill(dr_pho);

      if( (topmass > 183.0) || (topmass < 163.0) ) return true;
      cutFlow2->Fill(5.);
    
      h_photon1iso[9]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[0],cone03) );
      h_photon2iso[9]->Fill( tool.CombinedIsolationObservable(&selPhotonsColl[1],cone03) );
      h_photon1pt[9]->Fill( isoPho1.pt());
      h_photon2pt[9]->Fill( isoPho2.pt());
      h_diphotonM[9]->Fill( (isoph1+isoph2).M() );
      h_nphotons[9]->Fill( isoPhotonsColl.size() );
      h_njets[9]->Fill( selJetsColl.size() );
      h_nbjetsCSVM[9]->Fill( nbjetsCSVM );
      h_nbjetsCSVT[9]->Fill( nbjetsCSVT );
      h_nleptons[9]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[9]->Fill( selMuonsColl.size() );
      h_nelectrons[9]->Fill( selElecsColl.size() );
      h_MET[9]->Fill( event.rec()->MET().pt() );
      h_mtop[9]->Fill(topmass);
      h_drjetpho[9]->Fill(dr_pho);


    }
     
  }
  return true;
}

float FCNC_2P::isolation(int deltaR, const RecLeptonFormat& lept){
  //deltaR = 0 -> cone de 0.5
  //deltaR = 1 -> cone de 0.4
  //deltaR = 2 -> cone de 0.3
  //deltaR = 3 -> cone de 0.2

  std::vector<IsolationConeType> isol = lept.isolCones();

  //sum isol[deltaR].ntracks() ;
  float sumpt = isol[deltaR].sumPT()   ;
  //float sumet = isol[deltaR].sumET()   ;
  //float isolation = (sumpt+sumet)/lept.pt();
  float isolation = (sumpt)/lept.pt();

  return isolation;

}

float FCNC_2P::isolation(int deltaR, const RecPhotonFormat& lept){
  //deltaR = 0 -> cone de 0.5
  //deltaR = 1 -> cone de 0.4
  //deltaR = 2 -> cone de 0.3
  //deltaR = 3 -> cone de 0.2

  std::vector<IsolationConeType> isol = lept.isolCones();

  //sum isol[deltaR].ntracks() ;
  float sumpt = isol[deltaR].sumPT()   ;
  //float sumet = isol[deltaR].sumET()   ;
  //float isolation = (sumpt+sumet)/lept.pt();
  float isolation = (sumpt)/lept.pt();

  return isolation;

}


