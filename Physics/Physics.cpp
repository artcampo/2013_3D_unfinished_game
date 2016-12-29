#include  "Physics/Physics.hpp"

#include	"Physics/RigidBody.hpp"
#include	"Physics/RigidBodyExt.hpp"
#include	"Physics/RigidBodyOde.hpp"
#include	"Physics/StaticRigidBody.hpp"
#include	"Physics/MeshCol.hpp"
#include  "ObjEditor/Mesh/Mesh.hpp"
#include  "Geometry/2dDataStructures/KddTree2d.hpp"
#include  "Geometry/DxMathHelpers.hpp"

#ifdef _LOG_ENABLE_
  #include  "Misc/Log.hpp"
#endif

struct PairBodies{
  int               a;  
  bool              bIsDynamic;
  int               b;
  StaticRigidBody*  bStatic;
};

class Precision{
public:
  float   mEpsilonBarycentric;
  int     mIterationsReset;
};

struct Contact{
  D3DXVECTOR3 pointWS;      //  Point in first object
  D3DXVECTOR3 normalWS;     //  Going outwards from second object
  DxMath::Triangle    offender;     //  Triangle that has caused collision/contact
  float DEBUG_d;
  int   DEBUG_aId;
  int   DEBUG_bId;
  float DEBUG_dot;
};


Physics::Physics()
{
  mTime                       = 0.0f;
  mEpsilon                    = 0.5f;  
  mInterpenetrationThreshold  = 1.1f;
  mEpsilonBarycentric         = 0.1f;
  mContactDotEpsilon          = 0.5f;

  mMinimumTimeOfCollision     = 0.0000000001f;
  mMinimumTimeOfCollisionStep = 0.00001f;
  mMinimumForceIncrement      = 0.0000000001f;

  mBroadPhaseDistanceEpsilon = 2.5;  
  mDebugFrame = 0;

#ifdef _LOG_ENABLE_
  mLog = new Log();
#endif
  
}

void Physics::setParameters( float aGravityAcc, float aMaxStepTime )
{
  mGravityAcc   = aGravityAcc;
  mMaxStepTime  = aMaxStepTime;
}

Physics::~Physics()
{
  //TODO: smart or release:
  //std::vector<StaticRigidBody*> mStaBodies
  delete mLog;
}



//===========================================================================
/*!
    creates a new static rigid body
       
*/
//===========================================================================
void Physics::createStaticRigidBody ( Mesh* aMesh )
{
  //  Allocate
  int index = numStaBodies();
  mStaBodies.push_back ( new StaticRigidBody( aMesh ) ); 
  mStaBodies[index]->mDebugId = index;
}

void Physics::createStaticRigidBody ( MeshCol* aMesh )
{
  //  Allocate
  int index = numStaBodies();
  mStaBodies.push_back ( new StaticRigidBody( aMesh ) );
  mStaBodies[index]->mDebugId = index;
}





//===========================================================================
/*!
    creates a new rigid body
    \return     pointer to new rigid body
    
*/
//===========================================================================
std::auto_ptr<RigidBody> Physics::createRigidBody ( const D3DXVECTOR3& aPositionWS, 
                                      const float aMass,
                                      const float aBoundingRadius,
                                      const float aDragForce,
                                      const float aBounceCoefficient )
{
  //  Allocate
  int index = numDynBodies();

  mDynBodiesLastPos.resize( index + 1 );  
  mDynBodiesLastPos[index]  = aPositionWS;

  mDynBodiesExt.push_back( new RigidBodyExt( aMass, aBoundingRadius, aDragForce, aBounceCoefficient ) );
  mDynBodies.push_back( new RigidBody( aPositionWS, index, mDynBodiesExt[index] ) );  

  mPrecisionPerBody.resize( index + 1 );  
  mPrecisionPerBody[index].mEpsilonBarycentric = mEpsilonBarycentric;
  mPrecisionPerBody[index].mIterationsReset    = 0;

  return std::auto_ptr<RigidBody> ( mDynBodies[index] );
}

