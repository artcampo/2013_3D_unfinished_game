#ifndef _CONVEX_HULL_2D_HPP_
#define _CONVEX_HULL_2D_HPP_

#include  <vector>
#include	"dx_misc.hpp"

#include  "Geometry/Intersections2d.hpp"
#include	"Noise/RandomNumberGenerator.hpp"
//class BoundingRectangle;

/*
  Points are specified in CCW.
  D3DXVECTOR3 is used for points, and only (xz) is considered.
*/
class ConvexHull2d
{
public:
  ConvexHull2d ( const std::vector<D3DXVECTOR3>& aPoints );
  ~ConvexHull2d(){ delete mBoundingRectangle; };


  bool          contained ( const D3DXVECTOR3& aPoint ) const;
  D3DXVECTOR3   getRandomPoint( Rand::RandGen& aRandGen ) const;

  D3DXVECTOR3   vertex( const int aIndex ) const;
  int           numVertices() const;

  float         aproxArea() const;
  D3DXVECTOR3   center() const;
private:
  std::vector<D3DXVECTOR3>  mPoints;
  Intersections2d::BoundingRectangle*        mBoundingRectangle;
  
  
  //  debug
  friend class WorldTerrain;

};



#endif  //_CONVEX_HULL_2D_HPP_