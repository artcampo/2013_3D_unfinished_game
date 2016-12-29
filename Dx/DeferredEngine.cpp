#include  "Dx/DeferredEngine.hpp"
#include  "Noise/RandomNumberGenerator.hpp"
#include  "Shader/ShaderManager.hpp"
#include  "Dx/DefVertexLayouts.hpp"

//Creator
DeferredEngine::DeferredEngine( ID3D10Device* aDxDevice, IDXGISwapChain* aSwapChain, int width, int height ):
               mDxDevice            ( aDxDevice ),
               mSwapChain           ( aSwapChain ),
               mShaderManager       ( new ShaderManager( aDxDevice ) ),
               mDepthStencilBuffer  (0),
               mRenderTargetView    (0),
               mDepthStencilView    (0),
               mFont                (0),
               mFontBig             (0),
               mCurrentWidth        (width),
               mCurrentHeight       (height),               
               mClearColor          (WHITE),
               mNearPlane           (1.0f),
               mFarPlane            (4000.0f)

{
  init();
  onResize();
}


DeferredEngine::~DeferredEngine()
{
  mSwapChain->SetFullscreenState(false, 0);  
	mDxDevice->ClearState();

	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(mDxDevice);
  ReleaseCOM(mFont); 
  ReleaseCOM(mFontBig); 	

  mGBuffCol.release();
  mGBuffPos.release();
  mGBuffNor.release();
  mGBuffTan.release();
  mGBuffLig.release();
  mGBuffOcc.release();
  mGBuffOccBlur.release();
  mGBuffLightP.release();
}

bool DeferredEngine::init()
{  

  generatePermutationTexture();
  setUpDeferred();
  initDirectxFonts();
	
  //mShaderManager.init( mDxDevice );

  return true;
}



bool DeferredEngine::initDirectxFonts()
{
  
  //Init font
  D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 24;
  fontDesc.Width           = 0;
  fontDesc.Weight          = 0;
  fontDesc.MipLevels       = 1;
  fontDesc.Italic          = false;
  fontDesc.CharSet         = DEFAULT_CHARSET;
  fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
  fontDesc.Quality         = DEFAULT_QUALITY;
  fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
  wcscpy_s(fontDesc.FaceName, L"Verdana");
	D3DX10CreateFontIndirect(mDxDevice, &fontDesc, &mFont); 

  fontDesc.Height          = 36;
  D3DX10CreateFontIndirect(mDxDevice, &fontDesc, &mFontBig); 
  
  return true;
}


void DeferredEngine::startScene	()
{
  mDxDevice->ClearRenderTargetView(mRenderTargetView, mClearColor);
	mDxDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
  mDxDevice->OMSetDepthStencilState(0, 0);
  mDxDevice->ClearRenderTargetView(mGBuffCol.target, mClearColor);
  mDxDevice->ClearRenderTargetView(mGBuffPos.target, mClearColor);
  mDxDevice->ClearRenderTargetView(mGBuffNor.target, mClearColor);
}

void DeferredEngine::onResize()
{
  mGBuffCol.release();
  mGBuffPos.release();
  mGBuffNor.release();
  mGBuffTan.release();
  mGBuffLig.release();
  mGBuffOcc.release();
  mGBuffOccBlur.release();
  mGBuffLightP.release();

	ReleaseCOM( mRenderTargetView );
	ReleaseCOM( mDepthStencilView );
	ReleaseCOM( mDepthStencilBuffer );
	// Resize the swap chain and recreate the render target view.
	mSwapChain->ResizeBuffers(1, mCurrentWidth, mCurrentHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D10Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer));
	mDxDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	ReleaseCOM(backBuffer);
	// Create the depth/stencil buffer and view.
	D3D10_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width              = mCurrentWidth;
	depthStencilDesc.Height             = mCurrentHeight;
	depthStencilDesc.MipLevels          = 1;
	depthStencilDesc.ArraySize          = 1;
	depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage              = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags          = D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags     = 0; 
	depthStencilDesc.MiscFlags          = 0;

	mDxDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	mDxDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
	// Bind the render target view and depth/stencil view to the pipeline.
	mDxDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	// Set the viewport transform.
	D3D10_VIEWPORT vp;

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = mCurrentWidth;
	vp.Height   = mCurrentHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	mDxDevice->RSSetViewports(1, &vp);  

  mFov = ( float )D3DX_PI * 0.5f;
  D3DXMatrixPerspectiveFovLH( &mProjection, mFov, mCurrentWidth / (FLOAT)mCurrentHeight, mNearPlane, mFarPlane );
  
  //  Create deferred buffers
  //  Create G-buffers and RenderViews


  createRenderTarget( mGBuffCol,
                      DXGI_FORMAT_R32G32B32A32_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffPos,
                      DXGI_FORMAT_R32G32B32A32_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffNor,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffTan,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffLig,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );
