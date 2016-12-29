#ifndef INTERSECTIONS_HPP
#define INTERSECTIONS_HPP

#include  "dx_misc.hpp"
#include  "Geometry/DxMathHelpers.hpp"

namespace Intersections{

class Plane;
class BoundingSphere;
class CollisionTriangle;

/////////////////////////////////////////////////////
//  3d algorithms

float distance ( D3DXVECTOR3& a, D3DXVECTOR3& b );
bool areCollinear ( D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c );


float distancePointToSegment ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXVECTOR3 aPoint );
float distancePointToLine ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXVECTOR3 aPoint );
D3DXVECTOR3 distancePointToLineVector ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXVECTOR3 aPoint );

float distanceSegmentToLine ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE,
                              D3DXVECTOR3 aSegB, D3DXVECTOR3 aSegE,
                              D3DXVECTOR3& aPointInSegment );


float distanceRayPlane ( D3DXVECTOR3 aRayB,
                         D3DXVECTOR3 aRayE,
                         Plane& aPlane,
                         bool& aUniqueSolution );
bool  distanceRaySphere( const BoundingSphere& aSphere, D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, float& aD );

D3DXVECTOR3 intersectionLinePlane ( D3DXVECTOR3 aRayB,
                                    D3DXVECTOR3 aRayE,
                                    Plane& aPlane,
                                    bool& aUniqueSolution);

bool  frontFacing ( D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c, const D3DXVECTOR3 aCamPos );
bool  raySphere( const BoundingSphere& aSphere, D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE );
float  distancePointSphere( const BoundingSphere& aSphere, const D3DXVECTOR3& p );

bool intersectionPointTriangle( const D3DXVECTOR3 aPoint, 
                        const float aEpsilon,
                        const CollisionTriangle& aTriangle);

bool pointInTriangle( const D3DXVECTOR3& aPoint, 
                      const D3DXVECTOR3& a, 
                      const D3DXVECTOR3& b, 
                      const D3DXVECTOR3& c,
                      const float aEpsilonBarycentric );


/////////////////////////////////////////////////////
//  3d Data structures
class Plane{
public:
  Plane(){};
  // TODO remove this constructor
  Plane( D3DXVECTOR3 u, D3DXVECTOR3 v, D3DXVECTOR3 p);
  D3DXVECTOR3 normal();
  D3DXVECTOR3 point();
  static Plane planeBy3Points( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c );
  float implicitD();
  //Plane static planeBy2Vectors1Point( D3DXVECTOR3 u, D3DXVECTOR3 v, D3DXVECTOR3 p );
private:
  D3DXVECTOR3 mNormal;
  D3DXVECTOR3 mPoint;
};

class BoundingSphere{
public:
  D3DXVECTOR3 center;
  float       radius;
};

class CollisionTriangle{
  public:
    D3DXVECTOR3 p1;
    D3DXVECTOR3 p2;
    D3DXVECTOR3 p3;    
};


};

#endif // INTERSECTIONS_HPP
