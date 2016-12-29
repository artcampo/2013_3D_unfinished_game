#ifndef _MeshVisualizationModels_HPP
#define _MeshVisualizationModels_HPP

#include "ObjEditor/MeshCompilationOptions.hpp"

#ifdef _MESH_FOR_GAME_ENGINE
    #include "RenderModels\Compounded\EditableMeshModel.h"
    #include "RenderModels\Compounded\EditableMeshModelProcTex.h"
    #include "RenderModels\Compounded\DeferredMeshModel.h"
    #include "RenderModels\Basics\pointlistmodel.h"
#endif
#ifndef _MESH_FOR_GAME_ENGINE
    #include "RenderModels\Compounded\EditableMeshModel.h"
    #include "App\EditableMeshModelProcTex.h"
    #include "App\pointlistmodel.h"
#endif



#endif // _MeshVisualizationModels_HPP
