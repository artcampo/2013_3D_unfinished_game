#include	"Physics/RigidBodyExt.hpp"
#include	"Physics/PhysicsForces.hpp"
#include	"Physics/PhysicsTorques.hpp"
#include  "Geometry/Spline/SplineLengthMapped.hpp"
#include  "Project_Dependant/StoredSplines.hpp"


RigidBodyExt::RigidBodyExt( const float aMass,                 
                            const float aBoundingRadius,
                            const float aDragForce,
                            const float aBounceCoefficient )
{

  mHasMesh  = false;  
  
  mDragForce            = aDragForce;
  mBounceCoefficient    = aBounceCoefficient;  
  mBoundingSphereRadius = aBoundingRadius;  
  m                     = aMass;

  mMaxVelocity = 1000;

  iInvLS = D3DXMATRIX (
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    );

  iLS = D3DXMATRIX (
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    );

}


std::auto_ptr<TimeForce>  RigidBodyExt::attachTimeForce    ( D3DXVECTOR3* aApplicationVector, StoredSplineDesc* aFunction, const float aModule )
{  
  int index = mTimeForces.size();
  mTimeForces.resize ( index + 1 );
  mTimeForces[ index ] = new TimeForce( aApplicationVector, aFunction, aModule) ;
  return std::auto_ptr<TimeForce> ( mTimeForces[ index ] );
}

//===========================================================================
/*!
    Includes a time-Torque to body.   
*/
//===========================================================================
std::auto_ptr<TimeTorque> RigidBodyExt::attachTimeTorque   ( D3DXVECTOR3* aApplicationVectorLS, D3DXVECTOR3* aForceVectorLS, StoredSplineDesc* aFunction, const float aModule )

{
  int index = mTimeTorques.size();
  mTimeTorques.resize ( index + 1 );
  mTimeTorques[ index ] = new TimeTorque( aApplicationVectorLS, aForceVectorLS, aFunction, aModule );
  return std::auto_ptr<TimeTorque> ( mTimeTorques[ index ] );
}


void  RigidBodyExt::setMaxVelocity( const float aMaxVelocity )
{
  mMaxVelocity = aMaxVelocity;
}
