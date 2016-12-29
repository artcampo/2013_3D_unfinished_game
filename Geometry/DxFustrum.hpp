#ifndef _DxFustrum_HPP
#define _DxFustrum_HPP

#include "dx_misc.hpp"
//#include <vector>

class DxFustrum{
public:
  DxFustrum(){};
  DxFustrum( const D3DXMATRIX& aMatrix );
  bool  isInside ( const D3DXVECTOR3& aPoint, float& aDistanceToNearPlane ) const;
private:
  class Plane{
  public:
    float a, b, c, d;
    float distance ( const D3DXVECTOR3& aPoint ) const
    {
      return ( aPoint.x*a + aPoint.y*b + aPoint.z*c + d );
    };
    bool  inside( const D3DXVECTOR3& aPoint ) const
    {
      return ( distance( aPoint ) > 0.0f);
    };
  };
  Plane l, r, t, b, n, f;
};

#endif // _DxFustrum_HPP