//===========================================================================
/*!
    Given a time-point of the simulation where two objects collide,
    new impulses are applied to take them away
    \param  aObjects  Pair of colliding objects
*/
//===========================================================================
void Physics::resolveCollision( RigidBody* aBodyA, RigidBody* aBodyB, const Contact& aContact )
{
  aBodyA->computeTemporalValues();
  aBodyB->computeTemporalValues();

  D3DXVECTOR3 paDot = pointVelocity( aBodyA, aContact.pointWS );
  D3DXVECTOR3 pbDot = pointVelocity( aBodyB, aContact.pointWS );
  D3DXVECTOR3 n = aContact.normalWS;
  D3DXVECTOR3 ra = aContact.pointWS - aBodyA->xWS;
  D3DXVECTOR3 rb = aContact.pointWS - aBodyB->xWS;

  D3DXVECTOR3 pabDot = paDot - pbDot;
  double vRel = D3DXVec3Dot( &n, &pabDot );
  float bounceCoefficient = ( aBodyA->mBodyExt->mBounceCoefficient + aBodyB->mBodyExt->mBounceCoefficient ) * 0.5f ;
  double numerator = (1.0f + bounceCoefficient) * ( -1.0f * vRel);

  double term1 = 1.0f / ( aBodyA->mBodyExt->m );
  double term2 = 1.0f / ( aBodyB->mBodyExt->m );

  D3DXVECTOR3 out;
  D3DXVec3Cross( &out, &ra, &n );
  D3DXVec3TransformCoord( &out, &out, &aBodyA->iInvWS );
  D3DXVec3Cross( &out, &out, &ra );
  double term3 = D3DXVec3Dot( &n, &out );

  D3DXVec3Cross( &out, &rb, &n );
  D3DXVec3TransformCoord( &out, &out, &aBodyB->iInvWS );
  D3DXVec3Cross( &out, &out, &rb );
  double term4 = D3DXVec3Dot( &n, &out );
 
  double j = numerator /( term1 + term2 + term3 + term4 );
  D3DXVECTOR3 force = float(j)*n;

  aBodyA->pWS += force;
  aBodyB->pWS -= force;

  D3DXVec3Cross ( &out, &ra, &force);
  aBodyA->lWS += out;
  D3DXVec3Cross ( &out, &rb, &force);
  aBodyB->lWS -= out;

  aBodyA->vWS = aBodyA->pWS / aBodyA->mBodyExt->m;
  aBodyB->vWS = aBodyB->pWS / aBodyB->mBodyExt->m;

  D3DXVec3TransformCoord( &out, &aBodyA->lWS, &aBodyA->iInvWS );
  aBodyA->wWS = out;
  
  D3DXVec3TransformCoord( &out, &aBodyB->lWS, &aBodyB->iInvWS );
  aBodyB->wWS = out;

}

void Physics::resolveCollision( RigidBody* aBodyA, StaticRigidBody* aBodyB, const Contact& aContact )
{
  
  fixContactDegeneracies( aBodyA, aContact );

  aBodyA->computeTemporalValues();  

  D3DXVECTOR3 paDot = pointVelocity( aBodyA, aContact.pointWS );
  D3DXVECTOR3 pbDot = D3DXVECTOR3( 0, 0, 0 );
  D3DXVECTOR3 n = aContact.normalWS;
  D3DXVECTOR3 ra = aContact.pointWS - aBodyA->xWS;
  D3DXVECTOR3 rb = aContact.pointWS;

  D3DXVECTOR3 pabDot = paDot - pbDot;
  double vRel = D3DXVec3Dot( &n, &pabDot );
  double numerator = (1.0f + aBodyA->mBodyExt->mBounceCoefficient) * ( -1.0f * vRel);

  double term1 = 1.0f / ( aBodyA->mBodyExt->m );
  double term2 = 0;

  D3DXVECTOR3 out;
  D3DXVec3Cross( &out, &ra, &n );
  D3DXVec3TransformCoord( &out, &out, &aBodyA->iInvWS );
  D3DXVec3Cross( &out, &out, &ra );
  double term3 = D3DXVec3Dot( &n, &out );


  double term4 = 0;
 
  double j = numerator /( term1 + term2 + term3 + term4 );
  D3DXVECTOR3 force = float(j)*n;

  aBodyA->pWS += force;
  D3DXVec3Cross ( &out, &ra, &force);
  aBodyA->lWS += out;
  aBodyA->vWS = aBodyA->pWS / aBodyA->mBodyExt->m; 
  D3DXVec3TransformCoord( &out, &aBodyA->lWS, &aBodyA->iInvWS );
  aBodyA->wWS = out;
  
}

