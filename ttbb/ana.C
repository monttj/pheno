/*
root -l -b -q ana.C'("step_1.root", "step_1_plots.root")'
*/

//------------------------------------------------------------------------------
#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif
#include <vector>

GenParticle* MotherParticle(const GenParticle* p, const TClonesArray* branchParticle, int i = 1){

  if( p == 0) return 0;

  int PID = p->PID;
  int index[2];
  index[i-1] = p->M1;
  index[i-1] = p->M2;

  GenParticle * m = 0;

  if( index[i] > 0){
    m = (GenParticle *) branchParticle->At( index[i] );
    double mPID = m->PID;
    cout << "particle " << PID << " (" << p->P4().Pt() << ") " << " is from " << " mother (M" << i << ") " << mPID << " ( " << m->P4().Pt() << " ) " << endl;
  }else{
    cout << "No mother particle " << "M" << i << " from  " << PID << "  !!!" << endl;
  }

  return m; 

}

GenParticle* DaughterParticle(const GenParticle* p, const TClonesArray* branchParticle, int i = 0){

  if( p == 0) return 0;

  int PID = p->PID;
  int index[2];
  index[i] = p->D1;
  index[i] = p->D2;

  GenParticle * m = 0;

  if( index[i] > 0){
    m = (GenParticle *) branchParticle->At( index[i] );
    double mPID = m->PID;
    cout << "particle " << PID << " (" << p->P4().Pt() << ") " << " decays to " << " particle (D" << i << ") " << mPID << " ( " << m->P4().Pt() << " ) " << endl;
  }else{
    cout << "No decaying particle " << "D" << i << " from  " << PID << "  !!!" << endl;
  }

  return m;

}

