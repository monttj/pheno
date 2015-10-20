#ifndef FCNCcoupling_h
#define FCNCcoupling_h

// SampleAnalyzer header
#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

// STL header
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>


namespace MA5
{
struct FCNCcoupling
{
  // -------------------------------------------------------------
  //                        data members
  // -------------------------------------------------------------
 private:

  // width / g^2
  double a_;

 // current limit on BR
  double BRlimit_;

  // GeV
  bool gev_;     

  // top width NNLO
  static const double topWidth_ ;

  // -------------------------------------------------------------
  //                    public method members
  // -------------------------------------------------------------
  public:

  // Constructor without argument
  FCNCcoupling()
  {
    a_       = 0;
    BRlimit_ = 0;
    gev_     = 0;
  }

  // Constructor with argument
  FCNCcoupling(double a, double BRlimit, bool gev) 
  {
    a_       = a;
    BRlimit_ = BRlimit;
    gev_     = gev;
  }

  // Destructor
  ~FCNCcoupling() {}


  // -----------------------------------------------------------------------------
  // Print Parameters
  // -----------------------------------------------------------------------------
  std::string PrintParameters() const
  { 
    std::stringstream str;
    str << "a=" << std::setw(13) << std::left << a_ 
        << " ; dim_gev=" << gev_ 
        << " ; best limit BR=" << std::setw(13) << std::left << BRlimit_*100 << "%";
    return str.str();
  }

  // -----------------------------------------------------------------------------
  // Print Units
  // -----------------------------------------------------------------------------
  std::string PrintUnit() const
  { 
    std::stringstream str;
    str << "width in GeV ; ";
    if (gev_) str << "coupling in GeV^-1  [ 1 TeV^-1 = 1e-3 GeV^-1 ]";
    else str << "coupling HAS NO DIM";
    return str.str();
  }

  // -----------------------------------------------------------------------------
  // Getting Width by fixing coupling
  // -----------------------------------------------------------------------------
  double GetWidth_fixingCoupling(double g) const
  { return a_*g*g; }

  // -----------------------------------------------------------------------------
  // Getting BR by fixing coupling
  // -----------------------------------------------------------------------------
  double GetBR_fixingCoupling(double g) const
  { return GetWidth_fixingCoupling(g)/topWidth_; }

  // -----------------------------------------------------------------------------
  // Getting BR by fixing width
  // -----------------------------------------------------------------------------
  double GetBR_fixingWidth(double width) const
  { return width/topWidth_; }

  // -----------------------------------------------------------------------------
  // Getting Width by fixing BR
  // -----------------------------------------------------------------------------
  double GetWidth_fixingBR(double BR) const
  { return BR*topWidth_; }

  // -----------------------------------------------------------------------------
  // Getting Width by fixing BR best limit
  // -----------------------------------------------------------------------------
  double GetWidth_fixingBRlimit() const
  { return BRlimit_*topWidth_; }

  // -----------------------------------------------------------------------------
  // Getting Coupling by fixing BR best limit
  // -----------------------------------------------------------------------------
  double GetCoupling_fixingBRlimit() const
  { return GetCoupling_fixingBR(BRlimit_); }

  // -----------------------------------------------------------------------------
  // Getting coupling by fixing width
  // -----------------------------------------------------------------------------
  double GetCoupling_fixingWidth(double width) const
  { 
    if (a_==0) 
    { 
      ERROR << "[SignalSingleTop] a_=0" << endmsg;
      return -1.;
    }
    double value = width/a_;
    if (value<0)
    { 
      ERROR << "[SignalSingleTop] negative value" << endmsg;
      return -1.;
    }
    return sqrt(value);
  }


  // -----------------------------------------------------------------------------
  // Getting coupling by fixing BR
  // -----------------------------------------------------------------------------
  double GetCoupling_fixingBR(double BR) const
  {
    return GetCoupling_fixingWidth(BR*topWidth_); 
  }

  // -----------------------------------------------------------------------------
  // Getting BRlimit
  // -----------------------------------------------------------------------------
  double GetBRlimit() const
  { return BRlimit_; }


};
}

#endif
