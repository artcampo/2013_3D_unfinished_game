#ifndef _RIGIDBODYODE_HPP_
#define _RIGIDBODYODE_HPP_

#include <d3dx10.h>
#include <dxerr.h>
#include <string>
#include  <stdio.h>
#include <vector>

#include  "Project_Dependant/LogConditionalCompilation.hpp"
#ifdef _LOG_ENABLE_
  class Log;
  #include <memory>
#endif


#include	"dx_misc.hpp"
#include  "Noise/RandomNumberGenerator.hpp"
#include	"Physics/PhysicsForces.hpp"
#include	"Physics/PhysicsTorques.hpp"
#include	"Physics/RigidBody.hpp"
#include  "Geometry/DxMathHelpers.hpp"

//DEBUG
#include  "Global.hpp"

//===========================================================================
/*!
      ODE for rigid bodies
      \brief      Integration of movement for rigid bodies.
*/
//===========================================================================
class RigidBodyOde
{
public:
  ~RigidBodyOde() { delete mLog;}

  RigidBodyOde ( std::vector<RigidBody*>& aBodies, int aNumBodies, float aTime,
                            float aGravityAcc, float aContactDotEpsilon, const float aMinimumForceIncrement );
  void Step ( float aDeltaT );

  //  Objects
private:
  
  std::vector<RigidBody*>   mBodies;
  int                       mNumBodies;
  float                     mTime;
  float                     mMinimumForceIncrement;

#ifdef _LOG_ENABLE_
  Log*                  mLog;
#endif

  //  constants
private:
  float     mGravityAcc;  
  float     mContactDotEpsilon;

  //  Data inherent to simulation  
  const static int  mStateSize = 13;   /*!< num of floats that takes particle's state*/

  //  Typedefs and define to simplify access thru pointer
  typedef  void (RigidBodyOde::*FuncPoint)( float, float [], float [], float );
  #define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember)) 
  
  //  Functions for numerical solving
  void dxdt ( float aT, float aX[], float aXdot[], float deltaT );
  void stateToArray ( float aX[] );
  void arrayToState ( float aX[], std::vector<RigidBody*>& aBodies );
  void odeSolverEuler ( float aX0[], float aXF[], float aT, float aTF,
                            FuncPoint aDxdt );

  

};



#endif