bool isFromTop(const GenParticle* p, const TClonesArray* branchParticle){

  bool output = false;
  bool debug = false;
  bool printout = false;
  double m1 = p->M1;
  double m2 = p->M2;
  int PID = p->PID;
  int mPID1 = -1;
  int mPID2 = -1;

  while( m1 >= 0 || m2 >= 0){
    if( m1 >= 0 && m2 < 0){
      GenParticle *mother = (GenParticle *) branchParticle->At(m1);
      m1 = mother->M1;
      m2 = mother->M2;
      mPID1 = mother->PID;
      if(debug) cout << "case1 : mother = " << mother->PID << endl;
      if( abs(mother->PID) == 6 ) { 
        output = true;
        break;
      }
    }else if( m1 < 0 && m2 >= 0){
      GenParticle *mother = (GenParticle *) branchParticle->At(m2);
      m1 = mother->M1;
      m2 = mother->M2;
      mPID2 = mother->PID;
      if(debug) cout << "case2 : mother = " << mother->PID << endl;
      if( abs(mother->PID) == 6 ) {
        output = true;
        break;
      }
    }else if( m1 == m2 ){
      GenParticle *mother = (GenParticle *) branchParticle->At(m1);
      m1 = mother->M1;
      m2 = mother->M2;
      mPID1 = mother->PID;
      mPID2 = mother->PID;
      if(debug) cout << "case 3 : mother = " << mother->PID << endl;
      if( abs(mother->PID) == 6 ) {
        output = true;
        break;
      }
    }else{
      GenParticle *mother1 = (GenParticle *) branchParticle->At(m1);
      GenParticle *mother2 = (GenParticle *) branchParticle->At(m2);

      mPID1 = mother1->PID;
      mPID2 = mother2->PID;
      if(debug) cout << "case4 : mother = " << mother1->PID << " " << mother2->PID << endl;

      double m11 = mother1->M1;
      double m12 = mother1->M2;
      double m21 = mother2->M1;
      double m22 = mother2->M2;
      double mo1_m1_PID = -1;
      double mo1_m2_PID = -1;
      double mo2_m1_PID = -1;
      double mo2_m2_PID = -1;
      if( m11 >= 0 ) {
        GenParticle * mo1_m1 = (GenParticle*) branchParticle->At(m11);
        mo1_m1_PID = mo1_m1->PID;
      }
      if( m12 >= 0 ) {
        GenParticle * mo1_m2 = (GenParticle*) branchParticle->At(m12);
        mo1_m2_PID =  mo1_m2->PID;
      }
      if( m21 >= 0 ) {
        GenParticle * mo2_m1 = (GenParticle*) branchParticle->At(m21);
        mo2_m1_PID = mo2_m1->PID;
      }
      if( m22 >= 0 ) {
        GenParticle * mo2_m2 = (GenParticle*) branchParticle->At(m22);
        mo2_m2_PID = mo2_m2->PID;
      }

      bool fromtop = abs(mother1->PID) == 6 || abs(mother2->PID) == 6; 
      bool fromtop2 = abs(mo1_m1_PID) == 6 || abs(mo1_m2_PID) == 6;
      bool fromtop3 = abs(mo2_m1_PID) == 6 || abs(mo2_m2_PID) == 6;
      if( fromtop ) {
        mPID1 = mother1->PID;
        mPID2 = mother2->PID;
        output = true;
        break;
      }
      if( fromtop2 ) {
        mPID1 = mo1_m1_PID;
        mPID2 = mo1_m2_PID;
        output = true;
        break;
      }
      if( fromtop3 ) {
        mPID1 = mo2_m1_PID;
        mPID2 = mo2_m2_PID;
        output = true;
        break;
      }

  if( (m11 >= 0 && m12 >= 0) && (m21 < 0 && m22 < 0) ) {
        m1 = m11;
        m2 = m12;
      }else if( (m11 < 0 && m12 < 0) && (m21 >= 0 && m22 >= 0) ){
        m1 = m21;
        m2 = m22;
      }else if( (m11 >= 0 && m12 < 0) && (m21 >= 0 && m22 < 0) ){
        m1 = m11;
        m2 = m21;
      }else if( (m11 < 0 && m12 >= 0) && (m21 < 0 && m22 >= 0) ){
        m1 = m12;
        m2 = m22;
      }else if( (m11 >= 0 && m12 < 0) && (m21 < 0 && m22 >= 0) ){
        m1 = m11;
        m2 = m22;
      }else if( (m11 < 0 && m12 >= 0) && (m21 >= 0 && m22 < 0) ){ 
        m1 = m12;
        m2 = m21; 
      }else if( (m11 < 0 && m12 < 0) && (m21 < 0 && m22 < 0)  ){
        break;
      }else{
        if( abs(mo1_m1_PID) == 5 || abs(mo1_m2_PID) == 5){
          m1 = m11;
          m2 = m12;
        }else if( abs(mo2_m1_PID) == 5 || abs(mo2_m2_PID) == 5)  {
          m1 = m21;
          m2 = m22;
        }else{
          if(debug){
            cout << "===== DEBUG =====" << endl;
            cout << m11 << " " << m12 << " " << m21 << " " << m22 << endl;
            cout << mo1_m1_PID << " " << mo1_m2_PID << " " << mo2_m1_PID << " " << mo2_m2_PID << endl;
            cout << "===== BREAK =====" << endl;
          }
          break;
        }
      }

    }
  }
  if(printout) cout << "Ancestor PID = " << mPID1 << " " << mPID2 << " test particle ID = " << PID << endl;

  return output;
}

