#ifndef analysis_FCNC_2P_h
#define analysis_FCNC_2P_h

#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
class FCNC_2P : public AnalyzerBase
{
  INIT_ANALYSIS(FCNC_2P,"FCNC_2P")

 public:
  virtual bool Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters);
  virtual void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files);
  virtual bool Execute(SampleFormat& sample, const EventFormat& event);

 private:

 //-----------------------------------------
 //-----------------------------------------
 //output root file
 //-----------------------------------------
 //-----------------------------------------
 TFile* output;
 std::string outputName_;
 //-----------------------------------------
 //-----------------------------------------
 //histograms
 //-----------------------------------------
 //-----------------------------------------
 
 TH1F *cutFlow;
 TH1F *cutFlow2;

 TH1F *h_photon1pt[10];
 TH1F *h_photon2pt[10];
 TH1F *h_photon1iso[10];
 TH1F *h_photon2iso[10];
 TH1F *h_diphotonM[10];
 TH1F *h_MET[10];
 TH1F *h_nbjetsCSVM[10];
 TH1F *h_nbjetsCSVT[10];
 TH1F *h_njets[10];

 TH1F *h_muon1pt[10];
 TH1F *h_muon1iso[10];
 TH1F *h_elec1pt[10];
 TH1F *h_elec1iso[10];

 TH1F *h_nphotons[10];
 TH1F *h_nelectrons[10];
 TH1F *h_nmuons[10];
 TH1F *h_nleptons[10];
 TH1F *h_mtop[10];
 TH1F *h_drjetlep[10];
 TH1F *h_drjetpho[10];

 //-----------------------------------------
 //-----------------------------------------
 //for MC reweighting
 //-----------------------------------------
 //-----------------------------------------
 float xsec;
 float nevent;
 float luminosity;
 float weight;
 
 
 float isolation(int deltaR, const RecLeptonFormat& lept);
 float isolation(int deltaR, const RecPhotonFormat& lept);

};
}

#endif
