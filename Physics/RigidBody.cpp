#include	"Physics/RigidBody.hpp"

//===========================================================================
/*!
    Includes a time-force to body.   

*/
//===========================================================================



D3DXVECTOR3   RigidBody::getxWS () const
{
  return xWS;
}

D3DXVECTOR3   RigidBody::getvWS ()
{
  computeTemporalValues();
  return vWS;
}

void RigidBody::computeTemporalValues ()
{
  vWS    = pWS / mBodyExt->m;

  D3DXMatrixRotationQuaternion( &rWS, &qWS );
  D3DXMATRIX rotationTransposed;
  D3DXMatrixTranspose ( &rotationTransposed, &rWS );    
  iInvWS = rWS * mBodyExt->iLS * rotationTransposed;

  D3DXVECTOR4 tempVector;
  D3DXVec3Transform ( &tempVector, &lWS, &iInvWS );
  wWS = D3DXVECTOR3( tempVector.x, tempVector.y, tempVector.z );
}

void RigidBody::setVelWS( const D3DXVECTOR3& aVelWS )
{
  pWS = aVelWS*mBodyExt->m;
}

bool    RigidBody::isInRestingContact() const
{
  return mIsInRestingContact;
}

bool          RigidBody::hasUpwardsNormalOfContact() const
{
  if ( !isInRestingContact() )
    return false;
  else
  {
    bool has = false;
    for ( int i = 0; i < mContactNormal.size() ; ++i )
      if ( mContactNormal[i].y > 0.0f )
        has = true;
    return has;
  }
}

D3DXVECTOR3   RigidBody::upwardsNormalOfContact() const
{
  D3DXVECTOR3 ret = D3DXVECTOR3(0,0,0);
  for ( int i = 0; i < mContactNormal.size() ; ++i )
    if ( mContactNormal[i].y > 0.5f )
      ret = mContactNormal[i];
  return ret;
}

void RigidBody::limitVelocity()
{
  D3DXVECTOR3 v;
  v = getvWS();
  float l = D3DXVec3Length ( &v );
  if ( l > mBodyExt->mMaxVelocity )
  {
    D3DXVec3Normalize( &v, &v );
    v = v*mBodyExt->mMaxVelocity;
    setVelWS( v );
  }
}

RigidBody::RigidBody( const D3DXVECTOR3& aPositionWS,                       
                      const int aId,                      
                      RigidBodyExt* aBodyExt )
                      : mId (aId),
                        mBodyExt(aBodyExt),
                        mIsInRestingContact(false),
                        xWS( aPositionWS)
{     
  standstill();
}

void  RigidBody::setMaxVelocity( const float aMaxVelocity )
{
  mBodyExt->setMaxVelocity( aMaxVelocity );
}

void  RigidBody::standstill()
{
  qWS = D3DXQUATERNION(
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f
                      );
  pWS = D3DXVECTOR3(
                      0.0f,
                      0.0f,
                      0.0f
                      );
  lWS = D3DXVECTOR3(
                      0.0f,
                      0.0f,
                      0.0f
                      );
}

float         RigidBody::boundingRadius() const
{
  return mBodyExt->mBoundingSphereRadius;
}
