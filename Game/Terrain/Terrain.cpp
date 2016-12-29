#include  <memory>
#include  "Game/Terrain/Terrain.hpp"

#include	"Geometry\Computational_geometry\Delaunay.hpp"
#include  "Noise/RandomNumberGenerator.hpp"

#include  "Dx/DefVertexLayouts.hpp"
#include	"RenderModels/DxMeshModel.hpp"

#include	"Noise/Noise3d.hpp"

std::auto_ptr< DxMeshModelBatch<VertPNT> >  Terrain::compute ()
{  
  return computeTriangulation();
}

//===========================================================================
/*!
    Initialize a Shader from its stored form

    \param      aData  All data describing this shader.
*/
//===========================================================================
void  Terrain::configure (  )
{
  mHeight     = 0.0f;
  mHeightRand = 10.2f;

}

void Terrain::pointsAdd ( D3DXVECTOR3& aPoint )
{
  mPoints.push_back( aPoint );
}



void  Terrain::pointsSeaBase ( const std::vector<D3DXVECTOR3>& aPoints )
{
  Rand::RandGen r(42);
  mHeightRand *= 1.5f;
  Noise3d::Noise3d n;

  for ( int i = 0 ; i < aPoints.size() ; ++i )
  {
    D3DXVECTOR3 newPoint = aPoints[i];
    
    float t0    = n.noise( newPoint, D3DXVECTOR3 ( 0.03, 0.03, 0.03 ) );
    float t1    = n.noise( newPoint, D3DXVECTOR3 ( 0.06, 0.06, 0.06 ) );
    float t2    = n.noise( newPoint, D3DXVECTOR3 ( 0.12, 0.12, 0.12 ) );
    float _z    = newPoint.y  + 1.0f *mHeightRand*t0 
                              + 0.5f *mHeightRand*t1;
                              + 0.25f*mHeightRand*t2;
    newPoint.y  = _z;

    pointsAdd ( newPoint );
  }
}



std::auto_ptr< DxMeshModelBatch<VertPNT> >  Terrain::computeTriangulation ()
{
	

	std::vector<D3DXVECTOR3> enclosing_triangle = DelaunayEnclosingTriangle ( 2000.0f );
	IncDelaunay IncDel;
  IncDel.Init ( mPoints.size() , enclosing_triangle );

	for ( int point_i = 0 ; point_i < mPoints.size() ; ++point_i )
	{

    D3DXVECTOR3 v;
    v.x = mPoints[point_i].x;
    v.y = mPoints[point_i].z;
    v.z = mPoints[point_i].y;
		IncDel.AddPoint ( v );
		IncDel.FlipPending ( );
	}

	IncDel.Compute_Edge_of_Vertex();
  IncDel.DeleteEdgesAndFacesFromBoundary( 0, 1, 2 );
	IncDel.DeleteEnclosingTriangle();	
  
  //  Read all faces
  VertPNT* vertices;
  int numVertices = IncDel.number_of_faces*3;
  vertices = new VertPNT[numVertices];

  unsigned int i = 0;
  unsigned int stored_faces = 0;
  while(  i < (unsigned int)IncDel.number_of_faces )
	{
    if ( IncDel.IsFaceValid(i) )
    {
      int faceEdges[3];
      int faceVertices[3];
      IncDel.Face_ListEdgesAndVertices( i, faceEdges , faceVertices );

      (vertices+ 3*stored_faces + 0)->pos.x = ( (Vertex*) &IncDel.Vertexes[ faceVertices[2] ] )->pos.x;
      (vertices+ 3*stored_faces + 0)->pos.y = ( (Vertex*) &IncDel.Vertexes[ faceVertices[2] ])->pos.z;
      (vertices+ 3*stored_faces + 0)->pos.z = ( (Vertex*) &IncDel.Vertexes[ faceVertices[2] ])->pos.y;

      (vertices+ 3*stored_faces + 1)->pos.x = ( (Vertex*) &IncDel.Vertexes[ faceVertices[1] ])->pos.x;
      (vertices+ 3*stored_faces + 1)->pos.y = ( (Vertex*) &IncDel.Vertexes[ faceVertices[1] ])->pos.z;
      (vertices+ 3*stored_faces + 1)->pos.z = ( (Vertex*) &IncDel.Vertexes[ faceVertices[1] ])->pos.y;

      (vertices+ 3*stored_faces + 2)->pos.x = ( (Vertex*) &IncDel.Vertexes[ faceVertices[0] ])->pos.x;
      (vertices+ 3*stored_faces + 2)->pos.y = ( (Vertex*) &IncDel.Vertexes[ faceVertices[0] ])->pos.z;
      (vertices+ 3*stored_faces + 2)->pos.z = ( (Vertex*) &IncDel.Vertexes[ faceVertices[0] ])->pos.y;

      D3DXVECTOR3 tan;
      tan = *( (D3DXVECTOR3 *) &IncDel.Vertexes[ faceVertices[1] ]) - 
            *( (D3DXVECTOR3 *) &IncDel.Vertexes[ faceVertices[2] ]);
      D3DXVec3Normalize ( &tan, &tan );
      D3DXVECTOR3 bitan = 
            *( (D3DXVECTOR3 *) &IncDel.Vertexes[ faceVertices[0] ]) - 
            *( (D3DXVECTOR3 *) &IncDel.Vertexes[ faceVertices[2] ]);
      D3DXVec3Normalize ( &bitan, &bitan);
      D3DXVECTOR3 normal;
      
      D3DXVec3Cross ( &normal, &tan, &bitan);
      D3DXVec3Normalize ( &normal, &normal);


      (vertices+ 3*stored_faces + 0)->nor = normal;
      (vertices+ 3*stored_faces + 1)->nor = normal;
      (vertices+ 3*stored_faces + 2)->nor = normal;

      (vertices+ 3*stored_faces + 0)->tan   = tan;
      (vertices+ 3*stored_faces + 1)->tan   = tan;
      (vertices+ 3*stored_faces + 2)->tan   = tan;

      ++i;
      ++stored_faces;
    }
    else
    {
      ++i;
    }
  }

  //  Create mesh
  DxMeshModel<VertPNT> d( sizeof(VertPNT), stored_faces*3,  vertices );
  std::auto_ptr< DxMeshModelBatch<VertPNT> > delaunayMesh ( new DxMeshModelBatch<VertPNT>( sizeof(VertPNT) , D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
  delaunayMesh->addMeshModel( &d );
  delaunayMesh->createVB();

  return delaunayMesh;
}
