#include "DxMathHelpers.hpp"
// TODO:DEBUG:REMOVE
  #include "Global.hpp"
// TODO:DEBUG:REMOVE

namespace DxMath{


D3DXVECTOR3 unitVector( D3DXVECTOR3 aP1, D3DXVECTOR3 aP2 )
{
  D3DXVECTOR3 u = aP1 - aP2;
  D3DXVec3Normalize( &u, &u );
  return u;
}

D3DXVECTOR3 unitVectorMask( const D3DXVECTOR3& aP1, const D3DXVECTOR3& aP2, const D3DXVECTOR3& aMask )
{
  D3DXVECTOR3 u = aP1 - aP2;
  componentMult( u, aMask );
  D3DXVec3Normalize( &u, &u );
  return u;
}

// ccw triangle
D3DXVECTOR3 normalOfTriangle ( const D3DXVECTOR3 aP1, const D3DXVECTOR3 aP2, const D3DXVECTOR3 aP3 )
{
  D3DXVECTOR3 u = aP2 - aP1;
  D3DXVECTOR3 v = aP3 - aP1;
  D3DXVECTOR3 n;
  D3DXVec3Cross( &n, &u, &v );  
  DxMath::clean( n, 0.001 );
  D3DXVec3Normalize( &n, &n );
  return n;
}

//  right handed basis
D3DXVECTOR3 thirdVectorBasis ( D3DXVECTOR3 u, D3DXVECTOR3 v )
{
  D3DXVECTOR3 n;
  D3DXVec3Cross( &n, &u, &v );
  D3DXVec3Normalize( &n, &n );
  return n;
}

D3DXMATRIX  basisMatrixInv ( D3DXVECTOR3 i, D3DXVECTOR3 j, D3DXVECTOR3 k )
{
  D3DXMATRIX m ( i.x, j.x, k.x, 0,
                 i.y, j.y, k.y, 0,
                 i.z, j.z, k.z, 0,
                 0,   0,   0,   1 );
  D3DXMatrixInverse( &m, NULL, &m );
  return m;
}

D3DXMATRIX  basisMatrix ( D3DXVECTOR3 i, D3DXVECTOR3 j, D3DXVECTOR3 k )
{
  D3DXMATRIX m ( i.x, j.x, k.x, 0,
                 i.y, j.y, k.y, 0,
                 i.z, j.z, k.z, 0,
                 0,   0,   0,   1 );
  return m;
}

D3DXMATRIX  basisMatrix ( std::vector<D3DXVECTOR3> b )
{
  return basisMatrix( b[0], b[1], b[2] ) ;
}

void        componentMult ( D3DXVECTOR3& aP1, const D3DXVECTOR3& aP2 )
{
  aP1.x *= aP2.x;
  aP1.y *= aP2.y;
  aP1.z *= aP2.z;
}

bool        equalsZero ( const D3DXVECTOR3& v )
{
  bool ret = false;
  if (  ( fabs ( v.x ) < 0.000001f ) &&
        ( fabs ( v.y ) < 0.000001f ) &&
        ( fabs ( v.z ) < 0.000001f ) )
        ret = true;
  return ret;
}

bool        equalsZero ( const D3DXVECTOR3& v, const float epsilon )
{
  bool ret = false;
  if (  ( fabs ( v.x ) < epsilon ) &&
        ( fabs ( v.y ) < epsilon ) &&
        ( fabs ( v.z ) < epsilon ) )
        ret = true;
  return ret;
}

bool        equality( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const float epsilon )
{
  bool ret = false;
  if (  ( fabs ( v1.x - v2.x ) < epsilon ) &&
        ( fabs ( v1.y - v2.y ) < epsilon ) &&
        ( fabs ( v1.z - v2.z ) < epsilon ) )
        ret = true;
  return ret;
}

void        clean ( float& v, const float aEpsilon )
{  
  if ( fabs(v) < aEpsilon )  
    v = 0.0f;
}

void        clean ( D3DXVECTOR3& v )
{
  float epsilon = 0.0000000001f;
  if ( fabs(v.x) < epsilon )  
    v.x = 0.0f;
  if ( fabs(v.y) < epsilon )  
    v.y = 0.0f;
  if ( fabs(v.z) < epsilon )  
    v.z = 0.0f;
    
}

void        clean ( D3DXVECTOR3& v, const float aEpsilon )
{  
  if ( fabs(v.x) < aEpsilon )  
    v.x = 0.0f;
  if ( fabs(v.y) < aEpsilon )  
    v.y = 0.0f;
  if ( fabs(v.z) < aEpsilon )  
    v.z = 0.0f;    
}

void        reverse ( D3DXVECTOR3& v )
{
  if ( v.x != 0.0f )
    v.x *= -1.0f;
  if ( v.y != 0.0f )
    v.y *= -1.0f;
  if ( v.z != 0.0f )
    v.z *= -1.0f;
}

float distance ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 )
{
  D3DXVECTOR3 distance = v2 - v1;
  return D3DXVec3Length( &distance ); 
}

