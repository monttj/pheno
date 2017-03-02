void unfold(){

  gSystem->Load("/Users/tjkim/Work/Software/RooUnfold/libRooUnfold");

  bool scalestudy = false;

  TFile * f_167 = new TFile("sayaka/energy/hist_LO_167_sayaka_600K_3.root","read");
  TFile * f_170 = new TFile("sayaka/energy/hist_LO_170_sayaka_600K_3.root","read");
  TFile * f_173 = new TFile("sayaka/energy/hist_LO_173_sayaka_600K_3.root","read");
  TFile * f_176 = new TFile("sayaka/energy/hist_LO_176_sayaka_600K_3.root","read");
  TFile * f_179 = new TFile("sayaka/energy/hist_LO_179_sayaka_600K_3.root","read");

  TH1F * h_mea_S1_167 = (TH1F *) f_167->Get("h_lepton_energy_reco");
  TH1F * h_mea_S1_170 = (TH1F *) f_170->Get("h_lepton_energy_reco");
  TH1F * h_mea_S1_173 = (TH1F *) f_173->Get("h_lepton_energy_reco");
  TH1F * h_mea_S1_176 = (TH1F *) f_176->Get("h_lepton_energy_reco");
  TH1F * h_mea_S1_179 = (TH1F *) f_179->Get("h_lepton_energy_reco");

  TH1F * h_mea_167 = (TH1F *) f_167->Get("h_lepton_energy_reco_final");
  TH1F * h_mea_170 = (TH1F *) f_170->Get("h_lepton_energy_reco_final");
  TH1F * h_mea_173 = (TH1F *) f_173->Get("h_lepton_energy_reco_final");
  TH1F * h_mea_176 = (TH1F *) f_176->Get("h_lepton_energy_reco_final");
  TH1F * h_mea_179 = (TH1F *) f_179->Get("h_lepton_energy_reco_final");

  h_mea_S1_167->Rebin(20);
  h_mea_S1_170->Rebin(20);
  h_mea_S1_173->Rebin(20);
  h_mea_S1_176->Rebin(20);
  h_mea_S1_179->Rebin(20); 

  h_mea_167->Rebin(20);
  h_mea_170->Rebin(20);
  h_mea_173->Rebin(20);
  h_mea_176->Rebin(20);
  h_mea_179->Rebin(20);

  if( scalestudy ) {

    TFile * f_scaleup = new TFile("sayaka/energy/q2scale/hist_LO_173_sayaka_scale20.root","read");
    TFile * f_scalect = new TFile("sayaka/energy/q2scale/hist_LO_173_sayaka_scale10.root","read");
    TFile * f_scaledw = new TFile("sayaka/energy/q2scale/hist_LO_173_sayaka_scale05.root","read");

    TH1F * h_mea_S1_scaleup = (TH1F *) f_scaleup->Get("h_lepton_energy_reco");
    TH1F * h_mea_S1_scalect = (TH1F *) f_scalect->Get("h_lepton_energy_reco");
    TH1F * h_mea_S1_scaledw = (TH1F *) f_scaledw->Get("h_lepton_energy_reco");

    TH1F * h_mea_scaleup = (TH1F *) f_scaleup->Get("h_lepton_energy_reco_final");
    TH1F * h_mea_scalect = (TH1F *) f_scalect->Get("h_lepton_energy_reco_final");
    TH1F * h_mea_scaledw = (TH1F *) f_scaledw->Get("h_lepton_energy_reco_final");

    h_mea_scaleup->Rebin(20);
    h_mea_scalect->Rebin(20);
    h_mea_scaledw->Rebin(20);

  }
  

  TFile * f_response_167 = new TFile("sayaka/energy/hist_LO_167_sayaka_1200K.root","read");
  TFile * f_response_170 = new TFile("sayaka/energy/hist_LO_170_sayaka_1200K.root","read");
  TFile * f_response_173 = new TFile("sayaka/energy/hist_LO_173_sayaka_1200K.root","read");
  TFile * f_response_176 = new TFile("sayaka/energy/hist_LO_176_sayaka_1200K.root","read");
  TFile * f_response_179 = new TFile("sayaka/energy/hist_LO_179_sayaka_1200K.root","read");

  TH1F * h_res_mea_S1_167 = (TH1F *) f_response_167->Get("h_lepton_energy_reco");
  TH1F * h_res_mea_S1_170 = (TH1F *) f_response_170->Get("h_lepton_energy_reco");
  TH1F * h_res_mea_S1_173 = (TH1F *) f_response_173->Get("h_lepton_energy_reco");
  TH1F * h_res_mea_S1_176 = (TH1F *) f_response_176->Get("h_lepton_energy_reco");
  TH1F * h_res_mea_S1_179 = (TH1F *) f_response_179->Get("h_lepton_energy_reco");

  TH1F * h_res_mea_167 = (TH1F *) f_response_167->Get("h_lepton_energy_reco_final");
  TH1F * h_res_mea_170 = (TH1F *) f_response_170->Get("h_lepton_energy_reco_final");
  TH1F * h_res_mea_173 = (TH1F *) f_response_173->Get("h_lepton_energy_reco_final");
  TH1F * h_res_mea_176 = (TH1F *) f_response_176->Get("h_lepton_energy_reco_final");
  TH1F * h_res_mea_179 = (TH1F *) f_response_179->Get("h_lepton_energy_reco_final");

  h_res_mea_S1_167->Rebin(20);
  h_res_mea_S1_170->Rebin(20);
  h_res_mea_S1_173->Rebin(20);
  h_res_mea_S1_176->Rebin(20);
  h_res_mea_S1_179->Rebin(20);

  h_res_mea_167->Rebin(20);
  h_res_mea_170->Rebin(20);
  h_res_mea_173->Rebin(20);
  h_res_mea_176->Rebin(20);
  h_res_mea_179->Rebin(20);
 
  int nbins = h_mea_176->GetNbinsX();

  //h_mea_167 = h_mea_S1_167;
  //h_mea_170 = h_mea_S1_170;
  //h_mea_173 = h_mea_S1_173;
  //h_mea_176 = h_mea_S1_176;
  //h_mea_179 = h_mea_S1_179;
  //h_mea_173->Reset();
  for(int i = 1; i <= nbins; i++){
    if( i > 10 ){
 
      //double cor167 = h_res_mea_S1_167->GetBinContent(i)/h_res_mea_167->GetBinContent(i);
      //double cor170 = h_res_mea_S1_170->GetBinContent(i)/h_res_mea_170->GetBinContent(i);
      //double cor173 = h_res_mea_S1_173->GetBinContent(i)/h_res_mea_173->GetBinContent(i);
      //double cor176 = h_res_mea_S1_176->GetBinContent(i)/h_res_mea_176->GetBinContent(i);
      //double cor179 = h_res_mea_S1_179->GetBinContent(i)/h_res_mea_179->GetBinContent(i);

      double cor167 = h_mea_S1_167->GetBinContent(i)/h_mea_167->GetBinContent(i);
      double cor170 = h_mea_S1_170->GetBinContent(i)/h_mea_170->GetBinContent(i);
      double cor173 = h_mea_S1_173->GetBinContent(i)/h_mea_173->GetBinContent(i);
      double cor176 = h_mea_S1_176->GetBinContent(i)/h_mea_176->GetBinContent(i);
      double cor179 = h_mea_S1_179->GetBinContent(i)/h_mea_179->GetBinContent(i);

      double num167 = h_mea_167->GetBinContent(i) * cor167;
      double num170 = h_mea_170->GetBinContent(i) * cor170;
      double num173 = h_mea_173->GetBinContent(i) * cor173;
      double num176 = h_mea_176->GetBinContent(i) * cor176;
      double num179 = h_mea_179->GetBinContent(i) * cor179;

      h_mea_167->SetBinContent(i, num167);
      h_mea_170->SetBinContent(i, num170);
      h_mea_173->SetBinContent(i, num173);
      h_mea_176->SetBinContent(i, num176);
      h_mea_179->SetBinContent(i, num179);
  
      if( scalestudy ){

        double cor_scaleup = h_mea_S1_scaleup->GetBinContent(i)/h_mea_scaleup->GetBinContent(i);
        double cor_scalect = h_mea_S1_scalect->GetBinContent(i)/h_mea_scalect->GetBinContent(i);
        double cor_scaledw = h_mea_S1_scaledw->GetBinContent(i)/h_mea_scaledw->GetBinContent(i);

        double num_scaleup = h_mea_scaleup->GetBinContent(i) * cor_scaleup;
        double num_scalect = h_mea_scalect->GetBinContent(i) * cor_scalect;
        double num_scaledw = h_mea_scaledw->GetBinContent(i) * cor_scaledw;

        h_mea_scaleup->SetBinContent(i, num_scaleup);
        h_mea_scalect->SetBinContent(i, num_scalect);
        h_mea_scaledw->SetBinContent(i, num_scaledw);

      }
    }
  }

  if( scalestudy ){
    h_mea_scaleup->Scale(2.0);
    h_mea_scalect->Scale(2.0);
    h_mea_scaledw->Scale(2.0);
  }

  TH2F * m = (TH2F *) f_response_173->Get("h2_lepton_energy_response");
  m->Rebin2D(20,20);
  const TH1* h_gen = m->ProjectionY();
  const TH1* h_rec = m->ProjectionX();

  RooUnfoldResponse *response = new RooUnfoldResponse(h_rec, h_gen, m); 

  RooUnfold* unfold_167 = new RooUnfoldBinByBin(response, h_mea_167);
  RooUnfold* unfold_170 = new RooUnfoldBinByBin(response, h_mea_170);
  RooUnfold* unfold_173 = new RooUnfoldBinByBin(response, h_mea_173);
  RooUnfold* unfold_176 = new RooUnfoldBinByBin(response, h_mea_176);
  RooUnfold* unfold_179 = new RooUnfoldBinByBin(response, h_mea_179);
  if( scalestudy ){
    RooUnfold* unfold_scaleup = new RooUnfoldBinByBin(response, h_mea_scaleup);
    RooUnfold* unfold_scalect = new RooUnfoldBinByBin(response, h_mea_scalect);
    RooUnfold* unfold_scaledw = new RooUnfoldBinByBin(response, h_mea_scaledw);
  }
  //RooUnfold* unfold_167 = new RooUnfoldInvert(response, h_mea_167);
  //RooUnfold* unfold_170 = new RooUnfoldInvert(response, h_mea_170);
  //RooUnfold* unfold_173 = new RooUnfoldInvert(response, h_mea_173);
  //RooUnfold* unfold_176 = new RooUnfoldInvert(response, h_mea_176);
  //RooUnfold* unfold_179 = new RooUnfoldInvert(response, h_mea_179);
  //RooUnfold* unfold_167 = new RooUnfoldSvd(response, h_mea_167, 10);
  //RooUnfold* unfold_170 = new RooUnfoldSvd(response, h_mea_170, 10);
  //RooUnfold* unfold_173 = new RooUnfoldSvd(response, h_mea_173, 10);
  //RooUnfold* unfold_176 = new RooUnfoldSvd(response, h_mea_176, 10);
  //RooUnfold* unfold_179 = new RooUnfoldSvd(response, h_mea_179, 10);


  //RooUnfold::ErrorTreatment err = RooUnfold::kCovariance;
  RooUnfold::ErrorTreatment err = RooUnfold::kCovToy;
  TH1F* h_unfold_167 = (TH1F*) unfold_167->Hreco(err);
  TH1F* h_unfold_170 = (TH1F*) unfold_170->Hreco(err);
  TH1F* h_unfold_173 = (TH1F*) unfold_173->Hreco(err);
  TH1F* h_unfold_176 = (TH1F*) unfold_176->Hreco(err);
  TH1F* h_unfold_179 = (TH1F*) unfold_179->Hreco(err);

  if( scalestudy ){
    TH1F* h_unfold_scaleup = (TH1F*) unfold_scaleup->Hreco(err);
    TH1F* h_unfold_scalect = (TH1F*) unfold_scalect->Hreco(err);
    TH1F* h_unfold_scaledw = (TH1F*) unfold_scaledw->Hreco(err);
  }

  TH1F * h_gen_full = (TH1F *) f_response_173->Get("h_lepton_energy");
  h_gen_full->Rebin(20);
  TH1F* h_gen_full_scaled = (TH1F*) h_gen_full->Clone("h_gen_full_scaled");
  h_gen_full_scaled->Scale(0.5);
  for(int i = 1; i <= nbins; i++){
    if( i <= 15){
      h_unfold_167->SetBinContent(i,h_gen_full_scaled->GetBinContent(i));
      h_unfold_170->SetBinContent(i,h_gen_full_scaled->GetBinContent(i));
      h_unfold_173->SetBinContent(i,h_gen_full_scaled->GetBinContent(i));
      h_unfold_176->SetBinContent(i,h_gen_full_scaled->GetBinContent(i));
      h_unfold_179->SetBinContent(i,h_gen_full_scaled->GetBinContent(i));
      if( scalestudy){
        h_unfold_scaleup->SetBinContent(i,h_gen_full_scaled->GetBinContent(i));
        h_unfold_scalect->SetBinContent(i,h_gen_full_scaled->GetBinContent(i));
        h_unfold_scaledw->SetBinContent(i,h_gen_full_scaled->GetBinContent(i));
      }
    }else{
      double corr = h_gen_full->GetBinContent(i)/h_gen->GetBinContent(i);
      h_unfold_167->SetBinContent(i,h_unfold_167->GetBinContent(i)*corr);
      h_unfold_170->SetBinContent(i,h_unfold_170->GetBinContent(i)*corr);
      h_unfold_173->SetBinContent(i,h_unfold_173->GetBinContent(i)*corr);
      h_unfold_176->SetBinContent(i,h_unfold_176->GetBinContent(i)*corr);
      h_unfold_179->SetBinContent(i,h_unfold_179->GetBinContent(i)*corr);
      if( scalestudy){
        h_unfold_scaleup->SetBinContent(i,h_unfold_scaleup->GetBinContent(i)*corr);
        h_unfold_scalect->SetBinContent(i,h_unfold_scalect->GetBinContent(i)*corr);
        h_unfold_scaledw->SetBinContent(i,h_unfold_scaledw->GetBinContent(i)*corr);
      }
    }
  } 

  //systematic uncertainties
  TH1F * h_unfold_167_up = (TH1F*) h_unfold_167->Clone("h_unfold_167_up"); 
  TH1F * h_unfold_170_up = (TH1F*) h_unfold_170->Clone("h_unfold_170_up"); 
  TH1F * h_unfold_173_up = (TH1F*) h_unfold_173->Clone("h_unfold_173_up"); 
  TH1F * h_unfold_176_up = (TH1F*) h_unfold_176->Clone("h_unfold_176_up"); 
  TH1F * h_unfold_179_up = (TH1F*) h_unfold_179->Clone("h_unfold_179_up");

  TH1F * h_unfold_167_dw = (TH1F*) h_unfold_167->Clone("h_unfold_167_dw");
  TH1F * h_unfold_170_dw = (TH1F*) h_unfold_170->Clone("h_unfold_170_dw");
  TH1F * h_unfold_173_dw = (TH1F*) h_unfold_173->Clone("h_unfold_173_dw");
  TH1F * h_unfold_176_dw = (TH1F*) h_unfold_176->Clone("h_unfold_176_dw");
  TH1F * h_unfold_179_dw = (TH1F*) h_unfold_179->Clone("h_unfold_179_dw");
  
  TRandom *ran = new TRandom(); 

  for(int i = 1; i <= nbins; i++){
    double r = ran->Uniform();  
    if( i <= 20){
      h_unfold_167_up->SetBinContent(i, h_unfold_167->GetBinContent(i) + r*h_unfold_167->GetBinError(i));  
      h_unfold_170_up->SetBinContent(i, h_unfold_170->GetBinContent(i) + r*h_unfold_170->GetBinError(i));  
      h_unfold_173_up->SetBinContent(i, h_unfold_173->GetBinContent(i) + r*h_unfold_173->GetBinError(i));  
      h_unfold_176_up->SetBinContent(i, h_unfold_176->GetBinContent(i) + r*h_unfold_176->GetBinError(i));  
      h_unfold_179_up->SetBinContent(i, h_unfold_179->GetBinContent(i) + r*h_unfold_179->GetBinError(i));  
  
      h_unfold_167_dw->SetBinContent(i, h_unfold_167->GetBinContent(i) - r*h_unfold_167->GetBinError(i));
      h_unfold_170_dw->SetBinContent(i, h_unfold_170->GetBinContent(i) - r*h_unfold_170->GetBinError(i));
      h_unfold_173_dw->SetBinContent(i, h_unfold_173->GetBinContent(i) - r*h_unfold_173->GetBinError(i));
      h_unfold_176_dw->SetBinContent(i, h_unfold_176->GetBinContent(i) - r*h_unfold_176->GetBinError(i));
      h_unfold_179_dw->SetBinContent(i, h_unfold_179->GetBinContent(i) - r*h_unfold_179->GetBinError(i));      
    }else{
      h_unfold_167_up->SetBinContent(i, h_unfold_167->GetBinContent(i) - r*h_unfold_167->GetBinError(i));
      h_unfold_170_up->SetBinContent(i, h_unfold_170->GetBinContent(i) - r*h_unfold_170->GetBinError(i));
      h_unfold_173_up->SetBinContent(i, h_unfold_173->GetBinContent(i) - r*h_unfold_173->GetBinError(i));
      h_unfold_176_up->SetBinContent(i, h_unfold_176->GetBinContent(i) - r*h_unfold_176->GetBinError(i));
      h_unfold_179_up->SetBinContent(i, h_unfold_179->GetBinContent(i) - r*h_unfold_179->GetBinError(i));

      h_unfold_167_dw->SetBinContent(i, h_unfold_167->GetBinContent(i) + r*h_unfold_167->GetBinError(i));
      h_unfold_170_dw->SetBinContent(i, h_unfold_170->GetBinContent(i) + r*h_unfold_170->GetBinError(i));
      h_unfold_173_dw->SetBinContent(i, h_unfold_173->GetBinContent(i) + r*h_unfold_173->GetBinError(i));
      h_unfold_176_dw->SetBinContent(i, h_unfold_176->GetBinContent(i) + r*h_unfold_176->GetBinError(i));
      h_unfold_179_dw->SetBinContent(i, h_unfold_179->GetBinContent(i) + r*h_unfold_179->GetBinError(i));
    } 

  }


  TFile* f = TFile::Open("hist_LO_final_unfolded_mod_30.root", "recreate");

  TCanvas * c = new TCanvas("c","c",1);
  h_unfold_167->SetLineColor(kRed);
  h_unfold_170->SetLineColor(kRed+1);
  h_unfold_173->SetLineColor(kRed+2);
  h_unfold_176->SetLineColor(kRed+3);
  h_unfold_179->SetLineColor(kRed+4);
  h_gen->SetLineColor(4);
  h_gen_full_scaled->SetLineColor(6);
  h_gen->Draw();
  //h_gen_full_scaled->Draw("same");
  //h_unfold_167->Draw("Hsame");
  //h_unfold_170->Draw("Hsame");
  h_unfold_173->Draw("Hsame");
  //h_unfold_176->Draw("Hsame");
  //h_unfold_179->Draw("Hsame");
  //h_unfold_scalect->Draw("Hsame");
  //h_unfold_scalect->SetLineColor(2);
  //h_unfold_scalect->SetLineWidth(2); 

  h_unfold_167->Write("h_unfold_167");
  h_unfold_170->Write("h_unfold_170");
  h_unfold_173->Write("h_unfold_173");
  h_unfold_176->Write("h_unfold_176");
  h_unfold_179->Write("h_unfold_179");

  h_unfold_167_up->Write("h_unfold_167_up");
  h_unfold_170_up->Write("h_unfold_170_up");
  h_unfold_173_up->Write("h_unfold_173_up");
  h_unfold_176_up->Write("h_unfold_176_up");
  h_unfold_179_up->Write("h_unfold_179_up");

  h_unfold_167_dw->Write("h_unfold_167_dw");
  h_unfold_170_dw->Write("h_unfold_170_dw");
  h_unfold_173_dw->Write("h_unfold_173_dw");
  h_unfold_176_dw->Write("h_unfold_176_dw");
  h_unfold_179_dw->Write("h_unfold_179_dw");

  if( scalestudy ){
    h_unfold_scaleup->Write("h_unfold_scaleup");
    h_unfold_scalect->Write("h_unfold_scalect");
    h_unfold_scaledw->Write("h_unfold_scaledw");
  }

  f->Write();
  f->Close();
}
 
