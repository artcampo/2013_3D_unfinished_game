#ifndef _MESH_LOAD_HPP_
#define _MESH_LOAD_HPP_

#include "dx_misc.hpp"
#include <string>
#include <vector>

class Mesh;

namespace MeshFile{

  Mesh*  loadMesh ( const char* aFileName );

};

#endif // _MESH_LOAD_HPP_