/*
  createRenderTarget( mGBuffOcc,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth/4, mCurrentHeight/4 );

  createRenderTarget( mGBuffOccBlur,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth/2, mCurrentHeight/2 );
 */
  createRenderTarget( mGBuffOcc,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffOccBlur,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );

  createRenderTarget( mGBuffLightP,
                      DXGI_FORMAT_R16G16B16A16_FLOAT, mCurrentWidth, mCurrentHeight );
  
}







//===========================================================================
/*!
    Returns random vector over a semi sphere

    \return     D3DXVECTOR3 vector
*/
//===========================================================================
D3DXVECTOR3 DeferredEngine::vectorOverSemiSphere( void )
{  
  
  Rand::RandGen rand(42);
  D3DXVECTOR3 N(0.0f, 1.0f,0.0f);
  float x,y,z;
  while (true) {

    x = rand.randFloat_0_1()*2.0f - 1.0f; // random776 float between -1 and 1
    y = rand.randFloat_0_1()*2.0f - 1.0f;
    z = rand.randFloat_0_1()*2.0f - 1.0f;
    if (x * x + y * y + z * z > 1) continue; // ignore ones outside unit
                                             
     // sphere
    
    D3DXVECTOR3 v(x, y, z);
    //if (D3DXVec3Dot(&v, &N) < 0) continue; // ignore "down" dirs
    if (v.y < 0) continue; // ignore "down" dirs
    
    D3DXVECTOR3 d(x, y, z);
    //D3DXVec3Normalize( &d, &d);
    return d; // success!
  }
  
  return D3DXVECTOR3 (0.0f, 1.0f,0.0f);
}

