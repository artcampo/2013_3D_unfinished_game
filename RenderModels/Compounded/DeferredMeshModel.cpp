#include "DeferredMeshModel.h"

#include "RenderModels\DxMeshModel.hpp"

DeferredMeshModel::DeferredMeshModel( std::vector<VertPNT>& aTrianglePoints,
                          D3DXCOLOR& aColour,
                          ID3D10Device* aMd3dDevice)
{

  DxMeshModel<VertPNT>* mesh;
  mesh  = new DxMeshModel<VertPNT>( sizeof(VertPNT), aTrianglePoints.size(), (VertPNT*) (&aTrianglePoints[0]) );
  mMeshTriangles = new DxMeshModelBatch<VertPNT>( sizeof(VertPNT), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  mMeshTriangles->addMeshModel( mesh );
  mMeshTriangles->createVB( );


}

DeferredMeshModel::~DeferredMeshModel()
{
  delete mMeshTriangles;
}


void DeferredMeshModel::displayTriangles()
{
  mMeshTriangles->display();
}
