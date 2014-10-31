/*
root -l examples/Example1.C\(\"delphes_output.root\"\)
*/

//------------------------------------------------------------------------------

void analyzer()
{
  gSystem->Load("libDelphesMA5tune");

  TString processName = "ZJets";

  TFile* f = TFile::Open(Form("hist_%s.root", processName.Data()), "recreate");

  // Create chain of root trees
  TChain chain("DelphesMA5tune");
  chain.Add("/cms/home/tjkim/fcnc/sample/ZToLL50-0Jet_sm-no_masses/events_*.root");
  
  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();
  
  // Get pointers to branches used in this analysis
  TClonesArray *branchMuon = treeReader->UseBranch("DelphesMA5tuneMuon");
  
  // Book histograms
  TH1 *histDiMuonMass = new TH1F("dimuon_mass","Di-Muon Invariant Mass (GeV)",100, 50, 150);
 
  // Loop over all events

  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
 
    int nmuon = 0; 
    for( int i = 0; i < branchMuon->GetEntries(); i++)
    {
      Muon *muon = (Muon*) branchMuon->At(i);
      if( muon->PT <= 20 || abs(muon->Eta) >= 2.4 ) continue;
      nmuon = nmuon + 1 ;
    }

    if( nmuon >= 2){

      Muon *mu1 = (Muon*) branchMuon->At(0);
      Muon *mu2 = (Muon*) branchMuon->At(1);

      // Plot jet transverse momentum
      histDiMuonMass->Fill(((mu1->P4()) + (mu2->P4())).M());
    }
 
  }

  // Show resulting histograms
  histDiMuonMass->Write();

  f->Close();

}