//===========================================================================
/*!
    Given a time-point of the simulation where two objects collide,
    new impulses are applied to take them away
    \param    aBodyA  Body
    \param    aBodyB  Body
    \returns  point of contact between them
*/
//===========================================================================
Contact Physics::computeContact( RigidBody* aBodyA, RigidBody* aBodyB )
{
  Contact contact;

  aBodyA->computeTemporalValues();
  aBodyB->computeTemporalValues();

  D3DXVECTOR3 normal = aBodyA->xWS - aBodyB->xWS;
  D3DXVec3Normalize( &normal, &normal );

  D3DXVECTOR3 pointOfContact = 
    aBodyA->xWS + ( (-aBodyA->mBodyExt->mBoundingSphereRadius)*normal );

  contact.pointWS   = pointOfContact;
  contact.normalWS  = normal;

  return contact;
}


int Physics::interpenetrationOfContact( RigidBody* aBodyA, RigidBody* aBodyB, const Contact& aContact )
{
  D3DXVECTOR3 paDot = pointVelocity( aBodyA, aContact.pointWS );
  D3DXVECTOR3 pbDot = pointVelocity( aBodyB, aContact.pointWS );
  double vRel = D3DXVec3Dot( &(aContact.normalWS), &(paDot - pbDot) );

  if (vRel > mInterpenetrationThreshold )
    return eContactResult::eContactMovingAway;
  if (vRel > (-mInterpenetrationThreshold) )
    return eContactResult::eContactRestingContact;
  else
    return eContactResult::eContactInterpenetration;
}

int Physics::interpenetrationOfContact( RigidBody* aBodyA, 
                                       StaticRigidBody* aBodyB, 
                                       Contact& aContact,
                                       float& aVrel)
{    

  aBodyA->computeTemporalValues();
  D3DXVECTOR3 paDot = pointVelocity( aBodyA, aContact.pointWS );
  DxMath::clean( paDot, 0.00001f );
  D3DXVECTOR3 pbDot = D3DXVECTOR3( 0, 0, 0 );
  double vRel = D3DXVec3Dot( &(aContact.normalWS), &(paDot - pbDot) );

  aVrel = vRel;
  aContact.DEBUG_dot = vRel;

  int ret;
  if (vRel > mInterpenetrationThreshold )
    ret =  eContactResult::eContactMovingAway;
  else
    if (vRel > (-mInterpenetrationThreshold) )
      ret =  eContactResult::eContactRestingContact;
    else
      ret =  eContactResult::eContactInterpenetration;

  return ret;
}

//===========================================================================
/*!
    Computes the velocity of a given point in a body
    \param    aBody   body to consider
    \param    aPoint  point to consider
    \returns  point velocity
    
*/
//===========================================================================
D3DXVECTOR3 Physics::pointVelocity( RigidBody* aBody, D3DXVECTOR3 aPoint )
{
  aBody->computeTemporalValues();
  D3DXVECTOR3 velocity;
  D3DXVec3Cross( &velocity, &aBody->wWS, &(aPoint - aBody->xWS) );
  velocity = velocity + aBody->vWS;
  return velocity;
}

//===========================================================================
/*!

*/
//===========================================================================
int  Physics::numDynBodies() const
{
  return mDynBodies.size();
}

//===========================================================================
/*!

*/
//===========================================================================
int  Physics::numStaBodies() const
{
  return mStaBodies.size();
}

int Physics::getNumBodies() const
{
  return numDynBodies();
}

