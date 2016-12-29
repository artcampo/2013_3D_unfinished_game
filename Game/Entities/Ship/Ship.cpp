#include  "Game/Entities/Ship/Ship.hpp"

#include  "Physics/Physics.hpp" 
#include  "Physics/RigidBody.hpp"

#include  "Geometry/Spline/SplineLengthMapped.hpp"

Ship::~Ship()
{
}

Ship::Ship (  Physics& aPhysics,
              const D3DXVECTOR3& aInitPosition,
              const ShipDescription& aShipDesc)
      : 
      mBody ( aPhysics.createRigidBody( aInitPosition, 2.5f, 0.25, 0.5, 0.2 ) ),
      mPointingDirection (1,0,0),
      mCrossDirection(0,0,1),
      mShipDesc( aShipDesc )
{

  mShipTForceForw =
    mBody->mBodyExt->attachTimeForce( &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), 
                                        &gShipTForceFwdSpline, mShipDesc.fwdModule );

  mShipTForceBack =
    mBody->mBodyExt->attachTimeForce( &D3DXVECTOR3( -1.0f, 0.0f, 0.0f), 
                                        &gShipTForceFwdSpline, mShipDesc.bckModule );  

  mShipTForceLeft = 
    mBody->mBodyExt->attachTimeForce( &D3DXVECTOR3( 0.0f, 0.0f, 1.0f), 
                                        &gShipTForceFwdSpline, mShipDesc.sidModule );

  mShipTForceRght =
    mBody->mBodyExt->attachTimeForce( &D3DXVECTOR3( 0.0f, 0.0f, -1.0f), 
                                        &gShipTForceFwdSpline, mShipDesc.sidModule  );
       
  mShipTTorqueUp  = 
    mBody->mBodyExt->attachTimeTorque(  &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), 
                                        &D3DXVECTOR3( 0.0f, 1.0f, 0.0f), 
                                        &gShipTForceFwdSpline,
                                        mShipDesc.pitchModule );

  mShipTTorqueDown  = 
    mBody->mBodyExt->attachTimeTorque(  &D3DXVECTOR3( -1.0f, 0.0f, 0.0f), 
                                        &D3DXVECTOR3( 0.0f, 1.0f, 0.0f), 
                                        &gShipTForceFwdSpline,
                                        mShipDesc.pitchModule );

  mShipTTorqueLeft  =
    mBody->mBodyExt->attachTimeTorque(  &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), 
                                        &D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
                                        &gShipTForceFwdSpline,
                                        mShipDesc.yawModule );    

  mShipTTorqueRight  = 
    mBody->mBodyExt->attachTimeTorque(  &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), 
                                        &D3DXVECTOR3( 0.0f, 0.0f, -1.0f),
                                        &gShipTForceFwdSpline,
                                        mShipDesc.yawModule );  

}


void  Ship::update( const float aLapsedTime )
{
  updateCameraParameters();
  D3DXMATRIX matrixRotation;
  D3DXMatrixRotationQuaternion( &matrixRotation, &mBody->qWS );
  D3DXVec3TransformCoord( &mPointingDirection,  &D3DXVECTOR3(1,0,0), &matrixRotation );
  D3DXVec3TransformCoord( &mCrossDirection,     &D3DXVECTOR3(0,0,1), &matrixRotation );  
  D3DXVec3Normalize( &mPointingDirection, &mPointingDirection );
  D3DXVec3Normalize( &mCrossDirection, &mCrossDirection );
}



void Ship::updateCameraParameters()
{  
}

void  Ship::clearForces()
{
  mShipTForceForw->initialState();
  mShipTForceBack->initialState();
  mShipTForceLeft->initialState();
  mShipTForceRght->initialState();

  mShipTTorqueUp->initialState();
  mShipTTorqueDown->initialState();
}

void  Ship::moveFwd( const bool aMove, const float aLapsedTime )
{
  TimeForceState state;
  state.t         = aLapsedTime;
  state.advancing = aMove; 
  mShipTForceForw->updateState( state );
}
void  Ship::moveBck( const bool aMove, const float aLapsedTime )
{
  TimeForceState state;
  state.t         = aLapsedTime;
  state.advancing = aMove; 
  mShipTForceBack->updateState( state );
}
void  Ship::moveLft( const bool aMove, const float aLapsedTime )
{
  TimeForceState state;
  state.t         = aLapsedTime;
  state.advancing = aMove; 
  mShipTForceLeft->updateState( state );
}
void  Ship::moveRgt( const bool aMove, const float aLapsedTime )
{
  TimeForceState state;
  state.t         = aLapsedTime;
  state.advancing = aMove; 
  mShipTForceRght->updateState( state );
}

void  Ship::pitchUp( const bool aMove, const float aLapsedTime )
{
  TimeTorqueState state;
  state.t         = aLapsedTime;
  if (! aMove ) state.t = 0.0f;
  mShipTTorqueUp->setState( state );
}

void  Ship::pitchDown( const bool aMove, const float aLapsedTime )
{
  TimeTorqueState state;
  state.t         = aLapsedTime;  
  if (! aMove ) state.t = 0.0f;
  mShipTTorqueDown->setState( state );
}

void  Ship::turnLeft( const bool aMove, const float aLapsedTime )
{
  TimeTorqueState state;
  state.t         = aLapsedTime;  
  if (! aMove ) state.t = 0.0f;
  mShipTTorqueLeft->setState( state );
}

void  Ship::turnRight( const bool aMove, const float aLapsedTime )
{
  TimeTorqueState state;
  state.t         = aLapsedTime;
  if (! aMove ) state.t = 0.0f;
  mShipTTorqueRight->setState( state );
}

void  Ship::display() const
{
    /*
    D3DXMatrixTranslation(  &Global::mWorld, 
                            mShipBody->xWS.x,
                            mShipBody->xWS.y,
                            mShipBody->xWS.z);

    D3DXMATRIX matrixRotation;

    D3DXMatrixRotationQuaternion( &matrixRotation, &mShipBody->qWS );

    Global::mWorld = matrixRotation*Global::mWorld;
    D3DXMATRIX m;
    //D3DXMatrixScaling(&m, 0.1, 0.1, 0.1);
    D3DXMatrixScaling(&m, 0.2, 0.2, 0.2);
    Global::mWorld = m*Global::mWorld;    
    
    mShipModel->display();
    */
}

int   Ship::id() const
{
  return mBody->mId;
}

D3DXVECTOR3 Ship::pointingUnit() const
{
  return mPointingDirection;
}