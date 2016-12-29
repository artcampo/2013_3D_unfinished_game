/*!
 * \file StoredShaders.cpp
 * \brief Static data with shaders definition
 * \author Arturo
 */

#include		"Shader/Shader.hpp"
#include		"Shader/VertexLayout.hpp"
#include    "Project_Dependant/StoredShaders.hpp"

//===========================================================================
/*
  Identifiers of VertexLayouts 
*/
//===========================================================================

enum eIndexVertexLayout{
  indexVertexLayout_POS_TEX,
  indexVertexLayout_POS_NOR,
  indexVertexLayout_POS,
  indexVertexLayout_POS_NOR_TAN,
  indexVertexLayout_POS_NOR_COL,
  indexVertexLayout_POS_COL,
  indexVertexLayout_POS_NOR_TAN_UV
};



//===========================================================================
/*
  VertexLayout definitions.
*/
//===========================================================================

const static int numVertexLayouts = 7;



D3D10_INPUT_ELEMENT_DESC layout_1[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
};

D3D10_INPUT_ELEMENT_DESC layout_2[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

D3D10_INPUT_ELEMENT_DESC layout_3[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
};

D3D10_INPUT_ELEMENT_DESC layout_4[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

D3D10_INPUT_ELEMENT_DESC layout_5[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

D3D10_INPUT_ELEMENT_DESC layout_6[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},  
  { "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

D3D10_INPUT_ELEMENT_DESC layout_7[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

VertexLayout shaderVertexLayouts[numVertexLayouts] = 
{
  { &layout_1[0], 2},
  { &layout_2[0], 2},
  { &layout_3[0], 1},
  { &layout_4[0], 3},
  { &layout_5[0], 3},
  { &layout_6[0], 2},
  { &layout_7[0], 4},
};


//===========================================================================
/*
  Shaders definitions.
*/
//===========================================================================


// TODO: techOut_seedTex ->SetResource (mSeedTextureSRV);

ShaderClassData shaderDeferredComposition = 
  {
    indexShadDefComposition,           //id
    L"deferred_composition.fx",                 //file name
    indexVertexLayout_POS_TEX,  //vertexlayout index

    //  sizes
    0,    // numMatrices
    4,    // numRawValues
    6,    // numResources

    //  Techniques
    "Comp",

    //  Matrices
    {
    },

    //  Raw Values
    {
      "fogYPlaneTop",
      "fogYPlaneBottom",
      "fogCircRadius",
      "fogCircCenter",
    },

    //  Resources
   {
      "textCol",
      "textLig",
      "textNor",
      "textPos",
      "textSSDO",
      "PermTexture"
    },

  };



ShaderClassData shaderDeferredSSAO = 
  {
    indexShadDefSSAO,           //id
    L"ssao2.fx",                 //file name
    indexVertexLayout_POS_TEX,  //vertexlayout index

    //  sizes
    2,    // numMatrices
    8,    // numRawValues
    6,    // numResources

    //  Techniques
    "SSAO",

    //  Matrices
    {
      "View",
      "ViewProj",
      
    },

    //  Raw Values
    {
      "g_TanBias",
      "g_Radius",
      "g_Resolution",
      "g_InvResolution",
      "g_Focal",
      "g_InvFocal",
      "g_FarPlane",
      "g_NearPlane"
      
    },

    //  Resources
   {
      "textPos",
      "textNor",
      "textTan",
      "textCol",
      "textLig",
      "textRandDir"
    },

  };

ShaderClassData shaderDeferredSSDO = 
  {
    indexShadDefSSDO,           //id
    L"ssdo.fx",                 //file name
    indexVertexLayout_POS_TEX,  //vertexlayout index

    //  sizes
    2,    // numMatrices
    4,    // numRawValues
    6,    // numResources

    //  Techniques
    "SSDO",

    //  Matrices
    {
      "ModelView",
      "ModelViewProj",
      
    },

    //  Raw Values
    {
      "debug_sample_radius",
      "debug_depthBias",
      "debug_strength",
      "debug_singularity"
    },

    //  Resources
   {
      "textPos",
      "textNor",
      "textTan",
      "textCol",
      "textLig",
      "textSeed"
    },

  };

ShaderClassData shaderDeferredLighting = 
  {
    indexShadDefLight,                          //id
    L"ssdo_lighting.fx",        //file name
    indexVertexLayout_POS,  //vertexlayout index

    //  sizes
    3,    // numMatrices
    4,    // numRawValues
    5,    // numResources

    //  Techniques
    "SSDO_LIGHTING_PASS",

    //  Matrices
    {
      "mW",
      "mV",
      "mP"
    },

    //  Raw Values
    {
      "camPos",
      "lPos",
      "lCol",
      "lRad"
    },

    //  Resources
   {
      "textPos2",
      "textNor2",
      "textTan2",
      "textCol2",
      "textLightP"
    },
    
  };

ShaderClassData shaderPostBlur = 
  {
    indexShadPostBlur,           //id
    L"post_blur.fx",                 //file name
    indexVertexLayout_POS_TEX,  //vertexlayout index

    //  sizes
    0,    // numMatrices
    2,    // numRawValues
    3,    // numResources

    //  Techniques
    "BLUR",

    //  Matrices
    {
      
    },

    //  Raw Values
    {
      "Width",
      "Height"
    },

    //  Resources
   {
      "textCol",
      "textLig",
      "textSSDO"
    },

  };


ShaderClassData shaderDeferredGeometryV_PNT_Arch = 
  {
    indexShadDefGeomV_PNT_Arch,          //id
    L"DG_PNT_arch.fx",             //file name
    indexVertexLayout_POS_NOR_TAN,  //vertexlayout index
    //  sizes
    3,    // numMatrices
    0,    // numRawValues
    1,    // numResources
    //  Techniques
    "DeferredRender",
    //  Matrices
    {
      "World",
      "View",
      "Projection",
    },
    //  Raw Values
    {
    },
    //  Resources
    {
      "PermTexture"
    },    };

//===========================================================================
/*
  Shader table.
*/
//===========================================================================


ShaderClassData*  gStoredShaders[gNumStoredShaders] =
{
  &shaderDeferredComposition,
  &shaderDeferredSSDO,  
  &shaderDeferredSSAO,
  &shaderDeferredLighting,
  &shaderPostBlur,
  &shaderDeferredGeometryV_PNT_Arch
};