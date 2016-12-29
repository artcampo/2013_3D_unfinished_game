#include		"Camera.hpp"

Camera::Camera():
  Position(5.5f, 3.25f, 5.5f),  
  Up(0.0f,1.0f,0.0f),  
  At(0, 0, 0)
  
{
  recomputeTbnView();
}



void Camera::updateCamera(D3DXMATRIX *mView)
{
  D3DXMatrixLookAtLH( mView, &Position, &At, &Up );  
}



//  TODO: change names "atadd??!?!?"
void Camera::setPositionAndAt( D3DXVECTOR3* eyePos, D3DXVECTOR3* atAdd )
{
  Position  = *eyePos;
  At        = *atAdd;
}

void Camera::setUp( D3DXVECTOR3* aUp )
{  
  Up        = *aUp;
}


void    Camera::getEyeAt( D3DXVECTOR3* eyePos)
{
  *eyePos = At;
}

void    Camera::getEyePos( D3DXVECTOR3* eyePos)
{
  *eyePos = Position;
}


void    Camera::incrementEyePos( D3DXVECTOR3* incPos)
{
  
  recomputeTbnView();

  float w = 0.0f;
  D3DXVECTOR4 increment ( incPos->x, incPos->y, incPos->z, w );
  
  D3DXVECTOR4 transformed_increment;
  D3DXVec4Transform ( &transformed_increment, &increment, &camera_frame_basis );

  //transformed_increment *= 0.1f;

  Position  += D3DXVECTOR3(transformed_increment);
  At        += D3DXVECTOR3(transformed_increment);
}

void    Camera::rotateAroundTarget( D3DXVECTOR3* incPos)
{
  
  recomputeTbnView();

  float w = 0.0f;
  D3DXVECTOR4 increment ( incPos->x, incPos->y, incPos->z, w );
  
  D3DXVECTOR4 transformed_increment;
  D3DXVec4Transform ( &transformed_increment, &increment, &camera_frame_basis );

  //transformed_increment *= 0.1f;

  Position  += D3DXVECTOR3(transformed_increment);
  //At        += D3DXVECTOR3(transformed_increment);
}

void    Camera::incrementTargetPos( D3DXVECTOR3* incPos)
{
  At += *incPos ;
}

void  Camera::recomputeTbnView ()
{
  D3DXVECTOR3 tangent = At - Position;
  D3DXVECTOR3 u(0.0f, 1.0f, 0.0f ); 
  D3DXVECTOR3 normal;
  D3DXVec3Cross ( &normal, &u, &tangent);

  D3DXVECTOR3 binormal;
  D3DXVec3Cross ( &binormal, &tangent, &normal );

  D3DXVec3Normalize ( &tangent, &tangent );
  D3DXVec3Normalize ( &normal, &normal );
  D3DXVec3Normalize ( &binormal, &binormal );

  D3DXMATRIX d( normal.x, normal.y, normal.z, 0.0f,
                binormal.x, binormal.y, binormal.z, 0.0f,
                tangent.x, tangent.y, tangent.z, 0.0f,
                0.0f,0.0f,0.0f,1.0f );


  camera_frame_basis = d;
}

D3DXVECTOR3  Camera::getTransformedUp ()
{
  D3DXMATRIX view;
  D3DXMatrixLookAtLH( &view, &Position, &At, &Up );  
  D3DXVECTOR3 ret;
  D3DXVec3TransformCoord ( &ret, &(D3DXVECTOR3(0.0f,1.0f,0.0f)), &view );
  return ret;
}

void  dx_3dPerpendicular ( D3DXVECTOR3* n, D3DXVECTOR3* v )
{
  n->x = -(v->y);
  n->y = v->x;
  n->z = v->z;

}

//  aLength is the units backwards from the target
void    Camera::updateCameraForFustrum( D3DXMATRIX* mView, const float aLength ) const
{
  D3DXVECTOR3 dir = At-Position;
  D3DXVec3Normalize( &dir, &dir );
  D3DXVECTOR3 inc = aLength*dir;
  D3DXVECTOR3 fustAt = At - inc;
  D3DXVECTOR3 fustPosition = Position - inc;
  D3DXMatrixLookAtLH( mView, &fustPosition, &fustAt, &Up );  

}