// SamplaAnalyzer headers
#include "XsectionBkgExtractor.h"
using namespace std;
using namespace MA5;

#define UNUSED(expr) do { (void)(expr); } while (0)

// Initializing const values
const double XsectionBkgExtractor::ab=1e6;
const double XsectionBkgExtractor::fb=1e3;
const double XsectionBkgExtractor::pb=1;
const double XsectionBkgExtractor::nb=1e-3;
const double XsectionBkgExtractor::mb=1e-6;


// -----------------------------------------------------------------------------
// Getting Xsection
// -----------------------------------------------------------------------------
bool XsectionBkgExtractor::Initialize()
{
  double BR_ZToLL   =  (3.36*3)/100.;
  double BR_ZToNuNu = 20./100.;
  double BR_ZToJJ   = 1-(BR_ZToLL+BR_ZToNuNu);
  double BR_WToLNu  = (10.80*3)/100.;
  double BR_WToJJ   = 1-BR_WToLNu;
  double BR_TToWB   = 1;

  // Higgs BR
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3 for mHiggs=125GeV
  double BR_HToBB     = 5.77e-1;
  double BR_HToTauTau = 6.32e-2;
  double BR_HToMuMu   = 2.19e-4;
  double BR_HToEE     = 0;
  double BR_HToLL     = BR_HToEE+BR_HToMuMu+BR_HToTauTau;
  double BR_HToCC     = 2.91e-2;
  double BR_HToSS     = 2.46e-2;
  double BR_HToGG     = 8.57e-2;
  double BR_HToJJ     = BR_HToCC+BR_HToSS+BR_HToGG;
  double BR_HToAA     = 2.28e-3;
  double BR_HToZA     = 1.54e-3;
  double BR_HToWW     = 2.15e-1;
  double BR_HToZZ     = 2.64e-2;

	//Best limits from 8 TeV searches on top FCNC couplings
  double BR_T2cH_Limit_tcH  = 0.0083;
  double BR_T2uH_Limit_tuH  = 0.0083;
//  double BR_T2cZ_Limit_tcZ  = 0.0005;
// double BR_T2uZ_Limit_tuZ  = 0.0005;
	
  double xsection=0;
	// Conventions:
	// - upper and lower case can be used for the name. But at the end, the names will be case-insensible.
	// - using units conventions
	// - references must be specified:
	//		-> people can cross-check
	//      -> helpful for writing paperS
	// - do not hesitate to explicite calculation if necessary (ex: BR, k-factor, ...)


  // -----------------------------------------------
  // ELECTROWEAK
  // -----------------------------------------------

  // ----------- single boson ---------------------------------------------------
  // Reference: FEWZ (10.1016/j.cpc.2011.06.008, 10.1016/j.cpc.2012.09.005)
  // All results are given at the NNLO accuracy
  // ----------------------------------------------------------------------------

  // E1 DrellYan Mll in [10;50]
  // Global cross section computed by MG LO * 1.2 = 15729.2*1.2*pb
  // * fraction computed by MG LO
  xsection=15729.2*pb;
  Insert("ZToLL10-50-0Jet",0.707*xsection);
//  Insert("ZToLL10-50-0Jet_sm-no_masses",0.707*xsection);
  Insert("ZToLL10-50-1Jet",0.188*xsection);
//  Insert("ZToLL10-50-1Jet_sm-no_masses",0.188*xsection);
  Insert("ZToLL10-50-2Jet",0.075*xsection);
//  Insert("ZToLL10-50-2Jets_sm-no_masses",0.075*xsection);
  Insert("ZToLL10-50-3Jet",0.029*xsection);
//  Insert("ZToLL10-50-3Jets_sm-no_masses",0.029*xsection);
  Insert("ZToLL10-50-4Jet",0.029*xsection);
//  Insert("ZToLL10-50-4Jets_sm-no_masses",0.029*xsection);

  // E2 DrellYan Mll >50
  // Global cross section computed by MG LO * 1.2 = 5910 pb
  // * fraction computed by MG LO
  xsection=5910*pb;
  Insert("ZToLL50-0Jet",0.707*xsection);
  //  Insert("ZToLL50-0Jet_sm-no_masses",0.707*xsection);
  Insert("ZToLL50-1Jet",0.188*xsection);
  //  Insert("ZToLL50-1Jet_sm-no_masses",0.188*xsection);
  Insert("ZToLL50-2Jet",0.075*xsection);
  //  Insert("ZToLL50-2Jets_sm-no_masses",0.075*xsection);
  Insert("ZToLL50-3Jet",0.029*xsection);
  //  Insert("ZToLL50-3Jets_sm-no_masses",0.029*xsection);
  Insert("ZToLL50-4Jet",0.029*xsection);
  //  Insert("ZToLL50-4Jets_sm-no_masses",0.029*xsection);

  // E3 Z to nunu 
  // reference = MG@NLO NLO p p -> Z (no gamma*)
  // * fraction computed by MG LO
  xsection=5.410e4*pb*BR_ZToNuNu;
  Insert("ZToNuNu-0Jet",0.707*xsection);
  Insert("ZToNuNu-1Jet",0.189*xsection);
  Insert("ZToNuNu-2Jet",0.076*xsection);
  Insert("ZToNuNu-3Jet",0.029*xsection);

  // E4 W to l nu
  // reference = MG@NLO NLO
  // jet-fraction computed by MG LO
  xsection=1.773e5*BR_WToLNu*pb;
  Insert("WToLNu-0Jet",0.83323*xsection);
  Insert("WToLNu-1Jet",0.12155*xsection);
  Insert("WToLNu-2Jet",0.03358*xsection);
  Insert("WToLNu-3Jet",0.0861*xsection);
  Insert("WToLNu-4Jet",0.0303*xsection);

  // ----------- diboson --------------------------------------------------------
  // Reference: aMC@NLO (arXiv:1405.0301)
  // Cross-checks with MCFM (10.1103/PhysRevD.60.113006, 10.1007/JHEP07(2011)018)
  // All results are given at the NLO accuracy
  // ----------------------------------------------------------------------------

  // E5: WW to (l nu) (lnu)
  // All three flavors of leptons are considered
  // rk: 4FNS = no b-quark in the initial state -> removal of the top resonant diagrams
  Insert("WWToLLNuNu",1.028e2*pb*BR_WToLNu*BR_WToLNu);

  // E6: WZ to (l nu) (l l)
  // All three flavors of leptons are considered
  Insert("WZToLLLNu",4.487e1*pb*BR_WToLNu*BR_ZToLL);

  // E7: WZ to (j j) (l l)
  // All three flavors of leptons are considered
  Insert("WZToLLJJ",4.487e1*pb*BR_WToLNu*BR_ZToLL);

  // E8: WZ to (l nu) (nu nu)
  // All three flavors of leptons are considered
  Insert("WZToLNuNuNu",4.487e1*pb*BR_WToLNu*BR_ZToNuNu);

  // E9: WZ to (j j) (nu nu)
  Insert("WZToNuNujj",4.487e1*pb*BR_WToJJ*BR_ZToNuNu);

  // E10: ZZ to (l l) (l l)
  // All three flavors of leptons are considered
  Insert("ZZTo4L",1.415e1*pb*BR_ZToLL*BR_ZToLL);

  // E11: ZZ to (l l) (j j)
  // All three flavors of leptons are considered
  Insert("ZZToLLJJ",1.415e1*pb*BR_ZToLL*BR_ZToJJ*2);

  // E12: ZZ to (l l) (nu nu)
  // All three flavors of leptons are considered
  Insert("ZZToLLNuNu",1.415e1*pb*BR_ZToLL*BR_ZToNuNu*2);

  // E13: ZZ to (j j) (nu nu)
  Insert("ZZToJJNuNu",1.415e1*pb*BR_ZToJJ*BR_ZToNuNu*2);

  // E14: ZZ to (nu nu) (nu nu)
  Insert("ZZTo4Nu",1.415e1*pb*BR_ZToNuNu*BR_ZToNuNu);

  // E15: WW to (l nu) (j j)
  // All three flavors of leptons are considered
  // rk: 4FNS = no b-quark in the initial state -> removal of the top resonant diagrams
  Insert("WWToLNuJJ",1.028e2*pb*BR_WToLNu*BR_WToJJ*2);

  // E16: WZ to (l nu) (j j)
  // All three flavors of leptons are considered
  Insert("WZToLNuJJ",4.487e1*pb*BR_WToLNu*BR_ZToJJ);

  // E17: AA+jets
  // Total LO cross section with MG_aMC@NLO : 72.91 pb
  // K-factor calculated with MG_aMC@NLO : 214.8/77.1 = 2.79
  xsection=2.031e2*pb;
  Insert("AA-0Jet",0.55534*xsection);
  Insert("AA-1Jet",0.25410*xsection);
  Insert("AA-2Jet",0.12885*xsection);
  Insert("AA-3Jet",0.06170*xsection);

  // ----------- triboson (E17) --------------------------------------------------
  // Reference: aMC@NLO (arXiv:1405.0301)
  // Cross-checks with MCFM (10.1103/PhysRevD.60.113006, 10.1007/JHEP07(2011)018)
  // All results are given at the NLO accuracy
  // Inclusive : no BR
  // ---------------------------------------------------------------------------
  Insert("triboson",(2.109e-1+1.679e-1+5.550e-2+1.417e-2)*pb);


  Insert("ZAToLL",-1*pb*BR_ZToLL);
  Insert("WAToLNu",-1*pb*BR_WToLNu);


  // -----------------------------------------------
  // TOP PROCESSES
  // -----------------------------------------------

  // ----------- ttbar ----------------------------------------------------------
  // Reference: Top++ (10.1103/PhysRevLett.110.252004)
  // All results are given at the NNLO+NNLL accuracy
  // ----------------------------------------------------------------------------

  // T1 semileptonic decay mode
  // All three flavors of leptons are considered
  Insert("TTsemilep_madspin_1",6.741e2*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)/2.);
  Insert("TTsemilep_madspin_2",6.741e2*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)/2.);

  // T2: dileptonic decay mode
  // All three flavors of leptons are considered
  Insert("TTdilep_madspin",6.741e2*pb*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu));

  // T1 A decay mode
  xsection = 1.744*pb;
  Insert("TTsemilep_A_madspin_1",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)/2.);
  Insert("TTsemilep_A_madspin_2",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)/2.);
  Insert("TTdilep_A_madspin",xsection*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu));

  // T1 AA decay mode
  xsection = 4.402e-3*pb;
  Insert("TTsemilep_AA_madspin_1",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)/2.);
  Insert("TTsemilep_AA_madspin_2",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)/2.);
  Insert("TTdilep_AA_madspin",xsection*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu));
  
  
  
  // TTbar signal: p p > t t > bW cH  (tcH couplings)
  Insert("TT_Kappa_Hct_Htobb",6.741e2*pb*(BR_TToWB*BR_T2cH_Limit_tcH)*BR_HToBB*2);
  Insert("TT_Kappa_Hct_HtoWW",6.741e2*pb*(BR_TToWB*BR_T2cH_Limit_tcH)*BR_HToWW*2);
  Insert("TT_Kappa_Hct_HtoZZ",6.741e2*pb*(BR_TToWB*BR_T2cH_Limit_tcH)*BR_HToZZ*2);
  Insert("TT_Kappa_Hct_Htoaa",6.741e2*pb*(BR_TToWB*BR_T2cH_Limit_tcH)*BR_HToAA*2);
  Insert("TT_Kappa_Hct_Htotautau",6.741e2*pb*(BR_TToWB*BR_T2cH_Limit_tcH)*BR_HToTauTau*2);
  
  
  // TTbar signal: p p > t t > bW uH  (tuH couplings)
  Insert("TT_Kappa_Hut_Htobb",6.741e2*pb*(BR_TToWB*BR_T2uH_Limit_tuH)*BR_HToBB*2);
  Insert("TT_Kappa_Hut_HtoWW",6.741e2*pb*(BR_TToWB*BR_T2uH_Limit_tuH)*BR_HToWW*2);
  Insert("TT_Kappa_Hut_HtoZZ",6.741e2*pb*(BR_TToWB*BR_T2uH_Limit_tuH)*BR_HToZZ*2);
  Insert("TT_Kappa_Hut_Htoaa",6.741e2*pb*(BR_TToWB*BR_T2uH_Limit_tuH)*BR_HToAA*2);
  Insert("TT_Kappa_Hut_Htotautau",6.741e2*pb*(BR_TToWB*BR_T2uH_Limit_tuH)*BR_HToTauTau*2);
  
  
  // TTbar signal: p p > t t > bW cZ  (tcZ couplings)

  
  // TTbar signal: p p > t t > bW uZ  (tuZ couplings)


  // ----------- ttbar+V --------------------------------------------------------
  // Reference: aMC@NLO (arXiv:1405.0301)
  // Cross-checks with MCFM (10.1007/JHEP07(2012)052)
  // All results are given at the NLO accuracy
  // ----------------------------------------------------------------------------

  // T3: ttbar Z to (semileptonic mode) (l l)
  // All three flavors of leptons are considered
  Insert("TTsemilep_ZToLL_madspin_1",7.598e-1*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_ZToLL)/2.);
  Insert("TTsemilep_ZToLL_madspin_2",7.598e-1*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_ZToLL)/2.);

  // T4: ttbar Z to (dileptonic mode) (l l)
  // All three flavors of leptons are considered
  Insert("TTdilep_ZToLL",7.598e-1*pb*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu)*(BR_ZToLL));

  // T5: ttbar W to (semileptonic mode) (l nu)
  // All three flavors of leptons are considered
  Insert("TTsemilep_WToLNu_madspin_1",5.662e-1*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_WToLNu)/2.);
  Insert("TTsemilep_WToLNu_madspin_2",5.662e-1*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_WToLNu)/2.);

  // T7: ttbar W to (dileptonic mode) (l nu)
  // All three flavors of leptons are considered
  Insert("TTdilep_WToLNu",5.662e-1*pb*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu)*(BR_WToLNu));

  // ----------- ttbar+V V ------------------------------------------------------
  // Reference: aMC@NLO (arXiv:1405.0301)
  // All results are given at the NLO accuracy
  // ----------------------------------------------------------------------------

  // T8: ttbar W W to (semileptonic mode) (l nu) (l nu)
  // All three flavors of leptons are considered
  // rk: 4FNS = no b-quark in the initial state -> removal of the top resonant diagrams
  Insert("TTsemilep_WWToLLNuNu_madspin_1",9.904e-3*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_WToLNu*BR_WToLNu)/2.);
  Insert("TTsemilep_WWToLLNuNu_madspin_2",9.904e-3*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_WToLNu*BR_WToLNu)/2.);

  // T9: ttbar W W to (dileptonic mode) (l nu) (l nu)
  // All three flavors of leptons are considered
  // rk: 4FNS = no b-quark in the initial state -> removal of the top resonant diagrams
  Insert("TTdilep_WWToLLNuNu_madspin",9.904e-3*pb*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu)*(BR_WToLNu*BR_WToLNu));

  // T10: ttbar Z Z to (semileptonic mode) (l l) (l l)
  // All three flavors of leptons are considered
  Insert("TTsemilep_ZZToLLLL_madspin_1",1.840e-3*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_ZToLL*BR_ZToLL)/2.);
  Insert("TTsemilep_ZZToLLLL_madspin_2",1.840e-3*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_ZToLL*BR_ZToLL)/2.);

  // T11: ttbar Z Z to (dileptonic mode) (l l) (l l)
  // All three flavors of leptons are considered
  Insert("TTdilep_ZZToLLLL_madspin",1.840e-3*pb*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu)*(BR_ZToLL*BR_ZToLL));

  // T19: ttbar W Z to (semileptonic mode) (l nu) (l l)
  // All three flavors of leptons are considered
  Insert("TTsemilep_WZToLLLNu_madspin_1",3.525e-3*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_WToLNu*BR_ZToLL)/2.);
  Insert("TTsemilep_WZToLLLNu_madspin_2",3.525e-3*pb*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu*2)*(BR_WToLNu*BR_ZToLL)/2.);

  // T20: ttbar W Z to (dileptonic mode) (l nu) (l l)
  // All three flavors of leptons are considered
  Insert("TTdilep_WZToLLLNu",3.525e-3*pb*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu)*(BR_WToLNu*BR_ZToLL));

  // ----------- single top -----------------------------------------------------
  // BENJAMIN OLD REFERENCES:
  // Reference: aMC@NLO (arXiv:1405.0301)
  // Cross-checks with MCFM for the t-channel:  10.1103/PhysRevLett.102.182003
  // Cross-checks with MCFM for the s-channel:  10.1103/PhysRevD.70.094012   // Computation in the 4FNS
  // Cross-checks with MCFM for the tW-channel: 10.1016/j.nuclphysb.2005.08.015
  // Cross-checks with MCFM for the tZj-channel: 10.1103/PhysRevD.87.114006
  // All results are given at the NLO accuracy
  //
  // ERIC's REFERENCE: arXiv 1409.3654
  // ----------------------------------------------------------------------------

  // T17: s-channel
  xsection = 6.25 + 3.97; // top + antitop
  Insert("SingleTop_s_madspin",xsection*pb);

  // T16: t-channel
  xsection = 137 + 82.1; // top + antitop
  Insert("SingleTop_t_madspin",xsection*pb);

  // T18: tW-channel
  xsection = 19.3 + 29.2; // top + antitop
  Insert("SingleTop_W_madspin",xsection*pb);

  // T21 TZq -> trileptonic signature
  xsection =   6.993e-1*pb  // pp->tZj
             + 1.072e-2*pb; // pp->tZb
  Insert("TZq_madspin_dilep",xsection*BR_TToWB*BR_WToJJ*BR_ZToLL);
  Insert("TZq_madspin_monolep_met",xsection*BR_TToWB*BR_WToLNu*BR_ZToNuNu);
  Insert("TZq_madspin_trilep",xsection*BR_TToWB*BR_WToLNu*BR_ZToLL);

  // TZq W 
  // NLO not computed ! 
  // BR coming from direct computing with MG LO * arbitrary scale factor = 1.2
  xsection = 0.01135 * pb * 1.2;
  Insert("TZq2_W_trilep1",xsection*(BR_TToWB*BR_WToLNu)*(BR_WToJJ)*BR_ZToLL);
  Insert("TZq2_W_trilep2",xsection*(BR_TToWB*BR_WToJJ)*(BR_WToLNu)*BR_ZToLL);

  xsection = 1.539e-2*pb;
  Insert("TZq2_s_trilep",xsection*(BR_TToWB*BR_WToLNu)*BR_ZToLL);

  xsection = 5.657e-1*pb;
  Insert("TZq2_t4FS_trilep",xsection*(BR_TToWB*BR_WToLNu)*BR_ZToLL);

  xsection = 6.993e-1*pb;
  Insert("TZq2_t5FS_trilep",xsection*(BR_TToWB*BR_WToLNu)*BR_ZToLL);

  // Special TAq
  xsection = -1*pb;
  Insert("TAq",xsection*BR_TToWB*BR_WToLNu);

  // -----------------------------------------------
  // -----------------------------------------------
  // HIGGS
  // -----------------------------------------------
  // -----------------------------------------------


  // For avoiding warnings "variable not used"
  // this function does nothing
  UNUSED(BR_HToLL);
  UNUSED(BR_HToJJ);
  UNUSED(BR_HToZA);

  // ----------- single Higgs ---------------------------------------------------
  // Reference: aMC@NLO (arXiv:1405.0301)
  // Cross-checks with LHC Higgs Xsection working group (arXiv:1307.1347)
  // All results are given at the NLO accuracy
  // ----------------------------------------------------------------------------

  xsection = 3.261e1*pb; // p p -> h

  // H1-1
  Insert("HToAA",xsection*BR_HToAA);

  // H1-2
  Insert("HToBB",xsection*BR_HToBB);

  // H1-3 (inclusive in the W decays)
  Insert("HToWW",xsection*BR_HToWW);

  // H1-4 (inclusive in the Z decays)
  Insert("HToZZ",xsection*BR_HToZZ);

  // H1-5
  Insert("HToGG",xsection*BR_HToGG);

  // H1-6
  Insert("HToTauTau",xsection*BR_HToTauTau);

  // ----------- W + Higgs ------------------------------------------------------
  // Reference: aMC@NLO (arXiv:1405.0301)
  // Cross-checks with LHC Higgs Xsection working group (arXiv:1307.1347)
  // All results are given at the NLO accuracy
  // ----------------------------------------------------------------------------

  xsection = 1.419*pb; // p p -> h w

  // H2-4
  Insert("WHToAA",xsection*BR_HToAA);

  // H2-1
  Insert("WHToBB",xsection*BR_HToBB);

  // H2-2
  Insert("WHToWW",xsection*BR_HToWW);

  // H2-3
  Insert("WHToZZ",xsection*BR_HToZZ);

  // ----------- Z + Higgs ------------------------------------------------------
  // Reference: aMC@NLO (arXiv:1405.0301)
  // Cross-checks with LHC Higgs Xsection working group (arXiv:1307.1347)
  // All results are given at the NLO accuracy
  // ----------------------------------------------------------------------------

  xsection = 7.674e-1*pb; // p p -> h z

  // H3-4
  Insert("ZHToAA",xsection*BR_HToAA);

  // H3-1
  Insert("ZHToBB",xsection*BR_HToBB);

  // H3-2
  Insert("ZHToWW",xsection*BR_HToWW);

  // H3-3
  Insert("ZHToZZ",xsection*BR_HToZZ);

  // ----------- ttbar + Higgs --------------------------------------------------
  // Reference: aMC@NLO (arXiv:1405.0301)
  // Cross-checks with LHC Higgs Xsection working group (arXiv:1307.1347)
  // All results are given at the NLO accuracy
  // ----------------------------------------------------------------------------

  xsection = 4.608e-1*pb; // p p -> t t h

  // H4-1
  Insert("TTHToTTAA",xsection*BR_HToAA);
  Insert("TTsemilep_HToAA_madspin1",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu)*BR_HToAA);
  Insert("TTsemilep_HToAA_madspin2",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu)*BR_HToAA);
  Insert("TTdilep_HToAA_madspin",xsection*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu)*BR_HToAA);

  // H4-2
  Insert("TTHToTTBB",xsection*BR_HToBB);
  Insert("TTsemilep_HToBB_madspin1",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu)*BR_HToBB);
  Insert("TTsemilep_HToBB_madspin2",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu)*BR_HToBB);
  Insert("TTdilep_HToBB_madspin",xsection*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu)*BR_HToBB);

  // H4-3
  Insert("TTHToTTWW",xsection*BR_HToWW);
  Insert("TTsemilep_HToWW_madspin1",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu)*BR_HToWW);
  Insert("TTsemilep_HToWW_madspin2",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu)*BR_HToWW);
  Insert("TTdilep_HToWW_madspin",xsection*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu)*BR_HToWW);

  // H4-4
  Insert("TTHToTTZZ",xsection*BR_HToZZ);
  Insert("TTsemilep_HToZZ_madspin1",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu)*BR_HToZZ);
  Insert("TTsemilep_HToZZ_madspin2",xsection*(BR_TToWB*BR_TToWB)*(BR_WToJJ*BR_WToLNu)*BR_HToZZ);
  Insert("TTdilep_HToZZ_madspin",xsection*(BR_TToWB*BR_TToWB)*(BR_WToLNu*BR_WToLNu)*BR_HToZZ);

  // H4-5
  Insert("TTHToTTTauTau",xsection*BR_HToTauTau);

  return true;
}
