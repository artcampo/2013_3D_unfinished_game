#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include  "dx_misc.hpp"
#include  <memory>

class Physics;
class RigidBody;
class TimeForce;
class TimeTorque;


class ShipDescription{
  public:        
    float fwdModule;
    float bckModule;
    float sidModule;      
    float pitchModule;
    float yawModule;
};

class Ship {
public:  
  Ship( Physics&                aPhysics,
        const D3DXVECTOR3&      aInitPosition,
        const ShipDescription&  aShipDesc );
  ~Ship();

  void  display() const;
  void  update( const float aLapsedTime );
  void  clearForces();

  //  move functions to be used by both player and IA
  void  moveFwd( const bool aMove, const float aLapsedTime );
  void  moveBck( const bool aMove, const float aLapsedTime );
  void  moveLft( const bool aMove, const float aLapsedTime );
  void  moveRgt( const bool aMove, const float aLapsedTime );
  void  pitchUp( const bool aMove, const float aLapsedTime );
  void  pitchDown( const bool aMove, const float aLapsedTime );
  void  turnRight( const bool aMove, const float aLapsedTime );
  void  turnLeft( const bool aMove, const float aLapsedTime );

  int   id() const;
  
  //  Render
public:
  std::auto_ptr<RigidBody>      mBody;

  //  Static description
private:

  
  D3DXVECTOR3       mPointingDirection;
  D3DXVECTOR3       mCrossDirection;
  ShipDescription   mShipDesc;
  //  Physics vars
private:
  
  std::auto_ptr<TimeForce>      mShipTForceForw;
  std::auto_ptr<TimeForce>      mShipTForceBack;
  std::auto_ptr<TimeForce>      mShipTForceLeft;
  std::auto_ptr<TimeForce>      mShipTForceRght;
  std::auto_ptr<TimeTorque>     mShipTTorqueUp;
  std::auto_ptr<TimeTorque>     mShipTTorqueDown;  
  std::auto_ptr<TimeTorque>     mShipTTorqueLeft;
  std::auto_ptr<TimeTorque>     mShipTTorqueRight;
  


public:
  D3DXVECTOR3 pointingUnit() const;
  D3DXVECTOR3 movementVector() const;

private:
  void updateCameraParameters();

  friend class Ship_IA;
  

};


#endif  //_SHIP_HPP_