//===========================================================================
/*!
    Sets up deferred rendering

    \return     bool true <=> Shader compiled succesfully
*/
//===========================================================================
void DeferredEngine::setUpDeferred()
{
  pLights = new PointLight[maxDefLights];

  setUpSSDO();
  setUpSSAO();


	// Create full mWindowsScreen Quad Vertices
	{
		ScreenVertex svQuad[4];
		svQuad[0].pos = D3DXVECTOR4( -1.0f, 1.0f, 0.5f, 1.0f );
		svQuad[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
		svQuad[1].pos = D3DXVECTOR4( 1.0f, 1.0f, 0.5f, 1.0f );
		svQuad[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
		svQuad[2].pos = D3DXVECTOR4( -1.0f, -1.0f, 0.5f, 1.0f );
		svQuad[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
		svQuad[3].pos = D3DXVECTOR4( 1.0f, -1.0f, 0.5f, 1.0f );
		svQuad[3].tex = D3DXVECTOR2( 1.0f, 1.0f );

		D3D10_BUFFER_DESC vbdesc =
		{
			4 * sizeof( ScreenVertex ),
			D3D10_USAGE_DEFAULT,
			D3D10_BIND_VERTEX_BUFFER,
			0,
			0
		};
		D3D10_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = svQuad;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		mDxDevice->CreateBuffer( &vbdesc, &InitData, &mScreenQuadVB );
	}

  //  Load sphere for lighting
  //mSphere = new SphereModel( 4 );
}

void DeferredEngine::setUpSSAO()
{
  /*
    //  Create Vectors over a semisphere
    D3DXVECTOR3 data[mSsaoTexturePatternSize * mSsaoTexturePatternSize];
    for (int i = 0; i < mSsaoTexturePatternSize * mSsaoTexturePatternSize; i++)  
    {
        float angle = mRand.randFloat_0_1() *2*DxMath::msPI;
        data[i].x = sin( angle );
        data[i].y = cos( angle );
        data[i].z = mRand.randFloat_0_1()*0.5 + 0.5;
    }         

    D3D10_TEXTURE1D_DESC desc;
    desc.Width = mSsaoTexturePatternSize*mSsaoTexturePatternSize;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;    
    desc.Usage = D3D10_USAGE_IMMUTABLE;
    desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    
    D3D10_SUBRESOURCE_DATA dataDesc;
    dataDesc.pSysMem = data;    
    dataDesc.SysMemPitch = sizeof(float)*3;
    mDxDevice->CreateTexture1D(&desc, &dataDesc, &mSsaoTexture);
    
    D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;

    ZeroMemory( &descSRV, sizeof(descSRV) );
    descSRV.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
    descSRV.Texture2D.MipLevels = 1;
    descSRV.Texture2D.MostDetailedMip = 0;

    mDxDevice->CreateShaderResourceView( mSsaoTexture, &descSRV, &mSsaoTextureSRV ) ;
    */
}

void DeferredEngine::setUpSSDO()
{
  //  Create Vectors over a semisphere
  D3DXVECTOR3 data[mSsdoTexturePatternSize * mSsdoTexturePatternSize];
  for (int i = 0; i < mSsdoTexturePatternSize * mSsdoTexturePatternSize; i++)  
      data[i] = vectorOverSemiSphere();
  

  D3D10_TEXTURE1D_DESC desc;
  desc.Width = mSsdoTexturePatternSize*mSsdoTexturePatternSize;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;    
  desc.Usage = D3D10_USAGE_IMMUTABLE;
  desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;
  
  D3D10_SUBRESOURCE_DATA dataDesc;
  dataDesc.pSysMem = data;
  //dataDesc.SysMemPitch = mSsdoTexturePatternSize*sizeof(float)*3;
  dataDesc.SysMemPitch = sizeof(float)*3;
  mDxDevice->CreateTexture1D(&desc, &dataDesc, &mSemiSphereTexture);
  
  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;

  ZeroMemory( &descSRV, sizeof(descSRV) );
  descSRV.Format = DXGI_FORMAT_R32G32B32_FLOAT;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
  descSRV.Texture2D.MipLevels = 1;
  descSRV.Texture2D.MostDetailedMip = 0;

  mDxDevice->CreateShaderResourceView( mSemiSphereTexture, &descSRV, &mSeedTextureSRV ) ;
}


//===========================================================================
/*!
    Deferred rendering: To be called prior to rendering
*/
//===========================================================================
void DeferredEngine::set()
{
	
  ID3D10RenderTargetView* mRenderTargetViewTable[5];  

  mRenderTargetViewTable[0] = mGBuffPos.target;
  mRenderTargetViewTable[1] = mGBuffCol.target;
  mRenderTargetViewTable[2] = mGBuffNor.target;
  mRenderTargetViewTable[3] = mGBuffTan.target;
  mRenderTargetViewTable[4] = mGBuffLightP.target;
  mDxDevice->OMSetRenderTargets( 5, &mRenderTargetViewTable[0], mDepthStencilView);

  mDxDevice->ClearRenderTargetView(mGBuffPos.target, BLACK);
  mDxDevice->ClearRenderTargetView(mGBuffCol.target, WHITE);
  mDxDevice->ClearRenderTargetView(mGBuffNor.target, BLACK);
  mDxDevice->ClearRenderTargetView(mGBuffTan.target, BLACK);
  mDxDevice->ClearRenderTargetView(mGBuffLightP.target, WHITE);
  
}

//===========================================================================
/*!
    Deferred rendering: Composite final image
*/
//===========================================================================
void DeferredEngine::composite()
{	
  //compositeSSDO();
  //compositeSSAO();
  //compositeBlur();
  compositeFinal();
 
}


void DeferredEngine::compositeSSDO()
{
// Bind the render target view and depth/stencil view to the pipeline.
  mDxDevice->OMSetRenderTargets( 1, &mGBuffOcc.target, mDepthStencilView);
  //mDxDevice->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView);

  
}

void DeferredEngine::compositeSSAO()
{

  
}


void DeferredEngine::compositeBlur()
{

}

void DeferredEngine::compositeFinal()
{
  /////////////////////////////////////////////////////////
  //    Apply final composition
  mDxDevice->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView);

  //  Set input textures  

  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textSSDO")), mGBuffOcc.resource );
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textCol")), mGBuffCol.resource );
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textLig")), mGBuffLig.resource );
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textNor")), mGBuffNor.resource );  
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textPos")), mGBuffPos.resource );
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  D3DXVECTOR3 fogCircCenter   = D3DXVECTOR3(00,0,0);
  float       fogCircRadius   =  20;
  float       fogYPlaneTop    =  10;
  float       fogYPlaneBottom = -50;

  mShaderManager->mShaderTable[indexShadDefComposition].SetRawValue( &(std::string("fogCircCenter")),    (float*)&fogCircCenter  , 3 );
  mShaderManager->mShaderTable[indexShadDefComposition].SetRawValue( &(std::string("fogCircRadius")),    (float*)&fogCircRadius  , 1 );
  mShaderManager->mShaderTable[indexShadDefComposition].SetRawValue( &(std::string("fogYPlaneTop")),     (float*)&fogYPlaneTop   , 1 );
  mShaderManager->mShaderTable[indexShadDefComposition].SetRawValue( &(std::string("fogYPlaneBottom")),  (float*)&fogYPlaneBottom, 1 );

  //Draw quad covering screen
  UINT strides = sizeof( ScreenVertex );
	UINT offsets = 0;
	ID3D10Buffer* pBuffers[1] = { mScreenQuadVB };

  mDxDevice->IASetInputLayout( mShaderManager->mShaderTable[indexShadDefComposition].mVertexLayout );
  mDxDevice->IASetVertexBuffers( 0, 1, pBuffers, &strides, &offsets );
	mDxDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	mShaderManager->mShaderTable[indexShadDefComposition].mTech->GetPassByIndex( 0 )->Apply( 0 );
	// Composite the blurred model into the original image.
	mDxDevice->Draw( 4, 0 );

  //  release
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("PermTexture")), 0 );
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textSSDO")), 0 );
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textCol")) , 0 );
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textLig")) , 0 );
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textNor")) , 0 );
  mShaderManager->mShaderTable[indexShadDefComposition].SetResource( &(std::string("textPos")) , 0 );
  mShaderManager->mShaderTable[indexShadDefComposition].mTech->GetPassByIndex( 0 )->Apply( 0 );	
}

