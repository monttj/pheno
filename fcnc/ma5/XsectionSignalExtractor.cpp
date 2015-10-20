// SamplaAnalyzer headers
#include "XsectionSignalExtractor.h"
#include "FCNCcoupling.h"
using namespace MA5;

#define UNUSED(expr) do { (void)(expr); } while (0)

// STL headers
#include <algorithm>
#include <iomanip>

// Initializing const values
const double XsectionSignalExtractor::ab=1e6;
const double XsectionSignalExtractor::fb=1e3;
const double XsectionSignalExtractor::pb=1;
const double XsectionSignalExtractor::nb=1e-3;
const double XsectionSignalExtractor::mb=1e-6;

// -----------------------------------------------------------------------------
// Getting Xsection
// -----------------------------------------------------------------------------
bool XsectionSignalExtractor::Initialize()
{
  // Z BR
  double BR_ZToLL        = (3.36*3)/100.;
  double BR_ZToNuNu      = 20./100.;
  double BR_ZToJJ        = 1-(BR_ZToLL+BR_ZToNuNu);
  double BR_ZToHadrons   = BR_ZToJJ;
  double BR_ZToInvisible = BR_ZToNuNu;

  // W BR
  double BR_WToLNu  = (10.80*3)/100.;
  double BR_WToJJ   = 1-BR_WToLNu;

  // T BR
  double BR_TToWB   = 1;

  // Higgs BR
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  double BR_HToBB     = 5.77e-1;
  double BR_HToTauTau = 6.32e-2;
  double BR_HToMuMu   = 2.19e-4;
  double BR_HToEE     = 0.;
  double BR_HToLL     = BR_HToEE+BR_HToMuMu+BR_HToTauTau;
  double BR_HToCC     = 2.91e-2;
  double BR_HToSS     = 2.46e-2;
  double BR_HToGG     = 8.57e-2;
  double BR_HToJJ     = BR_HToCC+BR_HToSS+BR_HToGG;
  double BR_HToAA     = 2.28e-3;
  double BR_HToZA     = 1.54e-3;
  double BR_HToWW     = 2.15e-1;
  double BR_HToZZ     = 2.64e-2;

  // temporary variables
  double BR = 0;
  double BRaa = 0;
  double BRzz = 0;
  double BRgg = 0;

  // Avoiding warning during compilations
  // For avoiding warnings "variable not used"
  // this function does nothing
  UNUSED(BR_HToLL);
  UNUSED(BR_HToJJ);
  UNUSED(BR_HToZA);
  UNUSED(BR_WToJJ);
  UNUSED(BR_ZToInvisible);
  UNUSED(BR_ZToHadrons);

  // ----------------------------------------------
  // COUPLING TABLE
  // ----------------------------------------------

  // table with value = 
  // 1) partial width / g^2
  //   see https://sbgcmswikidoc.in2p3.fr/doku.php?id=topfcnc_xsection
  // 2) best limit on BR
  //   see https://indico.desy.de/getFile.py/access?contribId=232&sessionId=32&resId=0&materialId=slides&confId=8648 (slide 39)
  // 2-bis) best limit on tHq 
  //   see https://cds.cern.ch/record/1565103/files/ATLAS-CONF-2013-081.pdf (only tcH treated, we consider tuH as equal to tcH)
  // 3) unit: True=GeV-1, false=no unit
  std::map<std::string,FCNCcoupling> couplings;

  // - up quark
  couplings["kappa_gut"]=FCNCcoupling(3.665e5,0.0031/100.,true);
  couplings["kappa_aut"]=FCNCcoupling(1.989e4,0.016/100.,true);
  couplings["kappa_hut"]=FCNCcoupling(0.1836, 0.56/100.,false);
  couplings["kappa_zut"]=FCNCcoupling(1.637e4,0.05/100.,true);
  couplings["zeta_zut"] =FCNCcoupling(0.1685, 0.05/100.,false);

  // - charm quark
  couplings["kappa_gct"]=FCNCcoupling(3.665e5,0.016/100.,true);
  couplings["kappa_act"]=FCNCcoupling(1.989e4,0.182/100.,true);
  couplings["kappa_hct"]=FCNCcoupling(0.1836, 0.56/100.,false);
  couplings["kappa_zct"]=FCNCcoupling(1.637e4,0.05/100.,true);
  couplings["zeta_zct"] =FCNCcoupling(0.1685, 0.05/100.,false);

  // ----------------------------------------------
  // SINGLE-TOP XSECTION TABLE
  // ----------------------------------------------

  // table with processes: p p > t + 0 jet
  double kfac=1.5; // arxiv:1409.6959, conservative choice since it could be 54% or 58%
  Insert("t-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.979e10*kfac));
  Insert("t-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],8.666e9*kfac));

  // table with processes: p p > t + 0/1 jet (matched)
  Insert("t-1j-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.979e10*kfac));
  Insert("t-1j-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],8.666e9*kfac));

  // table with processes: p p > t a
  kfac=1; //?????
  Insert("tgamma-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],6.781e8*kfac));
  Insert("tgamma-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],1.349e8*kfac));
  kfac=1.4; //Lorenzo approval
  Insert("tgamma-kappa-aut",new SignalSingleTop(couplings["kappa_aut"],1.46e7*kfac));
  kfac=1.4; //Lorenzo approval
  Insert("tgamma-kappa-act",new SignalSingleTop(couplings["kappa_act"],1.884e6*kfac));

  // table with processes: p p > t H, H > a a
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  BR = BR_HToAA;
  kfac=1.; /// ??????
  Insert("tHToGammaGamma-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],4.884e7*BR*kfac));
  Insert("tHToGammaGamma-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],6.404e6*BR*kfac));
  kfac=1.5; //Lorenzo approval
  Insert("tHToGammaGamma-kappa-hut",new SignalSingleTop(couplings["kappa_hut"],74.12*BR*kfac));
  kfac=1.8; //Lorenzo approval
  Insert("tHToGammaGamma-kappa-hct",new SignalSingleTop(couplings["kappa_hct"],9.179*BR*kfac));

  // table with processes: p p > t H, H > W W
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  BR = BR_HToWW;
  kfac=1.; /// ??????
  Insert("tHToWW-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],4.884e7*BR*kfac));
  Insert("tHToWW-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],6.404e6*BR*kfac));
  kfac=1.5; //Lorenzo approval
  Insert("tHToWW-kappa-hut",new SignalSingleTop(couplings["kappa_hut"],74.12*BR*kfac));
  kfac=1.8; //Lorenzo approval
  Insert("tHToWW-kappa-hct",new SignalSingleTop(couplings["kappa_hct"],9.179*BR*kfac));

  // table with processes: p p > t H, H > Z Z 
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  BR = BR_HToZZ;
  kfac=1.; /// ??????
  Insert("tHToZZ-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],4.88e+7*BR*kfac));
  Insert("tHToZZ-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],6.40e+6*BR*kfac));
  kfac=1.5; //Lorenzo approval
  Insert("tHToZZ-kappa-hut",new SignalSingleTop(couplings["kappa_hut"],74.12*BR*kfac));
  kfac=1.8; //Lorenzo approval
  Insert("tHToZZ-kappa-hct",new SignalSingleTop(couplings["kappa_hct"],9.179*BR*kfac));

  // table with processes: p p > t H, H > Tau Tau 
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  BR = BR_HToTauTau;
  kfac=1.; /// ??????
  Insert("tHToTauTau-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],4.88e+7*BR*kfac));
  Insert("tHToTauTau-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],6.40e+6*BR*kfac));
  kfac=1.5; //Lorenzo approval
  Insert("tHToTauTau-kappa-hut",new SignalSingleTop(couplings["kappa_hut"],74.12*BR*kfac));
  kfac=1.8; //Lorenzo approval
  Insert("tHToTauTau-kappa-hct",new SignalSingleTop(couplings["kappa_hct"],9.179*BR*kfac));

  // table with processes: p p > t H, H > B B
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  BR = BR_HToBB;
  kfac=1.; /// ??????
  Insert("tHToBB-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],4.88e+7*BR*kfac));
  Insert("tHToBB-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],6.40e+6*BR*kfac));
  kfac=1.5; //Lorenzo approval
  Insert("tHToBB-kappa-hut",new SignalSingleTop(couplings["kappa_hut"],74.12*BR*kfac));
  kfac=1.8; //Lorenzo approval
  Insert("tHToBB-kappa-hct",new SignalSingleTop(couplings["kappa_hct"],9.179*BR*kfac));

  // table with processes: p p > t H, H > C C
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  BR = BR_HToCC;
  kfac=1.; /// ??????
  Insert("tHToCC-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],4.88e+7*BR*kfac));
  Insert("tHToCC-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],6.40e+6*BR*kfac));
  kfac=1.5; //Lorenzo approval
  Insert("tHToCC-kappa-hut",new SignalSingleTop(couplings["kappa_hut"],74.12*BR*kfac));
  kfac=1.8; //Lorenzo approval
  Insert("tHToCC-kappa-hct",new SignalSingleTop(couplings["kappa_hct"],9.179*BR*kfac));

  // table with processes: p p > t Z (trileptonic)
  BR = BR_TToWB * BR_WToLNu;
  kfac=1.; /// ??????
  Insert("tZ-kappa-aut",new SignalSingleTop(couplings["kappa_aut"],2.260e5*BR*kfac));
  Insert("tZ-kappa-act",new SignalSingleTop(couplings["kappa_act"],2.654e4*BR*kfac));
  Insert("tZ-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.272e7*BR*kfac));
  Insert("tZ-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],3.021e6*BR*kfac));
  kfac=1.4; /// Lorenzo approval
  Insert("tZ-kappa-zut",new SignalSingleTop(couplings["kappa_zut"],1.728e6*BR*kfac));
  Insert("tZ-kappa-zct",new SignalSingleTop(couplings["kappa_zct"],2.040e5*BR*kfac));
  Insert("tZ-zeta-zut", new SignalSingleTop(couplings["zeta_zut"], 7.484*BR*kfac));
  Insert("tZ-zeta-zct", new SignalSingleTop(couplings["zeta_zct"], 1.038*BR*kfac));

  // table with processes: p p > t Z (dileptonic)
  BR = BR_TToWB * (1-BR_WToLNu);
  kfac=1.; /// ??????
  Insert("tZ-dilep-kappa-aut",new SignalSingleTop(couplings["kappa_aut"],2.260e5*BR*kfac));
  Insert("tZ-dilep-kappa-act",new SignalSingleTop(couplings["kappa_act"],2.654e4*BR*kfac));
  Insert("tZ-dilep-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.272e7*BR*kfac));
  Insert("tZ-dilep-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],3.021e6*BR*kfac));
  kfac=1.4; /// Lorenzo approval
  Insert("tZ-dilep-kappa-zut",new SignalSingleTop(couplings["kappa_zut"],1.728e6*BR*kfac));
  Insert("tZ-dilep-kappa-zct",new SignalSingleTop(couplings["kappa_zct"],2.040e5*BR*kfac));
  Insert("tZ-dilep-zeta-zut", new SignalSingleTop(couplings["zeta_zut"], 7.484*BR*kfac));
  Insert("tZ-dilep-zeta-zct", new SignalSingleTop(couplings["zeta_zct"], 1.038*BR*kfac));

  // table with processes: p p > t Z (monolep had : t>l, Z>jj)
  BRaa = (5.*3. /* 5 quark,3 colors each */ /3. /* 3 leptons */) * BR_TToWB * BR_WToLNu;
  BRzz = BR_ZToHadrons/BR_ZToLL * BR_TToWB * BR_WToLNu;
  BRgg = 0.;
  kfac=1.; /// ??????
  Insert("tZ-monolep-kappa-aut",new SignalSingleTop(couplings["kappa_aut"],2.260e5*BRaa*kfac));
  Insert("tZ-monolep-kappa-act",new SignalSingleTop(couplings["kappa_act"],2.654e4*BRaa*kfac));
  Insert("tZ-monolep-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.272e7*BRgg*kfac));
  Insert("tZ-monolep-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],3.021e6*BRgg*kfac));
  kfac=1.4; /// Lorenzo approval
  Insert("tZ-monolep-kappa-zut",new SignalSingleTop(couplings["kappa_zut"],1.728e6*BRzz*kfac));
  Insert("tZ-monolep-kappa-zct",new SignalSingleTop(couplings["kappa_zct"],2.040e5*BRzz*kfac));
  Insert("tZ-monolep-zeta-zut", new SignalSingleTop(couplings["zeta_zut"], 7.484*BRzz*kfac));
  Insert("tZ-monolep-zeta-zct", new SignalSingleTop(couplings["zeta_zct"], 1.038*BRzz*kfac));
  //synonym
  kfac=1.; /// ??????
  Insert("tZ-monolep-had-kappa-aut",new SignalSingleTop(couplings["kappa_aut"],2.260e5*BRaa*kfac));
  Insert("tZ-monolep-had-kappa-act",new SignalSingleTop(couplings["kappa_act"],2.654e4*BRaa*kfac));
  Insert("tZ-monolep-had-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.272e7*BRgg*kfac));
  Insert("tZ-monolep-had-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],3.021e6*BRgg*kfac));
  kfac=1.4; /// Lorenzo approval
  Insert("tZ-monolep-had-kappa-zut",new SignalSingleTop(couplings["kappa_zut"],1.728e6*BRzz*kfac));
  Insert("tZ-monolep-had-kappa-zct",new SignalSingleTop(couplings["kappa_zct"],2.040e5*BRzz*kfac));
  Insert("tZ-monolep-had-zeta-zut", new SignalSingleTop(couplings["zeta_zut"], 7.484*BRzz*kfac));
  Insert("tZ-monolep-had-zeta-zct", new SignalSingleTop(couplings["zeta_zct"], 1.038*BRzz*kfac));

  // table with processes: p p > t Z (monolep had : t>l, Z>met)
  BR = BR_TToWB * BR_WToLNu;
  kfac=1.; /// ??????
  Insert("tZ-monolep-met-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],5.931e7*BR*kfac));
  Insert("tZ-monolep-met-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],5.176e6*BR*kfac));
  kfac=1.4; /// Lorenzo approval
  Insert("tZ-monolep-met-kappa-zut",new SignalSingleTop(couplings["kappa_zut"],3.446e6*BR*kfac));
  Insert("tZ-monolep-met-kappa-zct",new SignalSingleTop(couplings["kappa_zct"],4.070e5*BR*kfac));
  Insert("tZ-monolep-met-zeta-zut", new SignalSingleTop(couplings["zeta_zut"], 14.92*BR*kfac));
  Insert("tZ-monolep-met-zeta-zct", new SignalSingleTop(couplings["zeta_zct"], 2.068*BR*kfac));

  // ----------------------------------------------
  // TT XSECTION TABLE
  // ----------------------------------------------

  double TTxsection = 6.741e2*pb; // ttbar xsection NLO
  double TTsemi_xsection = TTxsection * BR_TToWB * 2 * ( BR_TToWB * BR_WToLNu );
  double xsection = 0;

  // table with processes: p p > t t, t > z q
  xsection = TTsemi_xsection * BR_ZToLL;
  Insert("TTto3L-kappa-zut",new SignalTT(couplings["kappa_zut"],xsection));
  Insert("TTto3L-kappa-zct",new SignalTT(couplings["kappa_zct"],xsection));
  Insert("TTto3L-zeta-zut", new SignalTT(couplings["zeta_zut"], xsection));
  Insert("TTto3L-zeta-zct", new SignalTT(couplings["zeta_zct"], xsection));

  // table with processes: p p > t t, t > a*(->ll) q
  xsection = TTsemi_xsection * BR_ZToLL; //????
  Insert("TTto3L-kappa-aut",new SignalTT(couplings["kappa_aut"],xsection));
  Insert("TTto3L-kappa-act",new SignalTT(couplings["kappa_act"],xsection));

  // table with processes: p p > t t, t > a q
  xsection = TTsemi_xsection;
  Insert("TTtoLGamma-kappa-aut",new SignalTT(couplings["kappa_aut"],xsection));
  Insert("TTtoLGamma-kappa-act",new SignalTT(couplings["kappa_act"],xsection));

  // table with processes: p p > t t, t > h q
  xsection = TTsemi_xsection * BR_HToZZ;
  Insert("TTtoLHtoZZ-kappa-hut",new SignalTT(couplings["kappa_hut"],xsection));
  Insert("TTtoLHtoZZ-kappa-hct",new SignalTT(couplings["kappa_hct"],xsection));
  xsection = TTsemi_xsection * BR_HToWW;
  Insert("TTtoLHtoWW-kappa-hut",new SignalTT(couplings["kappa_hut"],xsection));
  Insert("TTtoLHtoWW-kappa-hct",new SignalTT(couplings["kappa_hct"],xsection));
  xsection = TTsemi_xsection * BR_HToTauTau;
  Insert("TTtoLHtoTauTau-kappa-hut",new SignalTT(couplings["kappa_hut"],xsection));
  Insert("TTtoLHtoTauTau-kappa-hct",new SignalTT(couplings["kappa_hct"],xsection));
  xsection = TTsemi_xsection * BR_HToAA;
  Insert("TTtoLHtoGammaGamma-kappa-hut",new SignalTT(couplings["kappa_hut"],xsection));
  Insert("TTtoLHtoGammaGamma-kappa-hct",new SignalTT(couplings["kappa_hct"],xsection));
  xsection = TTsemi_xsection * BR_HToBB;
  Insert("TTtoLHtoBB-kappa-hut",new SignalTT(couplings["kappa_hut"],xsection));
  Insert("TTtoLHtoBB-kappa-hct",new SignalTT(couplings["kappa_hct"],xsection));

  return true;
}


// -----------------------------------------------------------------------------
// Initialize the table of cross sections
// -----------------------------------------------------------------------------
bool XsectionSignalExtractor::DetectAndLoadProcess(std::string filename, bool verbose)
{
  // Lower case
  std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
  
  // Loop over table for finding a matching pattern
  for (std::map<std::string,const SignalBase*>::const_iterator
         it = table_.begin(); it!=table_.end(); it++)
  {
    if (StartWith(filename,it->first)) 
    {
      std::cout << "[XsectionSignalExtractor] cross section found for '" 
                << filename << "' [matching pattern = '" << it->first << "']" << std::endl;
      std::cout << "[XsectionSignalExtractor] " << it->second->PrintUnit() << std::endl;
      process_=it->second;
      return true;
    }
  }

  // Not found
  if (verbose) ERROR << "[XsectionSignalExtractor] NO cross section found for '" 
                     << filename << "'" << endmsg;
  return false;
}


// -----------------------------------------------------------------------------
// Print the table of cross sections
// -----------------------------------------------------------------------------
void XsectionSignalExtractor::PrintTable() const
{
  // loop over table to get the max length of the names
  unsigned int maxLength=0;
  for (std::map<std::string,const SignalBase*>::const_iterator
         it=table_.begin(); it!=table_.end(); it++)
  {
    if (it->first.size()>maxLength) maxLength=it->first.size();     
  }

  // print beautiful line
  std::cout << std::setfill('x') << std::setw(10) << std::endl;
  std::cout << std::setfill(' ') << std::setw(1);

  // print table line by line
  for (std::map<std::string,const SignalBase*>::const_iterator
       it=table_.begin(); it!=table_.end(); it++)
  {
    std::cout.width(maxLength);
    std::cout << std::left << it->first << "*";
    std::cout << " ";
    std::cout << it->second->PrintParameters() << std::endl;
  }

  // print beautiful line
  std::cout << std::setfill('x') << std::setw(10) << std::endl;
  std::cout << std::setfill(' ') << std::setw(1);
}


// -----------------------------------------------------------------------------
// CheckTable
// -----------------------------------------------------------------------------
bool XsectionSignalExtractor::CheckTable() const
{
  std::map<std::string,const SignalBase*>::const_iterator it1;
  std::map<std::string,const SignalBase*>::const_iterator it2;

  // empty table
  if (table_.empty())
  {
    ERROR << "[XsectionSignalExtractor] Table of cross section is empty" << endmsg;
    return false;
  }

  // Print table line by line
  bool test=true;
  for (it1=table_.begin(); it1!=table_.end(); it1++)
    for (it2=it1;it2!=table_.end();it2++)
    {
      if (it1==it2) continue;

      if (StartWith(it1->first,it2->first) || 
         StartWith(it2->first,it1->first))
      {
        test=false;
        ERROR << "[XsectionSignalExtractor] Ambiguity found in the cross sections table between '"
              << it1->first << "*' and '"
              << it2->first << "*'" << endmsg;
      }
    }

  // Return the status
  return test;
}


// -----------------------------------------------------------------------------
// Insert a cross sections in the table
// -----------------------------------------------------------------------------
bool XsectionSignalExtractor::Insert(std::string name, const SignalBase* process)
{
  // set the name in lower case --> insensitive case
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);

  // try to insert the name in the table
  status_ = table_.insert( std::pair<std::string,const SignalBase*>(name,process) );
  if (!status_.second)
  {
    ERROR << "[XsectionSignalExtractor] the entry '" << name 
          << "' is already found in the table" << endmsg;
  }

  // return the insert result
  return status_.second;
}
