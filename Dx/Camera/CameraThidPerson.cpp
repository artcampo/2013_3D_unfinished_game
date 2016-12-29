#include	"Dx/Camera/CameraThidPerson.hpp"
#include  "Project_Dependant/LogConditionalCompilation.hpp"
#ifdef _LOG_ENABLE_
  #include  "Misc/Log.hpp"  
#endif

Camera3rdPersonView::Camera3rdPersonView( Camera* aCam )
{
  
  //  Camera for next to shoulder
  mElevationWS        = 8.25f;
  mDistanceToObjectWS = 8.0f;
  mDistanceFocusWS    = 8.0f;
  mCamTargetVel       = D3DXVECTOR3( 1,0,0 );

/*
  //  Camera for long view
  mElevationWS        = 300.25f;
  mDistanceToObjectWS = 100.0f;
  mDistanceFocusWS    = 100.0f;
  mCamTargetVel       = D3DXVECTOR3( 1,0,0 );
*/


  mFocusInc           = 0.5f;
  mCam                = aCam;

#ifdef _LOG_ENABLE_
  mLog = new Log();
#endif
}


void Camera3rdPersonView::updateTargetParameters( const D3DXVECTOR3& aCamTargetPos, 
                                                  const D3DXVECTOR3& aCamTargetVelocityDirection,
                                                  const D3DXVECTOR3& aCamTargetVelocity,
                                                  const float aLapsedTime )
{ 
  mCamTargetsWS         = aCamTargetPos;
  mCamTargetVelocityDirectionWS  = aCamTargetVelocityDirection;

  D3DXVECTOR3 update =DxMath::unitVector( mCamTargetVelocityDirectionWS, mCamTargetVel );
  
  mCamTargetVel += 10.0f*update*mFocusInc*aLapsedTime;
  mCamTargetVel = aCamTargetVelocity;

#ifdef _LOG_ENABLE_CAMERA_TARGET  
  *mLog->mFileShipPlayer<< "Camera Target: " 
                        << mCamTargetVel.x 
                        << ", "
                        << mCamTargetVel.y
                        << ", "
                        << mCamTargetVel.z 
                        << std::endl;  
#endif
}

void Camera3rdPersonView::updateCamera () const
{
  //Global::mView = mMatrix;

  D3DXVECTOR3 upWS = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

  D3DXVECTOR3 camPos  = mElevationWS*upWS + mCamTargetsWS - mCamTargetVel*mDistanceToObjectWS;
  D3DXVECTOR3 camAt   = mCamTargetsWS + mCamTargetVel*mDistanceFocusWS;

  mCam->setUp (
    &upWS
    );
  mCam->setPositionAndAt(
    &camPos, 
    &camAt );
  
}

