#ifndef _HEIGHT_ZONE_HPP_
#define _HEIGHT_ZONE_HPP_


#include	"dx_misc.hpp"
#include  <memory>
class ConvexHull2d;
#include  "Geometry/ConvexHull2d/ConvexHull2d.hpp"

class HeightZone
{
public:
  HeightZone (  ConvexHull2d* aOuter, 
                const float aHeight,
                const float aFadeDistance );
  ~HeightZone();
  float height( const D3DXVECTOR3& aPoint );

private:
  ConvexHull2d* mOuterHull;
  float mHeight;
  float mFadeDistance;

  ConvexHull2d* mInnerHull;
  
  //  debug
  friend class WorldTerrain;
};



#endif  //_HEIGHT_ZONE_HPP_