bool        equality( const float f1, const float f2, const float epsilon )
{
  bool ret = false;
  if ( fabs ( f1 - f2 ) < epsilon )
        ret = true;
  return ret;
}

bool        greaterThan( const float f1, const float f2, const float epsilon )
{
  bool ret =  ( f1 + epsilon >= f2 ) || 
              ( f1 - epsilon >= f2 );
  return ret;
}

bool        strictlyGreaterThan( const float f1, const float f2, const float epsilon )
{
  bool ret =  ( f1 - f2  >= epsilon );
  return ret;
}

bool        lesserThan( const float f1, const float f2, const float epsilon )
{
  bool ret =  ( f1 + epsilon < f2 ) || 
              ( f1 - epsilon < f2 );
  return ret;
}



D3DXVECTOR3 rotateThruVector( const D3DXVECTOR3& aAxis,
                              const D3DXVECTOR3& aV,
                              const float aTheta )
{
  D3DXMATRIX m;
  D3DXMatrixRotationAxis( &m, &aAxis, aTheta);
  
  D3DXVECTOR3 result;
  D3DXVec3TransformCoord( &result, &aV, &m );
  return result;
}


D3DXVECTOR3 alignToTangents( const D3DXVECTOR3& aInitialTan, 
                            const D3DXVECTOR3& aFinalTan, 
                            const D3DXVECTOR3& aVector )
{
  if ( !DxMath::equality(aInitialTan, aFinalTan, 0.00001f ) )
  {
    float theta = acos( D3DXVec3Dot ( &aInitialTan, &aFinalTan ) ); 
    D3DXVECTOR3 axis; 
    D3DXVec3Cross ( &axis, &aInitialTan, &aFinalTan ) ; 

    D3DXMATRIX m;
    D3DXMatrixRotationAxis( &m, &axis, -theta);
    
    D3DXVECTOR3 result;
    D3DXVec3TransformCoord( &result, &aVector, &m );
    D3DXVec3Normalize( &result, &result );
    return result;
  }
  else
    return aVector;
}

float       determinant ( const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3 )
{
  float d1 = p1.x* (p2.y*p3.z-p3.y*p2.z);
  float d2 = p1.y*(p2.x*p3.z-p3.x*p2.z);
  float d3 = p1.z* (p2.x*p3.y-p3.x*p2.y);

  return (d1 - d2 + d3);
}

bool      areCoplanar(  const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, 
                        const D3DXVECTOR3& p3, const D3DXVECTOR3& p4 )
{
  D3DXVECTOR3 x31 = p3 - p1;
  D3DXVECTOR3 x21 = p2 - p1;
  D3DXVECTOR3 x43 = p4 - p3;
  D3DXVECTOR3 cross;
  D3DXVec3Cross( &cross, &x21, &x43 );
  float d = D3DXVec3Dot( &x31, &cross );


  // TODO: COPLANARITY
  return equality( fabs(d), 0.0f, 0.1 );
}

