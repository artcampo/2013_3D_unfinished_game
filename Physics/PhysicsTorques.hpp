#ifndef _PHYSICS_TORQUES_HPP_
#define _PHYSICS_TORQUES_HPP_

#include  "dx_misc.hpp"
#include  <memory>

class SplineLengthMapped;
class StoredSplineDesc;


//===========================================================================
/*

     Individual particle
*/
//===========================================================================
struct TimeTorqueState{
  //  Constants
  float     t;        /*!< Current time in force space*/  
};

 

//===========================================================================
/*!
      Time Force
      \brief      A force which varies over time, which is described by
                  a two dimensional function.
*/
//===========================================================================
class TimeTorque
{
public:

  //int                       mNumBodies; /*!< Number of particles */
  //std::vector<RigidBody>    mBodies;   /*!< Vector of particles*/

  TimeTorque( D3DXVECTOR3* aApplicationVectorLS, D3DXVECTOR3* aForceVectorLS, StoredSplineDesc* aFunction, const float aModule );

  //void          updateState ( TimeTorqueState aState );
  void          setState ( TimeTorqueState aState );
  void          initialState ();
  D3DXVECTOR3   getTorque ( float aDeltaT, const D3DXMATRIX* aWorld );

  float getT () const;
  float getTimeMax () const;

private:

  float           mModule;
  float           mT;         /*!< Current time in force space*/
  float           mDeltaT;    /*!< Current time delta multiplier */
  float           mTimeMax;   /*!< Maximum time in force space*/

  D3DXVECTOR3     mApplicationPositionLS; /*!< in which point does the force act*/
  D3DXVECTOR3     mForceVectorLS;     /*!< in which direction does the force act*/
  std::auto_ptr<SplineLengthMapped>    mFunction; /*!< Spline with f(t)*/

  friend class Ball;
};



#endif