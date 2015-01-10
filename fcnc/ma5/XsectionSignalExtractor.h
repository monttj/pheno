#ifndef XsectionSignalExtractor_h
#define XsectionSignalExtractor_h

// SampleAnalyzer header
#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"
#include "SignalSingleTop.h"
#include "SignalTT.h"

// STL header
#include <iostream>
#include <string>
#include <vector>
#include <map>


namespace MA5
{
class XsectionSignalExtractor
{
  // -------------------------------------------------------------
  //                        data members
  // -------------------------------------------------------------
 private:

  // table of cross section
  std::map<std::string, const SignalBase*> table_;

  // variable used for checking if a given entry has been inserted
  mutable std::pair<std::map<std::string,const SignalBase*>::iterator,bool> status_;

  // current process
  const SignalBase* process_;

  // units
  static const double ab;
  static const double fb;
  static const double pb;
  static const double nb;
  static const double mb;


  // -------------------------------------------------------------
  //                    public method members
  // -------------------------------------------------------------
 public:

  // Constructor without argument
  XsectionSignalExtractor() { process_=0; }

  // Destructor
  ~XsectionSignalExtractor() 
  {
    process_ = 0;
    for (std::map<std::string,const SignalBase*>::const_iterator
           it = table_.begin(); it!=table_.end(); it++)
    {
      if (it->second!=0) delete it->second;
    }
  }

  // -----------------------------------------------------------------------------
  // Initialize the table of cross sections
  // -----------------------------------------------------------------------------
  bool Initialize();

  // -----------------------------------------------------------------------------
  // Initialize the table of cross sections
  // -----------------------------------------------------------------------------
  bool DetectAndLoadProcess(std::string name, bool verbose=false);

  // -----------------------------------------------------------------------------
  // Get cross section value
  // -----------------------------------------------------------------------------
  double GetXsection_fixingCoupling(double coupling) const
  {
    if (process_==0) return -1; else return process_->GetXsection_fixingCoupling(coupling);
  }
  double GetXsection_fixingWidth(double width) const
  {
    if (process_==0) return -1; else return process_->GetXsection_fixingWidth(width);
  }
  double GetXsection_fixingBR(double BR) const
  {
    if (process_==0) return -1; else return process_->GetXsection_fixingBR(BR);
  }
  double GetXsection_fixingBRlimit() const
  {
    if (process_==0) return -1; else return process_->GetXsection_fixingBRlimit();
  }

  // -----------------------------------------------------------------------------
  // Get width value
  // -----------------------------------------------------------------------------
  double GetWidth_fixingCoupling(double coupling) const
  {
    if (process_==0) return -1; else return process_->GetWidth_fixingCoupling(coupling);
  }
  double GetWidth_fixingXsection(double xsection) const
  {
    if (process_==0) return -1; else return process_->GetWidth_fixingXsection(xsection);
  }
  double GetWitdh_fixingBR(double BR) const
  {
    if (process_==0) return -1; else return process_->GetWidth_fixingBR(BR);
  }
  double GetWitdh_fixingBRlimit() const
  {
    if (process_==0) return -1; else return process_->GetWidth_fixingBRlimit();
  }

  // -----------------------------------------------------------------------------
  // Get BR value
  // -----------------------------------------------------------------------------
  double GetBR_fixingCoupling(double coupling) const
  {
    if (process_==0) return -1; else return process_->GetBR_fixingCoupling(coupling);
  }
  double GetBR_fixingXsection(double xsection) const
  {
    if (process_==0) return -1; else return process_->GetBR_fixingXsection(xsection);
  }
  double GetBR_fixingWidth(double width) const
  {
    if (process_==0) return -1; else return process_->GetWidth_fixingBR(width);
  }

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
  bool Insert(std::string name, const SignalBase* process);


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
