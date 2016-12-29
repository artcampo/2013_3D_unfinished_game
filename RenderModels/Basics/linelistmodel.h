#ifndef LINELISTMODEL_H
#define LINELISTMODEL_H

#include <vector>
#include "dx_misc.hpp"
#include "RenderModels/DxMeshModel.hpp"

template <class VertexLayout>
class LineListModel
{
public:
    DxMeshModel<VertexLayout> static getMesh( const std::vector<VertexLayout>& mVertices );
public:
    LineListModel( std::vector<VertexLayout>& mVertices );
    ~LineListModel();
    void display();
private:
    
    DxMeshModelBatch<VertexLayout>* mMesh;
};

#include "linelistmodel.cpp"

#endif // LINELISTMODEL_H
