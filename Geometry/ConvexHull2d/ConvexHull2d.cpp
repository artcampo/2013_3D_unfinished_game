#include  <memory>
#include  "Geometry/ConvexHull2d/ConvexHull2d.hpp"
#include	"Noise/Noise3d.hpp"
#include  "Geometry/Intersections2d.hpp"
#include  "Geometry/ConvexHull2d/ConvexHull2dHelpers.hpp"

//  Points must already form a convex hull
ConvexHull2d::ConvexHull2d ( const std::vector<D3DXVECTOR3>& aPoints ) :
  mPoints (aPoints),
  mBoundingRectangle ( new Intersections2d::BoundingRectangle (aPoints) )
{   
}


//  true <=> aPoint is contained inside the convex hull
bool  ConvexHull2d::contained ( const D3DXVECTOR3& aPoint ) const
{
  bool contained = true;
  const int N = mPoints.size();

  for ( int i = 0; i < N && contained ; ++i )      
    contained = ConvexHull2dHelpers::atLeft( mPoints[i], mPoints[ (i + 1) % N ], aPoint );

  return contained;
}

D3DXVECTOR3 ConvexHull2d::getRandomPoint( Rand::RandGen& aRandGen ) const
{
  bool inside = false;
  D3DXVECTOR3 ret;
  while (!inside )
  {
    ret.y = 0;
    ret.x = mBoundingRectangle->mLeft + aRandGen.randFloat_0_1() *
            ( mBoundingRectangle->mRight - mBoundingRectangle->mLeft);
    ret.z = mBoundingRectangle->mBottom + aRandGen.randFloat_0_1() *
            ( mBoundingRectangle->mTop - mBoundingRectangle->mBottom);
    inside = contained( ret );
  }
  return ret;
}


D3DXVECTOR3   ConvexHull2d::vertex( const int aIndex ) const
{
  return mPoints[aIndex];
}

int           ConvexHull2d::numVertices() const
{
  return mPoints.size();
}

float         ConvexHull2d::aproxArea() const
{
  return mBoundingRectangle->area();
}

D3DXVECTOR3     ConvexHull2d::center() const
{
  D3DXVECTOR3 center = D3DXVECTOR3(0,0,0);
  for ( int i = 0; i < mPoints.size() ; ++i )
    center += mPoints[i];
  return center*(1.0f/float(mPoints.size()) );
}