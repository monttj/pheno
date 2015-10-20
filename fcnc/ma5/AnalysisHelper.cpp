#include "SampleAnalyzer/User/Analyzer/AnalysisHelper.h"
using namespace MA5;
using namespace std;


// -----------------------------------------------------------------------------
// Initialize
// -----------------------------------------------------------------------------
void AnalysisHelper::Initialize(SampleFormat& sample, const EventFormat& event)
{
  // Reset
  partons_.clear();
  matchedjets_.clear();

  // Safety: no MC information
  if (event.mc()==0) return;

  // Storing b and c quarks
  for (unsigned int i=0;i<event.mc()->particles().size();i++)
  {
    const MCParticleFormat& part = event.mc()->particles()[i];

    // particle from MadGraph
    if (part.statuscode()!=3) continue;

    // keeping only partons
    unsigned int pid = std::abs(part.pdgid());
    if (pid==21) pid=0;
    if (pid>=6) continue;

    // Removing partons from initial state
    if (i<4) continue;
    if (part.pt()<5) continue;

    // Safety: this particle must have a mother
    if (part.mother1()==0) continue;

    // Taking into account radiation
    // -> taking parton before radiation
    // Case of gluon ????
    if (part.mother1()->pdgid()==part.pdgid()) continue;

    // Saving parton
    partons_.push_back(&part);
  }

  // Ordering partons by PT order
  for (unsigned int i=0;i<partons_.size();i++)
    for (unsigned int j=i+1;j<partons_.size();j++)
    {
      if (partons_[i]->pt()<partons_[j]->pt())
      {
        const MCParticleFormat* tmp=partons_[i];
        partons_[i]=partons_[j];
        partons_[j]=tmp;
      }
    }

  // Prepare collection of jets
  matchedjets_.resize(partons_.size(),0);

  // Skip ?
  if (partons_.size()==0) return;

  // Mask : only one jet for each parton
  std::vector<bool> mask(partons_.size(),false);

  // Ordering jets by PT order
  std::vector<const RecJetFormat*> jets;
  for (unsigned int i=0;i<event.rec()->jets().size();i++)
  {
    jets.push_back(&event.rec()->jets()[i]);
  }
  for (unsigned int i=0;i<jets.size();i++)
    for (unsigned int j=i+1;j<jets.size();j++)
    {
      if (jets[i]->pt()<jets[j]->pt())
      {
        const RecJetFormat* tmp=jets[i];
        jets[i]=jets[j];
        jets[j]=tmp;
      }
    }

  // Matching jets
  for (unsigned int i=0;i<jets.size();i++)
  {
    const RecJetFormat* jet = jets[i];

    // cut on the PT
    if (jet->pt()<1.e-6) continue;

    int bestcandidate = -1;
    double deltaRmin = 0;

    // loop over particles
    for (unsigned int j=0;j<partons_.size();j++)
    {
      // is the parton already used?
      if (mask[j]) continue;

      // computing DR
      const MCParticleFormat* part = partons_[j];
      double dr=jet->dr(*part);

      // does the parton match the jet ?
      if (dr<0.5)
      {
        if (bestcandidate==-1 || deltaRmin<dr)
        {
          deltaRmin=dr;
          bestcandidate=j;
        }
      }
    }

    // final matching
    if (bestcandidate!=-1)
    {
       matchedjets_[bestcandidate]=jet;
       mask[bestcandidate]=true;
    }
  }
}
  

// -----------------------------------------------------------------------------
// partonMatching
// -----------------------------------------------------------------------------
const MCParticleFormat* AnalysisHelper::partonMatching(const RecJetFormat* jet) const
{
  // safety
  if (jet==0) return 0;

  // who match?
  std::vector<unsigned int> match;

  // loop over matched jets
  for (unsigned int i=0;i<matchedjets_.size();i++)
  {
    const RecJetFormat* part = matchedjets_[i];
    if (part==0) continue;

    // deltaR 
    double dr=jet->dr(part);
    if (dr<1e-4) match.push_back(i);
  }
  if (match.size()==0) return 0;
  else if (match.size()==1) return partons_[match[0]];
  else {std::cout << "WARNING: several partons possibles for jet candidate" << std::endl; return partons_[match[0]];}

}


