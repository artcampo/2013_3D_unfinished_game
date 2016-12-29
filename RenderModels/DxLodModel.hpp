#ifndef			DX_LOD_MODEL_BASIC
#define			DX_LOD_MODEL_BASIC

#include  "dx_misc.hpp"
#include  <vector>
#include  "RenderModels/DxMeshModel.hpp"
#include  "Geometry/DxFustrum.hpp"


/////////////////////////////////////////////////////////////////////////////
//		templatizated over VertexLayout. 
/////////////////////////////////////////////////////////////////////////////

template <class VertexLayout>
class DxLodModel {

public:

  DxLodModel           (  const std::vector< std::vector<VertexLayout> >& aVerticesLod,
                          const std::vector<D3DXVECTOR3>& aTestPoints );
  ~DxLodModel();
  
  void display( const DxFustrum& aFustrum );

private:
  std::vector< DxMeshModelBatch<VertexLayout>* >  mModel;
  std::vector<D3DXVECTOR3>                        mTestPoints;
};


#include    "DxLodModel.cpp"

#endif //DX_LOD_MODEL_BASIC