//===========================================================================
/*!
    Returns pointer to ith Body
    \returns    pointer to body
    
*/
//===========================================================================
RigidBody*  Physics::getIthBody ( int i )
{
  return mDynBodies[i];
}

void Physics::printPos( int i )
{
}

void Physics::initStaticStructures()
{
  mStaBodiesKdd = new Kdd2d::KddTree2d<StaticRigidBody>( mStaBodies );
  updateContactList();

  mDynBodiesLastPos.resize( numDynBodies() );
  for ( int i = 0; i < numDynBodies(); ++i )  
    mDynBodiesLastPos[i] = mDynBodies[i]->xWS + D3DXVECTOR3(0, 0.5, 0 );
  checkContacts( mDynBodies );
}

void Physics::limitVelocities( std::vector<RigidBody*>& aBodies )
{
  for ( int i = 0; i < aBodies.size(); ++i )  
    aBodies[i]->limitVelocity();      
}

void Physics::fixContactDegeneracies ( RigidBody* aBody, const Contact& aContact )
{
  if ( aContact.DEBUG_d < 0.0f )
  {
    D3DXVECTOR3 inc = aContact.normalWS * -( aContact.DEBUG_d*0.2f  );
    aBody->xWS +=  inc;
  }
}

void Physics::storeRestingContact ( RigidBody* aBody, const Contact& aContact )
{
  aBody->mIsInRestingContact = true;
  int indexContact = aBody->mContactNormal.size();
  D3DXVECTOR3 normal = aContact.normalWS;
  DxMath::clean( normal );
  aBody->mContactNormal.push_back( normal );
  
  fixContactDegeneracies( aBody, aContact );
  

}



bool Physics::narrowCollision( RigidBody* aBodyA, 
                              StaticRigidBody* aBodyB, 
                              const float aEpsilon, 
                              Contact& aContact, const bool aExtendedDebug,
                              const float aEpsilonBarycentric )
{

  D3DXVECTOR3 rayD = aBodyA->xWS - mDynBodiesLastPos[aBodyA->mId];


  float d;
  bool interpenetration = aBodyB->rayIntesection( mDynBodies[ aBodyA->mId ]->xWS,
                                                  rayD,
                                                  aEpsilon,
                                                  mDynBodies[ aBodyA->mId ]->mBodyExt->mBoundingSphereRadius,
                                                  aContact.pointWS,
                                                  aContact.normalWS,
                                                  aContact.offender,
                                                  d,
                                                  aExtendedDebug,
                                                  aEpsilonBarycentric);
  aContact.DEBUG_d = d;
  aContact.DEBUG_aId = (int)aBodyA->mId;
  aContact.DEBUG_bId = (int)aBodyB->mDebugId;  

  //  Normal as returned by the function is of the triangle,
  //  and we want the opposite one

  
  D3DXVec3Normalize( &aContact.normalWS, &aContact.normalWS );  
  // TODO: why reverse???
  DxMath::reverse( aContact.normalWS ) ;    
  DxMath::clean( aContact.normalWS );

  return interpenetration;
}

void  Physics::advanceSimulation ( float aDeltaT )
{
  double targetTime           = aDeltaT;  
  double stillToAdvancedTime  = targetTime;
      

  int number_iterations_per_total_time_step = 0;
  while ( !DxMath::equality( stillToAdvancedTime, 0.0f, 0.000001f ) )
  {
    ++number_iterations_per_total_time_step;
    if ( stillToAdvancedTime < mMaxStepTime )
    {
      advanceSimulationWithMaxStep( stillToAdvancedTime );
      stillToAdvancedTime = 0.0f;
    }
    else
    {
      advanceSimulationWithMaxStep( mMaxStepTime );
      stillToAdvancedTime -= mMaxStepTime;
    }
  }
  
}

//===========================================================================
/*!
    Advances the simulation by a time step
    \param    aDeltaT   time step
    
*/
//===========================================================================

