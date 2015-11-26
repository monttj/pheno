void TTbarEvent(const char *inputFile)
{
  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);
  
  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();
  
  // Get pointers to branches used in this analysis
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
 
  GenParticle *particle;
  GenParticle *daughter1;
  GenParticle *daughter2;
  GenParticle *granddaughter1;
  GenParticle *granddaughter2;
 
  // Book histograms
  TH1 * channel = new TH1F("channel", "ttbar event categorization", 7, 0.0, 7.0);

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

        if( status != 3) continue;

        int id = particle->PID; 
        if( abs(id) == 6 ) {
          ntop++;

          particle = (GenParticle*) branchParticle->At( i ) ;

          bool lasttop  =  false ;
          while( !lasttop ){
            GenParticle * d = (GenParticle *) branchParticle->At( particle->D1 );
            if( abs(d->PID) != 6 ) {lasttop = true;}
            else { particle = d ; }
          } 

          daughter1 = (GenParticle*) branchParticle->At( particle->D1) ;
          daughter2 = (GenParticle*) branchParticle->At( particle->D2) ;
   
          bool lastW = false;

          while( !lastW) {
            GenParticle * d = (GenParticle *) branchParticle->At( daughter1->D1 );
            if( abs(d->PID) != 24 ) { lastW = true; }
            else {
              daughter1 = d ;
            }
          } 

          int d1_id = abs(daughter1->PID);
          int d2_id = abs(daughter2->PID);
 
          granddaughter1 = (GenParticle*) branchParticle->At( daughter1->D1) ;
          granddaughter2 = (GenParticle*) branchParticle->At( daughter1->D2) ;
     
          int gd1_id = abs(granddaughter1->PID);
          int gd2_id = abs(granddaughter2->PID);

          if( gd1_id == 11 || gd1_id == 12 ) nelectrons++;
          else if( gd1_id == 13 || gd1_id == 14 ) nmuons++;
          else if( gd1_id == 15 || gd1_id == 16 ) {
            ntaus++;
            GenParticle * taudaughter1 = (GenParticle*) branchParticle->At( granddaughter2->D1) ;
            GenParticle * taudaughter2 = (GenParticle*) branchParticle->At( granddaughter2->D2) ;
            int taud1_id = abs(taudaughter1->PID);
            int taud2_id = abs(taudaughter2->PID);

            //cout << "tau daughter = " << taud1_id << " " << taud2_id << endl;

            if( taud1_id == 11 || taud1_id == 12 ) ntauelectrons++;
            else if( taud1_id == 13 || taud1_id == 14 ) ntaumuons++;
            else if( taud1_id == 15 || taud1_id == 16 ) {
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
   
    //cout << "ntops = " << ntop << endl;

    int remaining = 0 ;
    int nleptons = nelectrons + nmuons + ntaus;
    if( nleptons == 2 && nhadrons == 0){
       ndileptonic++;
       if( ntaus ==0 || ( ntaus == 1 && (ntauelectrons+ntaumuons) == 1) || (ntaus == 2 && (ntauelectrons+ntaumuons) == 2)  ) { 
         ndileptonic2++; 
       }  
       if( ntaus == 0) ndileptonic3++;
    }else if( nleptons == 1 && nhadrons == 1){
       nsemileptonic++;
       if( ntaus ==0 || ( ntaus == 1 && (ntauelectrons+ntaumuons) == 1) ) nsemileptonic2++;
       if( ntaus == 0 ) nsemileptonic3++;
    }else if ( nleptons == 0 && nhadrons == 2 ){
       nhadronic++;
    }else{
       remaining++;
    }
  }
  if( remaining != 0 ) cout << "Someting is wrong" << endl;

  TCanvas * c = new TCanvas("c","c",1000,600);

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
  channel->Draw("HText0");

}
