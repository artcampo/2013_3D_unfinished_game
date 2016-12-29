#include	"Physics/StaticRigidBody.hpp"

//#define _DEBUG_EXTENDED_LOG   

StaticRigidBody::StaticRigidBody()
{
  mScaleWC = 1.0f;
}

StaticRigidBody::StaticRigidBody( Mesh* aMesh )
{
  mMesh = aMesh;
  mScaleWC = 1.0f;
  mIsMesh = true;
}

StaticRigidBody::StaticRigidBody( MeshCol* aMesh )
{
  mMeshCol = aMesh;
  mScaleWC = 1.0f;
  mIsMesh = false;
}



bool StaticRigidBody::rayIntesection ( const D3DXVECTOR3 aRayB, 
                        const D3DXVECTOR3 aRayD,
                        const float aEpsilon,
                        const float aBoundingSphereRadius,
                        D3DXVECTOR3& aPoint,
                        D3DXVECTOR3& aNormal,
                        DxMath::Triangle& aTriangle,
                        float& aDistance, const bool aExtendedDebug,
                        const float aEpsilonBarycentric   )
{
  bool collision;

  aDistance = -1;
  if ( mIsMesh )
    collision = rayIntesectionMesh ( aRayB, aRayD, aEpsilon, aBoundingSphereRadius, aPoint, aNormal, aTriangle, aDistance, aExtendedDebug  );
  else
    collision = rayIntesectionMeshCol ( aRayB, aRayD, aEpsilon, aBoundingSphereRadius, aPoint, aNormal, aTriangle, aDistance,  aExtendedDebug, aEpsilonBarycentric  );

  return collision;
}

bool StaticRigidBody::rayIntesectionMesh ( const D3DXVECTOR3 aRayB, 
                        const D3DXVECTOR3 aRayD,
                        const float aEpsilon,
                        const float aBoundingSphereRadius,
                        D3DXVECTOR3& aPoint,
                        D3DXVECTOR3& aNormal,
                        DxMath::Triangle& aTriangle,
                        float& aDistance, const bool aExtendedDebug  )
{
  bool collision = false;
  for ( unsigned int comp = 0 ; !collision && comp < mMesh->nComponents(); ++comp )
  {
    for ( int j = 0 ; !collision && j < mMesh->mMeshObject[comp]->nFaces(); ++j )
    {
      collision |= rayIntesectionMesh( j, comp, aRayB, aRayD, aEpsilon, aBoundingSphereRadius, aPoint, aNormal, aTriangle, aDistance );
    }
  }

  return collision;
}


bool StaticRigidBody::rayIntesectionMeshCol ( const D3DXVECTOR3 aRayB, 
                        const D3DXVECTOR3 aRayD,
                        const float aEpsilon,
                        const float aBoundingSphereRadius,
                        D3DXVECTOR3& aPoint,
                        D3DXVECTOR3& aNormal,
                        DxMath::Triangle& aTriangle,
                        float& aDistance, const bool aExtendedDebug,
                        const float aEpsilonBarycentric )
{
   bool collision = false;

  D3DXVECTOR3 rayD = aRayD;

  for ( unsigned int i = 0 ; !collision && i < mMeshCol->nFaces(); ++i )
  {
      const VertexPosNor* points = mMeshCol->triangularFace(i);
      VertexPosNor p1 = *(points + 0);
      VertexPosNor p2 = *(points + 1);
      VertexPosNor p3 = *(points + 2);
      p1.pos *= mScaleWC;
      p2.pos *= mScaleWC;
      p3.pos *= mScaleWC;

      DxMath::clean( p1.pos, 0.001f );
      DxMath::clean( p2.pos, 0.001f );
      DxMath::clean( p3.pos, 0.001f );
/*
      D3DXVECTOR3 center = p1.pos + p2.pos + p3.pos;
      center *= 1.0f/3.0f;

      p1.pos += 0.5f * DxMath::unitVector( p1.pos, center );
      p2.pos += 0.5f * DxMath::unitVector( p2.pos, center );
      p3.pos += 0.5f * DxMath::unitVector( p3.pos, center );
      */
/*
      p1.pos.y = 0.0f;
      p2.pos.y = 0.0f;
      p3.pos.y = 0.0f;
*/
      /*
      bool hit;
      float distanceRay, u, v;    

      hit = D3DXIntersectTri( &p1.pos, &p2.pos, &p3.pos, &aRayB, &rayD, &u, &v, &distanceRay );
*/                      

      //  Avoid collinear points in a face's edge
      float distance;
      Intersections::Plane plane =  Intersections::Plane::planeBy3Points( p3.pos, p2.pos, p1.pos );
      
      D3DXVECTOR3 n = plane.normal();
      float d = plane.implicitD();
      distance = fabs( D3DXVec3Dot( &n, &aRayB ) + d );


      aDistance = distance - aBoundingSphereRadius;
      DxMath::clean( aDistance, aEpsilon );
      
      if (  distance < (aEpsilon + aBoundingSphereRadius) )
      {
#ifdef _DEBUG_EXTENDED_LOG        
          if (aExtendedDebug)
          {
            *Global::mFileDebugColOffenders << "-----------  TESTING" << std::endl;
            *Global::mFileDebugColOffenders << "|||||||||||- t1: " << p1.pos.x << ", " << p1.pos.y << ", " << p1.pos.z << std::endl;
            *Global::mFileDebugColOffenders << "|||||||||||- t2: " << p2.pos.x << ", " << p2.pos.y << ", " << p2.pos.z << std::endl;
            *Global::mFileDebugColOffenders << "|||||||||||- t3: " << p3.pos.x << ", " << p3.pos.y << ", " << p3.pos.z << std::endl;
          }
#endif
          D3DXVECTOR3 projPoint = aRayB - n*( D3DXVec3Dot( &n, &aRayB ) );
          bool hit = Intersections::pointInTriangle( projPoint, p1.pos, p2.pos, p3.pos, aEpsilonBarycentric);

#ifdef _DEBUG_EXTENDED_LOG   
          if (aExtendedDebug && hit)
          {
              *Global::mFileDebugColOffenders << " any face passed" 
                << " X: " << aRayB.x << ", " << aRayB.y << ", " << aRayB.z;
          }
          if (aExtendedDebug && !hit)
          {
              *Global::mFileDebugColOffenders << " ray-triangle not passed" 
                << " X: " << aRayB.x << ", " << aRayB.y << ", " << aRayB.z;              
          }
#endif


          if ( hit )
          {
            collision = true;
            aNormal   = DxMath::normalOfTriangle( p1.pos, p2.pos, p3.pos );

            // normals are reverted?!?!? triangle ordering??
            aPoint    = aRayB + (aNormal)*aBoundingSphereRadius;
            aTriangle.p1 = p1.pos;
            aTriangle.p2 = p2.pos;
            aTriangle.p3 = p3.pos;  
          }      
      }
      
  }

  return collision;
}