void  Physics::advanceSimulationWithMaxStep ( float aDeltaT )
{
  float targetDeltaT = aDeltaT;
  float appliedDeltaT = 0.0f;

  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    if ( mPrecisionPerBody[i].mIterationsReset != 0 )
    {
      --mPrecisionPerBody[i].mIterationsReset;
      if ( mPrecisionPerBody[i].mIterationsReset == 0 )
        mPrecisionPerBody[i].mEpsilonBarycentric = mEpsilonBarycentric;
    }
    printPos(i);  
  }

  updateContactList();  



  D3DXVECTOR3 pos = mDynBodies[0]->xWS;
  int numIterationsTotal = 1;
  int numDecompositionsOfTimeUntilJustOneCollision = 0;
  
  while ( !DxMath::equality(  aDeltaT - appliedDeltaT, 0.0f, mMinimumTimeOfCollisionStep ) )
  {       
    ++numDecompositionsOfTimeUntilJustOneCollision;
    std::vector<RigidBody*> bodies( numDynBodies() );
    for ( int i = 0; i < numDynBodies(); ++i )
      bodies[i] = new RigidBody( *mDynBodies[i] );
        
    tryToAdvanceSimulation( targetDeltaT, bodies );
    

    PairBodies collidingObjects;
    Contact dummyContact;
    int dummyNum; bool dummyBool;

    
    
    if ( findContacts( bodies, collidingObjects, dummyContact, dummyNum, dummyBool ) )
    {
      Contact contact;

      
      //  Advance until first collision
      bool collisionFound;
      int numIterations;
      for ( int i = 0; i < numDynBodies(); ++i )
        *bodies[i] = *mDynBodies[i];
      targetDeltaT = findFirstTimeOfContact(  targetDeltaT, 
                                              bodies, 
                                              collidingObjects, 
                                              contact,
                                              collisionFound,
                                              numIterations);
      //tryToAdvanceSimulation( targetDeltaT, bodies );
      applySimulationStep( bodies );
      numIterationsTotal += numIterations;            

      appliedDeltaT += targetDeltaT;
      mTime += targetDeltaT;
      targetDeltaT = aDeltaT - appliedDeltaT;            

      if ( collisionFound )
      {
        //  Resolve collision
        //resolveCollision( bodies + collidingObjects.a, bodies + collidingObjects.b );
        if ( collidingObjects.bIsDynamic )
        {
          
          resolveCollision( mDynBodies[collidingObjects.a], 
                            mDynBodies[collidingObjects.b], 
                            contact );
        }
        else
        {          
          int id = contact.DEBUG_aId;

          if ( id == 1 )
          {
            bodies[id]->computeTemporalValues();
          
            resolveCollision( mDynBodies[collidingObjects.a],
                              collidingObjects.bStatic, 
                              contact );

            mDynBodies[id]->computeTemporalValues();
          
          }//end of log outpu

        }
      }
      
    }
    else
    {      
      //  No collision      
      applySimulationStep( bodies );
      appliedDeltaT += targetDeltaT;
      mTime += targetDeltaT;
      targetDeltaT = aDeltaT - appliedDeltaT;
    }

    // release memory
    for ( int i = 0; i < numDynBodies(); ++i )
      delete bodies[i];
  }
  D3DXVECTOR3 d = mDynBodies[0]->xWS - pos;
  pos = mDynBodies[0]->xWS;

  ++mDebugFrame; 
  D3DXVECTOR3 dif = mDynBodies[0]->xWS - mLastPos;
  

  mLastPos = mDynBodies[0]->xWS;

}

//===========================================================================
/*!
    Uses mDynBodies to simulate an step and leave the result in the array,
    starting at time position mTime
    \param    aDeltaT   time step
    \param    aBodies   where to store result from ODE
*/
//===========================================================================
void Physics::tryToAdvanceSimulation( float aDeltaT, std::vector<RigidBody*>& aBodies )
{
  limitVelocities( aBodies );  

  RigidBodyOde ode( aBodies, numDynBodies(), mTime, mGravityAcc, mContactDotEpsilon, mMinimumForceIncrement );

  ode.Step( aDeltaT );  

  limitVelocities( aBodies );
}

