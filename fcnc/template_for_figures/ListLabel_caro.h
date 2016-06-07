#ifndef ListLabel_h
#define ListLabel_h

// STL header
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>


namespace MA5
{
class ListLabel
{
  // -------------------------------------------------------------
  //                        data members
  // -------------------------------------------------------------
 private:

  // table of cross section
  std::map<std::string, std::string> table_;

  // variable used for checking if a given entry has been inserted
  std::pair<std::map<std::string,std::string>::iterator,bool> status_;


  // -------------------------------------------------------------
  //                    public method members
  // -------------------------------------------------------------
 public:

  // Constructor without argument
  ListLabel() { }

  // Destructor
  ~ListLabel() {}

  // -----------------------------------------------------------------------------
  // Initialize the table of cross sections
  // -----------------------------------------------------------------------------
  bool Initialize();

  // -----------------------------------------------------------------------------
  // GetName
  // -----------------------------------------------------------------------------
  std::string GetName(std::string filename) const;

  // -----------------------------------------------------------------------------
  // Print the table of cross sections
  // -----------------------------------------------------------------------------
  void PrintTable() const;

  // -----------------------------------------------------------------------------
  // Checking the table of cross sections
  // -----------------------------------------------------------------------------
  bool CheckTable() const;


  // -------------------------------------------------------------
  //                    public method members
  // -------------------------------------------------------------
 private:

  // -----------------------------------------------------------------------------
  // Insert a cross sections in the table
  // -----------------------------------------------------------------------------
  bool Insert(std::string name, std::string);


  // -----------------------------------------------------------------------------
  // Is the string 'name' started with the string 'pattern'
  // -----------------------------------------------------------------------------
  bool StartWith(const std::string& name, const std::string& pattern) const
  {
    std::size_t found = name.find(pattern);
    if (found==std::string::npos) return false;
    return (found==0);
  }




};






// -----------------------------------------------------------------------------
// Print the table of cross sections
// -----------------------------------------------------------------------------
void ListLabel::PrintTable() const
{
  // loop over table to get the max length of the names
  unsigned int maxLength=0;
  for (std::map<std::string,std::string>::const_iterator
         it=table_.begin(); it!=table_.end(); it++)
  {
    if (it->first.size()>maxLength) maxLength=it->first.size();     
  }

  // print beautiful line
  std::cout << std::setfill('x') << std::setw(10) << std::endl;
  std::cout << std::setfill(' ') << std::setw(1);

  // print table line by line
  for (std::map<std::string,std::string>::const_iterator
       it=table_.begin(); it!=table_.end(); it++)
  {
    std::cout.width(maxLength);
    std::cout << std::left << it->first << "*";
    std::cout << " ";
    std::cout << it->second << std::endl;
  }

  // print beautiful line
  std::cout << std::setfill('x') << std::setw(10) << std::endl;
  std::cout << std::setfill(' ') << std::setw(1);
}


// -----------------------------------------------------------------------------
// CheckTable
// -----------------------------------------------------------------------------
bool ListLabel::CheckTable() const
{
  std::map<std::string,std::string>::const_iterator it1;
  std::map<std::string,std::string>::const_iterator it2;

  // empty table
  if (table_.empty())
  {
    std::cout << "[ListLabel] Table of labels is empty" << std::endl;
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
        std::cout << "[ListLabel] Ambiguity found in the label table between '"
              << it1->first << "*' and '"
                  << it2->first << "*'" << std::endl;
      }
    }

  // Return the status
  return test;
}


// -----------------------------------------------------------------------------
// Insert a cross sections in the table
// -----------------------------------------------------------------------------
 bool ListLabel::Insert(std::string name, std::string label)
{
  // set the name in lower case --> insensitive case
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);

  // try to insert the name in the table
  status_ = table_.insert( std::pair<std::string,std::string>(name,label) );
  if (!status_.second)
  {
    std::cout << "[ListLabel] the entry '" << name 
              << "' is already found in the table with label: " 
              << label << std::endl;
  }

  // return the insert result
  return status_.second;
}


// -----------------------------------------------------------------------------
// GetName
// -----------------------------------------------------------------------------
std::string ListLabel::GetName(std::string filename) const
{
  // Lower case
  std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
  
  // Loop over table for finding a matching pattern
  for (std::map<std::string,std::string>::const_iterator
         it = table_.begin(); it!=table_.end(); it++)
  {
    if (StartWith(filename,it->first)) 
    {
      return it->second;
    }
  }

  // Not found
  std::cout << "[XsectionBkgExtractor] NO cross section found for '" 
            << filename << "'. 'Others' will be assigned." << std::endl;
  return "others";
}

