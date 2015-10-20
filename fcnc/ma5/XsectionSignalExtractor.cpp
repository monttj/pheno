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
  // BR values, Mh = 125.0 GeV
  double BR_Z2LL  =  (3.36*3)/100.;
  double BR_W2LNu = (10.80*3)/100.;
  double BR_Z2hadrons = 0.6991;
  double BR_Z2invisible = 0.2;
  double BR_H2AA = 2.28e-3;
  double BR_H2WW =2.15e-1;
  double BR_H2ZZ =2.64e-02;
  double BR_T2WB = 1.;
  double BR = 0;
  double BRaa = 0;
  double BRzz = 0;
  double BRgg = 0;

  // Avoiding warning during compilations
  UNUSED(BR_Z2LL);
  UNUSED(BR_Z2hadrons);
  UNUSED(BR_Z2invisible);

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
  couplings["kappa_gut"]=FCNCcoupling(3.665e5,0.0355/100.,true);
  couplings["kappa_aut"]=FCNCcoupling(1.989e4,0.016/100.,true);
  couplings["kappa_hut"]=FCNCcoupling(0.1836, 0.83/100.,false);
  couplings["kappa_zut"]=FCNCcoupling(1.637e4,0.05/100.,true);
  couplings["zeta_zut"] =FCNCcoupling(0.1685, 0.05/100.,false);

  // - charm quark
  couplings["kappa_gct"]=FCNCcoupling(3.665e5,0.0344/100.,true);
  couplings["kappa_act"]=FCNCcoupling(1.989e4,0.182/100.,true);
  couplings["kappa_hct"]=FCNCcoupling(0.1836, 0.83/100.,false);
  couplings["kappa_zct"]=FCNCcoupling(1.637e4,0.05/100.,true);
  couplings["zeta_zct"] =FCNCcoupling(0.1685, 0.05/100.,false);

  // ----------------------------------------------
  // SINGLE-TOP XSECTION TABLE
  // ----------------------------------------------

  // table with processes: p p > t
  Insert("t-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.979e10));
  Insert("t-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],8.666e9));

  // table with processes: p p > t a
  Insert("tgamma-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],6.781e8));
  Insert("tgamma-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],1.349e8));
  Insert("tgamma-kappa-aut",new SignalSingleTop(couplings["kappa_aut"],1.46e7));
  Insert("tgamma-kappa-act",new SignalSingleTop(couplings["kappa_act"],1.884e6));

  // table with processes: p p > t H, H > a a
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  BR = BR_H2AA;
  Insert("tHToGammaGamma-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],4.884e7*BR));
  Insert("tHToGammaGamma-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],6.404e6*BR));
  Insert("tHToGammaGamma-kappa-hut",new SignalSingleTop(couplings["kappa_hut"],74.12*BR));
  Insert("tHToGammaGamma-kappa-hct",new SignalSingleTop(couplings["kappa_hct"],9.179*BR));

  // table with processes: p p > t H, H > W W
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  BR = BR_H2WW;
  Insert("tHToWW-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],4.884e7*BR));
  Insert("tHToWW-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],6.404e6*BR));
  Insert("tHToWW-kappa-hut",new SignalSingleTop(couplings["kappa_hut"],74.12*BR));
  Insert("tHToWW-kappa-hct",new SignalSingleTop(couplings["kappa_hct"],9.179*BR));

  // table with processes: p p > t H, H > Z Z 
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  BR = BR_H2ZZ;
  Insert("tHToZZ-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],4.88e+7*BR));
  Insert("tHToZZ-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],6.40e+6*BR));
  Insert("tHToZZ-kappa-hut",new SignalSingleTop(couplings["kappa_hut"],74.12*BR));
  Insert("tHToZZ-kappa-hct",new SignalSingleTop(couplings["kappa_hct"],9.179*BR));

  // table with processes: p p > t Z (trileptonic)
  BR = BR_T2WB * BR_W2LNu;
  Insert("tZ-kappa-aut",new SignalSingleTop(couplings["kappa_aut"],2.260e5*BR));
  Insert("tZ-kappa-act",new SignalSingleTop(couplings["kappa_act"],2.654e4*BR));
  Insert("tZ-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.272e7*BR));
  Insert("tZ-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],3.021e6*BR));
  Insert("tZ-kappa-zut",new SignalSingleTop(couplings["kappa_zut"],1.728e6*BR));
  Insert("tZ-kappa-zct",new SignalSingleTop(couplings["kappa_zct"],2.040e5*BR));
  Insert("tZ-zeta-zut", new SignalSingleTop(couplings["zeta_zut"], 7.484*BR));
  Insert("tZ-zeta-zct", new SignalSingleTop(couplings["zeta_zct"], 1.038*BR));

  // table with processes: p p > t Z (dileptonic)
  BR = BR_T2WB * (1-BR_W2LNu);
  Insert("tZ-dilep-kappa-aut",new SignalSingleTop(couplings["kappa_aut"],2.260e5*BR));
  Insert("tZ-dilep-kappa-act",new SignalSingleTop(couplings["kappa_act"],2.654e4*BR));
  Insert("tZ-dilep-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.272e7*BR));
  Insert("tZ-dilep-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],3.021e6*BR));
  Insert("tZ-dilep-kappa-zut",new SignalSingleTop(couplings["kappa_zut"],1.728e6*BR));
  Insert("tZ-dilep-kappa-zct",new SignalSingleTop(couplings["kappa_zct"],2.040e5*BR));
  Insert("tZ-dilep-zeta-zut", new SignalSingleTop(couplings["zeta_zut"], 7.484*BR));
  Insert("tZ-dilep-zeta-zct", new SignalSingleTop(couplings["zeta_zct"], 1.038*BR));

  // table with processes: p p > t Z (monolep had : t>l, Z>jj)
  BRaa = (5.*3. /* 5 quark,3 colors each */ /3. /* 3 leptons */) * BR_T2WB * BR_W2LNu;
  BRzz = BR_Z2hadrons/BR_Z2LL * BR_T2WB * BR_W2LNu;
  BRgg = 0.;
  Insert("tZ-monolep-kappa-aut",new SignalSingleTop(couplings["kappa_aut"],2.260e5*BRaa));
  Insert("tZ-monolep-kappa-act",new SignalSingleTop(couplings["kappa_act"],2.654e4*BRaa));
  Insert("tZ-monolep-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],3.272e7*BRgg));
  Insert("tZ-monolep-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],3.021e6*BRgg));
  Insert("tZ-monolep-kappa-zut",new SignalSingleTop(couplings["kappa_zut"],1.728e6*BRzz));
  Insert("tZ-monolep-kappa-zct",new SignalSingleTop(couplings["kappa_zct"],2.040e5*BRzz));
  Insert("tZ-monolep-zeta-zut", new SignalSingleTop(couplings["zeta_zut"], 7.484*BRzz));
  Insert("tZ-monolep-zeta-zct", new SignalSingleTop(couplings["zeta_zct"], 1.038*BRzz));

  // table with processes: p p > t Z (monolep had : t>l, Z>met)
  BR = BR_T2WB * BR_W2LNu;
  Insert("tZ-monolep-met-kappa-gut",new SignalSingleTop(couplings["kappa_gut"],5.931e7*BR));
  Insert("tZ-monolep-met-kappa-gct",new SignalSingleTop(couplings["kappa_gct"],5.176e6*BR));
  Insert("tZ-monolep-met-kappa-zut",new SignalSingleTop(couplings["kappa_zut"],3.446e6*BR));
  Insert("tZ-monolep-met-kappa-zct",new SignalSingleTop(couplings["kappa_zct"],4.070e5*BR));
  Insert("tZ-monolep-met-zeta-zut", new SignalSingleTop(couplings["zeta_zut"], 14.92*BR));
  Insert("tZ-monolep-met-zeta-zct", new SignalSingleTop(couplings["zeta_zct"], 2.068*BR));

  // ----------------------------------------------
  // TT XSECTION TABLE
  // ----------------------------------------------

  double TTxsection = 6.741e2*pb; // ttbar xsection NLO

  // table with processes: p p > t t, t > z q
  double xsection = TTxsection * BR_T2WB * 2 * ( BR_T2WB * BR_W2LNu ) * BR_Z2LL;
  Insert("TTto3L-kappa-zut",new SignalTT(couplings["kappa_zut"],xsection));
  Insert("TTto3L-kappa-zct",new SignalTT(couplings["kappa_zct"],xsection));
  Insert("TTto3L-zeta-zut", new SignalTT(couplings["zeta_zut"], xsection));
  Insert("TTto3L-zeta-zct", new SignalTT(couplings["zeta_zct"], xsection));


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
