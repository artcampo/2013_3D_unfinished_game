#include "Geometry/Intersections2d.hpp"


namespace Intersections2d{

float signedArea( D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c)
{
  float a11 = b.x - a.x;
  float a12 = c.x - a.x;
  float a21 = b.y - a.y;
  float a22 = c.y - a.y;

  return 0.5f * (a11*a22 - a21*a12);
}

// true   <=> segments intersect, and the intersection is not in any vertex
// false  <=> segments do not intersect, or touch in either's vertex
bool segmentIntersectionExclusive ( D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r, D3DXVECTOR2 s)
{
  bool	intersect 	= false;
  bool	done		= false;

  float d_pqr = signedArea( p , q , r );
  float d_pqs = signedArea( p , q , s );
  float d_rsp = signedArea( r , s , p );
  float d_rsq = signedArea( r , s , q );

  //Case a: all points are colinear
  if ( d_pqr == 0.0f && d_pqs == 0.0f && d_rsp == 0.0f && d_rsq == 0.0f)
  {

          if ( third_is_in_between( p , q , r) || third_is_in_between( p , q , s) ||
               third_is_in_between( r , s , p) || third_is_in_between( r , s , q) )
                  {
                  intersect = true;
                  }
          done = true;
  }

  //Case b: one vertex shared
  if ((! done) && ( (d_pqr*d_pqs == 0.0f) && (d_rsp*d_rsq == 0.0f)))
  {
          intersect = false;
          done = true;
  }

  //Case c: one vertex in the other segment
  if ((! done) && (  (d_pqr*d_pqs == 0.0f) || (d_rsp*d_rsq == 0.0f)))
  {
          intersect = false;
          done = true;
  }

  //Case d: intersection
  if ( (!done) && ( ((d_pqr*d_pqs) < 0) && ((d_rsp*d_rsq) < 0) ) )
  {
          intersect = true;
          done = true;
  }

  return intersect;
}


bool third_is_in_between (D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c)
{
  if ( a.x == b.x )
          {
          return(
                  ( (a.y <= c.y) && (b.y >= c.y))
                  ||
                  ( (a.y >= c.y) && (b.y <= c.y))
                  );
          }
  else
          {
          return(
                  ( (a.x <= c.x) && (b.x >= c.x))
                  ||
                  ( (a.x >= c.x) && (b.x <= c.x))
                  );
          }
}

D3DXVECTOR2 pointOfIntersectingSegments ( D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3, D3DXVECTOR2 p4)
{
  float num = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x);
  float den = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y);
  float t = num/den;
  D3DXVECTOR2 point = p1 + t*(p2 - p1);
  return point;
}



};
