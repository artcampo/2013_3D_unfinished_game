#include  <memory>
#include  "Game/WorldTerrain/HeightZone.hpp"

//#include	"Noise/Noise3d.hpp"
#include  "Geometry/ConvexHull2d/ConvexHull2d.hpp"
#include  "Geometry/ConvexHull2d/ConvexHull2dHelpers.hpp"


HeightZone::HeightZone(
                ConvexHull2d* aOuter, 
                const float aHeight,
                const float aFadeDistance ) :
    mOuterHull ( aOuter ),
    mHeight( aHeight ),
    mFadeDistance ( aFadeDistance )
{

}

HeightZone::~HeightZone()
{
  delete mOuterHull;
}

float HeightZone::height( const D3DXVECTOR3& aPoint )
{
  float height = 0;
  if( mOuterHull->contained( aPoint ) )
  {
    height = mHeight;
    float d = ConvexHull2dHelpers::distance( mOuterHull, aPoint );
    float mult = 1;
    if ( d < mFadeDistance ) mult = d / mFadeDistance;
    height *= mult;
  }
  
  return height;
}