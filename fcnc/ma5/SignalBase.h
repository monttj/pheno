#ifndef SignalBase_h
#define SignalBase_h

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
class SignalBase
{
  // -------------------------------------------------------------
  //                        data members
  // -------------------------------------------------------------
 protected:


  // -------------------------------------------------------------
  //                    public method members
  // -------------------------------------------------------------
 public:

  // Constructor without argument
  SignalBase() 
  {}

  // Destructor
  virtual ~SignalBase() {}

  // -----------------------------------------------------------------------------
  // Print Parameters
  // -----------------------------------------------------------------------------
  virtual std::string PrintParameters() const = 0;


  // -----------------------------------------------------------------------------
  // Print Units
  // -----------------------------------------------------------------------------
  virtual std::string PrintUnit() const = 0;


  // -----------------------------------------------------------------------------
  // Getting Xsection by fixing ...
  // -----------------------------------------------------------------------------
  virtual double GetXsection_fixingCoupling(double g) const = 0;
  virtual double GetXsection_fixingBR(double BR) const = 0;
  virtual double GetXsection_fixingWidth(double width) const = 0;
  virtual double GetXsection_fixingBRlimit() const = 0;

  // -----------------------------------------------------------------------------
  // Getting ... by fixing xsection
  // -----------------------------------------------------------------------------
  virtual double GetCoupling_fixingXsection(double xsection) const = 0;
  virtual double GetBR_fixingXsection(double xsection) const = 0;
  virtual double GetWidth_fixingXsection(double xsection) const = 0;


  // -----------------------------------------------------------------------------
  // Shortcuts to method related to coupling
  // -----------------------------------------------------------------------------
  virtual double GetWidth_fixingCoupling(double g) const = 0;
  virtual double GetBR_fixingCoupling(double g) const = 0;
  virtual double GetBR_fixingWidth(double width) const = 0;
  virtual double GetWidth_fixingBR(double BR) const = 0;
  virtual double GetWidth_fixingBRlimit() const = 0;
  virtual double GetCoupling_fixingWidth(double width) const = 0;
  virtual double GetCoupling_fixingBR(double BR) const = 0;


};
}

#endif
