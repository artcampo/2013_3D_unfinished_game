#ifndef _SHADER_MANAGER_HPP_
#define _SHADER_MANAGER_HPP_

#include <d3dx10.h>
#include <dxerr.h>
#include <string>

#include "Shader/VertexLayout.hpp"
#include "Shader/Shader.hpp"
#include "Project_Dependant/StoredShaders.hpp"

/*
  Singleton over dx interface.
*/

class ShaderManager
{
public:
  ShaderManager ( ID3D10Device* aMd3dDevice );  

  static Shader      mShaderTable[gNumStoredShaders];

private:
  static bool       mInitialized;
  static int        mInitializedCount;

  static ID3D10Texture1D*           mWorleyTexture;  
  static const int                  mWorleyTextureSize = 256;     

  static ID3D10Device*           md3dDevice;

public:  
  static ID3D10ShaderResourceView*  mWorleyTextureSRV;

private:
  bool  initWorleyTexture();
  bool init         ( ID3D10Device* aMd3dDevice );
};




#endif  //_SHADER_MANAGER_HPP_