//===========================================================================
/*!
    Updates resting contacts lists for each body. To be called prior
    to ODE.
    \param    aDeltaT   time step
    \param    aBodies   bodies
*/
//===========================================================================
void Physics::checkContacts( std::vector<RigidBody*>& aBodies )
{

  //  clean resting contacts
  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    mDynBodies[i]->mIsInRestingContact = false;
    mDynBodies[i]->mContactNormal.clear();
  }  

  //  check each nearby object
  PairBodies collidingObjects;  
  Contact dummyContact;
  int dummyNum;
  bool dummyBool;
  findContacts( aBodies, collidingObjects, dummyContact, dummyNum, dummyBool );

}

//===========================================================================
/*!
    Copies the result of ODE back to mDynBodies
    \param    aBodies   result from ODE
*/
//===========================================================================
void Physics::applySimulationStep ( std::vector<RigidBody*>& aBodies )
{
  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    *mDynBodies[i] = *aBodies[i];
    if ( !DxMath::equality( mDynBodiesLastPos[i], mDynBodies[i]->xWS, 0.001f ) ) 
      mDynBodiesLastPos[i] = mDynBodies[i]->xWS;
  }
}


//===========================================================================
/*!
    Updates the contact list - broad phase - marking pairs
    of objects that are near enough to collide.
    They are kept into "mDynContactList".
    "mHasCollided" is cleared before use.
*/
//===========================================================================
void Physics::updateContactList()
{
  mDynContactList.resize( 0 );
  
  for ( int i = 0; i < numDynBodies() ; ++i )
    for ( int j = i + 1; j < numDynBodies() ; ++j )
    {
      PairBodies pair;
      pair.a          = i;
      pair.b          = j;
      pair.bIsDynamic = true;
      mDynContactList.push_back ( pair );
    }
  

  mStaContactList.resize( 0 );
  int sumNumStaticObjectsToCheckAgainst = 0;
  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    std::vector<StaticRigidBody*> staticBodies;
    staticBodies = mStaBodiesKdd->neighbours( mDynBodies[i]->xWS.x, 
                                              mDynBodies[i]->xWS.z, 
                                              mBroadPhaseDistanceEpsilon );

    sumNumStaticObjectsToCheckAgainst += staticBodies.size();
    for ( int j = 0; j < staticBodies.size() ; ++j )
    {
      PairBodies pair;
      pair.a          = i;   
      pair.bStatic    = staticBodies[j];
      pair.bIsDynamic = false;
      mStaContactList.push_back ( pair );
    }
  }   


}

