#ifndef XsectionBkgExtractor_h
#define XsectionBkgExtractor_h

// SampleAnalyzer header
#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

// STL header
#include <iostream>
#include <string>
#include <vector>
#include <map>


namespace MA5
{
class XsectionBkgExtractor
{
  // -------------------------------------------------------------
  //                        data members
  // -------------------------------------------------------------
 private:

  // table of cross section
  std::map<std::string, double> table_;

  // variable used for checking if a given entry has been inserted
  std::pair<std::map<std::string,double>::iterator,bool> status_;

  // units
  static const double ab;
  static const double fb;
  static const double pb;
  static const double nb;
  static const double mb;

  // current xsection
  const double* xsection_; // pb

  // -------------------------------------------------------------
  //                    public method members
  // -------------------------------------------------------------
 public:

  // Constructor without argument
  XsectionBkgExtractor() { xsection_=0; }

  // Destructor
  ~XsectionBkgExtractor() {}

  // -----------------------------------------------------------------------------
  // Initialize the table of cross sections
  // -----------------------------------------------------------------------------
  bool Initialize();

  // -----------------------------------------------------------------------------
  // Initialize the table of cross sections
  // -----------------------------------------------------------------------------
  bool DetectAndLoadProcess(std::string filename, bool verbose=false);

  // -----------------------------------------------------------------------------
  // Get cross section value [unit = pb]
  // -----------------------------------------------------------------------------
  double GetXsection_pb() const
  {
    if (xsection_==0) return -1; 
    else if (*xsection_<0) return -1;
    else return *xsection_*pb;
  }

  // -----------------------------------------------------------------------------
  // Get cross section value [unit = fb]
  // -----------------------------------------------------------------------------
  double GetXsection_fb() const
  {
    if (xsection_==0) return -1; 
    else if (*xsection_<0) return -1;
    else return *xsection_*fb;
  }

  // -----------------------------------------------------------------------------
  // Get cross section value [unit = pb]
  // -----------------------------------------------------------------------------
  double GetXsection() const
  { return GetXsection_pb(); }

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
  bool Insert(std::string name, double value);


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
}

#endif
