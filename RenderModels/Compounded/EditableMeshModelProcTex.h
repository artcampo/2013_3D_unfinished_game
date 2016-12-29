#ifndef _EditableMeshModelProcTex_H
#define _EditableMeshModelProcTex_H

#include <vector>
#include "dx_misc.hpp"

#include  "RenderModels\Basics\pointlistmodel.h"
#include  "RenderModels\DxMeshModel.hpp"
#include  "Shader/Shader.hpp"
#include  "Dx/DefVertexLayouts.hpp"

class EditableMeshModelProcTex
{
public:
    ~EditableMeshModelProcTex();
    EditableMeshModelProcTex( std::vector< std::vector<VertexPosNor> >& aTrianglePoints,
                              std::vector< Shader* > aShaders,
                              ID3D10Device* aMd3dDevice);

    void display(  D3DXVECTOR3& camPos,
                   D3DXMATRIX& mWorld,
                   D3DXMATRIX& mView,
                   D3DXMATRIX& mProjection,
                   ID3D10Device* aMd3dDevice );
    bool hasVertices();
private:
  std::vector< DxMeshModelBatch<VertexPosNor>* > mMeshTriangles;
  std::vector< Shader* > mShaders;
  int parts;
};

#endif // _EditableMeshModelProcTex_H
