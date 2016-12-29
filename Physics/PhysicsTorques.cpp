#include	"Physics/PhysicsTorques.hpp"
#include  "Geometry/Spline/SplineLengthMapped.hpp"
#include  "Project_Dependant/StoredSplines.hpp"

//===========================================================================
/*!
    Constructor.
    \param  aApplicationVector  in which direction does hte force act
*/
//===========================================================================

TimeTorque::TimeTorque( D3DXVECTOR3* aApplicationPositionLS, D3DXVECTOR3* aForceVectorLS, StoredSplineDesc* aFunction, const float aModule  )
{
  mModule   = aModule;
  mT        = 0;
  mDeltaT   = 0;
  mTimeMax  = 2;

  mApplicationPositionLS  = *aApplicationPositionLS;
  mForceVectorLS          = *aForceVectorLS;
  mFunction = std::auto_ptr<SplineLengthMapped> ( new SplineLengthMapped( aFunction ) );
}


//===========================================================================
/*!
    Updates the state of the force: in which time position of its
    own space it is, and if it's going forward on time
    \param      aState    New state: time ellapsed since last update
*/
//===========================================================================


void    TimeTorque::setState ( TimeTorqueState aState )
{
  mT = aState.t;  
   
  //  Check bounds
  if ( mT < 0.0f ) mT = 0.0f;
  if ( mT > mTimeMax ) mT = mTimeMax;
}
  
//===========================================================================
/*!
    Sets the force to its repose state (initial).
*/
//===========================================================================

void TimeTorque::initialState ()
{
  mT = 0;
}

//===========================================================================
/*!
    Gets the force at aDeltaT
    \param      aDeltaT     time position, simulation space
    \param      aWorld      matrix that transforms LS into WS
*/
//===========================================================================

D3DXVECTOR3 TimeTorque::getTorque ( float aDeltaT, const D3DXMATRIX *aWorld )
{
  D3DXVECTOR3 ret;


  float interpolation = mFunction->getPoint( mT/mTimeMax );  
  float module = mModule*interpolation;  

  D3DXVECTOR3 fLS = mForceVectorLS*module;
  D3DXVECTOR3 posWS, fWS;

  D3DXVec3TransformCoord ( &posWS, &mApplicationPositionLS, aWorld );
  D3DXVec3TransformCoord ( &fWS, &fLS, aWorld );

  D3DXVec3Cross ( &ret, &posWS, &fWS ); 

  return ret;
}

float TimeTorque::getT () const
{
  return mT;
}
float TimeTorque::getTimeMax () const
{
  return mTimeMax;
}