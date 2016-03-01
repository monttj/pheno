void LeptonEnergy(const char *inputFile)
{
  gSystem->Load("libDelphes");

  //TFile* f = TFile::Open("hist_LO_noleptoncut.root", "recreate");
  TFile* f = TFile::Open("hist_LO_new.root", "recreate");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);
 
  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  // Get pointers to branches used in this analysis
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
 
  GenParticle *particle;
  GenParticle *daughter1;
  GenParticle *daughter2;
  GenParticle *granddaughter1_1;
  GenParticle *granddaughter1_2;
  GenParticle *granddaughter2_1;
  GenParticle *granddaughter2_2;

  GenParticle *genelectron;
  GenParticle *genmuon;
 
  // Book histograms
  TH1 * channel = new TH1F("channel", "ttbar event categorization", 7, 0.0, 7.0);
  TH1 * h_muon_energy = new TH1F("h_muon_energy", "muon energy distribution", 400, 0, 400);
  TH1 * h_electron_energy = new TH1F("h_electron_energy", "electron energy distribution", 400, 0, 400);
  TH1 * h_lepton_energy = new TH1F("h_lepton_energy", "lepton energy distribution", 400, 0, 400);
  TH1 * h_muon_energy_reco = new TH1F("h_muon_energy_reco", "muon energy distribution at RECO", 400, 0, 400);
  TH1 * h_electron_energy_reco = new TH1F("h_electron_energy_reco", "electron energy distribution at RECO", 400, 0, 400);
  TH1 * h_lepton_energy_reco = new TH1F("h_lepton_energy_reco", "lepton energy distribution at RECO", 400, 0, 400);

  int ndileptonic = 0; //ee, mm, tautau
  int ndileptonic2 = 0; //ee, mm, tau->ee, mm
  int ndileptonic3 = 0; //ee, mm
  int nsemileptonic = 0;
  int nsemileptonic2 = 0;
  int nsemileptonic3 = 0;
  int nhadronic = 0;

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    //if( entry == 10000) break;
    if( entry%1000 == 0) cout << "starting with " << entry << endl;
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
 
    int nmuons = 0;
    int nelectrons= 0;
    int ntaumuons = 0;
    int ntauelectrons= 0;
    int ntaus = 0 ;
    int nhadrons = 0 ;
    // If event contains at least 1 particle
    int ntop = 0;
    if(branchParticle->GetEntries() > 0)
    {
      
      for(int i = 0; i < branchParticle->GetEntriesFast() ; i++){
        if(ntop == 2) break;

        particle = (GenParticle *) branchParticle->At(i);
    
        int status = particle->Status; 

        bool LO = true;

        if( status != 3) continue;

        int id = particle->PID; 

        if( LO) {
          if( abs(id) == 11 ){
            genelectron = particle;
            double energy = genelectron->E;
            h_electron_energy->Fill( energy );
            h_lepton_energy->Fill( energy );
          }else if( abs(id) == 13 ){
            genmuon = particle;
            double energy = genmuon->E;
            h_muon_energy->Fill( energy );
            h_lepton_energy->Fill( energy );
          }
        }else if( abs(id) == 6 ) {
          ntop++;
          particle = (GenParticle*) branchParticle->At( i ) ;
          if( particle->D1 >= branchParticle->GetEntries() ) continue;
          bool lasttop  =  false ;
          while( !lasttop ){
            if( particle->D1 >= branchParticle->GetEntries() ) break;
            GenParticle * d = (GenParticle *) branchParticle->At( particle->D1 );
            if( abs(d->PID) != 6 ) { 
              lasttop = true;
            }
            else { particle = d ; }
          } 

          if( particle->D1 >= branchParticle->GetEntries() || particle->D2 >= branchParticle->GetEntries() ){
           continue;
          }

          daughter1 = (GenParticle*) branchParticle->At( particle->D1) ;
          daughter2 = (GenParticle*) branchParticle->At( particle->D2) ;
   
          bool lastW = false;
          int d1_id = abs(daughter1->PID);
          int d2_id = abs(daughter2->PID);

          //cout << "top daughter  " << d1_id  << " , " << d2_id << endl;

          while( !lastW) {
            if( daughter1->D1 >= branchParticle->GetEntries() ) break;
            GenParticle * d = (GenParticle *) branchParticle->At( daughter1->D1 );
            if( abs(d->PID) != 24 ) { lastW = true; }
            else {
              daughter1 = d ;
            } 
          } 

          if( daughter1->D1 >= branchParticle->GetEntries() || daughter1->D2 >= branchParticle->GetEntries() ){
           continue;
          }

          granddaughter1_1 = (GenParticle*) branchParticle->At( daughter1->D1) ;
          granddaughter1_2 = (GenParticle*) branchParticle->At( daughter1->D2) ;
          granddaughter2_1 = (GenParticle*) branchParticle->At( daughter2->D1) ;
          granddaughter2_2 = (GenParticle*) branchParticle->At( daughter2->D2) ;    
 
          int gd1_1_id = abs(granddaughter1_1->PID);
          int gd1_2_id = abs(granddaughter1_2->PID);
          int gd2_1_id = abs(granddaughter2_1->PID);
          int gd2_2_id = abs(granddaughter2_2->PID);

          //cout << "W daughters = " << gd1_1_id << " , " << gd1_2_id << " , " << gd2_1_id << " , " << gd2_2_id << endl;
          if( gd1_1_id < gd1_2_id ) cout << "Something is WRONG ! " << endl;

          if( gd1_2_id == 11 ) { 
            nelectrons++;
            genelectron = granddaughter1_2;
          }
          else if( gd1_2_id == 13 ) { 
            nmuons++;
            genmuon = granddaughter1_2;
          }
          else if( gd1_2_id == 15 ) {
            ntaus++;
            //cout << "tau dau id1 = " <<  granddaughter1_2->D1 << " , " << granddaughter1_2->D2  << endl;
            if( granddaughter1_2->D1 >= branchParticle->GetEntries() || granddaughter1_2->D2 >= branchParticle->GetEntries() ){
              continue;
            }

            GenParticle * taudaughter1 = (GenParticle*) branchParticle->At( granddaughter1_2->D1) ;
            GenParticle * taudaughter2 = (GenParticle*) branchParticle->At( granddaughter1_2->D2) ;
            int taud1_id = abs(taudaughter1->PID);
            int taud2_id = abs(taudaughter2->PID);

            //cout << "tau daughter = " << taud1_id << " " << taud2_id << endl;

            if( taud1_id == 11 || taud1_id == 12 ) ntauelectrons++;
            else if( taud1_id == 13 || taud1_id == 14 ) ntaumuons++;
            else if( taud1_id == 15 || taud1_id == 16 ) {
              if( taudaughter1->D1 >= branchParticle->GetEntries() || taudaughter1->D2 >= branchParticle->GetEntries() ){
              continue;
            }

              GenParticle * taugranddaughter1 = (GenParticle*) branchParticle->At( taudaughter1->D1) ;
              GenParticle * taugranddaughter2 = (GenParticle*) branchParticle->At( taudaughter1->D2) ;
              int taugd1_id = abs(taugranddaughter1->PID);
              int taugd2_id = abs(taugranddaughter2->PID);
              //cout << "tau grand daughter = " << taugd1_id << " " << taugd2_id << endl;
              if( taugd1_id == 11 || taugd1_id == 12 ) ntauelectrons++;
              else if( taugd1_id == 13 || taugd1_id == 14 ) ntaumuons++;
              else { continue; }
          
            } else { continue; }
          }else{
            nhadrons++;
          }
          //cout << "nelectrons = " << nelectrons << " nmuons = " << nmuons << " ntaus = " << ntaus << " nhadrons = " << nhadrons << endl;
        }
      }
    }
   
    if( LO ){
     
    }else{
      int remaining = 0 ;
      int nleptons = nelectrons + nmuons + ntaus;
      if( nleptons == 2 && nhadrons == 0){
         //cout << "dilepton" << endl;
         ndileptonic++;
         if( ntaus ==0 || ( ntaus == 1 && (ntauelectrons+ntaumuons) == 1) || (ntaus == 2 && (ntauelectrons+ntaumuons) == 2)  ) { 
           ndileptonic2++; 
         }  
         if( ntaus == 0) ndileptonic3++;
      }else if( nleptons == 1 && nhadrons == 1){
         //cout << "lepton+jets" << endl;
         nsemileptonic++;
         if( ntaus ==0 || ( ntaus == 1 && (ntauelectrons+ntaumuons) == 1) ) nsemileptonic2++;
         if( ntaus == 0 ) {
           nsemileptonic3++;
           if( nmuons ) {
             h_muon_energy->Fill(genmuon->E);
             h_lepton_energy->Fill(genmuon->E);
           }
           if( nelectrons ) {
             h_electron_energy->Fill(genelectron->E);
             h_lepton_energy->Fill(genelectron->E);
           }
         }
      }else if ( nleptons == 0 && nhadrons == 2 ){
         //cout << "hadronic" << endl;
         nhadronic++;
      }else{
         //cout << "remaining" << endl;
         remaining++;
      }
    }


    if(branchMuon->GetEntries() > 0)
    {

      for(int i = 0; i < branchMuon->GetEntriesFast() ; i++){ 
        Muon * muon =  (Muon *) branchMuon->At(i);
        h_muon_energy_reco->Fill(muon->PT);
        h_lepton_energy_reco->Fill(muon->PT);
        break;
      }

    }

    if(branchElectron->GetEntries() > 0)
    {

      for(int i = 0; i < branchElectron->GetEntriesFast() ; i++){
        Electron * electron =  (Electron *) branchElectron->At(i);
        h_electron_energy_reco->Fill(electron->PT);
        h_lepton_energy_reco->Fill(electron->PT);
        break;
      }

    }



  }
  if( remaining != 0 ) cout << "Someting is wrong" << endl;
  //TCanvas * c = new TCanvas("c","c",1000,600);
  channel->SetBinContent(1,ndileptonic);
  channel->SetBinContent(2,ndileptonic2);
  channel->SetBinContent(3,ndileptonic3);
  channel->SetBinContent(4,nsemileptonic);
  channel->SetBinContent(5,nsemileptonic2);
  channel->SetBinContent(6,nsemileptonic3);
  channel->SetBinContent(7,nhadronic);

  channel->GetXaxis()->SetBinLabel(1,"Dileptonic");
  channel->GetXaxis()->SetBinLabel(2,"DileptonicTau");
  channel->GetXaxis()->SetBinLabel(3,"DileptonicNoTau");
  channel->GetXaxis()->SetBinLabel(4,"Semileptonic");
  channel->GetXaxis()->SetBinLabel(5,"SemileptonicTau");
  channel->GetXaxis()->SetBinLabel(6,"SemileptonicNoTau");
  channel->GetXaxis()->SetBinLabel(7,"Hadronic");

  // Show resulting histograms
  channel->SetStats(0000);
  double scale = 1.0/numberOfEntries;
  channel->Scale( scale );  
  //channel->Draw("HText0");


  
  channel->Write();
  h_muon_energy->Write();
  h_electron_energy->Write();
  h_lepton_energy->Write();

  h_muon_energy_reco->Write();
  h_electron_energy_reco->Write();
  h_lepton_energy_reco->Write();


  f->Close();
}
