#ifndef _DX_CAMERA_MANAGER_
#define _DX_CAMERA_MANAGER_

#include <d3dx10.h>
#include <dxerr.h>

class Camera
{
  public:
    Camera();
    void    updateCamera      ( D3DXMATRIX* mView );
    void    getEyePos         ( D3DXVECTOR3* eyePos);
    void    getEyeAt          ( D3DXVECTOR3* eyePos);
    void    incrementEyePos   ( D3DXVECTOR3* incPos);
    void    rotateAroundTarget( D3DXVECTOR3* incPos);
    void    incrementTargetPos( D3DXVECTOR3* incPos);
    

    D3DXVECTOR3  getTransformedUp ();

    //  to control camera with a path
    void    setPositionAndAt( D3DXVECTOR3* eyePos, D3DXVECTOR3* atAdd );
    void    setUp( D3DXVECTOR3* aUp );

    //  to use with a fustrum
    
    void    updateCameraForFustrum      ( D3DXMATRIX* mView, const float aLength ) const;

    D3DXVECTOR3 Up;
  private:
    D3DXVECTOR3 At;
    D3DXVECTOR3 Position;
    

    D3DXMATRIX  camera_frame_basis;

    void recomputeTbnView();
};


//---------------------------------------------
// EXTERN FUNCTIONS
//---------------------------------------------

void  dx_3dPerpendicular ( D3DXVECTOR3* n, D3DXVECTOR3* v );

#endif