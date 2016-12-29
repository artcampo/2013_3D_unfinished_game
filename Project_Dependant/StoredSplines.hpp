#ifndef _STOREDSPLINES_HPP_
#define _STOREDSPLINES_HPP_

/*!
 * \file SotredSSplines.hpp
 * \brief Static data with splines arrays
 * \author Arturo
 */

#include "dx_misc.hpp"

struct StoredSplineDesc{
    float *data;          
    int   numElem;
};

extern StoredSplineDesc gShipTForceFwdSpline;

namespace StoredSplines {
  static const int gSplineSliceNumElems = 22;
};

#endif