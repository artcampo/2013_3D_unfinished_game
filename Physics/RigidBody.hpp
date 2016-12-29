#ifndef _RIGIDBODY_HPP_
#define _RIGIDBODY_HPP_

#include <d3dx10.h>
#include <dxerr.h>
#include <string>
#include  <stdio.h>
#include <vector>



#include	"dx_misc.hpp"
#include  "Noise/RandomNumberGenerator.hpp"
#include	"Physics/PhysicsForces.hpp"
#include	"Physics/PhysicsTorques.hpp"
#include	"Physics/RigidBodyExt.hpp"

//===========================================================================
/*
     Individual Body.
     This class relies on the use of pointers to forces to avoid 
     shallow copies
*/
//===========================================================================
class RigidBody{
public:
    
  RigidBody(  const D3DXVECTOR3& aPositionWS,                
              const int aIdconst,
              RigidBodyExt* aBodyExt );

  void          standstill();
  void          setVelWS( const D3DXVECTOR3& aVelWS );

  bool          isInRestingContact() const;
  bool          hasUpwardsNormalOfContact() const;
  D3DXVECTOR3   upwardsNormalOfContact() const;
  D3DXVECTOR3   getvWS();
  D3DXVECTOR3   getxWS() const;
  void          setMaxVelocity( const float aMaxVelocity );

  void          limitVelocity();

  float         boundingRadius() const;

  // WARNING: this should be private!
  void computeTemporalValues ();

public:  
  int             mId;
  RigidBodyExt*   mBodyExt;

public:

  bool        mIsInRestingContact;
  std::vector<D3DXVECTOR3> mContactNormal;

  //  State variables
  D3DXVECTOR3     xWS;   /*!< Position*/
  D3DXQUATERNION  qWS;   /*!< Rotation quaternion*/
  D3DXVECTOR3     pWS;   /*!< Linear Momentum*/
  D3DXVECTOR3     lWS;   /*!< Angular Momentum*/
private:
  //  Obtained via dtdx, valid for a given step
  D3DXVECTOR3   tWS;    /*!< Torque*/
  D3DXVECTOR3   fWS;    /*!< Force*/

  //  Computed temporally, used only while integrating
  D3DXMATRIX    iInvWS; /*!< Inverse of inertia tensor*/
  D3DXMATRIX    rWS;    /*!< Rotation matrix*/
  D3DXVECTOR3   vWS;    /*!< Linear velocity*/
  D3DXVECTOR3   wWS;    /*!< Angular velocity*/


  friend class RigidBodyOde;
  friend class Physics;
  friend class Ball;

};


#endif