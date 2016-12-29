#ifndef _DX_COMMON_HPP_
#define _DX_COMMON_HPP_

class DxRenderTarget{
public:
  DxRenderTarget() : target(NULL), texture(NULL), resource(NULL) {};

  ID3D10RenderTargetView*   target;
  ID3D10Texture2D*          texture;
  ID3D10ShaderResourceView* resource;

  void release()
  {
    ReleaseCOM( target );
    ReleaseCOM( texture );
    ReleaseCOM( resource );
  };

};



#endif //_DX_COMMON_HPP_