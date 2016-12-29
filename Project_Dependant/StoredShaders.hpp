#ifndef _STOREDSHADERS_HPP_
#define _STOREDSHADERS_HPP_

/*!
 * \file SotredShaders.hpp
 * \brief Static data with shaders definition
 * \author Arturo
 */

#include		"Shader/Shader.hpp"
#include		"Shader/VertexLayout.hpp"


//===========================================================================
/*
  Shader table.
*/
//===========================================================================


extern ShaderClassData*  gStoredShaders[gNumStoredShaders];

enum eIndexShaders{
  indexShadDefComposition,
  indexShadDefSSDO,  
  indexShadDefSSAO, 
  indexShadDefLight,
  indexShadPostBlur,  
  indexShadDefGeomV_PNT_Arch

};


#endif