#ifndef DXMATHHELPERS_HPP
#define DXMATHHELPERS_HPP

#include "dx_misc.hpp"
#include <vector>

namespace DxMath{
/*
static const float msPI;
static const float sSquareOneHalf;
*/
const float msPI = 3.141592654f;
const float sSquareOneHalf = 0.707106781f;


struct TnbBasis{
  D3DXVECTOR3 tan;
  D3DXVECTOR3 nor;
  D3DXVECTOR3 bin;
};

struct Triangle{
  D3DXVECTOR3 p1;
  D3DXVECTOR3 p2;
  D3DXVECTOR3 p3;
};

D3DXVECTOR3 unitVector( D3DXVECTOR3 aP1, D3DXVECTOR3 aP2 );
D3DXVECTOR3 normalOfTriangle ( const D3DXVECTOR3 aP1, const D3DXVECTOR3 aP2, const D3DXVECTOR3 aP3 );
D3DXVECTOR3 thirdVectorBasis ( D3DXVECTOR3 u, D3DXVECTOR3 v );
D3DXMATRIX  basisMatrix ( std::vector<D3DXVECTOR3> b );

D3DXMATRIX  basisMatrix     ( D3DXVECTOR3 i, D3DXVECTOR3 j, D3DXVECTOR3 k );
D3DXMATRIX  basisMatrixInv  ( D3DXVECTOR3 i, D3DXVECTOR3 j, D3DXVECTOR3 k );

void        componentMult ( D3DXVECTOR3& aP1, const D3DXVECTOR3& aP2 );

//////////////////////////////////////////////////////////
//  Comparisons
bool        equalsZero ( const D3DXVECTOR3& v );
bool        equalsZero ( const D3DXVECTOR3& v, const float epsilon );
bool        equality( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const float epsilon );

bool        equality( const float f1, const float f2, const float epsilon );
bool        greaterThan( const float f1, const float f2, const float epsilon );
bool        strictlyGreaterThan( const float f1, const float f2, const float epsilon );
bool        lesserThan( const float f1, const float f2, const float epsilon );

void        clean ( D3DXVECTOR3& v );
void        clean ( D3DXVECTOR3& v, const float aEpsilon );
void        clean ( float& v, const float aEpsilon );
void        reverse ( D3DXVECTOR3& v );



D3DXVECTOR3 alignToTangents(const D3DXVECTOR3& aInitialTan, 
                            const D3DXVECTOR3& aFinalTan, 
                            const D3DXVECTOR3& aVector );

bool      areCoplanar(  const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, 
                        const D3DXVECTOR3& p3, const D3DXVECTOR3& p4 );

float       determinant ( const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3 );
bool        areTrianglesCoplanar( const Triangle& aTriang1, const Triangle& aTriang2 );
                                 

//////////////////////////////////////////////////////////
//  Rotations
D3DXVECTOR3 rotateThruVector( const D3DXVECTOR3& aAxis,
                              const D3DXVECTOR3& aV,
                              const float aTheta );

float   degToRad( const float aDegrees );
float   radToDeg( const float aRad );
float   angleBetweenVectors( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 );
D3DXVECTOR3 unitVectorMask( const D3DXVECTOR3& aP1, const D3DXVECTOR3& aP2, const D3DXVECTOR3& aMask );

//  Polar coordinates
float       azimut( const D3DXVECTOR3& aVec );
float       polar( const D3DXVECTOR3& aVec );
D3DXVECTOR3 cross( const D3DXVECTOR3& aVec1, const D3DXVECTOR3& aVec2 ); 
void        forcePerpendicularity( const D3DXVECTOR3& aVec1, D3DXVECTOR3& aVec2 );
void        limitAzimut( D3DXVECTOR3& aVec, float aMaxAzimut );
D3DXVECTOR3 sphericalToCartesian ( const float aAzimut, const float aPolar, const float aLength);



//////////////////////////////////////////////////////////
//  Geometry
float       distance ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 );
D3DXVECTOR3 projectionAgainstVector ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 );

float     orientationAgainst2dVector( const D3DXVECTOR2& a, 
                                      const D3DXVECTOR2& b,
                                      const D3DXVECTOR2& c );
const D3DXVECTOR3 vectorOnXZ ( const float radius, const float angle );

};

#endif // DXMATHHELPERS_HPP
