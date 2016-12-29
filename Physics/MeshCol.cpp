#include "MeshCol.hpp"


MeshCol::MeshCol( const std::vector<VertexPosNor>& aVertices )
{
  mVertices = aVertices;
  computeBoundingSphere();
  mRotationMatrixUsed = false;
}

MeshCol::MeshCol( const std::vector<VertexPosNor>& aVertices, D3DXMATRIX* aRotMatrix )
{
  mVertices           = aVertices;
  computeBoundingSphere();
  mRotationMatrixUsed = true;
  mRotMatrix          = aRotMatrix;
  mRotatedVertices.resize(3);
}

int MeshCol::nFaces() const
{
  return (mVertices.size() / 3 );
}

const VertexPosNor*  MeshCol::triangularFace( int i )
{
  if ( !mRotationMatrixUsed )
    return ( &mVertices[i*3] );
  else
  {
    for ( int j = 0; j < 3; ++j )
    {
      D3DXVec3TransformCoord( &mRotatedVertices[j].pos, &mVertices[i*3].pos, mRotMatrix );
      D3DXVec3TransformCoord( &mRotatedVertices[j].nor, &mVertices[i*3].nor, mRotMatrix );
    }
    return &mRotatedVertices[0];
  }
}

void  MeshCol::computeBoundingSphere()
{
  int n = mVertices.size();
  mBoundingSphere.center = D3DXVECTOR3( 0,0,0 );
  for( int i =0 ; i< n; ++i)  
    mBoundingSphere.center += mVertices[i].pos;
  
  mBoundingSphere.center *= ( 1.0f / float(n) );

  float maxDistance = 0.0f;
  for( int i =0 ; i< n; ++i)
  {
    D3DXVECTOR3 v = mBoundingSphere.center - mVertices[i].pos;
    float distance = D3DXVec3Length( &v );
    if ( distance > maxDistance )
      maxDistance = distance;
  }
  mBoundingSphere.radius = maxDistance;

}

Intersections::BoundingSphere MeshCol::boundingSphere() const
{
  return mBoundingSphere;
}

//  makes a MeshCol from a mesh and a base matrix
//  HomoMatrix consist of trans, size and rot
//  (attention: fails if no homo)
MeshCol::MeshCol( Mesh* aMesh, D3DXMATRIX* aHomoMatrix )
{

  for (int i = 0; i < aMesh->mMeshObject[0]->nFaces() ; ++i)
  {
    Face f = aMesh->mMeshObject[0]->face( i );
    for (int k = 0; k < f.nDecompositionTriangles() ; ++k )
      {
        Face::TriangleDecomposition triangle = f.decompTriangle(k);
        int p1Id = f.vId( triangle.v1 );
        int p2Id = f.vId( triangle.v2 );
        int p3Id = f.vId( triangle.v3 );

        VertexPosNor p1; p1.pos = aMesh->mMeshObject[0]->vertex( p1Id ).pos;
        VertexPosNor p2; p2.pos = aMesh->mMeshObject[0]->vertex( p2Id ).pos;
        VertexPosNor p3; p3.pos = aMesh->mMeshObject[0]->vertex( p3Id ).pos;

        D3DXVECTOR3 normal = DxMath::normalOfTriangle( p3.pos, p2.pos, p1.pos );
        p1.nor = normal;
        p2.nor = normal;
        p3.nor = normal;

        D3DXVec3TransformCoord( &p1.pos, &p1.pos, aHomoMatrix );
        D3DXVec3TransformCoord( &p2.pos, &p2.pos, aHomoMatrix );
        D3DXVec3TransformCoord( &p3.pos, &p3.pos, aHomoMatrix );

        mVertices.push_back( p1 );
        mVertices.push_back( p2 );
        mVertices.push_back( p3 );
      }
  }

  computeBoundingSphere();
  mRotationMatrixUsed = false;
}

//  used when we want to transform a mesh into many meshcols, one for each triangle
void MeshCol::meshColNonConvex( Mesh* aMesh, D3DXMATRIX* aHomoMatrix, std::vector<MeshCol*>& aMeshColModels )
{
  for (int i = 0; i < aMesh->mMeshObject[0]->nFaces() ; ++i)
  {
    Face f = aMesh->mMeshObject[0]->face( i );
    std::vector<VertexPosNor> vertices;
    for (int k = 0; k < f.nDecompositionTriangles() ; ++k)
      {
        Face::TriangleDecomposition triangle = f.decompTriangle(k);
        int p1Id = f.vId( triangle.v1 );
        int p2Id = f.vId( triangle.v2 );
        int p3Id = f.vId( triangle.v3 );

        VertexPosNor p1; p1.pos  = aMesh->mMeshObject[0]->vertex( p1Id ).pos;
        VertexPosNor p2; p2.pos  = aMesh->mMeshObject[0]->vertex( p2Id ).pos;
        VertexPosNor p3; p3.pos  = aMesh->mMeshObject[0]->vertex( p3Id ).pos;

        //D3DXVECTOR3 normal = DxMath::normalOfTriangle( p1.pos, p2.pos, p3.pos );

        D3DXVec3TransformCoord( &p1.pos, &p1.pos, aHomoMatrix );
        D3DXVec3TransformCoord( &p2.pos, &p2.pos, aHomoMatrix );
        D3DXVec3TransformCoord( &p3.pos, &p3.pos, aHomoMatrix );
        //D3DXVec3TransformCoord( &normal, &normal, aHomoMatrix );
        D3DXVECTOR3 normal = DxMath::normalOfTriangle( p1.pos, p2.pos, p3.pos );

        p1.nor = normal;
        p2.nor = normal;
        p3.nor = normal;

        vertices.push_back( p1 );
        vertices.push_back( p2 );
        vertices.push_back( p3 );

        aMeshColModels.push_back( new MeshCol( vertices ) );
      }
  }
}

//  used when we want to transform a mesh into many meshcols, one for each triangle
MeshCol* MeshCol::meshColConvex( Mesh* aMesh, D3DXMATRIX* aHomoMatrix)
{  
  std::vector<VertexPosNor> vertices;
  for (int i = 0; i < aMesh->mMeshObject[0]->nFaces() ; ++i)
  {
    Face f = aMesh->mMeshObject[0]->face( i );
    
    for (int k = 0; k < f.nDecompositionTriangles() ; ++k)
      {
        Face::TriangleDecomposition triangle = f.decompTriangle(k);
        int p1Id = f.vId( triangle.v1 );
        int p2Id = f.vId( triangle.v2 );
        int p3Id = f.vId( triangle.v3 );
        VertexPosNor p1; p1.pos = aMesh->mMeshObject[0]->vertex( p1Id ).pos;
        VertexPosNor p2; p2.pos = aMesh->mMeshObject[0]->vertex( p2Id ).pos;
        VertexPosNor p3; p3.pos = aMesh->mMeshObject[0]->vertex( p3Id ).pos;

        D3DXVec3TransformCoord( &p1.pos, &p1.pos, aHomoMatrix );
        D3DXVec3TransformCoord( &p2.pos, &p2.pos, aHomoMatrix );
        D3DXVec3TransformCoord( &p3.pos, &p3.pos, aHomoMatrix );

        D3DXVECTOR3 normal = DxMath::normalOfTriangle( p3.pos, p2.pos, p1.pos );
        p1.nor = normal;
        p2.nor = normal;
        p3.nor = normal;

        vertices.push_back( p1 );
        vertices.push_back( p2 );
        vertices.push_back( p3 );        
      }
  }
  return new MeshCol( vertices );
}