void ana(const char *inputFile, const char *outputFile)
{
 gSystem->Load("libDelphes");

 // Create chain of root trees
 TChain chain("Delphes");
 chain.Add(inputFile);

 TString filename = inputFile;
 
 // Switch for single lepton or dilepton
 bool isdilepton = false;
 if( filename.Contains("di") == true ){
   isdilepton = true;
   cout<<"Dilepton Channel"<<endl;
 }
 else cout<<"Single Lepton Channel"<<endl;

 //Output
 TFile *fout = TFile::Open(outputFile,"RECREATE");
 fout->cd();

 //DNN variables
 int signal, channel, event;
 int njets, nbjets;
 double dR, M;
 double addbjet1_pt, addbjet1_eta, addbjet1_phi, addbjet1_e;//AddGen bjet1
 double addbjet2_pt, addbjet2_eta, addbjet2_phi, addbjet2_e;//AddGen bjet2
 double pt1, eta1, phi1, e1;//Reco bjet1
 double pt2, eta2, phi2, e2;//Reco bjet2

 //Tree variables ( minimum deltaR )
 double bjet1_pt;
 double bjet1_eta;
 double bjet1_phi;
 double bjet1_e;
 double bjet2_pt;
 double bjet2_eta;
 double bjet2_phi;
 double bjet2_e;
 double bjet3_pt;
 double bjet3_eta;
 double bjet3_phi;
 double bjet3_e;

 unsigned short nJet, nbJet, nMuon, nElectron;
 double Jet_pt, Jet_eta, Jet_phi, Jet_e;
 double Electron1_pt, Electron1_eta, Electron1_phi, Electron1_e;
 double Electron2_pt, Electron2_eta, Electron2_phi, Electron2_e;
 double Muon1_pt, Muon1_eta, Muon1_phi, Muon1_e;
 double Muon2_pt, Muon2_eta, Muon2_phi, Muon2_e;

 //Tree for Deep learning input 
 TTree * dnn_tree = new TTree( "dnn_input", "tree for dnn");
 dnn_tree->Branch("signal",&signal,"signal/i");
 dnn_tree->Branch("channel",&channel,"channel/i");
 dnn_tree->Branch("event",&event,"event/i");
 dnn_tree->Branch("njets",&njets,"njets/i");
 dnn_tree->Branch("nbjets",&nbjets,"nbjets/i"); 
 dnn_tree->Branch("dR",&dR,"dR/d");
 dnn_tree->Branch("M",&M,"M/d");
 dnn_tree->Branch("addbjet1_pt",&addbjet1_pt,"addbjet1_pt/d");
 dnn_tree->Branch("addbjet1_eta",&addbjet1_eta,"addbjet1_eta/d");
 dnn_tree->Branch("addbjet1_phi",&addbjet1_phi,"addbjet1_phi/d");
 dnn_tree->Branch("addbjet1_e",&addbjet1_e,"addbjet1_e/d");
 dnn_tree->Branch("addbjet2_pt",&addbjet2_pt,"addbjet2_pt/d");
 dnn_tree->Branch("addbjet2_eta",&addbjet2_eta,"addbjet2_eta/d");
 dnn_tree->Branch("addbjet2_phi",&addbjet2_phi,"addbjet2_phi/d");
 dnn_tree->Branch("addbjet2_e",&addbjet2_e,"addbjet2_e/d");
 dnn_tree->Branch("pt1",&pt1,"pt1/d");
 dnn_tree->Branch("eta1",&eta1,"eta1/d");
 dnn_tree->Branch("phi1",&phi1,"phi1/d");
 dnn_tree->Branch("e1",&e1,"e1/d");
 dnn_tree->Branch("pt2",&pt2,"pt2/d");
 dnn_tree->Branch("eta2",&eta2,"eta2/d");
 dnn_tree->Branch("phi2",&phi2,"phi2/d");
 dnn_tree->Branch("e2",&e2,"e2/d");

 //Tree for minimum dR analysis
 TTree * tree = new TTree( "tree", "tree for ttbb");
 tree->Branch("nbJet",&nbJet,"nbJet/s");
 tree->Branch("bjet1_pt",&bjet1_pt,"bjet1_pt/d");
 tree->Branch("bjet1_eta",&bjet1_eta,"bjet1_eta/d");
 tree->Branch("bjet1_phi",&bjet1_phi,"bjet1_phi/d");
 tree->Branch("bjet1_e",&bjet2_e,"bjet1_e/d");
 tree->Branch("bjet2_pt",&bjet2_pt,"bjet2_pt/d");
 tree->Branch("bjet2_eta",&bjet2_eta,"bjet2_eta/d");
 tree->Branch("bjet2_phi",&bjet2_phi,"bjet2_phi/d");
 tree->Branch("bjet2_e",&bjet2_e,"bjet2_e/d");
 tree->Branch("bjet3_pt",&bjet3_pt,"bjet3_pt/d");
 tree->Branch("bjet3_eta",&bjet3_eta,"bjet3_eta/d");
 tree->Branch("bjet3_phi",&bjet3_phi,"bjet3_phi/d");
 tree->Branch("bjet3_e",&bjet3_e,"bjet3_e/d");

 tree->Branch("nJet",&nJet,"nJet/s");
 tree->Branch("Jet_pt",&Jet_pt,"Jet_pt/d");
 tree->Branch("Jet_eta",&Jet_eta,"Jet_eta/d");
 tree->Branch("Jet_phi",&Jet_phi,"Jet_phi/d");
 tree->Branch("Jet_e",&Jet_e,"Jet_e/d");

 tree->Branch("nElectron",&nElectron,"nElectron/s");
 tree->Branch("Electron1_pt",&Electron1_pt,"Electron1_pt/d");
 tree->Branch("Electron1_eta",&Electron1_eta,"Electron1_eta/d");
 tree->Branch("Electron1_phi",&Electron1_phi,"Electron1_phi/d");
 tree->Branch("Electron1_e",&Electron1_e,"Electron1_e/d");
 tree->Branch("Electron2_pt",&Electron2_pt,"Electron2_pt/d");
 tree->Branch("Electron2_eta",&Electron2_eta,"Electron2_eta/d");
 tree->Branch("Electron2_phi",&Electron2_phi,"Electron2_phi/d");
 tree->Branch("Electron2_e",&Electron2_e,"Electron2_e/d");
 
 tree->Branch("nMuon",&nMuon,"nMuon/s");
 tree->Branch("Muon1_pt",&Muon1_pt,"Muon1_pt/d");
 tree->Branch("Muon1_eta",&Muon1_eta,"Muon1_eta/d");
 tree->Branch("Muon1_phi",&Muon1_phi,"Muon1_phi/d");
 tree->Branch("Muon1_e",&Muon1_e,"Muon1_e/d");
 tree->Branch("Muon2_pt",&Muon2_pt,"Muon2_pt/d");
 tree->Branch("Muon2_eta",&Muon2_eta,"Muon2_eta/d");
 tree->Branch("Muon2_phi",&Muon2_phi,"Muon2_phi/d");
 tree->Branch("Muon2_e",&Muon2_e,"Muon2_e/d");

 // Create object of class ExRootTreeReader
 ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
 Long64_t numberOfEntries = treeReader->GetEntries();

 // Get pointers to branches used in this analysis
 TClonesArray *branchJet  = treeReader->UseBranch("Jet");
 TClonesArray *branchParticle  = treeReader->UseBranch("Particle");
 TClonesArray *branchElectron = treeReader->UseBranch("Electron");
 TClonesArray *branchMuon = treeReader->UseBranch("Muon");

 // Book histograms
 TH1 *histnjet = new TH1F("h_njet", "Number of jets", 14, 0.0, 14.0);
 TH1 *histnElectron = new TH1F("h_nElectron", "Number of electrons", 3, 0.0, 3.0);
 TH1 *histnMuon = new TH1F("h_nMuon", "Number of muons", 3, 0.0, 3.0);
 
 TH1 *histnbjet = new TH1F("h_nbjet", "Number of b-jets", 10, 0.0, 10.0);
 TH1 *histMbb = new TH1F("h_mbb", "M_{inv}(b, b)", 40, 20.0, 180.0);
 TH1 *histdRbb = new TH1F("h_dRbb", "dR(b, b)", 40, 0, 4.0);

 TH1 *hist_gennbjet = new TH1F("h_gennbjet", "Number of b-jets", 6, 0.0, 6.0);
 TH1 *hist_genMbb = new TH1F("h_genmbb", "M_{inv}(b, b)", 40, 20.0, 180.0);
 TH1 *hist_gendRbb = new TH1F("h_gendRbb", "dR(b, b)", 40, 0, 4.0);

 TH1 *hist_matchednbjet = new TH1F("h_matchednbjet", "Number of b-jets", 6, 0.0, 6.0);
 TH1 *hist_matchedMbb = new TH1F("h_matchedmbb", "M_{inv}(b, b)", 40, 20.0, 180.0);
 TH1 *hist_matcheddRbb = new TH1F("h_matcheddRbb", "dR(b, b)", 40, 0, 4.0);

 Int_t numberOfSelectedEvents = 0;
 Int_t numberOfMatchedEvents = 0;

 vector<Jet *> bJets;
 vector<Jet *> Jets;
 vector<Electron *> Electrons;
 vector<Muon *> Muons;

 TLorentzVector p4[2];
 Jet *jet;
 Electron *electron;
 Muon *muon;

 int entry, i;
 bool pass = false;

 // Loop over all events
 for(entry = 0; entry < numberOfEntries; ++entry)
 {
   if(entry%1000 == 0) cout << "event number: " << entry << endl;

   // Load selected branches with data from specified event
   treeReader->ReadEntry(entry);
   
   // Initialization
   Jets.clear();
   Electrons.clear();
   Muons.clear(); 
   bJets.clear();
   // dnn variables
   signal = -1;
   channel = 999;
   event = -1;
   njets = 999;
   nbjets = 999;
   dR = 999;
   M = 999;
   addbjet1_pt = 999;
   addbjet1_eta = 999;
   addbjet1_phi = 999;
   addbjet1_e = 999;
   addbjet2_pt = 999;
   addbjet2_eta = 999;
   addbjet2_phi = 999;
   addbjet2_e = 999;
   pt1 = 999;
   eta1 = 999;
   phi1 = 999;
   e1 = 999;
   pt2 = 999;
   eta2 = 999;
   phi2 = 999;
   e2 = 999;

   // tree variables
   Jet_pt = 999;
   Jet_eta = 999;
   Jet_phi = 999;
   Jet_e = 999;

   Electron1_pt = 999;
   Electron1_eta = 999;
   Electron1_phi = 999;
   Electron1_e = 999;
   Electron2_pt = 999;
   Electron2_eta = 999;
   Electron2_phi = 999;
   Electron2_e = 999;

   Muon1_pt = 999;
   Muon1_eta = 999;
   Muon1_phi = 999;
   Muon1_e = 999;
   Muon2_pt = 999;
   Muon2_eta = 999;
   Muon2_phi = 999;
   Muon2_e = 999;

   bjet1_pt = 999;
   bjet1_eta = 999;
   bjet1_phi = 999;
   bjet1_e = 999;
   bjet2_pt = 999 ;
   bjet2_eta = 999;
   bjet2_phi = 999;
   bjet2_e = 999;
   bjet3_pt = 999 ;
   bjet3_eta = 999;
   bjet3_phi = 999;
   bjet3_e = 999;

   // Jet and b-tag cuts
   nJet = 0;
   nbJet = 0;
   for(i = 0; i < branchJet->GetEntriesFast(); ++i){
     jet = (Jet*) branchJet->At(i);
     if( (jet->PT > 30) & (fabs(jet->Eta) < 2.5) ){
       nJet++;
       Jets.push_back(jet);
       if( jet->BTag ) {
         nbJet++;
         bJets.push_back(jet);
       }
     }
   }
   //Electron cut
   nElectron = 0;
   for(i = 0; i < branchElectron->GetEntries(); ++i){
     electron = (Electron*) branchElectron->At(i);
     if( (electron->PT > 30) & (fabs(electron->Eta) < 2.5)){
       nElectron++;
       Electrons.push_back(electron);
     }
   }
   //Muon cut
   nMuon = 0;
   for(i = 0; i < branchMuon->GetEntries(); ++i){
     muon = (Muon*) branchMuon->At(i);
     if( (muon->PT > 30) & (fabs(muon->Eta) < 2.5) ){
       nMuon++;
       Muons.push_back(muon);
     }
   }

   // Dilepton channel cuts
   if(isdilepton) {
     pass = ((nElectron >= 2) || (nMuon >= 2) || (nElectron >= 1 & nMuon >= 1)) & (nJet >= 6) & (nbJet >= 3);
     //cout<<"dicut"<<endl;
   }
   // Single lepton channel cuts
   else{
     pass = (nElectron == 1 || nMuon == 1) & (nJet >= 6) & (nbJet >= 3);
     //cout<<"singlecut"<<endl;
   }

   if(!pass) continue;

   // Fill the ntuples
   Jet_pt = Jets[0]->P4().Pt();
   Jet_eta = Jets[0]->P4().Eta();
   Jet_phi = Jets[0]->P4().Phi();
   Jet_e = Jets[0]->P4().E();
   if( nElectron == 1){
     Electron1_pt = Electrons[0]->P4().Pt();
     Electron1_eta = Electrons[0]->P4().Eta();
     Electron1_phi = Electrons[0]->P4().Phi();
     Electron1_e = Electrons[0]->P4().E();
   }
   if( nMuon ==1 ){
     Muon1_pt = Muons[0]->P4().Pt();
     Muon1_eta = Muons[0]->P4().Eta();
     Muon1_phi = Muons[0]->P4().Phi();
     Muon1_e = Muons[0]->P4().E();
   }
   if(isdilepton){
     if(nElectron == 2){
       Electron2_pt = Electrons[1]->P4().Pt();
       Electron2_eta = Electrons[1]->P4().Eta();
       Electron2_phi = Electrons[1]->P4().Phi();
       Electron2_e = Electrons[1]->P4().E();
     }
     if(nMuon == 2){
       Muon2_pt = Muons[1]->P4().Pt();
       Muon2_eta = Muons[1]->P4().Eta();
       Muon2_phi = Muons[1]->P4().Phi();
       Muon2_e = Muons[1]->P4().E();
     }
   }
   
   histnjet->Fill( nJet );
   histnElectron->Fill( nElectron );
   histnMuon->Fill( nMuon );

   int nb = 0;
   int nbFromTop = 0;
   int nb_status3 = 0; 
   int ntop = 0;
   vector<GenParticle*> GenAddbJets;
   for(i = 0; i < branchParticle->GetEntriesFast(); ++i){
     GenParticle *genP = (GenParticle*) branchParticle->At(i);
     if( abs(genP->PID) == 6 ) {
       GenParticle *dauP1 = (GenParticle *) branchParticle->At(genP->D1);
       GenParticle *dauP2 = (GenParticle *) branchParticle->At(genP->D2);
       double dauPID1 = dauP1->PID;
       double dauPID2 = dauP2->PID;
       if( abs(dauPID1) != 6 && abs(dauPID2) != 6){
         ntop++;
       }
     }
     if( abs(genP->PID) == 5){
       if( genP->Status == 2) nb_status3++;
       //check if this is the last b quark 
       GenParticle *dauP1 = (GenParticle *) branchParticle->At(genP->D1);
       GenParticle *dauP2 = (GenParticle *) branchParticle->At(genP->D2);
       double dauPID1 = dauP1->PID;
       double dauPID2 = dauP2->PID;
       if( abs(dauPID1) == 5 || abs(dauPID2) == 5) continue; 
       //cout << "test b quark = " << genP->P4().Pt() << " decays to " << dauPID1 << " and " << dauPID2 << endl;
       nb++;
       bool fromTop = isFromTop(genP, branchParticle);
       if(fromTop) {
         nbFromTop++;
       }else{
         GenAddbJets.push_back(genP); 
       }
     }
   }
   TLorentzVector addbjet1;
   TLorentzVector addbjet2;
   if( GenAddbJets.size() >= 2){
   addbjet1 = GenAddbJets[0]->P4();
   addbjet2 = GenAddbJets[1]->P4();
   }
   //cout << "=========" << " Number of top = " << ntop << " number of b = " << nb << " (from top = " << nbFromTop << " )" << "=========" << endl;

   vector<Jet*> matchedbjets; 
   for( int j = 0; j < bJets.size(); j++){
     for(int k = 0 ; k < GenAddbJets.size(); k++){
       TLorentzVector recobjet = bJets[j]->P4();
       TLorentzVector genbjet = GenAddbJets[k]->P4();
       double dR = recobjet.DeltaR( genbjet );
       //cout << "test dR = " << dR << endl;
       if( dR < 0.4 ) matchedbjets.push_back( bJets[j] ) ;
     }
   }

   //cout << "matched = " << matchedbjets.size() << endl;
   histnbjet->Fill(bJets.size());
   hist_gennbjet->Fill(GenAddbJets.size());
   if( GenAddbJets.size() > 1){
     double genMbb = ( GenAddbJets[0]->P4() + GenAddbJets[1]->P4() ).M();
     double gendRbb = GenAddbJets[0]->P4().DeltaR( GenAddbJets[1]->P4() );
     hist_genMbb->Fill( genMbb );
     hist_gendRbb->Fill( gendRbb );
   }
   hist_matchednbjet->Fill( matchedbjets.size() );
   if( matchedbjets.size() > 1){
     double matched_mbb = (matchedbjets[0]->P4() + matchedbjets[1]->P4() ).M();
     double matched_dRbb = matchedbjets[0]->P4().DeltaR( matchedbjets[1]->P4() ); 
     hist_matchedMbb->Fill(matched_mbb);
     hist_matcheddRbb->Fill(matched_dRbb);
   }

   // select events with at least 2 b-jets and 2 opposite sign muons
   bjet1_pt = bJets[0]->P4().Pt();
   bjet1_eta = bJets[0]->P4().Eta();
   bjet1_phi = bJets[0]->P4().Phi();
   bjet1_e = bJets[0]->P4().E();
   bjet2_pt = bJets[1]->P4().Pt();
   bjet2_eta = bJets[1]->P4().Eta();
   bjet2_phi = bJets[1]->P4().Phi();
   bjet2_e = bJets[1]->P4().E();

   if(bJets.size() >=3){
     bjet3_pt = bJets[2]->P4().Pt();
     bjet3_eta = bJets[2]->P4().Eta();
     bjet3_phi = bJets[2]->P4().Phi();
     bjet3_e = bJets[2]->P4().E();
   }

   float mbb = 999;
   float dRbb = 999;

   // select two bjets with minimum dR
   TLorentzVector RecoAddJets[2];
   for(int b1 = 0; b1 < bJets.size()-1; b1++){
     for(int b2 = b1+1; b2 < bJets.size(); b2++){
       p4[0] = bJets[b1]->P4();
       p4[1] = bJets[b2]->P4();

       float tmp_mbb = ((p4[0]) + (p4[1])).M();
       float tmp_dRbb = p4[0].DeltaR(p4[1]);
       if(tmp_dRbb < dRbb) {
          dRbb = tmp_dRbb;
          mbb = tmp_mbb; 
          RecoAddJets[0] = p4[0];
          RecoAddJets[1] = p4[1];
       }
 
       bool p4_1_matched = std::find(matchedbjets.begin(), matchedbjets.end(), bJets[b1]) != matchedbjets.end(); 
       bool p4_2_matched = std::find(matchedbjets.begin(), matchedbjets.end(), bJets[b2]) != matchedbjets.end(); 
       bool matched_signal = p4_1_matched && p4_2_matched;

       if (matched_signal) signal = 1;
       else signal = 0;

       if (nElectron == 1) channel = 1;
       else if (nMuon == 1) channel = 0;

       event = entry;

       njets = nJet;
       nbjets = nbJet;

       pt1 = p4[0].Pt();
       eta1 = p4[0].Eta();
       phi1 = p4[0].Phi();
       e1 = p4[0].E();
       pt2 = p4[1].Pt();
       eta2 = p4[1].Eta();
       phi2 = p4[1].Phi();
       e2 = p4[1].E();

       addbjet1_pt = addbjet1.Pt();
       addbjet1_eta = addbjet1.Eta();
       addbjet1_phi = addbjet1.Phi();
       addbjet1_e = addbjet1.E();
       addbjet2_pt = addbjet2.Pt();
       addbjet2_eta = addbjet2.Eta();
       addbjet2_phi = addbjet2.Phi();
       addbjet2_e = addbjet2.E();

       M = tmp_mbb;
       dR = tmp_dRbb;
       dnn_tree->Fill();
     }
   }

   bool matched = false;
   bool matched1 = false;
   bool matched2 = false;
   for(int j = 0 ; j < GenAddbJets.size(); j++){
     if( RecoAddJets[0].DeltaR( GenAddbJets[j]->P4() ) < 0.5 )  matched1 = true;
     if( RecoAddJets[1].DeltaR( GenAddbJets[j]->P4() ) < 0.5 )  matched2 = true;
    }
   if( matched1 && matched2 ) matched = true;

   //cout<<"nElectron = "<< nElectron <<" nMuon = "<< nMuon <<" nJet = "<< nJet <<" nbJet = "<< nbJet << endl;

   ++numberOfSelectedEvents;
  if(matched) numberOfMatchedEvents++;

   histMbb->Fill(mbb);
   histdRbb->Fill(dRbb);
   tree->Fill();

 }

 cout << "Total number of events = " << numberOfSelectedEvents << endl;;
 cout << "Total number of matched events = " << numberOfMatchedEvents << endl;;
 double eff = (double) numberOfMatchedEvents/ (double) numberOfSelectedEvents;
 cout << "Matching eff. = " << eff << endl;
 fout->Write();
 fout->Close();

}
