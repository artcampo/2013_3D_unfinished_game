#include	"Physics/RigidBodyOde.hpp"

#include  "Project_Dependant/LogConditionalCompilation.hpp"

//#define _DEBUG_LOG_BODIES_EXTENDED
#define _DEBUG_HIDE_FORCES
#define _DEBUG_SHOW_ONLY_DOT_INFO


#ifdef _LOG_ENABLE_
  #include  "Misc/Log.hpp"
#endif


//===========================================================================
/*!
    Constructor: prepare the integration
    \param      aBodies       pointer to vector with bodies
    \param      aNumBodies    Number of rigidBodys to add
    \param      aTime         initial time of integration
*/
//===========================================================================
RigidBodyOde::RigidBodyOde ( std::vector<RigidBody*>& aBodies, int aNumBodies, float aTime,
                            float aGravityAcc, float aContactDotEpsilon, const float aMinimumForceIncrement )
{  
  mBodies     = aBodies;
  mNumBodies  = aNumBodies;
  mTime       = aTime;
  mGravityAcc = aGravityAcc;  
  mContactDotEpsilon = aContactDotEpsilon;
  mMinimumForceIncrement = aMinimumForceIncrement;

#ifdef _LOG_ENABLE_
  mLog = new Log();
#endif
}


//===========================================================================
/*!
    Advances an step of the simulation.

    \param     aDeltaT  increment of time
*/
//===========================================================================
void RigidBodyOde::Step ( float aDeltaT )
{
  float* initialState = new float [ mNumBodies * mStateSize ];
  float* finalState   = new float [ mNumBodies * mStateSize ];

  stateToArray ( initialState );

  odeSolverEuler( initialState, finalState, mTime, mTime + aDeltaT, &RigidBodyOde::dxdt );
           
  arrayToState (finalState, mBodies );

  mTime = mTime + aDeltaT;

  delete [] initialState;
  delete [] finalState;
}

