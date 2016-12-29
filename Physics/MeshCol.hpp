#ifndef _MESHCOL_H
#define _MESHCOL_H

#pragma once
#include <vector>
#include <list>

#include "ObjEditor/Mesh/mesh.hpp"
#include "Geometry/Intersections.hpp"
#include "Geometry/DxMathHelpers.hpp"
#include "dx_misc.hpp"
#include <vector>



class MeshCol
{
public:

  MeshCol( const std::vector<VertexPosNor>& aVertices );  
  MeshCol( const std::vector<VertexPosNor>& aVertices, D3DXMATRIX* aHomoMatrix );
  MeshCol( Mesh* aMesh, D3DXMATRIX* aRotMatrix );

  int                   nFaces() const;
  const VertexPosNor*    triangularFace( int i );

  void      static meshColNonConvex( Mesh* aMesh, D3DXMATRIX* aHomoMatrix, std::vector<MeshCol*>& aMeshColModels ); 
  static MeshCol*   meshColConvex( Mesh* aMesh, D3DXMATRIX* aHomoMatrix ); 

public:
  std::vector<VertexPosNor>      mVertices;
  std::vector<VertexPosNor>      mRotatedVertices;
  Intersections::BoundingSphere boundingSphere() const;

private:
  bool        mRotationMatrixUsed;
  D3DXMATRIX* mRotMatrix;

private:
  void  computeBoundingSphere();
  Intersections::BoundingSphere            mBoundingSphere;

};

#endif // _MESHCOL_H