bool StaticRigidBody::rayIntesectionMesh ( 
                        const int aFaceId, 
                        const int aCompId,
                        const D3DXVECTOR3 aRayB, 
                        const D3DXVECTOR3 aRayD,
                        const float aEpsilon,
                        const float aBoundingSphereRadius,
                        D3DXVECTOR3& aPoint,
                        D3DXVECTOR3& aNormal,
                        DxMath::Triangle& aTriangle,
                        float& aDistance )
{

  Face f = mMesh->mMeshObject[aCompId]->face( aFaceId );

  bool collided = false;
  D3DXVECTOR3 rayD = aRayD;
  
  for (int k = 0; k < f.nDecompositionTriangles() ; ++k)
    {
      Face::TriangleDecomposition triangle = f.decompTriangle(k);
      int p1Id = f.vId( triangle.v1 );
      int p2Id = f.vId( triangle.v2 );
      int p3Id = f.vId( triangle.v3 );
      D3DXVECTOR3 p1 = mMesh->mMeshObject[aCompId]->vertex( p1Id ).pos;
      D3DXVECTOR3 p2 = mMesh->mMeshObject[aCompId]->vertex( p2Id ).pos;
      D3DXVECTOR3 p3 = mMesh->mMeshObject[aCompId]->vertex( p3Id ).pos;
      p1 *= mScaleWC;
      p2 *= mScaleWC;
      p3 *= mScaleWC;

      DxMath::clean( p1, 0.001f );
      DxMath::clean( p2, 0.001f );
      DxMath::clean( p3, 0.001f );

      rayD = DxMath::normalOfTriangle( p1, p2, p3 );         

      //  Avoid collinear points in a face's edge
      float distance, u, v;            
      bool hit = false;
      if ( ! Intersections::areCollinear(p1,p2,p3) )        
      {
          hit = D3DXIntersectTri( &p1, &p2, &p3, &aRayB, &rayD, &u, &v, &distance );
          hit |= D3DXIntersectTri( &p3, &p2, &p1, &aRayB, &(-rayD), &u, &v, &distance );
      }

      if ( hit )
      {
        aDistance = distance;
        float lengthD   = D3DXVec3Length( &rayD );
        float lengthT   = distance*D3DXVec3Length( &rayD );       
        float epsilonT  = aEpsilon / lengthT;
        D3DXVECTOR3 dist = distance*rayD;              
        float d = D3DXVec3Length( &dist );      
        if (  d < aEpsilon )
        {
          aNormal   = DxMath::normalOfTriangle( p1, p2, p3 );         
          aPoint    = p1 + u*(p2 - p1) + v*(p3 - p1);
          //aPoint    = distance*rayD + aRayB;
          aPoint    = aRayB + (aNormal)*aBoundingSphereRadius;

          aTriangle.p1 = p1;
          aTriangle.p2 = p2;
          aTriangle.p3 = p3;
        }
      }

    }

  return collided;
}

float StaticRigidBody::distanceBoundingSphere( const D3DXVECTOR3 aPoint )
{
  float ret = 0;
  if ( mIsMesh )
  {
    ret = Intersections::distancePointSphere( mMesh->boundingSphere() , aPoint );
  }
  else
  {
    ret = Intersections::distancePointSphere( mMeshCol->boundingSphere(), aPoint );
  }
  return ret;
}

D3DXVECTOR3   StaticRigidBody::centerBoundingSphere() const
{
  if ( mIsMesh )
  {
    return mMesh->boundingSphere().center;
  }
  else
  {
    return mMeshCol->boundingSphere().center;
  }
}