//===========================================================================
/*!
    Computes derivative of mNumBodies rigidBodys.
    From (aT,aX) it computes  (aXdot)

    \param      aT    time
    \param      aX    state of rigidBodys (must be already allocated)
    \param      aXdot derivatives of rigidBodys (must be already allocated)
*/
//===========================================================================
void RigidBodyOde::dxdt ( float aT, float aX[], float aXdot[], float deltaT )
{

  std::vector<RigidBody*> rigidBodys ( mNumBodies );
  for ( int i = 0; i < mNumBodies; ++i )  
    rigidBodys[i] = new RigidBody( *mBodies[i] );    
  

  //  Copy state to temp rigidBodys
  arrayToState( aX, rigidBodys );

  //  For each RigidBody compute force
  for ( int i = 0; i < mNumBodies ; ++i)
  {
    //rigidBodys[i]->computeTemporalValues();
    rigidBodys[i]->fWS = D3DXVECTOR3( 0.0f , 0.0f , 0.0f  );    

    for ( unsigned int j = 0; j < mBodies[i]->mBodyExt->mTimeForces.size() ; ++j)
    {
      D3DXMATRIX ident;
      D3DXMatrixIdentity( &ident );
      //rigidBodys[i]->fWS += mBodies[i]->mBodyExt->mTimeForces[j]->getForce( aT, &ident );    
      //  forces supplied on object space
      rigidBodys[i]->fWS += mBodies[i]->mBodyExt->mTimeForces[j]->getForce( aT, &rigidBodys[i]->rWS );    
    }

#ifdef _LOG_ENABLE_PHYSICS_PLAYER_FORCE
  *mLog->mFileShipPlayer<< "F: " 
                        << rigidBodys[i]->fWS.x 
                        << ", "
                        << rigidBodys[i]->fWS.y
                        << ", "
                        << rigidBodys[i]->fWS.z 
                        << std::endl;  
#endif


#ifndef _DEBUG_SHOW_ONLY_DOT_INFO
    *Global::mFileDebugBody[i] 
      << "Object: " << i << " integration, with t: ";
    Global::mFileDebugBody[i]->precision(10);
    *Global::mFileDebugBody[i] 
        << deltaT;
        Global::mFileDebugBody[i]->precision(2);
    *Global::mFileDebugBody[i] 
          << std::endl;

    if ( rigidBodys[i]->isInRestingContact() )
      *Global::mFileDebugBody[i]  << " resting contact";
    else
      *Global::mFileDebugBody[i]  << " en l'air";

  #ifndef _DEBUG_HIDE_FORCES
      *Global::mFileDebugBody[i] 
          << std::endl
          << " F acceleration:: " << rigidBodys[i]->fWS.x << ", " << rigidBodys[i]->fWS.y << ", " << rigidBodys[i]->fWS.z << std::endl;
  #endif
#endif



    //  Compute drag force
    mBodies[i]->computeTemporalValues();
    D3DXVECTOR3 dragF = ( (-mBodies[i]->mBodyExt->mDragForce) * mBodies[i]->vWS );
    DxMath::clean( dragF, 0.01f );
    rigidBodys[i]->fWS += dragF;


#ifndef _DEBUG_SHOW_ONLY_DOT_INFO
  #ifndef _DEBUG_HIDE_FORCES
      *Global::mFileDebugBody[i]  
          << " F drag:: " << rigidBodys[i]->fWS.x << ", " << rigidBodys[i]->fWS.y << ", " << rigidBodys[i]->fWS.z << std::endl;
  #endif
  #ifdef _DEBUG_LOG_BODIES_EXTENDED        
      *Global::mFileDebugBody[i]  
          << " -> from: " << rigidBodys[i]->vWS.x << ", " << rigidBodys[i]->vWS.y << ", " << rigidBodys[i]->vWS.z << std::endl
          << " -> and: " << mBodies[i]->mBodyExt->m << std::endl;
  #endif
#endif

	  //	Compute gravity
    rigidBodys[i]->fWS += D3DXVECTOR3( 0.0f , mGravityAcc , 0.0f  ) * mBodies[i]->mBodyExt->m;    

#ifndef _DEBUG_SHOW_ONLY_DOT_INFO
  #ifndef _DEBUG_HIDE_FORCES
  *Global::mFileDebugBody[i]  
          << " F gravity:: " << rigidBodys[i]->fWS.x << ", " << rigidBodys[i]->fWS.y << ", " << rigidBodys[i]->fWS.z << std::endl;
  #endif
#endif

    ///////////////////////////
    //  Compute toques
    rigidBodys[i]->tWS = D3DXVECTOR3( 0.0f , 0.0f , 0.0f  );


    for ( unsigned int j = 0; j < mBodies[i]->mBodyExt->mTimeTorques.size() ; ++j)
    {
      rigidBodys[i]->tWS += mBodies[i]->mBodyExt->mTimeTorques[j]->getTorque( aT, &rigidBodys[i]->rWS );


      #ifdef _LOG_ENABLE_PHYSICS_PLAYER_TORQUE_SPLINE
          if ( i == 0 )
          *mLog->mFileShipPlayer
                              << "TSpline: " 
                              << mBodies[0]->mBodyExt->mTimeTorques[j]->getT()
                              << " / " 
                              <<  mBodies[0]->mBodyExt->mTimeTorques[j]->getTimeMax()
                              << std::endl;  
      #endif
    }
#ifdef _LOG_ENABLE_PHYSICS_PLAYER_TORQUE
  *mLog->mFileShipPlayer<< "T: " 
                        << rigidBodys[i]->tWS.x 
                        << ", "
                        << rigidBodys[i]->tWS.y
                        << ", "
                        << rigidBodys[i]->tWS.z 
                        << std::endl;  
#endif

    rigidBodys[i]->computeTemporalValues();
    
    if ( rigidBodys[i]->wWS != D3DXVECTOR3(0.0f , 0.0f , 0.0f ) )
    {
      D3DXMATRIX rotationTransposed;
      D3DXMatrixTranspose ( &rotationTransposed, &rigidBodys[i]->rWS );    
      D3DXMATRIX iWS = rigidBodys[i]->rWS * mBodies[i]->mBodyExt->iInvLS * rotationTransposed;

      D3DXVECTOR3 wWS = (-0.75f)*rigidBodys[i]->wWS;
      D3DXVECTOR3 tempVector;
      D3DXVec3TransformCoord ( &tempVector, &wWS, &iWS );      
      tempVector *= 10.0f;
      rigidBodys[i]->tWS += tempVector;

#ifdef _LOG_ENABLE_PHYSICS_PLAYER_TORQUE_DRAG
  *mLog->mFileShipPlayer<< "Tdrag: " 
                        << tempVector.x 
                        << ", "
                        << tempVector.y
                        << ", "
                        << tempVector.z 
                        << std::endl;  
#endif
    }

    //Compute any counter-forces due to resting contact
    if ( mBodies[i]->mIsInRestingContact )         
    {      

      //D3DXQUATERNION ballRotation = D3DXQUATERNION(0,0,0,0);
      for ( int cpoint = 0; cpoint < mBodies[i]->mContactNormal.size(); ++cpoint)      
      {

        //  Clean Force
        float dot = D3DXVec3Dot ( &mBodies[i]->mContactNormal[cpoint], &rigidBodys[i]->fWS );
        //TODO: HARDCODED, but belongs to physics, not ODE!

        //if ( dot < mContactDotEpsilon )        
        {  
          D3DXVECTOR3 counterForce; 
          counterForce = mBodies[i]->mContactNormal[cpoint] * fabs( dot );          
          DxMath::clean( counterForce, 0.000000001f );         
          rigidBodys[i]->fWS += counterForce;    
          DxMath::clean( rigidBodys[i]->fWS, mMinimumForceIncrement );

        }

        //  Clean velocity
        D3DXVECTOR3 v = rigidBodys[i]->vWS;
        //DxMath::clean( v );
        float proj = D3DXVec3Dot( &v, &rigidBodys[i]->mContactNormal[cpoint] );

        {          
          D3DXVECTOR3 unit  = rigidBodys[i]->mContactNormal[cpoint];
          D3DXVECTOR3 vn    = unit * proj;	        
          v += vn;
          DxMath::clean( v, mMinimumForceIncrement );
          rigidBodys[i]->setVelWS( v );
          rigidBodys[i]->computeTemporalValues();
         
        }

      //  end of process resting contact
      }
    }    
#ifndef _DEBUG_SHOW_ONLY_DOT_INFO    
  #ifndef _DEBUG_HIDE_FORCES
      *Global::mFileDebugBody[i]  
          << " F final:: " << rigidBodys[i]->fWS.x << ", " << rigidBodys[i]->fWS.y << ", " << rigidBodys[i]->fWS.z << std::endl;
  #endif
#endif

  }


  //  Write out derivatives
  for ( int i = 0; i < mNumBodies ; ++i)
  {
    aXdot[ i*mStateSize + 0] = rigidBodys[i]->vWS.x;
    aXdot[ i*mStateSize + 1] = rigidBodys[i]->vWS.y;
    aXdot[ i*mStateSize + 2] = rigidBodys[i]->vWS.z;

    D3DXQUATERNION  tempQuat;
    D3DXQuaternionMultiply( &tempQuat, 
                            &D3DXQUATERNION( rigidBodys[i]->wWS.x, rigidBodys[i]->wWS.y, rigidBodys[i]->wWS.z, 0.0f),
                            &rigidBodys[i]->qWS );
    tempQuat *= 0.5f;

    aXdot[ i*mStateSize + 3] = tempQuat.x;
    aXdot[ i*mStateSize + 4] = tempQuat.y;
    aXdot[ i*mStateSize + 5] = tempQuat.z;
    aXdot[ i*mStateSize + 6] = tempQuat.w;

    aXdot[ i*mStateSize + 7] = rigidBodys[i]->fWS.x;
    aXdot[ i*mStateSize + 8] = rigidBodys[i]->fWS.y;
    aXdot[ i*mStateSize + 9] = rigidBodys[i]->fWS.z;

    aXdot[ i*mStateSize + 10] = rigidBodys[i]->tWS.x;
    aXdot[ i*mStateSize + 11] = rigidBodys[i]->tWS.y;
    aXdot[ i*mStateSize + 12] = rigidBodys[i]->tWS.z;
  }

  for ( int i = 0; i < mNumBodies; ++i )  
    delete rigidBodys[i];
}

