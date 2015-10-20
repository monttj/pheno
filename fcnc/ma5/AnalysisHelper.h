#ifndef AnalysisHelper_h
#define AnalysisHelper_h

// SampleAnalyzer header
#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

// STL header
#include <iostream>
#include <string>
#include <vector>

// Fucking ROOT header
#include <TRandom.h>


namespace MA5
{
class AnalysisHelper
{
  // -------------------------------------------------------------
  //                        data members
  // -------------------------------------------------------------
 private:

  std::vector<const MCParticleFormat*> partons_;
  std::vector<const RecJetFormat*> matchedjets_;



  // -------------------------------------------------------------
  //                        method members
  // -------------------------------------------------------------
 public:

  enum  ConeType {CONE05=0, CONE04=1, CONE03=2, CONE02=3};

  // Constructor without argument
  AnalysisHelper() {}

  // Destructor
  ~AnalysisHelper() {}

  // Initialize method
  void Initialize(SampleFormat& sample, const EventFormat& event);

  // -----------------------------------------------------------------------------
  // dump jet-parton matching
  // -----------------------------------------------------------------------------
  void PrintJetPartonMatching() const
  {
    std::cout << "-------------------------------------" << std::endl;
    for (unsigned int i=0;i<partons_.size();i++)
    {
      std::cout << "- partons: ID=" << partons_[i]->pdgid() << "[" << partons_[i]->statuscode() << "] PT=" << partons_[i]->pt() << "\t -> jets: ";
      if (matchedjets_[i]==0) std::cout << "NULL" << std::endl;
      else std::cout << matchedjets_[i]->pt() << std::endl;
    }
    std::cout << "-------------------------------------" << std::endl;
  }
  

  // -----------------------------------------------------------------------------
  // partonMatching
  // -----------------------------------------------------------------------------
  const MCParticleFormat* partonMatching(const RecJetFormat* jet) const;

 
  // -----------------------------------------------------------------------------
  // ComingFromHadronicDecay
  // 0 = False, 1 = True, 2 = Undecided
  // -----------------------------------------------------------------------------
  unsigned int comingFromHadronicDecay(const MCParticleFormat* initial_part) const;
  unsigned int comingFromHadronicDecay(const MCParticleFormat& initial_part) const
  { return comingFromHadronicDecay(&initial_part); }


  // -----------------------------------------------------------------------------
  // motherID
  // return 0 if no mother found
  //        pdgID of the mother
  // -----------------------------------------------------------------------------
  int motherID(const MCParticleFormat* part) const;
  int motherID(const MCParticleFormat& part) const
  { return motherID(&part); }


  // -----------------------------------------------------------------------------
  // track isolation
  // -----------------------------------------------------------------------------
  double TrackIsolationObservable(const RecLeptonFormat* part, ConeType cone) const;
  bool isTrackIsolated(const RecLeptonFormat* part, const double& threshold, ConeType cone) const
  {
    double isolation = TrackIsolationObservable(part,cone);
    if (isolation<0) return false;
    else return ( isolation < threshold );
  }

  bool isTrackIsolated(const RecLeptonFormat& part, double threshold, ConeType cone) const
  { return isTrackIsolated(&part,threshold,cone); }


  // -----------------------------------------------------------------------------
  // calorimeter isolation
  // -----------------------------------------------------------------------------
  double CalorimeterIsolationObservable(const RecLeptonFormat* part, ConeType cone) const;
  bool isCalorimeterIsolated(const RecLeptonFormat* part, const double& threshold, ConeType cone) const
  {
    double isolation = CalorimeterIsolationObservable(part,cone);
    if (isolation<0) return false;
    else return ( isolation < threshold );
  }

  bool isCalorimeterIsolated(const RecLeptonFormat& part, double threshold, ConeType cone) const
  { return isCalorimeterIsolated(&part,threshold,cone); }


  // -----------------------------------------------------------------------------
  // PFlow isolation
  // -----------------------------------------------------------------------------
  double PFlowIsolationObservable(const RecLeptonFormat* part, ConeType cone) const;
  bool isPFlowIsolated(const RecLeptonFormat* part, const double& threshold, ConeType cone) const
  {
    double isolation = PFlowIsolationObservable(part,cone);
    if (isolation<0) return false;
    else return ( isolation < threshold );
  }

  bool isPFlowIsolated(const RecLeptonFormat& part, double threshold, ConeType cone) const
  { return isPFlowIsolated(&part,threshold,cone); }
  
  
  
  // -----------------------------------------------------------------------------
  // combined isolation
  // -----------------------------------------------------------------------------
  double CombinedIsolationObservable(const RecLeptonFormat* part, ConeType cone) const;
  bool passCombinedIsolation(const RecLeptonFormat* part, const double& threshold, ConeType cone) const
  {
    double isolation = CombinedIsolationObservable(part,cone);
    if (isolation<0) return false;
    else return ( isolation < threshold );
  }

  bool passCombinedIsolation(const RecLeptonFormat& part, double threshold, ConeType cone) const
  { return passCombinedIsolation(&part,threshold,cone); }


  
  
  
  

  // -----------------------------------------------------------------------------
  // JetCleaning
  // -----------------------------------------------------------------------------
  std::vector<const RecJetFormat*> 
     JetCleaning(const EventFormat& event, 
                 const std::vector<const RecLeptonFormat*> leptons,
                 double DeltaRmax = 0.1, double PTmin = 0.5) const;


  // -----------------------------------------------------------------------------
  // isCSVL
  // -----------------------------------------------------------------------------
  bool isCSVL(int flavour, float pT, float a_random) const;
  bool isCSVL(int flavour, float pT) const
  {return isCSVL(flavour,pT,gRandom->Uniform());}
  bool isCSVL(const RecJetFormat* jet) const
  { 
    const MCParticleFormat* parton = partonMatching(jet);
    int pid = 21;
    if (parton!=0) pid=parton->pdgid();
    return isCSVL(pid,jet->pt());
  }


  // -----------------------------------------------------------------------------
  // isCSVM
  // -----------------------------------------------------------------------------
  bool isCSVM(int flavour, float pT, float a_random) const;
  bool isCSVM(int flavour, float pT) const
  {return isCSVM(flavour,pT,gRandom->Uniform());}
  bool isCSVM(const RecJetFormat* jet) const
  { 
    const MCParticleFormat* parton = partonMatching(jet);
    int pid = 21;
    if (parton!=0) pid=parton->pdgid();
    return isCSVM(pid,jet->pt());
  }


  // -----------------------------------------------------------------------------
  // isCSVT
  // -----------------------------------------------------------------------------
  bool isCSVT(int flavour, float pT, float a_random) const;
  bool isCSVT(int flavour, float pT) const
  {return isCSVT(flavour,pT,gRandom->Uniform());}
  bool isCSVT(const RecJetFormat* jet) const
  { 
    const MCParticleFormat* parton = partonMatching(jet);
    int pid = 21;
    if (parton!=0) pid=parton->pdgid();
    return isCSVT(pid,jet->pt());
  }


  // -----------------------------------------------------------------------------
  // isCSV
  // -----------------------------------------------------------------------------
  unsigned int isCSV(int flavour, float pT, float a_random) const;
  unsigned int isCSV(int flavour, float pT) const
  {return isCSV(flavour,pT,gRandom->Uniform());}
  unsigned int isCSV(const RecJetFormat* jet) const
  { 
    const MCParticleFormat* parton = partonMatching(jet);
    int pid = 21;
    if (parton!=0) pid=parton->pdgid();
    return isCSV(pid,jet->pt());
  }


};
}

#endif