// -----------------------------------------------------------------------------
// ComingFromHadronicDecay
// 0 = False, 1 = True, 2 = Undecided
// -----------------------------------------------------------------------------
unsigned int AnalysisHelper::comingFromHadronicDecay(const MCParticleFormat* initial_part) const
{
  // initialize algorithm
  const MCParticleFormat* part = initial_part;

  // loop over mothers
  // -> counter is defined to avoid infinite loop
  unsigned int loop_counter=0;
  while(loop_counter<100)
  {
    loop_counter++;

    // case 1: pointer null
    // -> unknown
    if(part==0) return 2;

    // case 2: particle with statuscode 3
    //         = produced by MadGraph
    // -> no hadronic decay
    else if (part->statuscode()==3) return 0;

    // case 3: particle produced by Pythia
    else
    {
      // absolute value of the PID 
      unsigned int pid = std::abs(part->pdgid());
        
      // coming from baryon or meson?
      bool hadronic = ( (pid>=100 && pid<=999) ||
                        (pid>=1000 && pid<=9999) );
      if (hadronic) return 1;
        
      // replace the current particle by its mother
      part = part->mother1();
    }
  }
  ERROR << "infinite loop in AnalysisHelper" << endmsg;
  return 2;
}


// -----------------------------------------------------------------------------
// motherID
// return 0 if no mother found
//        pdgID of the mother
// -----------------------------------------------------------------------------
int AnalysisHelper::motherID(const MCParticleFormat* part) const
{
  bool found = false;
  int pdgid_mother = -10000;
  unsigned int loop_counter=0;

  // particle
  const MCParticleFormat* mypart = part;

  // loop over mothers
  while(!found)
  {
    // avoiding infinite loop in case of problem
    loop_counter++;
    if (loop_counter>100)
    {
      ERROR << "infinite loop in AnalysisHelper" << endmsg;
      break;
    }

    // pointer null
    if(mypart==0)
    { 
      pdgid_mother = 0;
      found = true;
    }

    // no mother
    else if (mypart->mother1()==0)
    { 
      pdgid_mother = 0;
      found = true;
    }

    // mother found
    else
    {
      // mother id = particle id
      if( mypart->pdgid() != mypart->mother1()->pdgid() )
      {
        pdgid_mother = mypart->mother1()->pdgid();
        found = true;
      }
      else
      {
        mypart = mypart->mother1();
      }
    }
  }
  return pdgid_mother;
}



// -----------------------------------------------------------------------------
// isCSVL
// -----------------------------------------------------------------------------
bool AnalysisHelper::isCSVL(int flavour, float pT, float a_random) const
{
  float eff=0;
  flavour=std::abs(flavour);
  if (flavour==5) // b
  {      
    if (pT<=180)  eff = 0.62998+0.00422927*pT-2.28273e-05*pT*pT+3.67139e-08*pT*pT*pT;
    else          eff = 1.02904-0.00135858*pT+2.96676e-06*pT*pT-2.28016e-09*pT*pT*pT;
  }
  else if (flavour==4) // c
  {
    eff = 0.35;
  }
  else // light
  {                
    if (pT<=50)       eff = 0.23;
    else if (pT<=100) eff = 0.11;
    else              eff = 0.000071*pT + 0.093; 
  }
  if (a_random<eff) return true;
  else return false;
}


// -----------------------------------------------------------------------------
// isCSVM
// -----------------------------------------------------------------------------
bool AnalysisHelper::isCSVM(int flavour, float pT, float a_random) const
{
  float eff=0;
  flavour=std::abs(flavour);
  if (flavour==5) // b
  {
    if (pT<=180) eff = 0.41136+0.00754662*pT-5.25669e-05*pT*pT+1.14005e-07*pT*pT*pT;
    else         eff = 0.783171-0.000266654*pT-5.61403e-08*pT*pT;
  }
  else if (flavour==4)  // c
  {
    eff = 0.2;
  }
  else // light
  {
    if (pT<=50) eff = 0.016;
    else        eff = 0.000032*pT + 0.0104;
  }
  if (a_random<eff) return true;
  else return false;
}


// -----------------------------------------------------------------------------
// isCSVT
// -----------------------------------------------------------------------------
bool AnalysisHelper::isCSVT(int flavour, float pT, float a_random) const
{
  float eff=0;
  flavour=std::abs(flavour);
  if (flavour==5) // b
  {
    if (pT<=180)  eff = 0.7*(0.41136+0.00754662*pT-5.25669e-05*pT*pT+1.14005e-07*pT*pT*pT);
    else          eff = 0.7*(0.783171-0.000266654*pT-5.61403e-08*pT*pT);
  }
  else if (flavour==4) // c
  {
    eff = 0.055;
  }
  else // light
  {
    if (pT<=50) eff = 0.0018;
    else        eff = 0.11*(0.000032*pT + 0.0104);
  }
  if (a_random<eff) return true;
  else return false;
}


// -----------------------------------------------------------------------------
// isCSV
// -----------------------------------------------------------------------------
unsigned int AnalysisHelper::isCSV(int flavour, float pT, float a_random) const
{
  if (!isCSVL(flavour, pT, a_random)) return 0; // no-btagging
  else if (!isCSVM(flavour, pT, a_random)) return 1; // CSVL
  else if (!isCSVT(flavour, pT, a_random)) return 2; // CSVM
  else return 3; // CSVT
}


