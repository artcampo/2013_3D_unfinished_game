
template <class VertexLayout>
DxLodModel<VertexLayout>::DxLodModel (  const std::vector< std::vector<VertexLayout> >& aVerticesLod,
                                        const std::vector<D3DXVECTOR3>& aTestPoints )
{
  mTestPoints = aTestPoints;

  
  
  mModel.resize(3);
  for ( int i = 0; i < 3; ++i )
  {
    DxMeshModel<VertexLayout>* mesh 
     = new DxMeshModel<VertexLayout>( 
                  sizeof(VertexLayout),    
                  aVerticesLod[i].size(),
                  &aVerticesLod[i][0]
                  );

    mModel[i] = new DxMeshModelBatch<VertexLayout>( sizeof(VertexLayout), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    mModel[i]->addMeshModel( mesh );
    mModel[i]->createVB( );
    delete mesh;
        
  }
}



template <class VertexLayout>
DxLodModel<VertexLayout>::~DxLodModel()
{
  delete mModel[0];
  delete mModel[1];
  delete mModel[2];
}


template <class VertexLayout>
void  
DxLodModel<VertexLayout>::
display( const DxFustrum& aFustrum )
{
  float distance;
  bool  inside = false;

  for ( int i = 0; !inside && i < mTestPoints.size(); ++i )  
    inside |= aFustrum.isInside( mTestPoints[i], distance );  

  if(inside)
  {
    if (distance < 100)       mModel[0]->DisplaySansShader();
    else if (distance < 300)  mModel[1]->DisplaySansShader();
    else                      mModel[2]->DisplaySansShader();
  }
}