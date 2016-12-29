#ifndef _DeferredEngine_HPP_
#define _DeferredEngine_HPP_


#include	"dx_misc.hpp"
#include  "Dx/DxCommon.hpp"
//#include  <string>
#include  "Windows/WindowsHelpers.hpp"

class ShaderManager;
class PointLight;

class DeferredEngine
{
public:
  DeferredEngine( ID3D10Device* aDxDevice, IDXGISwapChain* aSwapChain, int width, int height );
  virtual ~DeferredEngine();

  bool init();
  void onResize();
  void startScene();
  void resize(int width, int height);

  

  //  Draw
  void set();   
  void composite();

  //  Others
  void drawText ( std::string& aString, bool aCentered, D3DXCOLOR aCol ) const;

public:
  D3DXMATRIX              mProjection;
  D3DXMATRIX              mWorld;
  D3DXMATRIX              mView;   
  ID3D10ShaderResourceView*           mPermTextureSRV;

private:
  //		Dx device
  ID3D10Device*           mDxDevice;
  IDXGISwapChain*         mSwapChain;
  ID3D10Texture2D*        mDepthStencilBuffer;
  ID3D10RenderTargetView* mRenderTargetView;
  ID3D10DepthStencilView* mDepthStencilView;
  ShaderManager*          mShaderManager;

  //  Transformation matrices
  int                     mCurrentWidth, mCurrentHeight;    
  float                   mFov;
  float                   mNearPlane;
  float                   mFarPlane;

  D3DXCOLOR               mClearColor;      //Background color   

  ID3DX10Font*            mFont;
  ID3DX10Font*            mFontBig;

//  other deferred data
  ID3D10Buffer*						mScreenQuadVB;
  //DxAnimModel*                        sphere;
      
  const static int                    mSsdoTexturePatternSize = 32;
  const static int                    mSsaoTexturePatternSize = 64;
  ID3D10Texture1D*                    mSemiSphereTexture;    
  ID3D10ShaderResourceView*           mSeedTextureSRV;
  ID3D10Texture1D*                    mSsaoTexture;    
  ID3D10ShaderResourceView*           mSsaoTextureSRV;
  ID3D10Texture2D*                    mPermTexture;
  

  DxRenderTarget      mGBuffCol;
  DxRenderTarget      mGBuffPos;
  DxRenderTarget      mGBuffNor;
  DxRenderTarget      mGBuffTan;
  DxRenderTarget      mGBuffLig;
  DxRenderTarget      mGBuffOcc;
  DxRenderTarget      mGBuffOccBlur;
  DxRenderTarget      mGBuffLightP;

public:
  PointLight*                         pLights;
  int                                 nLights;
  const static int                    maxDefLights = 4000;    

private:
  bool initDirectxFonts();
  void applyLightsDeferred();
  

  D3DXVECTOR3   vectorOverSemiSphere( void );
  void          setUpDeferred();
  void          setUpSSDO();
  void          setUpSSAO();
  void          drawLights	( PointLight* pLights, int* nLights );

  bool createRenderTarget ( DxRenderTarget& aRenderTarget,
                            int             aFormat,
                            int             aWidth,
                            int             aHeight);

  void compositeSSDO();
  void compositeSSAO();
  void compositeBlur();
  void compositeFinal();

  void generatePermutationTexture();

};

#endif //_DeferredEngine_HPP_