#include	"Shader/ShaderManager.hpp"
#include  "Project_Dependant/StoredShaders.hpp"
#include  "Noise/RandomNumberGenerator.hpp"

#include	"dx_misc.hpp"
#include  "Global.hpp"
#include   <stdio.h>

int    ShaderManager::mInitializedCount = 0;
bool   ShaderManager::mInitialized = false;
Shader ShaderManager::mShaderTable[gNumStoredShaders];

ID3D10Device*               ShaderManager::md3dDevice;
ID3D10Texture1D*            ShaderManager::mWorleyTexture;
ID3D10ShaderResourceView*   ShaderManager::mWorleyTextureSRV;
//int                         ShaderManager::mWorleyTextureSize = 256;

ShaderManager::ShaderManager ( ID3D10Device* aMd3dDevice )
{
  init(aMd3dDevice);
}

bool ShaderManager::init ( ID3D10Device* aMd3dDevice )
{
  if ( mInitialized )
  {
    ++mInitializedCount;
    return true;
  }

  md3dDevice = aMd3dDevice;

  //  Init shaders
  for ( int i = 0; i < gNumStoredShaders ; ++i)
  {
    mShaderTable[i].Init( gStoredShaders[i] );
    if ( ! mShaderTable[i].Compile() )
      return false;
  }
  
  initWorleyTexture();

  mInitialized = true;
  mInitializedCount = 1;
  return true;
}

bool  ShaderManager::initWorleyTexture()
{
  Rand::RandGen r(42);
  
  //  Create Vectors over a semisphere
  float data[mWorleyTextureSize];
  for (int i = 0; i < mWorleyTextureSize; i++)  
  {
      float num = 1.0 + r.randFloat_0_1()*4;
      data[i] = num;
  }         

  D3D10_TEXTURE1D_DESC desc;
  desc.Width = mWorleyTextureSize;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R32_FLOAT;    
  desc.Usage = D3D10_USAGE_IMMUTABLE;
  desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;
  
  D3D10_SUBRESOURCE_DATA dataDesc;
  dataDesc.pSysMem = data;    
  dataDesc.SysMemPitch = sizeof(float);
  md3dDevice->CreateTexture1D(&desc, &dataDesc, &mWorleyTexture);
  
  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
  ZeroMemory( &descSRV, sizeof(descSRV) );
  descSRV.Format = DXGI_FORMAT_R32_FLOAT;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
  descSRV.Texture2D.MipLevels = 1;
  descSRV.Texture2D.MostDetailedMip = 0;

  md3dDevice->CreateShaderResourceView( mWorleyTexture, &descSRV, &mWorleyTextureSRV ) ;

  return true;
}
