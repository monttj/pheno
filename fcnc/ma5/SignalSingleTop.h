#ifndef SignalSingleTop_h
#define SignalSingleTop_h

// SampleAnalyzer header
#include "SignalBase.h"
#include "FCNCcoupling.h"

namespace MA5
{
class SignalSingleTop : public SignalBase
{
  // -------------------------------------------------------------
  //                        data members
  // -------------------------------------------------------------
 private:

  // xsection / coupling^2
  double xsection_normalized_;

  // information related to the coupling
  FCNCcoupling coupling_;


  // -------------------------------------------------------------
  //                    public method members
  // -------------------------------------------------------------
 public:

  // Constructor without argument
  SignalSingleTop() 
  {
    xsection_normalized_=0;
  }

  // Constructor with argument
  SignalSingleTop(const FCNCcoupling& coupling, double xsection_normalized)
  {
    coupling_            = coupling;
    xsection_normalized_ = xsection_normalized;
  }

  // Destructor
  virtual ~SignalSingleTop() {}

  // -----------------------------------------------------------------------------
  // Print Parameters
  // -----------------------------------------------------------------------------
  virtual std::string PrintParameters() const
  { 
    std::stringstream str;
    str << "xsection_normalized=" << std::setw(13) << std::left << xsection_normalized_
        << coupling_.PrintParameters();
    return str.str();
  }


  // -----------------------------------------------------------------------------
  // Print Units
  // -----------------------------------------------------------------------------
  virtual std::string PrintUnit() const
  { 
    std::stringstream str;
    str << "Reminder: xsection in pb ; " << coupling_.PrintUnit();
    return str.str();
  }


  // -----------------------------------------------------------------------------
  // Getting Xsection by fixing coupling
  // -----------------------------------------------------------------------------
  virtual double GetXsection_fixingCoupling(double g) const
  { 
    if (g<0) return -1;
    else return xsection_normalized_*g*g; 
  }

  // -----------------------------------------------------------------------------
  // Other method for computing xsection
  // -----------------------------------------------------------------------------
  virtual double GetXsection_fixingBR(double BR) const
  { return GetXsection_fixingCoupling(coupling_.GetCoupling_fixingBR(BR)); }
  virtual double GetXsection_fixingWidth(double width) const
  { return GetXsection_fixingCoupling(coupling_.GetCoupling_fixingWidth(width)); }
  virtual double GetXsection_fixingBRlimit() const
  { return GetXsection_fixingCoupling(coupling_.GetCoupling_fixingBRlimit()); }


  // -----------------------------------------------------------------------------
  // Getting Coupling by fixing xsection
  // -----------------------------------------------------------------------------
  virtual double GetCoupling_fixingXsection(double xsection) const
  { 
    if (xsection<0) return -1;
    if (xsection_normalized_<=0) return -1.;
    double g2 = xsection/xsection_normalized_;
    if (g2<0) return -1;
    return std::sqrt(g2);
  }


  // -----------------------------------------------------------------------------
  // Other method using xsection as argument
  // -----------------------------------------------------------------------------
  virtual double GetBR_fixingXsection(double xsection) const
  { return coupling_.GetBR_fixingCoupling(GetCoupling_fixingXsection(xsection));  }
  virtual double GetWidth_fixingXsection(double xsection) const
  { return coupling_.GetWidth_fixingCoupling(GetCoupling_fixingXsection(xsection)); }


  // -----------------------------------------------------------------------------
  // Shortcuts to method related to coupling
  // -----------------------------------------------------------------------------
  virtual double GetWidth_fixingCoupling(double g) const
  { return coupling_.GetWidth_fixingCoupling(g); }
  virtual double GetBR_fixingCoupling(double g) const
  { return coupling_.GetBR_fixingCoupling(g); }
  virtual double GetBR_fixingWidth(double width) const
  { return coupling_.GetBR_fixingWidth(width); }
  virtual double GetWidth_fixingBR(double BR) const
  { return coupling_.GetWidth_fixingBR(BR); }
  virtual double GetWidth_fixingBRlimit() const
  { return coupling_.GetWidth_fixingBRlimit(); }
  virtual double GetCoupling_fixingWidth(double width) const
  { return coupling_.GetCoupling_fixingWidth(width); }
  virtual double GetCoupling_fixingBR(double BR) const
  { return coupling_.GetCoupling_fixingBR(BR); }


};
}

#endif
