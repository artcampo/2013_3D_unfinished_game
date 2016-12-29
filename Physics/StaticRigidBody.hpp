#ifndef _StaticRigidBody_HPP_
#define _StaticRigidBody_HPP_

#include <d3dx10.h>
#include <dxerr.h>
#include <string>
#include <stdio.h>
#include <vector>

#include	"dx_misc.hpp"
#include  "Noise/RandomNumberGenerator.hpp"
#include	"Physics/PhysicsForces.hpp"
#include	"Physics/PhysicsTorques.hpp"
#include  "ObjEditor/Mesh/Mesh.hpp"
#include  "Physics/MeshCol.hpp"

//===========================================================================
/*
     Individual Body.
     This class relies on the use of pointers to forces to avoid 
     shallow copies
*/
//===========================================================================
class StaticRigidBody{
public:
  StaticRigidBody();
  StaticRigidBody( Mesh* aMesh );
  StaticRigidBody( MeshCol* aMesh );

  bool rayIntesection ( const D3DXVECTOR3 aRayB, 
                        const D3DXVECTOR3 aRayD,
                        const float aEpsilon,
                        const float aBoundingSphereRadius,
                        D3DXVECTOR3& aPoint,
                        D3DXVECTOR3& aNormal,
                        DxMath::Triangle& aTriangle,
                        float& aDistance, const bool aExtendedDebug,
                        const float aEpsilonBarycentric );

  float distanceBoundingSphere( const D3DXVECTOR3 aPoint );
  D3DXVECTOR3   centerBoundingSphere() const;
public:
  int mDebugId;
private:
  Mesh*       mMesh;
  MeshCol*    mMeshCol;
  bool        mIsMesh;

  float   mScaleWC;

private:
  bool rayIntesectionMeshCol ( const D3DXVECTOR3 aRayB, 
                        const D3DXVECTOR3 aRayD,
                        const float aEpsilon,
                        const float aBoundingSphereRadius,
                        D3DXVECTOR3& aPoint,
                        D3DXVECTOR3& aNormal,
                        DxMath::Triangle& aTriangle,
                        float& aDistance, const bool aExtendedDebug,
                        const float aEpsilonBarycentric   );

  bool rayIntesectionMesh ( const D3DXVECTOR3 aRayB, 
                        const D3DXVECTOR3 aRayD,
                        const float aEpsilon,
                        const float aBoundingSphereRadius,
                        D3DXVECTOR3& aPoint,
                        D3DXVECTOR3& aNormal,
                        DxMath::Triangle& aTriangle,
                        float& aDistance, const bool aExtendedDebug );

  bool rayIntesectionMesh ( const int aFaceId, 
                        const int aCompId,
                        const D3DXVECTOR3 aRayB, 
                        const D3DXVECTOR3 aRayD,
                        const float aEpsilon,
                        const float aBoundingSphereRadius,
                        D3DXVECTOR3& aPoint,
                        D3DXVECTOR3& aNormal,
                        DxMath::Triangle& aTriangle,
                        float& aDistance );
 
};


#endif