#ifndef _DeferredMeshModel_H
#define _DeferredMeshModel_H

#include <vector>
#include "dx_misc.hpp"
#include  "Dx/DefVertexLayouts.hpp"



template <typename T> class DxMeshModelBatch;

class DeferredMeshModel
{
public:
    ~DeferredMeshModel();
    DeferredMeshModel( std::vector<VertPNT>& aTrianglePoints,
                 D3DXCOLOR& aColour,
                 ID3D10Device* aMd3dDevice);

    void displayTriangles();

private:
  DxMeshModelBatch<VertPNT>* mMeshTriangles;
};

#endif // _DeferredMeshModel_H