//===========================================================================
/*!
    Deferred rendering: Update lighting buffer
*/
//===========================================================================
void DeferredEngine::applyLightsDeferred()
{

}

void DeferredEngine::drawLights	( PointLight* pLights, int* nLights )
{
 
}

bool DeferredEngine::createRenderTarget ( DxRenderTarget& aRenderTarget,
                          int                         aFormat,
                          int                         aWidth,
                          int                         aHeight )
{
  aRenderTarget.texture  = NULL;
  aRenderTarget.target   = NULL;
  aRenderTarget.resource = NULL;

  // Create the render target texture
  D3D10_TEXTURE2D_DESC desc;
  ZeroMemory( &desc, sizeof(desc) );
  desc.Width = aWidth;
  desc.Height = aHeight;
  desc.MipLevels = 1;
  desc.ArraySize = 1;  
  desc.Format = DXGI_FORMAT(aFormat);
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D10_USAGE_DEFAULT;
  desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;

  HRESULT hr;
  hr = mDxDevice->CreateTexture2D( &desc, NULL, &aRenderTarget.texture );
  if ( FAILED(hr) ) return false;

  //  Create its render target
  D3D10_RENDER_TARGET_VIEW_DESC rtDesc;
  rtDesc.Format = desc.Format;
  rtDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
  rtDesc.Texture2D.MipSlice = 0;

  
  hr = mDxDevice->CreateRenderTargetView( aRenderTarget.texture, &rtDesc, &aRenderTarget.target);
  if ( FAILED(hr) ) return false;
 
	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT(aFormat);
	srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;
  
  hr = mDxDevice->CreateShaderResourceView( aRenderTarget.texture, &srDesc, &aRenderTarget.resource );
  if ( FAILED(hr) ) return false;

  return true;  
}

void DeferredEngine::generatePermutationTexture()
{
  
  //  Create Vectors over a semisphere
  Rand::RandGen r(42);
  std::vector<int> perm = r.genPermutations( 1024 );
 
  D3D10_TEXTURE2D_DESC desc;
  desc.Width  = 32;
  desc.Height = 32;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R16_UINT;    
  desc.Usage = D3D10_USAGE_IMMUTABLE;
  desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  
  D3D10_SUBRESOURCE_DATA dataDesc;
  dataDesc.pSysMem = &perm[0];
  //dataDesc.SysMemPitch = mSsdoTexturePatternSize*sizeof(float)*3;
  dataDesc.SysMemPitch = sizeof(int);
  dataDesc.SysMemSlicePitch = 0;
  mDxDevice->CreateTexture2D(&desc, &dataDesc, &mPermTexture);
  
  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;

  ZeroMemory( &descSRV, sizeof(descSRV) );
  descSRV.Format = DXGI_FORMAT_R16_UINT;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  descSRV.Texture2D.MipLevels = 1;
  descSRV.Texture2D.MostDetailedMip = 0;

  mDxDevice->CreateShaderResourceView( mPermTexture, &descSRV, &mPermTextureSRV ) ;
  
}

void DeferredEngine::resize(int width, int height)
{
  mCurrentWidth   = width;
  mCurrentHeight  = height;
}

void DeferredEngine::drawText ( std::string& aString, bool aCentered, D3DXCOLOR aCol ) const
{
  std::wstring s = WinIO::s2ws( aString );    
  //RECT Rect = {mCurrentWidth/2, mCurrentHeight/2, 0, 0};
  RECT Rect = {0, mCurrentHeight/2, mCurrentWidth, mCurrentHeight};
  if (!aCentered ) Rect.top = 0;
  mFontBig->DrawText(0, s.c_str(), -1, &Rect, DT_NOCLIP | DT_CENTER , aCol);	  
}