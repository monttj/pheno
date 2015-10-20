#include "FCNCcoupling.h"
using namespace std;
using namespace MA5;

// Phys Rev Lett. 2013 Jan 25;110(4):042001. Epub 2013 Jan 24.
// arxiv.org/pdf/1210.2808v3.pdf
// table I

// mtop = 172.5
const double FCNCcoupling::topWidth_ = 1.4806*(1+(-0.26-1.49+1.68-8.58-2.09)/100.);

// mtop = 173.5
//double SignalSingleTop::topWidth_ = 1.5109*(1+(-0.26-1.49+1.69-8.58-2.09)/100.);

// mtop = 174.5
//double SignalSingleTop::topWidth_ = 1.5115*(1+(-0.25-1.48+1.69-8.58-2.09)/100.);
