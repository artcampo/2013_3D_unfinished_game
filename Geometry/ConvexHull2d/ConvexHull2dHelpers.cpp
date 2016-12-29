#include  "Geometry/ConvexHull2d/ConvexHull2dHelpers.hpp"
#include  "Geometry/Intersections.hpp"
#include  "Geometry/ConvexHull2d/ConvexHull2d.hpp"
#include  <list>

namespace ConvexHull2dHelpers
{

ConvexHull2d* randomConvexHull (  const D3DXVECTOR3& aCenter, 
                                  const float aRadius,
                                  const int aN, 
                                  Rand::RandGen& aRandGen )
{
  std::vector<D3DXVECTOR3> points (3);
  float angle = 2*DxMath::msPI*aRandGen.randFloat_0_1();
  points[0] = aCenter + DxMath::vectorOnXZ( aRadius, angle );
  points[1] = aCenter + DxMath::vectorOnXZ( aRadius, angle + 2*DxMath::msPI*1/3 );
  points[2] = aCenter + DxMath::vectorOnXZ( aRadius, angle + 2*DxMath::msPI*2/3 );

  for ( int i = 0 ; i < aN ; ++i )
  {
    bool newPoint = false;
    while ( !newPoint )
    {
      int   indexOfEdge = aRandGen.randInt( points.size() );

      float length      = DxMath::distance( points[indexOfEdge], points[(indexOfEdge + 1) % points.size() ] );
      D3DXVECTOR3 p     = ( points[indexOfEdge] + points[(indexOfEdge + 1) % points.size() ] ) * 0.5f;
      D3DXVECTOR3 perp  = perpendicular( DxMath::unitVector( points[indexOfEdge], points[(indexOfEdge + 1) % points.size() ] ));
      p += perp*0.5f*length*aRandGen.randFloat_0_1();
         
      bool atLeftOfPrevious =  atLeft(  points[(indexOfEdge - 1) % points.size() ],
                                        points[(indexOfEdge ) % points.size() ],
                                        p );
      bool atLeftOfNext =      atLeft(  points[(indexOfEdge + 1) % points.size() ],
                                        points[(indexOfEdge + 2) % points.size() ],
                                        p );

      newPoint = atLeftOfPrevious & atLeftOfNext;

      if ( newPoint )
        points.insert( points.begin() + indexOfEdge + 1, p );
    }
  }

  return new ConvexHull2d( points );
}

float   distance( const ConvexHull2d* aHull, const D3DXVECTOR3& aPoint )
{
  const int N = aHull->numVertices();
  float minD = 1000;
  for ( int i = 0; i < N; ++i )
  {
    D3DXVECTOR3 b = aHull->vertex( i );
    D3DXVECTOR3 e = aHull->vertex( (i + 1) % N );
    
    float d = Intersections::distancePointToSegment( b, e, aPoint );
    if ( d < minD ) minD = d;
  }

  return minD;
}

bool    atLeft( const D3DXVECTOR3& aBeg, const D3DXVECTOR3& aEnd, const D3DXVECTOR3& aP )
{
  D3DXVECTOR2 beg = D3DXVECTOR2( aBeg.x, aBeg.z );
  D3DXVECTOR2 end = D3DXVECTOR2( aEnd.x, aEnd.z );

  return ( Intersections2d::signedArea( beg, end, D3DXVECTOR2( aP.x, aP.z ) ) > 0 );
}

D3DXVECTOR3 perpendicular ( const D3DXVECTOR3& aPoint )
{
  return D3DXVECTOR3( -aPoint.z, aPoint.y, aPoint.x );
}

ConvexHull2d* convexHull ( const std::vector<D3DXVECTOR3>& aPoints )
{
  std::vector<D3DXVECTOR3> points (aPoints);
  int           minimumIndex  = 0;
  D3DXVECTOR3   minimum       = aPoints[0];
  for ( int i = 1; i < aPoints.size() ; ++i )
  {    
    if ( aPoints[i].x < minimum.x )
    {
      minimum       = aPoints[i];
      minimumIndex  = i;
    }
  }

  std::swap<D3DXVECTOR3> ( points[minimumIndex], points[0] );

  D3DXVECTOR2 u   = D3DXVECTOR2(1,0);
  int n         = 1;      //elements in the convex hull
  bool finished = false;
  while ( !finished )
  {
    int   indexMin      = 0;
    float minPolarAngle = 2*3.2;
    if ( n > 2)
    {
      D3DXVECTOR3 polar = points[0] - points[ n - 1];
      D3DXVECTOR2 v     = D3DXVECTOR2( -u.y, u.x );      
      D3DXVECTOR2 p = D3DXVECTOR2(  polar.x * u.x + polar.z*u.y,
                                    polar.x * v.x + polar.z*v.y );
      minPolarAngle  = atan2( p.y, p.x );
    }

      
    for ( int i = n; i < points.size() ; ++i )
    {
      D3DXVECTOR3 polar = points[i] - points[ n - 1];
      D3DXVECTOR2 v     = D3DXVECTOR2( -u.y, u.x );
      
      //  change coordinates
      
      D3DXVECTOR2 p = D3DXVECTOR2(  polar.x * u.x + polar.z*u.y,
                                    polar.x * v.x + polar.z*v.y );

      float polarAngle  = atan2( p.y, p.x );

      if ( polarAngle < minPolarAngle )
      {
        minPolarAngle = polarAngle;
        indexMin      = i;
      }
    }

    if ( indexMin == 0 )
      finished = true;
    else
    {
      std::swap<D3DXVECTOR3> ( points[n], points[indexMin] );
      u   = D3DXVECTOR2(  points[n].x - points[n - 1].x,
                          points[n].z - points[n - 1].z);
      D3DXVec2Normalize( &u, &u );

      bool good = true;
      for ( int i = n + 1; i < points.size() ; ++i )
      {
        if ( i != n && i != n - 1 )
        {
          good &= atLeft( points[n - 1 ], points[n], points[i] );
          if(!good)
            int b = 2;
        }
      }
      int a =2 ;

      ++n;
    }
  }

  points.resize(n);
  return new ConvexHull2d( points );  
}

};