#include "SampleAnalyzer/User/Analyzer/tcH.h"
#include "SampleAnalyzer/User/Analyzer/AnalysisHelper.h"
#include "SampleAnalyzer/User/Analyzer/XsectionBkgExtractor.h"

using namespace MA5;
using namespace std;

// -----------------------------------------------------------------------------
// Initialize
// function called one time at the beginning of the analysis
// -----------------------------------------------------------------------------
bool tcH::Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters)
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

  for(int i = 0; i < 7 ; i++){

    TDirectory * dir = output->mkdir(Form("S%i",i));
    dir->cd();

    h_photon1pt[i]       = new TH1F("photon1pt", "photon1pt", 100, 0, 100);
    h_photon2pt[i]       = new TH1F("photon2pt", "photon2pt", 100, 0, 100);
    h_photon1iso[i]       = new TH1F("photon1iso", "photon1iso", 100, 0, 1);
    h_photon2iso[i]       = new TH1F("photon2iso", "photon2iso", 100, 0, 1);
    h_diphotonM[i]       = new TH1F("diphotonM", "diphotonM", 100, 80, 180);
    h_MET[i]       = new TH1F("MET", "MET", 100, 0, 100);
    h_nbjets[i]       = new TH1F("nbjets", "nbjets", 5, 0, 5); 
    h_njets[i]       = new TH1F("njets", "njets", 10, 0, 10); 
    h_nphotons[i]       = new TH1F("nphotons", "nphotons", 5, 0, 5); 
    h_nelectrons[i]       = new TH1F("nelectrons", "nelectrons", 5, 0, 5); 
    h_nmuons[i]       = new TH1F("nmuons", "nmuons", 5, 0, 5); 
    h_nleptons[i]       = new TH1F("nleptons", "nleptons", 5, 0, 5); 
    h_mtop[i]           = new TH1F("mtop","mtop",200, 130, 230);
    h_drjetlep[i]           = new TH1F("drjetlep","drjetlep", 200, 0, 2);
    h_drjetpho[i]           = new TH1F("drjetpho","drjetpho", 200, 0, 2);

    output->cd();
  }
 
  cutFlow->Sumw2();

  //----------------------------------------------------
  //List here the sample names 
  //and assign the corresponding  cross section
  //if(outputName_ == "ttbar_jets_2l"               ) xsec= 27.2;
  xsec = -1;

  // Initialization of cross section table
  XsectionBkgExtractor bkg_table;
  if (!bkg_table.Initialize()) return false;
  if (!bkg_table.CheckTable()) return false;
  if (bkg_table.DetectAndLoadProcess(cfg.GetInputName()))
  {
    xsec = bkg_table.GetXsection(); // pb
  }
  else
  {
    if(outputName_ == "TT_Kappa_Hct_HtoAA") xsec = 7.6*0.00228; //Random chosen number in order to get some results -> needs to be changed to something plausible
    else std::cout << "ERROR: no xsection found for the file " << cfg.GetInputName() << std::endl;
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
void tcH::Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files)
{
  cout << "BEGIN Finalization" << endl;
  // saving histos
  // saving histos
  float scale = weight/nevent;
  std::cout << "nevent= " << nevent << std::endl;
  std::cout << "scale= " << scale << std::endl; 
  cutFlow->Scale(scale); 
  cutFlow->GetXaxis()->SetBinLabel(1, "no sel");
  cutFlow->GetXaxis()->SetBinLabel(2, "2 photons");
  cutFlow->GetXaxis()->SetBinLabel(3, ">= 2 jets");
  cutFlow->GetXaxis()->SetBinLabel(4, ">= 1 btagged jets");
  cutFlow->GetXaxis()->SetBinLabel(5, "= 1 btagged jet");
  cutFlow->GetXaxis()->SetBinLabel(6, "hadronic");
  cutFlow->GetXaxis()->SetBinLabel(7, "leptonic");

  for(int i = 0 ; i < 7; i++){ 
    h_photon1pt[i]->Scale(scale);
    h_photon2pt[i]->Scale(scale);
    h_photon1iso[i]->Scale(scale);
    h_photon2iso[i]->Scale(scale);
    h_diphotonM[i]->Scale(scale);
    h_MET[i]->Scale(scale);
    h_nbjets[i]->Scale(scale);
    h_njets[i]->Scale(scale);
    h_nphotons[i]->Scale(scale);
    h_nelectrons[i]->Scale(scale);
    h_nmuons[i]->Scale(scale);
    h_nleptons[i]->Scale(scale);
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
bool tcH::Execute(SampleFormat& sample, const EventFormat& event)
{
  // ***************************************************************************
  // Example of analysis with generated particles
  // Concerned samples : LHE/STDHEP/HEPMC
  // ***************************************************************************

  AnalysisHelper tool; //Initialize the AnalysisHelper, with many very interesting tools
  tool.Initialize(sample,event);

  if (event.rec()!=0)
  {

    nevent = nevent+1;

    cutFlow->Fill(0., weight);
  
    std::vector< RecPhotonFormat > selPhotonsColl;
    std::vector< RecPhotonFormat > isoPhotonsColl;

    //****************************************
    //************ photon selection **********
    //****************************************
 
    for (unsigned int i=0;i<event.rec()->photons().size();i++)
    {
      const RecPhotonFormat& pho = event.rec()->photons()[i];
      double reliso = isolation(3, pho);
      if(pho.pt() > 30 && fabs(pho.eta()) < 2.5 ){
        selPhotonsColl.push_back(pho);
        if(reliso < 0.02 ) isoPhotonsColl.push_back(pho);
      }
    } 

    //****************************************
    //************ lepton selection **********
    //****************************************
  
    std::vector< RecLeptonFormat > selElecsColl;
    std::vector< RecLeptonFormat > selMuonsColl;
    std::vector<const RecLeptonFormat*> selLeptonsColl;   
 
    for (unsigned int i=0;i<event.rec()->electrons().size();i++)
    {
      const RecLeptonFormat& elec = event.rec()->electrons()[i];
      double reliso = isolation(3, elec);
      if(elec.pt() > 20 && fabs(elec.eta()) < 2.5 && reliso < 0.1){
        selElecsColl.push_back(elec);
        selLeptonsColl.push_back(&elec);
      }
    }
    
     for (unsigned int i=0;i<event.rec()->muons().size();i++)
    {
      const RecLeptonFormat& mu = event.rec()->muons()[i];
      double reliso = isolation(3, mu);
      if(mu.pt() > 20 && fabs(mu.eta()) < 2.5 && reliso < 0.1){
        selMuonsColl.push_back(mu);
        selLeptonsColl.push_back(&mu);
      }
    }
    
     
    //****************************************
    //************* jet selection ************
    //****************************************
    
    std::vector<const RecJetFormat*> cleanJetsColl = tool.JetCleaning(event, selLeptonsColl, 0.5, 30);  //Clean the jets which are associated to isolated electrons and muons

    std::vector< RecJetFormat > selJetsColl;
    std::vector< RecJetFormat > selBJetsColl;
    int nbjets=0;

    for (unsigned int i=0; i < cleanJetsColl.size();i++)
    {
      const RecJetFormat& jet = *cleanJetsColl[i];
      TLorentzVector tmpjet;
      tmpjet.SetPtEtaPhiM( jet.pt(), jet.eta(), jet.phi(), jet.m() );

      bool overlap = false;
      for(unsigned int j = 0 ; j < isoPhotonsColl.size() ; j++){
        TLorentzVector tmpisoph;
        tmpisoph.SetPtEtaPhiM( isoPhotonsColl[j].pt(), isoPhotonsColl[j].eta(), isoPhotonsColl[j].phi(), 0);
        double dr = tmpjet.DeltaR(tmpisoph); 
        if( dr < 0.5 ) overlap = true;
      } 
      if( overlap ) continue;

      if(jet.pt()> 30 && fabs(jet.eta()) < 2.5 && jet.EEoverHE() > 0.3){
        selJetsColl.push_back(jet);
        bool btagged = tool.isCSVT(&jet);
        if( btagged ) nbjets++;
      }
    }

    //Fill histograms at Step 0
    if( selPhotonsColl.size() > 1){
      h_photon1pt[0]->Fill( selPhotonsColl[0].pt());
      h_photon2pt[0]->Fill( selPhotonsColl[1].pt());
      double photon1Iso = isolation(3, selPhotonsColl[0] );
      double photon2Iso = isolation(3, selPhotonsColl[1] );
      h_photon1iso[0]->Fill( photon1Iso );
      h_photon2iso[0]->Fill( photon2Iso );
    }

    h_nphotons[0]->Fill( selPhotonsColl.size() );
    h_njets[0]->Fill( selJetsColl.size() );
    h_nbjets[0]->Fill( nbjets );
    h_nleptons[0]->Fill( selElecsColl.size() + selMuonsColl.size() );
    h_nmuons[0]->Fill( selMuonsColl.size() );
    h_nelectrons[0]->Fill( selElecsColl.size() );
    h_MET[0]->Fill( event.rec()->MET().pt() );

    //****************************************
    //************ events selection **********
    //****************************************
    bool pre=false;
    if( isoPhotonsColl.size() > 1){
      pre = isoPhotonsColl[0].pt() > 60 && isoPhotonsColl[0].pt() > 30;     
    }    

    //at least 2 photons
    if( !pre ) return true;

 
    cutFlow->Fill(1.);
    
    std::string thechannel = "";
    if(     (selElecsColl.size() + selMuonsColl.size()) > 0 )    thechannel = "leptonic";
    else                                                         thechannel = "hadronic";


    TLorentzVector ph1, ph2;
    ph1.SetPtEtaPhiM( selPhotonsColl[0].pt(), selPhotonsColl[0].eta(), selPhotonsColl[0].phi(), 0);
    ph2.SetPtEtaPhiM( selPhotonsColl[1].pt(), selPhotonsColl[1].eta(), selPhotonsColl[1].phi(), 0);

    TLorentzVector isoph1, isoph2;
    isoph1.SetPtEtaPhiM( isoPhotonsColl[0].pt(), isoPhotonsColl[0].eta(), isoPhotonsColl[0].phi(), 0);
    isoph2.SetPtEtaPhiM( isoPhotonsColl[1].pt(), isoPhotonsColl[1].eta(), isoPhotonsColl[1].phi(), 0);
    double diphotonM = (isoph1+isoph2).M();

    double topmass = 0;
    double diff = 999;
    double dr_lep = 999;
    double dr_pho = 999;
    for (unsigned int i=0; i < selJetsColl.size() ;i++)
    {
      TLorentzVector tmpjet;
      tmpjet.SetPtEtaPhiM( selJetsColl[i].pt(), selJetsColl[i].eta(), selJetsColl[i].phi(), selJetsColl[i].m() ); 
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
    h_photon1iso[1]->Fill( isolation(3, selPhotonsColl[0] ) );
    h_photon2iso[1]->Fill( isolation(3, selPhotonsColl[1] ) );
    h_photon1pt[1]->Fill( isoPhotonsColl[0].pt());
    h_photon2pt[1]->Fill( isoPhotonsColl[1].pt());
    h_diphotonM[1]->Fill( diphotonM );
    h_nphotons[1]->Fill( isoPhotonsColl.size() );
    h_njets[1]->Fill( selJetsColl.size() );
    h_nbjets[1]->Fill( nbjets );
    h_nleptons[1]->Fill( selElecsColl.size() + selMuonsColl.size() );
    h_nmuons[1]->Fill( selMuonsColl.size() );
    h_nelectrons[1]->Fill( selElecsColl.size() );
    h_MET[1]->Fill( event.rec()->MET().pt() );
    h_mtop[1]->Fill(topmass);
    h_drjetlep[1]->Fill(dr_lep);
    h_drjetpho[1]->Fill(dr_pho);

    //at least 2 jets
    if(selJetsColl.size() < 2)  return true;
    cutFlow->Fill(2.);
   
    //Fill histograms at Step 2 
    h_photon1iso[2]->Fill( isolation(3, selPhotonsColl[0] ) );
    h_photon2iso[2]->Fill( isolation(3, selPhotonsColl[1] ) );
    h_photon1pt[2]->Fill( isoPhotonsColl[0].pt());
    h_photon2pt[2]->Fill( isoPhotonsColl[1].pt());
    h_diphotonM[2]->Fill( diphotonM );
    h_nphotons[2]->Fill( isoPhotonsColl.size() );
    h_njets[2]->Fill( selJetsColl.size() );
    h_nbjets[2]->Fill( nbjets );
    h_nleptons[2]->Fill( selElecsColl.size() + selMuonsColl.size() );
    h_nmuons[2]->Fill( selMuonsColl.size() );
    h_nelectrons[2]->Fill( selElecsColl.size() );
    h_MET[2]->Fill( event.rec()->MET().pt() );
    h_mtop[2]->Fill(topmass);
    h_drjetlep[2]->Fill(dr_lep);
    h_drjetpho[2]->Fill(dr_pho);


    //at least 2 bjets
    if(nbjets < 1 ) return true;
    cutFlow->Fill(3.);
    
    //Fill histograms at Step 3 
    h_photon1iso[3]->Fill( isolation(3, selPhotonsColl[0] ) );
    h_photon2iso[3]->Fill( isolation(3, selPhotonsColl[1] ) );
    h_photon1pt[3]->Fill( isoPhotonsColl[0].pt());
    h_photon2pt[3]->Fill( isoPhotonsColl[1].pt());
    h_diphotonM[3]->Fill( diphotonM );
    h_nphotons[3]->Fill( isoPhotonsColl.size() );
    h_njets[3]->Fill( selJetsColl.size() );
    h_nbjets[3]->Fill( nbjets );
    h_nleptons[3]->Fill( selElecsColl.size() + selMuonsColl.size() );
    h_nmuons[3]->Fill( selMuonsColl.size() );
    h_nelectrons[3]->Fill( selElecsColl.size() );
    h_MET[3]->Fill( event.rec()->MET().pt() );
    h_mtop[3]->Fill(topmass);
    h_drjetlep[3]->Fill(dr_lep);
    h_drjetpho[3]->Fill(dr_pho);


    //met > 20 GeV
    //if( event.rec()->MET().pt() < 20 ) return true;
    // exactly 1 b-tag
    if(nbjets > 1 ) return true;
    cutFlow->Fill(4.);
   
    //Fill histograms at Step 4 
    h_photon1iso[4]->Fill( isolation(3, selPhotonsColl[0] ) );
    h_photon2iso[4]->Fill( isolation(3, selPhotonsColl[1] ) );
    h_photon1pt[4]->Fill( isoPhotonsColl[0].pt());
    h_photon2pt[4]->Fill( isoPhotonsColl[1].pt());
    h_diphotonM[4]->Fill( diphotonM );
    h_nphotons[4]->Fill( isoPhotonsColl.size() );
    h_njets[4]->Fill( selJetsColl.size() );
    h_nbjets[4]->Fill( nbjets );
    h_nleptons[4]->Fill( selElecsColl.size() + selMuonsColl.size() );
    h_nmuons[4]->Fill( selMuonsColl.size() );
    h_nelectrons[4]->Fill( selElecsColl.size() );
    h_MET[4]->Fill( event.rec()->MET().pt() ); 
    h_mtop[4]->Fill(topmass);
    h_drjetlep[4]->Fill(dr_lep);
    h_drjetpho[4]->Fill(dr_pho);
 
    if( (selElecsColl.size() + selMuonsColl.size()) < 1 ) {
      cutFlow->Fill(5.);

      h_photon1iso[5]->Fill( isolation(3, isoPhotonsColl[0] ) );
      h_photon2iso[5]->Fill( isolation(3, isoPhotonsColl[1] ) );
      h_photon1pt[5]->Fill( isoPhotonsColl[0].pt());
      h_photon2pt[5]->Fill( isoPhotonsColl[1].pt());
      h_diphotonM[5]->Fill( (isoph1+isoph2).M() );
      h_nphotons[5]->Fill( isoPhotonsColl.size() );
      h_njets[5]->Fill( selJetsColl.size() );
      h_nbjets[5]->Fill( nbjets );
      h_nleptons[5]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[5]->Fill( selMuonsColl.size() );
      h_nelectrons[5]->Fill( selElecsColl.size() );
      h_MET[5]->Fill( event.rec()->MET().pt() );
      h_mtop[5]->Fill(topmass);
      h_drjetlep[5]->Fill(dr_lep);
      h_drjetpho[5]->Fill(dr_pho);

    }else{
      cutFlow->Fill(6.);

      h_photon1iso[6]->Fill( isolation(3, isoPhotonsColl[0] ) );
      h_photon2iso[6]->Fill( isolation(3, isoPhotonsColl[1] ) );
      h_photon1pt[6]->Fill( isoPhotonsColl[0].pt());
      h_photon2pt[6]->Fill( isoPhotonsColl[1].pt());
      h_diphotonM[6]->Fill( (isoph1+isoph2).M() );
      h_nphotons[6]->Fill( isoPhotonsColl.size() );
      h_njets[6]->Fill( selJetsColl.size() );
      h_nbjets[6]->Fill( nbjets );
      h_nleptons[6]->Fill( selElecsColl.size() + selMuonsColl.size() );
      h_nmuons[6]->Fill( selMuonsColl.size() );
      h_nelectrons[6]->Fill( selElecsColl.size() );
      h_MET[6]->Fill( event.rec()->MET().pt() );
      h_mtop[6]->Fill(topmass);
      h_drjetlep[6]->Fill(dr_lep);
      h_drjetpho[6]->Fill(dr_pho);
    }
     
  }

  return true;
}

float tcH::isolation(int deltaR, const RecLeptonFormat& lept){
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

float tcH::isolation(int deltaR, const RecPhotonFormat& lept){
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


