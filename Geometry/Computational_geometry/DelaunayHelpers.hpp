#ifndef _DELAUNAY_HELPERS_HPP_
#define _DELAUNAY_HELPERS_HPP_

#include "dx_misc.hpp"

namespace DelaunayHelpers{

  float	signed_area							          ( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c );
  bool 	third_is_in_between 				      ( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c );
  float	point_to_circumference_alignment	( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c, D3DXVECTOR3 x );
  bool	pointIsInsideTriangle 				    ( D3DXVECTOR3 t1 , D3DXVECTOR3 t2 , D3DXVECTOR3 t3 , D3DXVECTOR3 x );

  D3DXVECTOR3 	barycenter 					( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c);
  bool 			Segment_intersection 		( D3DXVECTOR3 p, D3DXVECTOR3 q, D3DXVECTOR3 r, D3DXVECTOR3 s);
};

#endif  //_DELAUNAY_HELPERS_HPP_
