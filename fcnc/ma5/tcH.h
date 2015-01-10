#ifndef analysis_tcH_h
#define analysis_tcH_h

#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
class tcH : public AnalyzerBase
{
  INIT_ANALYSIS(tcH,"tcH")

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

 TH1F *h_photon1pt[7];
 TH1F *h_photon2pt[7];
 TH1F *h_photon1iso[7];
 TH1F *h_photon2iso[7];
 TH1F *h_diphotonM[7];
 TH1F *h_MET[7];
 TH1F *h_nbjets[7];
 TH1F *h_njets[7];
 TH1F *h_nphotons[7];
 TH1F *h_nelectrons[7];
 TH1F *h_nmuons[7];
 TH1F *h_nleptons[7];
 TH1F *h_mtop[7];
 TH1F *h_drjetlep[7];
 TH1F *h_drjetpho[7];

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
