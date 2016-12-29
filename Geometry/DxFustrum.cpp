#include "DxFustrum.hpp"
#include "Project_Dependant/ConditionalCompilation.hpp"

DxFustrum::DxFustrum( const D3DXMATRIX& aMatrix )
{
  l.a = aMatrix._14 + aMatrix._11;
  l.b = aMatrix._24 + aMatrix._21;
  l.c = aMatrix._34 + aMatrix._31;
  l.d = aMatrix._44 + aMatrix._41;
  // Right clipping plane
  r.a = aMatrix._14 - aMatrix._11;
  r.b = aMatrix._24 - aMatrix._21;
  r.c = aMatrix._34 - aMatrix._31;
  r.d = aMatrix._44 - aMatrix._41;
  // Top clipping plane
  t.a = aMatrix._14 - aMatrix._12;
  t.b = aMatrix._24 - aMatrix._22;
  t.c = aMatrix._34 - aMatrix._32;
  t.d = aMatrix._44 - aMatrix._42;
  // Bottom clipping plane
  b.a = aMatrix._14 + aMatrix._12;
  b.b = aMatrix._24 + aMatrix._22;
  b.c = aMatrix._34 + aMatrix._32;
  b.d = aMatrix._44 + aMatrix._42;
  // Far clipping plane
  f.a = aMatrix._14 - aMatrix._13;
  f.b = aMatrix._24 - aMatrix._23;
  f.c = aMatrix._34 - aMatrix._33;
  f.d = aMatrix._44 - aMatrix._43;
  // Near clipping plane
  n.a = aMatrix._13;
  n.b = aMatrix._23;
  n.c = aMatrix._33;
  n.d = aMatrix._43;
  // Normalize it
  float m = D3DXVec3Length( &D3DXVECTOR3( n.a, n.b, n.c ) );
  n.a /= m;
  n.b /= m;
  n.c /= m;
  n.d /= m;
}

bool  DxFustrum::isInside ( const D3DXVECTOR3& aPoint, float& aDistanceToNearPlane ) const
{
#ifdef _GBP_DISABLE_FUSTRUM_CULLING_  
  aDistanceToNearPlane = 10;
  return true;
#endif
#ifndef _GBP_DISABLE_FUSTRUM_CULLING_  
  if (  n.inside( aPoint ) &&
        f.inside( aPoint ) &&
        l.inside( aPoint ) &&
        r.inside( aPoint ) &&
        t.inside( aPoint ) &&
        b.inside( aPoint ) )
  {
    aDistanceToNearPlane = n.distance( aPoint );
    return true;
  }
  else
    return false;
#endif

}