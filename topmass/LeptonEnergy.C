//void LeptonEnergy(const char *inputFile = "sourceFiles/LO/ttbar_LO_total.root")
void LeptonEnergy(const TString & file)
{

  //const char *inputFile = Form("/home/tjkim/work/pheno/topmass/sourceFiles/LO/fromSayaka/ttbar_%s.root",file.Data());
  //gSystem->Load("/export/apps/delphes//libDelphes");
  const char *inputFile = Form("/data/users/seohyun/analysis/ttbar_%s.root",file.Data());
  gSystem->Load("/home/seohyun/delphes/libDelphes.so");
/*
  TFile *f2 = TFile::Open("weightfunc2.root");
  TFile *f3 = TFile::Open("weightfunc3.root");
  TFile *f5 = TFile::Open("weightfunc5.root");
  TFile *f15 = TFile::Open("weightfunc15.root");

  const int nmass = 151;
  float mymass[ nmass ];
  float integral2[ nmass ];
  float integral3[ nmass ];
  float integral5[ nmass ];
  float integral15[ nmass ];
  for(int i=0; i < nmass ; i++){
    integral2[i] = 0.0;
    integral3[i] = 0.0;
    integral5[i] = 0.0;
    integral15[i] = 0.0;
  }

  TGraph * g2[nmass];
  TGraph * g3[nmass];
  TGraph * g5[nmass];
  TGraph * g15[nmass];

  TIter next(f2->GetListOfKeys());
  TKey *key;
  int i = 0;
  while( (key = (TKey*) next())) {
    TClass *cl = gROOT->GetClass( key->GetClassName());
    if( !cl->InheritsFrom("TGraph")) continue;
    g2[i] = (TGraph*) key->ReadObj();
    string mass = g2[i]->GetName(); 
    float temp = atof(mass.c_str()); 
    mymass[i] = temp;
    i++;
  }

  TIter next(f3->GetListOfKeys());
  i = 0;
  while( (key = (TKey*) next())) {
    TClass *cl = gROOT->GetClass( key->GetClassName());
    if( !cl->InheritsFrom("TGraph")) continue;
    g3[i] = (TGraph*) key->ReadObj();
    i++;
  }

  TIter next(f5->GetListOfKeys());
  i = 0;
  while( (key = (TKey*) next())) {
    TClass *cl = gROOT->GetClass( key->GetClassName());
    if( !cl->InheritsFrom("TGraph")) continue;
    g5[i] = (TGraph*) key->ReadObj();
    i++;
  }

  TIter next(f15->GetListOfKeys());
  i = 0;
  while( (key = (TKey*) next())) {
    TClass *cl = gROOT->GetClass( key->GetClassName());
    if( !cl->InheritsFrom("TGraph")) continue;
    g15[i] = (TGraph*) key->ReadObj();
    i++;
  } 

  TFile * res = TFile::Open("hist_LO_res_v3.root");
  TH1F * h_acc = (TH1F*) res->Get("h_totalacc_lepton"); 
*/

  //TFile* f = TFile::Open("hist_LO_res_60.root", "recreate");
  TFile* f = TFile::Open(Form("170717/hist_%s.root",file.Data()), "recreate");

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
  TClonesArray *branchJet = treeReader->UseBranch("Jet");

  TClonesArray *branchEvent = treeReader->UseBranch("Event");
 
  GenParticle *particle;
  GenParticle *daughter1;
  GenParticle *daughter2;
  GenParticle *granddaughter1_1;
  GenParticle *granddaughter1_2;
  GenParticle *granddaughter2_1;
  GenParticle *granddaughter2_2;

  GenParticle *genelectron;
  GenParticle *genmuon;

  LHEFEvent * event;

  // Create TTree
  //Float_t Muon_E;
  //Float_t Electron_E;
  //Float_t Lepton_E;
  //Float_t Lepton_E_reco;
  //TTree * tree = new TTree("tree","lepton energy");
  //tree->Branch("Lepton_E",&Lepton_E,"Lepton_E/F"); 
  //tree->Branch("Lepton_E_reco",&Lepton_E_reco,"Lepton_E_reco/F"); 
  //tree->Branch("Muon_E",&Muon_E,"Muon_E/F"); 
  //tree->Branch("Electron_E",&Electron_E,"Electron_E/F"); 

  // Book histograms
  TH1 * channel = new TH1F("channel", "ttbar event categorization", 7, 0.0, 7.0);

  TH1 * h_muon_energy = new TH1F("h_muon_energy", "muon energy distribution", 5000, 0, 500);
  TH1 * h_electron_energy = new TH1F("h_electron_energy", "electron energy distribution", 5000, 0, 500);
  TH1 * h_lepton_energy = new TH1F("h_lepton_energy", "lepton energy distribution", 5000, 0, 500);

  //TH1 * h_muon_energy_acc = new TH1F("h_muon_energy_acc", "muon energy distribution", 5000, 0, 500);
  //TH1 * h_electron_energy_acc = new TH1F("h_electron_energy_acc", "electron energy distribution", 5000, 0, 500);
  TH1 * h_lepton_energy_acc = new TH1F("h_lepton_energy_acc", "lepton energy distribution", 5000, 0, 500);

  //TH1 * h_muon_energy_reco = new TH1F("h_muon_energy_reco", "muon energy distribution at RECO", 5000, 0, 500);
  //TH1 * h_electron_energy_reco = new TH1F("h_electron_energy_reco", "electron energy distribution at RECO", 5000, 0, 500);
  TH1 * h_lepton_energy_reco = new TH1F("h_lepton_energy_reco", "lepton energy distribution at RECO", 5000, 0, 500);
  TH2 * h2_lepton_energy_response = new TH2F("h2_lepton_energy_response", "lepton energy response", 5000, 0, 500,5000,0,500);

  //TH1 * h_muon_energy_reco_S2 = new TH1F("h_muon_energy_reco_S2", "muon energy distribution at RECO", 5000, 0, 500);
  //TH1 * h_electron_energy_reco_S2 = new TH1F("h_electron_energy_reco_S2", "electron energy distribution at RECO", 5000, 0, 500);
  //TH1 * h_lepton_energy_reco_S2 = new TH1F("h_lepton_energy_reco_S2", "lepton energy distribution at RECO", 5000, 0, 500);
  //TH1 * h_lepton_nbjets_reco_S2 = new TH1F("h_lepton_nbjets_reco_S2","number of b jets",5,0,5);

  //TH1 * h_muon_energy_reco_final = new TH1F("h_muon_energy_reco_final", "muon energy distribution at RECO", 5000, 0, 500);
  //TH1 * h_electron_energy_reco_final = new TH1F("h_electron_energy_reco_final", "electron energy distribution at RECO", 5000, 0, 500);
  TH1 * h_lepton_energy_reco_final = new TH1F("h_lepton_energy_reco_final", "lepton energy distribution at RECO", 5000, 0, 500);
  TH2 * h2_lepton_energy_final_response = new TH2F("h2_lepton_energy_final_response", "lepton energy response", 5000, 0, 500,5000,0,500);

  //std::vector<float> lepton_E;
  //std::vector<float> lepton_E_final;

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
    //if( entry == 100000) break;
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

    double genweight = 1.0;
    if(branchEvent->GetEntries() > 0)
    {

      event = (LHEFEvent * ) branchEvent->At(0);
      genweight = event->Weight;
      //cout << "event number = " << event->Number << endl;
      //cout << "event weight = " << event->Weight << endl;

    }

    //Lepton_E = -1.0;
    
    if(branchParticle->GetEntries() > 0)
    {
      
      for(int i = 0; i < branchParticle->GetEntriesFast() ; i++){
        if(ntop == 2) break;

        particle = (GenParticle *) branchParticle->At(i);
    
        int status = particle->Status; 

        bool LO = true;
        //if( LO ) cout << "THIS IS LO..." << endl;
        if( status != 3) continue;

        int id = particle->PID; 

        double gen_pt = particle->PT;
        double gen_eta = particle->Eta;
        //Leading order
        if( LO) {
          if( abs(id) == 11 ){
            genelectron = particle;
            double energy = genelectron->E;
            h_electron_energy->Fill( energy, genweight );
            h_lepton_energy->Fill( energy, genweight );
            //Lepton_E = energy;
            //for(int i=0; i < nmass; i++){
            //  float w = g2[i]->Eval( energy );
            //  integral2[i] = integral2[i] + w ;
            //}
            //lepton_E.push_back( energy );
            if( energy > 20 && fabs(gen_eta) < 2.4) {
              //h_electron_energy_acc->Fill( energy, genweight );
              h_lepton_energy_acc->Fill( energy, genweight );
              nmuons++;
            }
            //daughter1 = (GenParticle*) branchParticle->At( particle->D1);
            //daughter2 = (GenParticle*) branchParticle->At( particle->D2);
            //int d1_id = abs(daughter1->PID);
            //int d2_id = abs(daughter2->PID);
            //cout << "electron daughter  " << d1_id  << " , " << d2_id << endl;     
          }else if( abs(id) == 13 ){
            genmuon = particle;
            double energy = genmuon->E;
            h_muon_energy->Fill( energy, genweight );
            h_lepton_energy->Fill( energy, genweight );
            //Lepton_E = energy;
            //for(int i=0; i < nmass; i++){
            //  float w = g2[i]->Eval( energy );
            //  integral2[i] = integral2[i] + w ;
            //}
            //lepton_E.push_back( energy );
            if( energy > 20 && fabs(gen_eta) < 2.4) {
              //h_muon_energy_acc->Fill( energy, genweight );
              h_lepton_energy_acc->Fill( energy, genweight );
              nelectrons++;
            }
            //int d1_id = -1;
            //int d2_id = -1;
            //if( particle->D1 >= branchParticle->GetEntries()){
            //  daughter1 = (GenParticle*) branchParticle->At( particle->D1);
            //  int d1_id = abs(daughter1->PID);
            //}
            //if( particle->D1 >= branchParticle->GetEntries()){
            //  daughter1 = (GenParticle*) branchParticle->At( particle->D1);
            //  int d1_id = abs(daughter1->PID);
            //}
            //cout << "muon daughter  " << d1_id  << " , " << d2_id << endl;            
          }
        //NLO
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
          int W_dau_status = granddaughter1_1->Status ;

          //if( gd1_1_id > gd1_2_id ) cout << "Something is WRONG ! " << endl;

          GenParticle * le = (GenParticle * ) branchParticle->At( granddaughter1_1->D1 );
          //GenParticle * leda = (GenParticle * ) branchParticle->At( le->D1);
          if( gd1_1_id == 11 || gd1_1_id == 13 ){
            cout << le->D1 << " , " << le->D2 << endl;
          //  cout << " original id and status = " << gd1_1_id << " , " <<  W_dau_status  << " le id and status = " << le->PID << " , " << le->Status <<  " leda id and status = " << leda->PID << " , " << leda->Status << endl;
          }

          if( gd1_1_id == 11 ) { 
            nelectrons++;
            //genelectron = granddaughter1_2;
            genelectron = le;
          }
          else if( gd1_1_id == 13 ) { 
            nmuons++;
            //genmuon = granddaughter1_2;
            genmuon = le;
          }
          else if( gd1_1_id == 15 ) {
            ntaus++;

            /*
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
            ㅜㅜ  else { continue; }
          
            } else { continue; }
            */
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
             h_muon_energy->Fill(genmuon->E, genweight);
             h_lepton_energy->Fill(genmuon->E, genweight);
           }
           if( nelectrons ) {
             h_electron_energy->Fill(genelectron->E, genweight);
             h_lepton_energy->Fill(genelectron->E, genweight);
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

    Muon * mymuon;
    Electron * myelectron; 
    bool passmuon = false; 
    bool passelectron = false; 

    if(branchMuon->GetEntries() > 0)
    {

      for(int i = 0; i < branchMuon->GetEntriesFast() ; i++){ 
        Muon * muon =  (Muon *) branchMuon->At(i);
        if( muon->P4().E() > 20 && fabs( muon->P4().Eta() < 2.4) ){
          mymuon = muon;
        }
        break;
      }
     
      if( mymuon && ( nmuons > 0 || nelectrons > 0 ) ){ 
        //h_muon_energy_reco->Fill(mymuon->P4().E(), genweight);
        h_lepton_energy_reco->Fill(mymuon->P4().E(), genweight);
        h2_lepton_energy_response->Fill(mymuon->P4().E(), genmuon->E, genweight);
        passmuon = true;
      }

    }

    
    if(branchElectron->GetEntries() > 0)
    {
      for(int i = 0; i < branchElectron->GetEntriesFast() ; i++){
        Electron * electron =  (Electron *) branchElectron->At(i);
        if( electron->P4().E() > 20 && fabs( electron->P4().Eta() < 2.4) ){
          myelectron = electron;
        }
        break;
      }
      if( myelectron && ( nmuons > 0 || nelectrons > 0 ) ){
        //h_electron_energy_reco->Fill(myelectron->P4().E(), genweight);
        h_lepton_energy_reco->Fill(myelectron->P4().E(), genweight);
        h2_lepton_energy_response->Fill(myelectron->P4().E(), genelectron->E, genweight);
        passelectron = true;
      }
    }

    if(branchJet->GetEntries() > 0 )
    {
      int njets = 0;
      int nbjets = 0;
      for(int i = 0; i < branchJet->GetEntriesFast() ; i++){
        Jet * jet =  (Jet *) branchJet->At(i);
        if( jet->P4().Pt() > 30 && fabs( jet->P4().Eta() < 2.5) ){
          njets++;
          if( jet->BTag ) nbjets++;
        }
      }
    }

    //Muon_E = -9.0;
    //Electron_E = -9.0;
    //Lepton_E_reco = -1.0;
    float Energy = 9.0;
    if( passelectron && !passmuon && njets >= 4){
      float myele_energy = myelectron->P4().E();
      //h_electron_energy_reco_S2->Fill(myele_energy, genweight);
      //h_lepton_energy_reco_S2->Fill(myele_energy, genweight);
      //h_lepton_nbjets_reco_S2->Fill(nbjets);
      if( nbjets >= 2 ){
        //h_electron_energy_reco_final->Fill(myele_energy, genweight);
        h_lepton_energy_reco_final->Fill(myele_energy, genweight);
        h2_lepton_energy_final_response->Fill(myele_energy, genelectron->E, genweight);
      }
      //lepton_E_final.push_back( myelectron->P4().E() );
      //for(int i=0; i < nmass; i++){
      //  float corr = 1.0/ h_acc->Interpolate( myelectron->P4().E() );
      //  float w = g2[i]->Eval( myelectron->P4().E() );
        //integral2[i] = integral2[i] + w*corr ;
      //}
      //Electron_E = myele_energy;
      //Lepton_E_reco = myele_energy;
    }

    if( passmuon && !passelectron && njets >= 4){
      float mymuon_energy = mymuon->P4().E();
      //h_muon_energy_reco_S2->Fill(mymuon_energy, genweight);
      //h_lepton_energy_reco_S2->Fill(mymuon_energy, genweight);
      //h_lepton_nbjets_reco_S2->Fill(nbjets);
      if( nbjets >= 2  ){
      //  h_muon_energy_reco_final->Fill(mymuon_energy, genweight);
        h_lepton_energy_reco_final->Fill(mymuon_energy, genweight);
        h2_lepton_energy_final_response->Fill(mymuon_energy, genmuon->E, genweight);
      }
      //lepton_E_final.push_back( mymuon->P4().E() );
      //for(int i=0; i < nmass; i++){
      //  float corr = 1.0/ h_acc->Interpolate( mymuon->P4().E() );
      //  float w = g2[i]->Eval( mymuon->P4().E() );
        //integral2[i] = integral2[i] + w*corr ;
      //}
      //Muon_E = mymuon_energy;
      //Lepton_E_reco = mymuon_energy;
    }

/*
    for(int i=0; i < nmass; i++){
    //for(int i=0; i < 0; i++){
      float lenergy = -9;
      if( Muon_E > 0 && Electron_E < 0 ) lenergy = Muon_E;
      if( Muon_E < 0 && Electron_E > 0 ) lenergy = Electron_E;
      float acc = h_acc->Interpolate( lenergy );
      integral2[i] = integral2[i] +  g2[i]->Eval( lenergy ) /acc ;
      integral3[i] = integral3[i] +  g3[i]->Eval( lenergy ) /acc ;
      integral5[i] = integral5[i] +  g5[i]->Eval( lenergy ) /acc ;
      integral15[i] = integral15[i] +  g15[i]->Eval( lenergy ) /acc ;
    }
*/
    //if( passmuon && passelectron) cout << "Lepton E = " << Lepton_E << endl;
    //tree->Fill();
  }

  //tree->Print();
//  for(int m=0; m < nmass; m++){
//    for(int i=0; i < 400;i++){
//      float bincenter = h_lepton_energy->GetBinCenter(i+1);
//      float binconten = h_lepton_energy->GetBinContent(i+1);
//      float weight_value = g2[m]->Eval( bincenter );
//      integral2[m] = integral2[m] + weight_value*binconten;
//    }
//  }

/*
  for(int m=0; m < nmass; m++){
    for(int i=0; i < lepton_E_final.size() ;i++){
      float energy = lepton_E_final[i];
      float corr = 1.0/ h_acc->Interpolate( energy );
      float weight_value2 = g2[m]->Eval( bincenter );
      float weight_value3 = g3[m]->Eval( bincenter );
      float weight_value5 = g5[m]->Eval( bincenter );
      float weight_value15 = g15[m]->Eval( bincenter );
      integral2[m] = integral2[m] + weight_value2*corr;
      integral3[m] = integral3[m] + weight_value3*corr;
      integral5[m] = integral5[m] + weight_value5*corr;
      integral15[m] = integral15[m] + weight_value15*corr;
    }
  }

  TGraph * final2 = new TGraph();
  TGraph * final3 = new TGraph();
  TGraph * final5 = new TGraph();
  TGraph * final15 = new TGraph();
  for (Int_t i=0;i<nmass;i++) {
    final2->SetPoint(i, mymass[i], integral2[i]);
    final3->SetPoint(i, mymass[i], integral3[i]);
    final5->SetPoint(i, mymass[i], integral5[i]);
    final15->SetPoint(i, mymass[i], integral15[i]);
  }
  final2->SetName("n2");
  final3->SetName("n3");
  final5->SetName("n5");
  final15->SetName("n15");
  final2->Write();
  final3->Write();
  final5->Write();
  final15->Write();
*/

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

  //int nBins = 400;
  //h_lepton_energy->AddBinContent(nBins, h_lepton_energy->GetBinContent(nBins+1));
  //h_lepton_energy_reco_final->AddBinContent(nBins, h_lepton_energy_reco_final->GetBinContent(nBins+1));
  
  // Show resulting histograms
  channel->SetStats(0000);
  double scale = 1.0/numberOfEntries;
  channel->Scale( scale );  
  //channel->Draw("HText0");
/*  
  channel->Write();
  h_muon_energy->Write();
  h_electron_energy->Write();
  h_lepton_energy->Write();

  h_muon_energy_acc->Write();
  h_electron_energy_acc->Write();
  h_lepton_energy_acc->Write();

  h_muon_energy_reco->Write();
  h_electron_energy_reco->Write();
  h_lepton_energy_reco->Write();
 
  h_muon_energy_reco_final->Write();
  h_electron_energy_reco_final->Write();
  h_lepton_energy_reco_final->Write();
*/  
  f->Write();
  f->Close();
}

