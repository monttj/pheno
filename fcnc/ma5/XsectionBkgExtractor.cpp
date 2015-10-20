// SamplaAnalyzer headers
#include "XsectionBkgExtractor.h"
using namespace MA5;

// STL headers
#include <algorithm>
#include <iomanip>

// -----------------------------------------------------------------------------
// DetectAndLoadProcess
// -----------------------------------------------------------------------------
bool XsectionBkgExtractor::DetectAndLoadProcess(std::string filename,
                                                bool verbose)
{
  // Lower case
  std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
  
  // Loop over table for finding a matching pattern
  for (std::map<std::string,double>::const_iterator
         it = table_.begin(); it!=table_.end(); it++)
  {
    if (StartWith(filename,it->first)) 
    {
      std::cout << "[XsectionBkgExtractor] cross section found for '" 
                << filename << "': " << it->second
                << " pb [matching pattern = '" << it->first << "']" << std::endl;
      xsection_ = &(it->second);
      return true;
    }
  }

  // Not found
  if (verbose) ERROR << "[XsectionBkgExtractor] NO cross section found for '" 
                     << filename << "'" << endmsg;
  return false;
}


// -----------------------------------------------------------------------------
// Print the table of cross sections
// -----------------------------------------------------------------------------
void XsectionBkgExtractor::PrintTable() const
{
  // loop over table to get the max length of the names
  unsigned int maxLength=0;
  for (std::map<std::string,double>::const_iterator
         it=table_.begin(); it!=table_.end(); it++)
  {
    if (it->first.size()>maxLength) maxLength=it->first.size();     
  }

  // print beautiful line
  std::cout << std::setfill('x') << std::setw(10) << std::endl;
  std::cout << std::setfill(' ') << std::setw(1);

  // print table line by line
  for (std::map<std::string,double>::const_iterator
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
bool XsectionBkgExtractor::CheckTable() const
{
  std::map<std::string,double>::const_iterator it1;
  std::map<std::string,double>::const_iterator it2;

  // empty table
  if (table_.empty())
  {
    ERROR << "Table of cross section is empty" << endmsg;
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
        ERROR << "[XsectionBkgExtractor] Ambiguity found in the cross sections table between '"
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
bool XsectionBkgExtractor::Insert(std::string name, double value)
{
  // set the name in lower case --> insensitive case
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);

  // try to insert the name in the table
  status_ = table_.insert( std::pair<std::string,double>(name,value) );
  if (!status_.second)
  {
    ERROR << "[XsectionBkgExtractor] the entry '" << name 
          << "' is already found in the table" << endmsg;
  }

  // return the insert result
  return status_.second;
}
