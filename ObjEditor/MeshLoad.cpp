#include "ObjEditor/MeshLoad.hpp"
#include "ObjEditor/Mesh/Mesh.hpp"

namespace MeshFile{

  Mesh*  loadMesh ( const char* aFileName )
  {
    using namespace std;
    Mesh* mesh = new Mesh();  
    fstream fileStd ( aFileName, fstream::in );  

    if ( fileStd.good() )
    {  
      mesh->load( fileStd );
      fileStd.close();
      return mesh;
    }
    else 
      return NULL;
  };
};