// -----------------------------------------------------------------------------
// Initialize
// -----------------------------------------------------------------------------
bool ListLabel::Initialize()
{
  std::string label;

  label = "V+jets";
  Insert("ZToLL10-50-0Jet",label);
  Insert("ZToLL10-50-1Jet",label);
  Insert("ZToLL10-50-2Jet",label);
  Insert("ZToLL10-50-3Jet",label);

  Insert("ZToLL50-0Jet",label);
  Insert("ZToLL50-1Jet",label);
  Insert("ZToLL50-2Jet",label);
  Insert("ZToLL50-3Jet",label);
  Insert("ZToLL50-4Jet",label);

  label = "V+jets";
  Insert("ZToNuNu-0Jet",label);
  Insert("ZToNuNu-1Jet",label);
  Insert("ZToNuNu-2Jet",label);
  Insert("ZToNuNu-3Jet",label);

  label = "V+jets";
  Insert("WToLNu-0Jet",label);
  Insert("WToLNu-1Jet",label);
  Insert("WToLNu-2Jet",label);
  Insert("WToLNu-3Jet",label);

  label = "VV+jets";
  Insert("WWToLLNuNu", label);
  Insert("WZToLLLNu",  label);
  Insert("WZToLLJJ",   label);
  Insert("WZToLNuNuNu",label);
  Insert("WZToNuNujj", label);
  Insert("ZZTo4L",     label);
  Insert("ZZToLLJJ",   label);
  Insert("ZZToLLNuNu", label);
  Insert("ZZToJJNuNu", label);
  Insert("ZZTo4Nu",    label);
  Insert("WWToLNuJJ",  label);
  Insert("WZToLNuJJ",  label);

  label = "VVV";
  Insert("triboson","VVV");

  label = "VA+jets";
  Insert("ZAToLL", label);
  Insert("WAToLNu",label);

  label = "TT+jets";
  Insert("TTsemilep_inclusive_madspin_1",label);
  Insert("TTsemilep_inclusive_madspin_2",label);
  Insert("TTsemilep_madspin_1",label);
  Insert("TTsemilep_madspin_2",label);
  Insert("TTdilep_madspin",              label);

  label = "TT+Gamma+jets";
  Insert("TTsemilep_A_madspin_1",label);
  Insert("TTsemilep_A_madspin_2",label);
  Insert("TTdilep_A_madspin",    label);

  label = "TT+GammaGamma+jets";
  Insert("TTsemilep_AA_madspin_1",label);
  Insert("TTsemilep_AA_madspin_2",label);
  Insert("TTdilep_AA_madspin",    label);

  label = "TT+V+jets";
  Insert("TTsemilep_ZToLL_madspin_1",label);
  Insert("TTsemilep_ZToLL_madspin_2",label);
  Insert("TTdilep_ZToLL",label);
  Insert("TTsemilep_WToLNu_madspin_1",label);
  Insert("TTsemilep_WToLNu_madspin_2",label);
  Insert("TTdilep_WToLNu",label);

  label = "TT+VV+jets";
  Insert("TTsemilep_WWToLLNuNu_madspin_1",label);
  Insert("TTsemilep_WWToLLNuNu_madspin_2",label);
  Insert("TTdilep_WWToLLNuNu_madspin",label);
  Insert("TTsemilep_ZZToLLLL_madspin_1",label);
  Insert("TTsemilep_ZZToLLLL_madspin_2",label);
  Insert("TTdilep_ZZToLLLL_madspin",label);
  Insert("TTsemilep_WZToLLLNu_madspin_1",label);
  Insert("TTsemilep_WZToLLLNu_madspin_2",label);
  Insert("TTdilep_WZToLLLNu",label);

  label = "T+jets";
  Insert("SingleTop_s_madspin",label);
  Insert("SingleTop_t_madspin",label);
  Insert("SingleTop_t_4FS_madspin",label);
  Insert("SingleTop_W_madspin",label);

  label = "TZq";
  Insert("TZqToB3LNuJ","TZq");

  label = "TAq";
  Insert("TAq","TAq");

  label="H";
  Insert("HToAA",label);
  Insert("HToBB",label);
  Insert("HToWW",label);
  Insert("HToZZ",label);
  Insert("HToGG",label);

  label="WH";
  Insert("WHToAA",label);
  Insert("WHToBB",label);
  Insert("WHToWW",label);
  Insert("WHToZZ",label);
  Insert("ZHToAA",label);
  Insert("ZHToBB",label);
  Insert("ZHToWW",label);
  Insert("ZHToZZ",label);

  label="TTH";
  Insert("TTHToTTAA",label);
  Insert("TTHToTTBB",label);
  Insert("TTHToTTWW",label);
  Insert("TTHToTTZZ",label);
  Insert("TTHToTTTauTau",label);

  label="Signal gct";
  Insert("t-Kappa-gct",label);
  Insert("t-1j-Kappa-gct",label);
  label="Signal gut";
  Insert("t-Kappa-gut",label);
  Insert("t-1j-Kappa-gut",label);
  label="original gct";
  Insert("original",label);


/*
  Insert("tGamma-Kappa-act", "Signal ta act");
  Insert("tGamma-Kappa-aut", "Signal ta aut");
  Insert("tGamma-Kappa-gct", "Signal ta gct");
  Insert("tGamma-Kappa-gut", "Signal ta gut");
  Insert("TTtoLGamma-Kappa-act", "Signal tt act");
  Insert("TTtoLGamma-Kappa-aut", "Signal tt aut");
  Insert("tZ-monolep-met-Kappa-gct", "Signal tz gct");
  Insert("tZ-monolep-met-Kappa-gut", "Signal tz gut");
  Insert("tZ-monolep-met-Kappa-zct", "Signal tz zct");
  Insert("tZ-monolep-met-Kappa-zut", "Signal tz zut");
  Insert("tZ-monolep-met-Zeta-zct", "Signal tz Zzct");
  Insert("tZ-monolep-met-Zeta-zut", "Signal tz Zzut");
  Insert("tHToZZ-Kappa-gct", "Signal tH gut");
  Insert("tHToZZ-Kappa-gut", "Signal tH gut");
  Insert("tHToZZ-Kappa-hct", "Signal tH hct");
  Insert("tHToZZ-Kappa-hut", "Signal tH hut");
  Insert("TTtoLHtoZZ-Kappa-hut", "Signal tt hut");
  Insert("TTtoLHtoZZ-Kappa-hct", "Signal tt hct");
*/

/*
  Insert("tGamma-Kappa-act", "Signal ta $K_{act}$");
  Insert("tGamma-Kappa-aut", "Signal ta $K_{aut}$");
  Insert("tGamma-Kappa-gct", "Signal ta $K_{gct}$");
  Insert("tGamma-Kappa-gut", "Signal ta $K_{gut}$");
  Insert("TTtoLGamma-Kappa-act", "Signal tt$\\rightarrow$la $K_{act}$");
  Insert("TTtoLGamma-Kappa-aut", "Signal tt$\\rightarrow$la $K_{aut}$");
  Insert("tZ-monolep-met-Kappa-gct", "Signal tZ $K_{gct}$");
  Insert("tZ-monolep-met-Kappa-gut", "Signal tZ $K_{gut}$");
  Insert("tZ-monolep-met-Kappa-zct", "Signal tZ $K_{zct}$");
  Insert("tZ-monolep-met-Kappa-zut", "Signal tZ $K_{zut}$");
  Insert("tZ-monolep-met-Zeta-zct", "Signal tZ $Z_{zct}$");
  Insert("tZ-monolep-met-Zeta-zut", "Signal tZ $Z_{zut}$");
  Insert("tHToZZ-Kappa-gct", "Signal tH $K_{gut}$");
  Insert("tHToZZ-Kappa-gut", "Signal tH $K_{gut}$");
  Insert("tHToZZ-Kappa-hct", "Signal tH $K_{hct}$");
  Insert("tHToZZ-Kappa-hut", "Signal tH $K_{hut}$");
  Insert("TTtoLHtoZZ-Kappa-hut", "Signal tt$\\rightarrow$lH $K_{hut}$");
  Insert("TTtoLHtoZZ-Kappa-hct", "Signal tt$\\rightarrow$lH $K_{hct}$");
*/

  Insert("tGamma-Kappa-act", "Signal t+a $K_{act}$");
  Insert("TTtoLGamma-Kappa-act", "Signal t+a $K_{act}$");

  Insert("tGamma-Kappa-aut", "Signal t+a $K_{aut}$");
  Insert("TTtoLGamma-Kappa-aut", "Signal t+a $K_{aut}$");

  Insert("tGamma-Kappa-gct", "Signal t+a/inv $K_{gct}$");
  Insert("tZ-monolep-met-Kappa-gct", "Signal t+a/inv $K_{gct}$");
  Insert("tHToZZ-Kappa-gct", "Signal t+a/inv $K_{gut}$");
  Insert("tGamma-Kappa-gut", "Signal t+a/inv $K_{gut}$");
  Insert("tZ-monolep-met-Kappa-gut", "Signal t+a/inv $K_{gut}$");
  Insert("tHToZZ-Kappa-gut", "Signal t+a/inv $K_{gut}$");

  Insert("tZ-monolep-met-Kappa-zct", "Signal t+inv $K_{zct}$");
  Insert("tZ-monolep-met-Kappa-zut", "Signal t+inv $K_{zut}$");

  Insert("tZ-monolep-met-Zeta-zct", "Signal t+inv $Z_{zct}$");
  Insert("tZ-monolep-met-Zeta-zut", "Signal t+inv $Z_{zut}$");

  Insert("tHToZZ-Kappa-hct", "Signal t+H $K_{hct}$");
  Insert("TTtoLHtoZZ-Kappa-hct", "Signal t+H $K_{hct}$");
  Insert("tHToZZ-Kappa-hut", "Signal t+H $K_{hut}$");
  Insert("TTtoLHtoZZ-Kappa-hut", "Signal t+H $K_{hut}$");


  return true;
}

}

#endif
