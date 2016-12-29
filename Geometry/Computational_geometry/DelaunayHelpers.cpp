#include  "Geometry\Computational_geometry\DelaunayHelpers.hpp"

namespace DelaunayHelpers{

float signed_area(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{
	float a11 = b.x - a.x;
	float a12 = c.x - a.x;
	float a21 = b.y - a.y;
	float a22 = c.y - a.y;

	float ret = 0.5f * (a11*a22 - a21*a12);
	return ret;
}

bool third_is_in_between (D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
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

float point_to_circumference_alignment ( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c, D3DXVECTOR3 x )
{
	float m00 = b.x - a.x;
	float m01 = b.y - a.y;
	float m02 = (b.x - a.x)*(b.x + a.x) + (b.y - a.y)*(b.y + a.y);

	float m10 = c.x - a.x;
	float m11 = c.y - a.y;
	float m12 = (c.x - a.x)*(c.x + a.x) + (c.y - a.y)*(c.y + a.y);

	float m20 = x.x - a.x;
	float m21 = x.y - a.y;
	float m22 = (x.x - a.x)*(x.x + a.x) + (x.y - a.y)*(x.y + a.y);

	float det = 	+ m00 * ( m11*m22 - m12*m21 )
			- m01 * ( m10*m22 - m12*m20 )
			+ m02 * ( m10*m21 - m11*m20 );

	return det;
}

bool	pointIsInsideTriangle ( D3DXVECTOR3 t1 , D3DXVECTOR3 t2 , D3DXVECTOR3 t3 , D3DXVECTOR3 x )
{
	bool done		= false;
	bool counterclockwise	= true;
	bool point_in_vertex 	= false;
	bool point_in_edge 	= false;
	bool point_inside 	= false;
	bool point_outside 	= false;

	//Compute determinants
	float det_t1t2P = signed_area ( t1 , t2 , x );
	float det_t2t3P = signed_area ( t2 , t3 , x );
	float det_t3t1P = signed_area ( t3 , t1 , x );


	//Case a) D3DXVECTOR3 is in vertex
	if ( (!done) && (det_t1t2P == 0.0f) && (det_t2t3P == 0.0f) )
		{
		point_in_vertex = true;
		done = true;
		}

	if ( (!done) && (det_t2t3P == 0.0f) && (det_t3t1P == 0.0f) )
		{
		point_in_vertex = true;
		done = true;
		}

	if ( (!done) && (det_t1t2P == 0.0f) && (det_t3t1P == 0.0f) )
		{
		point_in_vertex = true;
		done = true;
		}

	//Case b) D3DXVECTOR3 is in edge
	if ( (!done) && ( (det_t1t2P*det_t2t3P*det_t3t1P) == 0.0f ) )
		{
		if ( (det_t1t2P == 0.0f) && third_is_in_between( t1, t2, x ) )
			{
			point_in_edge = true;
			done = true;
			}
		if ( (det_t2t3P == 0.0f) && third_is_in_between( t2, t3, x ) )
			{
			point_in_edge = true;
			done = true;
			}
		if ( (det_t3t1P == 0.0f) && third_is_in_between( t3, t1, x ) )
			{
			point_in_edge = true;
			done = true;
			}
		}

	//Case c) D3DXVECTOR3 is either: inside, outside
	if (!done)
		{
		if (counterclockwise)
			if( (det_t1t2P > 0.0f) && (det_t2t3P > 0.0f) && (det_t3t1P > 0.0f) )
			//if( (det_t1t2P*det_t2t3P*det_t3t1P) > 0.0f )
				{
				point_inside = true;
				done = true;
				}
			else
				{
				point_outside = true;
				done = true;
				}
		else
			if( (det_t1t2P < 0.0f) && (det_t2t3P < 0.0f) && (det_t3t1P < 0.0f) )
			//iromf( (det_t1t2P*det_t2t3P*det_t3t1P) < 0.0f )
				{
				point_inside = true;
				done = true;
				}
			else
				{
				point_outside = true;
				done = true;
				}
		}

	return point_inside;

}

//	Input segments: (p,q) && (r,s)
//	Returns true if they do intersect
bool Segment_intersection ( D3DXVECTOR3 p, D3DXVECTOR3 q, D3DXVECTOR3 r, D3DXVECTOR3 s)
{

	bool	intersect 	= false;
	bool	done		= false;

	float d_pqr = signed_area( p , q , r );
	float d_pqs = signed_area( p , q , s );
	float d_rsp = signed_area( r , s , p );
	float d_rsq = signed_area( r , s , q );

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
		intersect = true;
		done = true;
	}

	//Case c: one vertex in the other segment
	if ((! done) && (  (d_pqr*d_pqs == 0.0f) || (d_rsp*d_rsq == 0.0f))) 
	{

		if (d_pqr == 0.0f)
			intersect = third_is_in_between( p , q , r);

		if ( (!intersect) && (d_pqs == 0.0f) )
			intersect = third_is_in_between( p , q , s);

		if ( (!intersect) && (d_rsp == 0.0f) )
			intersect = third_is_in_between( r , s , p);

		if ( (!intersect) && (d_rsq == 0.0f) )
			intersect = third_is_in_between( r , s , q);
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

D3DXVECTOR3 barycenter ( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{
	D3DXVECTOR3 result;

	result	 = a;
	result	+= b;
	result	+= c;
	result	/= 3.0f;

	return result;
}

};  ////_DELAUNAY_HELPERS