//===========================================================================
/*!
    Dumps RigidBody's state to an array    
    \param      aX    state of rigidBodys (must be already allocated)
    
*/
//===========================================================================
void RigidBodyOde::stateToArray ( float aX[] )
{
  for ( int i = 0; i < mNumBodies ; ++i)
  {
    aX[ i*mStateSize + 0] = mBodies[i]->xWS.x;
    aX[ i*mStateSize + 1] = mBodies[i]->xWS.y;
    aX[ i*mStateSize + 2] = mBodies[i]->xWS.z;    

    aX[ i*mStateSize + 3] = mBodies[i]->qWS.x;
    aX[ i*mStateSize + 4] = mBodies[i]->qWS.y;
    aX[ i*mStateSize + 5] = mBodies[i]->qWS.z;
    aX[ i*mStateSize + 6] = mBodies[i]->qWS.w;

    aX[ i*mStateSize + 7] = mBodies[i]->pWS.x;
    aX[ i*mStateSize + 8] = mBodies[i]->pWS.y;
    aX[ i*mStateSize + 9] = mBodies[i]->pWS.z;    

    aX[ i*mStateSize + 10] = mBodies[i]->lWS.x;
    aX[ i*mStateSize + 11] = mBodies[i]->lWS.y;
    aX[ i*mStateSize + 12] = mBodies[i]->lWS.z;    
  }
}


