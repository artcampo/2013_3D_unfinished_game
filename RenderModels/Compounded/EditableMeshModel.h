#ifndef _EditableMeshModel_H
#define _EditableMeshModel_H

#include <vector>
#include "dx_misc.hpp"

#include "RenderModels\Basics\pointlistmodel.h"
#include "RenderModels\DxMeshModel.hpp"
#include  "Dx/DefVertexLayouts.hpp"

class EditableMeshModel
{
public:
    ~EditableMeshModel();
    EditableMeshModel( std::vector<VertexPosNorCol>& aTrianglePoints,
                       std::vector<VertexPosCol>& aEdgePoints,
                       PointListModel* aVertices,
                       bool aHasVertices,
                 D3DXCOLOR& aColour,
                 ID3D10Device* aMd3dDevice);

    void displayTriangles();
    void displayEdges();
    void displayVertices();
    bool hasVertices();
private:
  DxMeshModelBatch<VertexPosNorCol>* mMeshTriangles;
  DxMeshModelBatch<VertexPosCol>* mMeshEdges;
  PointListModel*                 mMeshVertices;
  bool  mHasVertices;
};

#endif // _EditableMeshModel_H
