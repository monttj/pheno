/*
root -l examples/Example1.C\(\"delphes_output.root\"\)
*/

//------------------------------------------------------------------------------

void makeHistogramPhoton(const char *inputFile, const TString & processName)
{
  //gSystem->Load("libDelphes");
  gSystem->Load("libDelphesMA5tune");

  TFile* f = TFile::Open(Form("hist_%s.root", processName.Data()), "recreate");

  // Create chain of root trees
  //TChain chain("Delphes");
  TChain chain("DelphesMA5tune");
  chain.Add(inputFile);
  
  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();
  
  // Get pointers to branches used in this analysis
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchPhoton = treeReader->UseBranch("Photon");
  
  // Book histograms
  TH1 *nEvents = new TH1F("nEvents", "nEvents", 2, 0, 2);
  TH1 *histJet1PT = new TH1F("jet1_pt", "jet P_{T}", 100, 0.0, 100.0);
  TH1 *histJet2PT = new TH1F("jet2_pt", "jet P_{T}", 100, 0.0, 100.0);
  TH1 *histJet3PT = new TH1F("jet3_pt", "jet P_{T}", 100, 0.0, 100.0);
  TH1 *histJet4PT = new TH1F("jet4_pt", "jet P_{T}", 100, 0.0, 100.0);
  TH1 *histJet1Eta = new TH1F("jet1_eta", "jet Eta", 60, -3, 3);
  TH1 *histJet2Eta = new TH1F("jet2_eta", "jet Eta", 60, -3, 3);
  TH1 *histJet3Eta = new TH1F("jet3_eta", "jet Eta", 60, -3, 3);
  TH1 *histJet4Eta = new TH1F("jet4_eta", "jet Eta", 60, -3, 3);
  TH1 *histnJet = new TH1F("nJet", "jet Multiplicity", 10, 0, 10);
  TH1 *histnbJet = new TH1F("nbJet", "b-Jet Multiplicity", 5, 0, 5);
  TH1 *histnElectron = new TH1F("nElectron", "Electron Multiplicity", 5, 0, 5);
  TH1 *histnMuon = new TH1F("nMuon", "Muon Multiplicity", 5, 0, 5);
  TH1 *histnPhoton = new TH1F("nPhoton", "Photon Multiplicity", 5, 0, 5);
  TH1 *histPhotonPT = new TH1F("photon_pt", "photon P_{T}", 40, 0, 200);
  TH1 *histPhotonPT_wide = new TH1F("photon_pt_wide", "photon P_{T}", 20, 0, 200);
  TH1 *histPhotonPT_nocut = new TH1F("photon_pt_nocut", "photon P_{T}", 100, 0, 200);
  TH1 *histPhoton1PT_nocut = new TH1F("photon_pt1_nocut", "photon P_{T}", 100, 0, 200);
  TH1 *histPhoton2PT_nocut = new TH1F("photon_pt2_nocut", "photon P_{T}", 100, 0, 200);
  TH1 *histPhotonEta = new TH1F("photon_eta", "photon eta", 60, -3, 3);
  TH1 *histPhoton1PT = new TH1F("photon1_pt", "photon P_{T}", 40, 0, 200);
  TH1 *histPhoton2PT = new TH1F("photon2_pt", "photon P_{T}", 40, 0, 200);
  TH1 *histPhoton1Eta = new TH1F("photon1_eta", "photon Eta", 60, -3, 3);
  TH1 *histPhoton2Eta = new TH1F("photon2_eta", "photon Eta", 60, -3, 3);
  TH1 *histDiPhotonMass = new TH1F("diphoton_mass","Di-Photon Invariant Mass (GeV)",80, 105,145);
 
  // Loop over all events
  nEvents->SetBinContent(1, numberOfEntries);
  int neve_onephoton = 0;
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
  
    int nb = 0;
    int nj = 0;
    int np = 0;
    // If event contains at least 1 jet
    for( int i = 0; i < branchJet->GetEntries(); i++)
    {
      // Take first jet
      Jet *jet = (Jet*) branchJet->At(i);

      if( jet->PT <= 20 || abs(jet->Eta) >= 2.4 ) continue;
      
      // Plot jet transverse momentum
      if( nj == 0) { 
        histJet1PT->Fill(jet->PT);
        histJet1Eta->Fill(jet->Eta); 
      }
      if( nj == 1) {
        histJet2PT->Fill(jet->PT);
        histJet2Eta->Fill(jet->Eta);
      }
      if( nj == 2) {
        histJet3PT->Fill(jet->PT);
        histJet3Eta->Fill(jet->Eta);
      }
      if( nj == 3) {
        histJet4PT->Fill(jet->PT);
        histJet4Eta->Fill(jet->Eta);
      }


      if(jet->BTag) nb = nb + 1;
      nj = nj + 1 ;
      // Print jet transverse momentum
    }


    histnJet->Fill( nj ); 
    histnbJet->Fill(nb);

     for( int i = 0; i < branchPhoton->GetEntries(); i++)
    {
      // Take first jet
      Photon *photon = (Photon*) branchPhoton->At(i);
      
      histPhotonPT_nocut->Fill(photon->PT);
      if(i==0) histPhoton1PT_nocut->Fill(photon->PT);
      if(i==1) histPhoton2PT_nocut->Fill(photon->PT);

      if( photon->PT <= 20 || abs(photon->Eta) >= 2.4 ) continue;

      histPhotonPT->Fill(photon->PT);
      histPhotonPT_wide->Fill(photon->PT);
      histPhotonEta->Fill(photon->Eta);
      np = np + 1 ;
      // Print jet transverse momentum
    }

    histnPhoton->Fill(np);

    if(np < 1) continue;
    neve_onephoton++;

    if( np >= 2){
      Photon *pho1 = (Photon*) branchPhoton->At(0);
      Photon *pho2 = (Photon*) branchPhoton->At(1);

      // Plot jet transverse momentum
      histPhoton1PT->Fill(pho1->PT);
      histPhoton1Eta->Fill(pho1->Eta);
      histPhoton2PT->Fill(pho2->PT);
      histPhoton2Eta->Fill(pho2->Eta);

      histDiPhotonMass->Fill(((pho1->P4()) + (pho2->P4())).M());
    }
    // lepton multiplicity
    histnElectron->Fill( branchElectron->GetEntries() );
    histnMuon->Fill( branchMuon->GetEntries() );
 
  }

  nEvents->SetBinContent(2, neve_onephoton);

  // Show resulting histograms
  nEvents->Write();
  histJet1PT->Write();
  histJet2PT->Write();
  histJet3PT->Write();
  histJet4PT->Write();
  histJet1Eta->Write();
  histJet2Eta->Write();
  histJet3Eta->Write();
  histJet4Eta->Write();
  histnJet->Write();
  histnbJet->Write();
  histnElectron->Write();
  histnMuon->Write();
  histPhotonPT->Write();
  histPhotonPT_wide->Write();
  histPhotonPT_nocut->Write();
  histPhotonEta->Write();
  histPhoton1PT->Write();
  histPhoton2PT->Write();
  histPhoton1Eta->Write();
  histPhoton2Eta->Write();
  histnPhoton->Write();
  histDiPhotonMass->Write();

  f->Close();

}
