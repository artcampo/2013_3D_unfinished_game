#ifndef _RIGIDBODYEXT_HPP_
#define _RIGIDBODYEXT_HPP_

#include <d3dx10.h>
#include <dxerr.h>
#include <string>
#include  <stdio.h>
#include <vector>



#include	"dx_misc.hpp"
#include  "Noise/RandomNumberGenerator.hpp"

class StoredSplineDesc;
class TimeForce;
class TimeTorque;
class StaticRigidBody;

//===========================================================================
/*
     Individual Body.
     This class relies on the use of pointers to forces to avoid 
     shallow copies
*/
//===========================================================================
class RigidBodyExt{
public:
  
  RigidBodyExt( const float aMass,                 
                const float aBoundingRadius,
                const float aDragForce,
                const float aBounceCoefficient );

  std::auto_ptr<TimeForce>  attachTimeForce    ( D3DXVECTOR3* aApplicationVector, StoredSplineDesc* aFunction, const float aModule );
  std::auto_ptr<TimeTorque> attachTimeTorque   ( D3DXVECTOR3* aApplicationVectorLS, D3DXVECTOR3* aForceVectorLS, StoredSplineDesc* aFunction, const float aModule );

  void          setMaxVelocity( const float aMaxVelocity );

private:
  bool          mHasMesh;  
  float         mMaxVelocity;

  float         m;      /*!< Mass*/
  D3DXMATRIX    iLS;    /*!< Inertia tensor*/
  D3DXMATRIX    iInvLS; /*!< Inverse of inertia tensor*/
  float         mDragForce;  /*Drag against linear velocity*/
  float         mBounceCoefficient; /*!< Bounciness of collision*/
  
  std::vector <TimeForce*>  mTimeForces; /*!< Vector of vectors: for each object, their time-forces*/
  std::vector <TimeTorque*> mTimeTorques; /*!< Vector of vectors: for each object, their time-forces*/
  
  float     mBoundingSphereRadius;

  //debug
  StaticRigidBody*  mPreviousContactSurface;

public:
  
  friend class RigidBody;
  friend class Ball;
  friend class RigidBodyOde;
  friend class Physics;


};


#endif