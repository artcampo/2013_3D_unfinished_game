#ifndef _CONVEX_HULL_2D_HELPERS_HPP_
#define _CONVEX_HULL_2D_HELPERS_HPP_

#include  <vector>
#include  <memory>
#include	"dx_misc.hpp"
#include	"Noise/RandomNumberGenerator.hpp"

class ConvexHull2d;

namespace ConvexHull2dHelpers
{
  ConvexHull2d* randomConvexHull (  const D3DXVECTOR3& aCenter, 
                                    const float aRadius, 
                                    const int aN,
                                    Rand::RandGen& aRandGen );

  ConvexHull2d* convexHull ( const std::vector<D3DXVECTOR3>& aPoints );

  float   distance( const ConvexHull2d* aHull, const D3DXVECTOR3& aPoint );

  bool    atLeft( const D3DXVECTOR3& aBeg, const D3DXVECTOR3& aEnd, const D3DXVECTOR3& aP );
  D3DXVECTOR3 perpendicular ( const D3DXVECTOR3& aPoint );
};



#endif  //_CONVEX_HULL_2D_HELPERS_HPP_