bool        areTrianglesCoplanar( const Triangle& aTriang1, const Triangle& aTriang2 )
{  
  if ( !areCoplanar( aTriang1.p1, aTriang1.p2, aTriang1.p3, aTriang2.p1 ) )
    return false;
  
  if ( !areCoplanar( aTriang1.p1, aTriang1.p2, aTriang1.p3, aTriang2.p2 ) )
    return false;
  
  if ( !areCoplanar( aTriang1.p1, aTriang1.p2, aTriang1.p3, aTriang2.p3 ) )
    return false;

  return true;
}


float   azimut( const D3DXVECTOR3& aVec )
{
  return acos( aVec.y / D3DXVec3Length( &aVec ) );
}

float       polar( const D3DXVECTOR3& aVec )
{
  return atan2( aVec.z, aVec.x );
}


float   degToRad( const float aDegrees )
{
  return aDegrees*msPI/180.0f;
}

float   radToDeg( const float aDegrees )
{
  return aDegrees*180.0f/msPI;
}

D3DXVECTOR3 cross( const D3DXVECTOR3& aVec1, const D3DXVECTOR3& aVec2 )
{
  D3DXVECTOR3 ret;
  D3DXVec3Cross( &ret, &aVec1, &aVec2 );
  return ret;
}

void        forcePerpendicularity( const D3DXVECTOR3& aVec1, D3DXVECTOR3& aVec2 )
{
  if ( ! equality( aVec1.y, 0 , 0.00001f ) )
  {
    float dotSans = aVec1.x*aVec2.x + aVec1.z*aVec2.z; 
    float newcomp = -(dotSans)/aVec1.y;
    aVec2.y       = newcomp;
    D3DXVec3Normalize( &aVec2, &aVec2 );
    return;
  }
  if ( ! equality( aVec1.x, 0 , 0.00001f ) )
  {
    float dotSans = aVec1.y*aVec2.y + aVec1.z*aVec2.z; 
    float newcomp = -(dotSans)/aVec1.x;
    aVec2.x       = newcomp;
    D3DXVec3Normalize( &aVec2, &aVec2 );
    return;
  }
  if ( ! equality( aVec1.z, 0 , 0.00001f ) )
  {
    float dotSans = aVec1.x*aVec2.x + aVec1.y*aVec2.y; 
    float newcomp = -(dotSans)/aVec1.z;
    aVec2.z       = newcomp;
    D3DXVec3Normalize( &aVec2, &aVec2 );
    return;
  }
}

void    limitAzimut( D3DXVECTOR3& aVec, float aMaxAzimut )
{
  float a = azimut( aVec );
  float p = polar( aVec );
  float l = D3DXVec3Length( &aVec );

  if ( a > aMaxAzimut )
  {
    aVec = sphericalToCartesian( aMaxAzimut, p, l );
  }

}

D3DXVECTOR3   sphericalToCartesian ( const float aAzimut, const float aPolar, const float aLength)
{
  D3DXVECTOR3 ret;
  ret.x = aLength*cos( aPolar )*sin(aAzimut);
  ret.z = aLength*sin( aPolar )*sin(aAzimut);
  ret.y = aLength*cos(aAzimut);
  return ret;
}

D3DXVECTOR3 projectionAgainstVector ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 )
{
  D3DXVECTOR3 unit;  
  D3DXVec3Normalize( &unit, &v1 );  
  return unit * D3DXVec3Dot( &v1, &v2 );
}

float   angleBetweenVectors( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 )
{
  D3DXVECTOR3 u1, u2;
  D3DXVec3Normalize( &u1, &v1 );  
  D3DXVec3Normalize( &u2, &v2 );  
  return acos( D3DXVec3Dot( &u1, &u2 ));
}

/*
  orientation of c, in respect to line ab
*/
float     orientationAgainst2dVector( const D3DXVECTOR2& a, 
                                      const D3DXVECTOR2& b,
                                      const D3DXVECTOR2& c )
{
  return (a.x - c.x)*(b.y - c.y) - (b.x - c.x)*(a.y - c.y );
}

const D3DXVECTOR3 vectorOnXZ ( const float radius, const float angle )
{
  return D3DXVECTOR3( radius*cos(angle), 0, radius*sin(angle ) );
}

};