// -----------------------------------------------------------------------------
// track isolation
// -----------------------------------------------------------------------------
double AnalysisHelper::TrackIsolationObservable(const RecLeptonFormat* part, AnalysisHelper::ConeType cone) const
{
  // safety
  if (part==0) return -999;
  if (part->pt()<1e-6) return -999;
  if (part->isolCones().size()==0) 
  {
    ERROR << "No isolation cone is found for the particle." << endmsg;
    return -999;
  }

  // return isolation
  return part->isolCones()[static_cast<unsigned int>(cone)].sumPT()/part->pt();
}


// -----------------------------------------------------------------------------
// calorimeter isolation
// -----------------------------------------------------------------------------
double AnalysisHelper::CalorimeterIsolationObservable(const RecLeptonFormat* part, AnalysisHelper::ConeType cone) const
{
  // safety
  if (part==0) return -999;
  if (part->pt()<1e-6) return -999;
  if (part->isolCones().size()==0) 
  {
    ERROR << "No isolation cone is found for the particle." << endmsg;
    return -999;
  }

  // return isolation
  return part->isolCones()[static_cast<unsigned int>(cone)].sumET()/part->pt();
}


// -----------------------------------------------------------------------------
// PFlow isolation
// -----------------------------------------------------------------------------
double AnalysisHelper::PFlowIsolationObservable(const RecLeptonFormat* part, AnalysisHelper::ConeType cone) const
{
  // safety
  if (part==0) return -999;
  if (part->pt()<1e-6) return -999;
  if (part->isolCones().size()==0) 
  {
    ERROR << "No isolation cone is found for the particle." << endmsg;
    return -999;
  }

  // return isolation
  return part->isolCones()[static_cast<unsigned int>(cone)].sumPTeflow()/part->pt();
}

// -----------------------------------------------------------------------------
// combined isolation
// -----------------------------------------------------------------------------
double AnalysisHelper::CombinedIsolationObservable(const RecLeptonFormat* part, AnalysisHelper::ConeType cone) const
{
  // safety
  if (part==0) return -999;
  if (part->pt()<1e-6) return -999;
  if (part->isolCones().size()==0) 
  {
    ERROR << "No isolation cone is found for the particle." << endmsg;
    return -999;
  }

  // return isolation
  return (part->isolCones()[static_cast<unsigned int>(cone)].sumET()     + 
          part->isolCones()[static_cast<unsigned int>(cone)].sumPT())/part->pt();
}


// -----------------------------------------------------------------------------
// JetCleaning
// -----------------------------------------------------------------------------
std::vector<const RecJetFormat*> 
   AnalysisHelper::JetCleaning(const EventFormat& event, 
                               const std::vector<const RecLeptonFormat*> leptons, 
                               double DeltaRmax, double PTmin) const
{
  // cleaned collection of jets
  std::vector<const RecJetFormat*> cleaned_jets;

  // safety
  if (event.rec()==0) return cleaned_jets;

  // mask for jets
  // -> true = to remove
  std::vector<bool> mask(event.rec()->jets().size(),false);

  // apply the cut on PT
  for (unsigned int i=0;i<event.rec()->jets().size();i++)
  {
    if (event.rec()->jets()[i].pt()<PTmin) mask[i]=true;
  }

  // loop over leptons
  for (unsigned int i=0;i<leptons.size();i++)
  {
    // safety 
    if (leptons[i]==0) continue;
    if (leptons[i]->pt()<1e-6) continue;

    // 
    int jet_index=-1;
    double deltaR_min=0;

    // loop over jtes
    for (unsigned int j=0;j<event.rec()->jets().size();j++)
    {
      // shortcut to jet
      const RecJetFormat& jet = event.rec()->jets()[j];

      // mask: the lepton has been already removed
      if (mask[j]) continue;

      // cut on DeltaR
      double dr = leptons[i]->momentum().DeltaR(jet.momentum());
      if (dr>DeltaRmax) continue;
      
      // is it the closest lepton to the jet?
      if (jet_index==-1 || dr<deltaR_min)
      {
        deltaR_min=dr;
        jet_index=j;
      }
    }

    // is a lepton matched the jet?
    if (jet_index!=-1)
    {
      mask[jet_index]=true;
    }

  }

  // save the jets
  for (unsigned int i=0;i<event.rec()->jets().size();i++)
  {
    if (!mask[i]) cleaned_jets.push_back(&event.rec()->jets()[i]);
  }

  // return the cleaned collection
  return cleaned_jets;
}