//===========================================================================
/*!
    Finds all the pairs of objects that collide.
    Store result in "mHasCollided"
    \param  aObjects  Pair of colliding objects
    \return   true<=> one or more objects are interpetrating
*/
//===========================================================================
bool Physics::findContacts( std::vector<RigidBody*>& aBodies,
                            PairBodies& aCollidingObjects,
                            Contact& aContact,
                            int& aNumberOfIntepenetrations,
                            bool& aAllCollisionsAreFromSameObject)
{

  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    aBodies[i]->mIsInRestingContact = false;
    aBodies[i]->mContactNormal.clear();
  }  
  
  
  bool collisionFound = false;
  aNumberOfIntepenetrations = 0;

  Contact currentContact;


  for ( unsigned int i = 0; i < mDynContactList.size() ; ++i )
  {
    
    bool pairCollides = narrowCollision(  aBodies[ mDynContactList[i].a ], 
                                          aBodies[ mDynContactList[i].b ], 
                                          mEpsilon,
                                          currentContact );

    if ( pairCollides )   
    {
      int resultContact = interpenetrationOfContact(  aBodies[ mDynContactList[i].a ], 
                                                      aBodies[ mDynContactList[i].b ], 
                                                      currentContact );
      pairCollides &= ( resultContact == eContactResult::eContactInterpenetration);

      if ( resultContact == eContactResult::eContactInterpenetration)
      {
        ++aNumberOfIntepenetrations;        
        aCollidingObjects = mDynContactList[i];                          
        aContact = currentContact;
      }
    }

    collisionFound      |= pairCollides;    
  }

  Contact prevContact;
  int     prevContactRigidBodyId;

  for ( unsigned int i = 0; i < mStaContactList.size() ; ++i )
  {
    int id        = mStaContactList[i].a;
    int staticId  = mStaContactList[i].bStatic->mDebugId;

    if ( id == 1 && staticId == 69373 )
      int b = 2;

    bool pairCollides = narrowCollision(  aBodies[ id ], 
                                          mStaContactList[i].bStatic, 
                                          mEpsilon,
                                          currentContact,
                                          (aBodies[id]->mBodyExt->mPreviousContactSurface == mStaContactList[i].bStatic),
                                          mPrecisionPerBody[id].mEpsilonBarycentric);

    bool currentCollision = false;
    if ( pairCollides )
    {
      float vRel;
      int resultContact = interpenetrationOfContact(  aBodies[ id ], 
                                                      mStaContactList[i].bStatic, 
                                                      currentContact,
                                                      vRel );

      currentCollision = ( resultContact == eContactResult::eContactInterpenetration );



      if (  resultContact == eContactResult::eContactInterpenetration  ||
            resultContact == eContactResult::eContactRestingContact)
      {
        aBodies[id]->mBodyExt->mPreviousContactSurface = mStaContactList[i].bStatic;
        aBodies[id]->computeTemporalValues();
        
      }

      //  discard current collision if its plane is coplanar to a previous collision
      //  of the same object
      if (  currentCollision && 
            aNumberOfIntepenetrations == 1 && 
            id == prevContactRigidBodyId )
      {            
        if ( DxMath::areTrianglesCoplanar( prevContact.offender, currentContact.offender ) )        
          currentCollision = false;              
      }

      //  count if all the collisions are of the same object
      if (  currentCollision && aNumberOfIntepenetrations >= 1 )
          aAllCollisionsAreFromSameObject = (id == prevContactRigidBodyId);

      pairCollides &= currentCollision;

      if ( resultContact == eContactResult::eContactRestingContact )              
      {
        storeRestingContact( aBodies[ id ], currentContact );        
      }
      

      if ( currentCollision )         
      {
        ++aNumberOfIntepenetrations;        
        aCollidingObjects = mStaContactList[i];        
        prevContact = currentContact;
        aContact = currentContact;
        prevContactRigidBodyId = id;
        collisionFound      = true;        
      }

    }
        
  }


  return collisionFound;
}


//===========================================================================
/*!
    Computes a fine-grain collision between a pair of objects.
    
    \param    aBodyA  first object
    \param    aBodyB  second object    
    \return   true<=> objects interpenetrate
*/
//===========================================================================
bool Physics::narrowCollision( RigidBody* aBodyA, RigidBody* aBodyB, const float aEpsilon, Contact& aContact )
{
  float distanceCenters = D3DXVec3Length( &(aBodyA->xWS - aBodyB->xWS) );
  bool interpenetration = false;

  float d = distanceCenters - (aBodyA->mBodyExt->mBoundingSphereRadius + aBodyB->mBodyExt->mBoundingSphereRadius);
  if ( d < aEpsilon )
  {
    interpenetration = true;    
  }

  if ( interpenetration )
    aContact = computeContact( aBodyA, aBodyB );

  return interpenetration;
}

