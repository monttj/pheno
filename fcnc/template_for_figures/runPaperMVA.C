
{

 gROOT->ProcessLine(".L PlotPaperStack.C+");
 gROOT->SetStyle("Plain");
 gStyle->SetPalette(1);
 gStyle->SetOptStat(0);
 
 
 
std::vector< TString > samplelist;
std::vector< int >     colortype;
std::vector< int >     styletype;
float scale_signal1= 0.50/0.016;
float scale_signal2= 0.20/0.0031;

samplelist.push_back( "t-1j-Kappa-gct");                       colortype.push_back(1); styletype.push_back(1);  
samplelist.push_back( "t-1j-Kappa-gut");                       colortype.push_back(1); styletype.push_back(2); 
samplelist.push_back( "SingleTop_W_madspin");                  colortype.push_back(4); styletype.push_back(1); 
samplelist.push_back( "SingleTop_s_madspin");                  colortype.push_back(4); styletype.push_back(1); 
samplelist.push_back( "SingleTop_t_4FS_madspin");              colortype.push_back(4); styletype.push_back(1); 


samplelist.push_back("TTsemilep_madspin_1");                   colortype.push_back(2); styletype.push_back(1); 
samplelist.push_back("TTsemilep_madspin_2");                   colortype.push_back(2); styletype.push_back(1); 
samplelist.push_back("TTdilep_madspin");                       colortype.push_back(2); styletype.push_back(1); 

samplelist.push_back("WToLNu-0Jet_sm-no_masses");              colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("WToLNu-1Jet_sm-no_masses");              colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("WToLNu-2Jets_sm-no_masses");             colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("WToLNu-3Jets_sm-no_masses");             colortype.push_back(8); styletype.push_back(1); 

//samplelist.push_back("ZToLL10-50-0Jet_sm-no_masses");        colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("ZToLL10-50-1Jet_sm-no_masses");          colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("ZToLL10-50-2Jets_sm-no_masses");         colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("ZToLL10-50-3Jets_sm-no_masses");         colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("ZToLL50-0Jet_sm-no_masses");             colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("ZToLL50-1Jet_sm-no_masses");             colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("ZToLL50-2Jets_sm-no_masses");            colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("ZToLL50-3Jets_sm-no_masses_split");      colortype.push_back(8); styletype.push_back(1); 
samplelist.push_back("ZToLL50-4Jets_sm-no_masses_split");      colortype.push_back(8); styletype.push_back(1); 


PlotStack("MVA", samplelist, colortype, scale_signal1, scale_signal2,  "BDT output corresponding to the gut training",0);

}