//===========================================================================
/*!
    Recovers RigidBody's state from an array    
    Take care as mBodies's constants are read from here.
    \param      aX      state of rigidBodys (must be already allocated)
    \param      aBodies pointer to consecutive bodies where to store
    
*/
//===========================================================================
void RigidBodyOde::arrayToState ( float aX[], std::vector<RigidBody*>& aBodies )
{
  for ( int i = 0; i < mNumBodies ; ++i )
  {
    aBodies[i]->xWS = D3DXVECTOR3( aX[ i*mStateSize + 0 ], aX[ i*mStateSize + 1 ], aX[ i*mStateSize + 2 ] );
    aBodies[i]->qWS = D3DXQUATERNION( aX[ i*mStateSize + 3 ], aX[ i*mStateSize + 4 ], aX[ i*mStateSize + 5 ], aX[ i*mStateSize + 6 ] );
    aBodies[i]->pWS = D3DXVECTOR3( aX[ i*mStateSize + 7 ], aX[ i*mStateSize + 8 ], aX[ i*mStateSize + 9 ] );
    aBodies[i]->lWS = D3DXVECTOR3( aX[ i*mStateSize + 10 ], aX[ i*mStateSize + 11 ], aX[ i*mStateSize + 12 ] );

    //  Compute temporal values
    aBodies[i]->vWS    = aBodies[i]->pWS / mBodies[i]->mBodyExt->m;

    D3DXMatrixRotationQuaternion( &aBodies[i]->rWS, &mBodies[i]->qWS );
    D3DXMATRIX rotationTransposed;
    D3DXMatrixTranspose ( &rotationTransposed, &aBodies[i]->rWS );    
    aBodies[i]->iInvWS = aBodies[i]->rWS * mBodies[i]->mBodyExt->iLS * rotationTransposed;

    D3DXVECTOR4 tempVector;
    D3DXVec3Transform ( &tempVector, &aBodies[i]->lWS, &aBodies[i]->iInvWS );
    aBodies[i]->wWS = D3DXVECTOR3( tempVector.x, tempVector.y, tempVector.z );
  }
}

//===========================================================================
/*!
    Numerical solver: Euler.
    \param      aX0   initial values
    \param      aXF   final values
    \param      aT    intial time
    \param      aTF   final time
    \param      aDxdt function that returns the derivatives of a given state
    
*/
//===========================================================================
void RigidBodyOde::odeSolverEuler ( float aX0[], float aXF[], float aT, float aTF,
                            FuncPoint aDxdt  )
{
  float* derivatives = new float [ mNumBodies * mStateSize ];

  //(RigidSys::*aDxdt)( aT, aX0, derivatives );

  float deltaT = aTF - aT;
  CALL_MEMBER_FN( *this, aDxdt )( aT, aX0, derivatives, deltaT );
  

  for ( int i = 0; i < mNumBodies ; ++i)
  {
    for ( int j = 0; j < mStateSize ; ++j)
      aXF[ i*mStateSize + j] = aX0[i*mStateSize + j] + derivatives[i*mStateSize + j]*deltaT;
  }
  delete[] derivatives;
}