bool Physics::narrowCollision( RigidBody* aBodyA, 
                              StaticRigidBody* aBodyB, 
                              const D3DXVECTOR3& aRayB,
                              const D3DXVECTOR3& aRayD,
                              const float aEpsilon, 
                              Contact& aContact, const bool aExtendedDebug,
                              const float aEpsilonBarycentric   )
{

  float d;
  bool interpenetration = aBodyB->rayIntesection( aRayB,
                                                  aRayD, 
                                                  aEpsilon,
                                                  mDynBodies[ aBodyA->mId ]->mBodyExt->mBoundingSphereRadius,
                                                  aContact.pointWS,
                                                  aContact.normalWS,
                                                  aContact.offender,
                                                  d,
                                                  aExtendedDebug,
                                                  aEpsilonBarycentric);

  //  Normal as returned by the function is of the triangle,
  //  and we want the opposite one

  D3DXVec3Normalize( &aContact.normalWS, &aContact.normalWS );
  if ( aContact.normalWS.x != 0.0f )
    aContact.normalWS.x *= -1.0f;
  if ( aContact.normalWS.y != 0.0f )
    aContact.normalWS.y *= -1.0f;
  if ( aContact.normalWS.z != 0.0f )
    aContact.normalWS.z *= -1.0f;
  DxMath::clean( aContact.normalWS );

  return interpenetration;
}

//===========================================================================
/*!
    Finds the time-position of the first interpenetration. As such if more
    than one is present, more resolution is needed in time step.
    Takes into account "mEpsilon" to decide if they collide.
    \param    aDeltaT   time resolution of the pretended step
    \param    aBodies   bodies (input/output)
    \param    aCollidingObjects   pair of objects that collide
    \param    aContact  their point of collision
    \param
    \return   time-position of the first interpenetration
*/
//===========================================================================
float Physics::findFirstTimeOfContact( float aDeltaT, 
                                      std::vector<RigidBody*>& aBodies, 
                                      PairBodies& aCollidingObjects,
                                      Contact& aContact,
                                      bool& aTimePointFound,
                                      int& aNumIterations)
{
  float timeOfInterPenetration;

  float timeStart             = 0.0f;
  float timeEnd               = aDeltaT;
  bool  timeOfUniqueCollision = false;
  aNumIterations = 0;
  Contact prevContact;
  int     prevContactRigidBodyId;
  

  while ( !timeOfUniqueCollision)
  {    
    ++aNumIterations;
    float testTimePosition = (timeEnd - timeStart) * 0.5f;

    std::vector<RigidBody*> bodies( numDynBodies() );
    for ( int i = 0; i < numDynBodies(); ++i )
      bodies[i] = new RigidBody( *aBodies[i] );
    
    tryToAdvanceSimulation ( timeStart + testTimePosition, bodies );

    int   numberOfIntepenetrations;
    bool  suitableInterpenetration = false; 
    bool  allCollisionsComeFromSameObject = false;
    
    suitableInterpenetration = findContacts(  bodies, 
                                              aCollidingObjects, 
                                              aContact, 
                                              numberOfIntepenetrations,
                                              allCollisionsComeFromSameObject);
          
  //  Check underflow
    if ((timeEnd - timeStart)< (mMinimumTimeOfCollision))
    {                
      // More than one collision was found, but given we can
      // no longer increase resolution, return just one of them
      timeOfUniqueCollision   = true;
      timeOfInterPenetration  = timeStart + testTimePosition;

      // copy back
      aTimePointFound = true;
      for ( int i = 0; i < numDynBodies(); ++i )
        *aBodies[i] = *bodies[i];
      return timeOfInterPenetration;
    }

    //  First check if we have already found an interpenetration we can solve
    if ( suitableInterpenetration && numberOfIntepenetrations == 1)
    {
      
      timeOfUniqueCollision   = true;
      timeOfInterPenetration  = timeStart + testTimePosition;
      

      // copy back
      aTimePointFound = true;
      for ( int i = 0; i < numDynBodies(); ++i )
        *aBodies[i] = *bodies[i];
    }
    else
    {
      //  Otherwise halve time interval
      ( numberOfIntepenetrations == 0 ) ?
            timeStart = timeStart + testTimePosition
          : timeEnd = timeStart + testTimePosition;

      if ( allCollisionsComeFromSameObject )
      {
        int id = aContact.DEBUG_aId;
        mPrecisionPerBody[id].mEpsilonBarycentric *= 0.5f;
        mPrecisionPerBody[id].mIterationsReset    = 30;
      }      
      
    } //end of test for unique intersection
 
    for ( int i = 0; i < numDynBodies(); ++i )
      delete bodies[i];
  }
  
  return timeOfInterPenetration;
}