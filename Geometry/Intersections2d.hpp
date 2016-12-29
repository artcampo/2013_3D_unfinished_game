#ifndef INTERSECTIONS_2D_HPP
#define INTERSECTIONS_2D_HPP

#include  <vector>
#include  "dx_misc.hpp"
#include  "Geometry/DxMathHelpers.hpp"

namespace Intersections2d{

class BoundingRectangle;

/////////////////////////////////////////////////////
//  2d algorithms
float signedArea( D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c);
bool  third_is_in_between (D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c);
bool  segmentIntersectionExclusive ( D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r, D3DXVECTOR2 s);
D3DXVECTOR2 pointOfIntersectingSegments ( D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3, D3DXVECTOR2 p4 );

/////////////////////////////////////////////////////
//  2d data structures
class BoundingRectangle
{
public:
  BoundingRectangle ( float l, float r, float t, float b ):
    mLeft ( l ), mRight ( r ), mTop ( t ),mBottom ( b ) {};

  BoundingRectangle ( const std::vector<D3DXVECTOR3>& aPoints )
  {
    mLeft = aPoints[0].x;
    mRight = aPoints[0].x;
    mTop = aPoints[0].z;
    mBottom = aPoints[0].z;
    for ( int i = 1; i < aPoints.size() ; ++i )
    {
      if ( aPoints[i].x < mLeft ) mLeft = aPoints[i].x;
      if ( aPoints[i].x > mRight ) mRight = aPoints[i].x;
      if ( aPoints[i].z < mBottom ) mBottom = aPoints[i].z;
      if ( aPoints[i].z > mTop ) mTop = aPoints[i].z;
    }
  }

  bool inside ( D3DXVECTOR2 p ) 
  {
    return ( p.x >= mLeft && p.x <= mRight && p.y <= mTop && p.y >= mBottom );
  }

  float area() const
  {
    return (mTop - mBottom)*(mRight - mLeft);
  }

  float mLeft;
  float mRight;
  float mTop;
  float mBottom;
};

};

#endif // INTERSECTIONS_2D_HPP
