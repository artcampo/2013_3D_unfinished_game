#ifndef POINTLISTMODEL_H
#define POINTLISTMODEL_H

#include <vector>
#include "dx_misc.hpp"
#include "RenderModels/DxMeshModel.hpp"
#include  "Dx/DefVertexLayouts.hpp"

class PointListModel
{
private:
      DxMeshModelBatch<VertexPointPSC>* mMesh;
public:
    PointListModel( std::vector<VertexPointPSC>& mVertices, ID3D10Device* aMd3dDevice );
    ~PointListModel();
    void display();

};

#endif // POINTLISTMODEL_H
