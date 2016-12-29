#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#pragma once
#include  "Project_Dependant/LogConditionalCompilation.hpp"
#include  <vector>
#include  "dx_misc.hpp"



class RigidBody;
class RigidBodyExt;
class RigidBodyOde;
class StaticRigidBody;
class MeshCol;
class Mesh;
namespace Kdd2d{  template <class T> class KddTree2d; };

// specific of physics
struct  PairBodies;
class   Precision;
struct  Contact;

#ifdef _LOG_ENABLE_
  class Log;
  #include <memory>
#endif



class Physics {
 
public:	
	Physics();
	~Physics();

  //  init functions
  std::auto_ptr<RigidBody> createRigidBody (  const D3DXVECTOR3& aPositionWS,
                                const float aMass,
                                const float aBoundingRadius,
                                const float aDragForce,
                                const float aBounceCoefficient);
  void  createStaticRigidBody ( Mesh* aMesh );
  void  createStaticRigidBody ( MeshCol* aMesh );

  void  initStaticStructures();
  void  setParameters( float aGravityAcc, float aMaxStepTime );

  //  simulation functions
  void  advanceSimulation ( float aDeltaT );
  


  //  TODO: put an iterator here
  
  int         getNumBodies() const;
  RigidBody*  getIthBody ( int i );

  //  Rewritten
private:
  int         numDynBodies() const;
  int         numStaBodies() const;

  //  Constants of simulation
private:
  float   mGravityAcc;  /*Gravity*/
  float   mEpsilon;   /*!< precision of interpenetration*/
  
  float   mMaxStepTime;   /*!< Maximum time that can be performed in one step */
  float   mInterpenetrationThreshold; /*!< Threshold of distance*/
  float   mMinimumTimeOfCollision;    /*!< Fine-grain threshold for collisions,
                                           also minimum step of update*/
  float   mMinimumTimeOfCollisionStep;  /*!< */
  float   mMinimumForceIncrement;  /*!< */
  float   mContactDotEpsilon;
  float   mEpsilonBarycentric;
  std::vector<Precision> mPrecisionPerBody;

  float   mBroadPhaseDistanceEpsilon;

#ifdef _LOG_ENABLE_
  Log*                  mLog;
#endif

  //  Simulation data
private:
  float       mTime;        /*!< Current time */  
  std::vector<RigidBody*>       mDynBodies;     /*!< Vector of Dynamic bodies*/  
  std::vector<RigidBodyExt*>    mDynBodiesExt;  /*!< Vector of Dynamic bodies*/  
  std::vector<StaticRigidBody*> mStaBodies;     /*!< Vector of static bodies: will be sorted by distance to the only moving object*/  
  
  std::vector<D3DXVECTOR3>      mDynBodiesLastPos;
  D3DXVECTOR3   mLastPos;

  Kdd2d::KddTree2d<StaticRigidBody>* mStaBodiesKdd;

  //  Per-step temporal information
private:
  std::vector<PairBodies>       mDynContactList; /*!< List of possible colisions*/
  std::vector<PairBodies>       mStaContactList; /*!< List of possible colisions*/
  

private:
  void    advanceSimulationWithMaxStep ( float aDeltaT );
  void    updateContactList       ();
  void    checkContacts( std::vector<RigidBody*>& aBodies );
  bool    findContacts            ( std::vector<RigidBody*>& aBodies,
                                    PairBodies& aCollidingObjects,
                                    Contact& aContact,
                                    int& aNumberOfIntepenetrations,
                                    bool& aAllCollisionsAreFromSameObject);
  float   findFirstTimeOfContact  ( float aDeltaT, 
                                    std::vector<RigidBody*>& aBodies, 
                                    PairBodies& aCollidingObjects, 
                                    Contact& aContact,
                                    bool& aTimePointFound,
                                    int& aNumIterations );
  
  void    tryToAdvanceSimulation  ( float aDeltaT, std::vector<RigidBody*>& aBodies );
  void    applySimulationStep     ( std::vector<RigidBody*>& aBodies );

  //  Against dynamic bodies
  bool    narrowCollision           ( RigidBody* aBodyA, RigidBody* aBodyB, const float aEpsilon, Contact& aContact );
  Contact computeContact            ( RigidBody* aBodyA, RigidBody* aBodyB );
  int     interpenetrationOfContact ( RigidBody* aBodyA, RigidBody* aBodyB, const Contact& aContact );
  void    resolveCollision          ( RigidBody* aBodyA, RigidBody* aBodyB, const Contact& aContact );

  //  Against static bodies
  bool    narrowCollision           ( RigidBody* aBodyA, 
                                      StaticRigidBody* aBodyB, 
                                      const float aEpsilon, 
                                      Contact& aContact, const bool aExtendedDebug,
                                      const float aEpsilonBarycentric );  

  bool    narrowCollision           ( RigidBody* aBodyA, 
                                      StaticRigidBody* aBodyB, 
                                      const D3DXVECTOR3& aRayB,
                                      const D3DXVECTOR3& aRayD,
                                      const float aEpsilon, 
                                      Contact& aContact, const bool aExtendedDebug,
                                      const float aEpsilonBarycentric ); 
                                
  int     interpenetrationOfContact ( RigidBody* aBodyA, 
                                      StaticRigidBody* aBodyB, 
                                      Contact& aContact,
                                      float& aVrel );
  void    resolveCollision          ( RigidBody* aBodyA, StaticRigidBody* aBodyB, const Contact& aContact );

  void storeRestingContact    ( RigidBody* aBody, const Contact& aContact );
  void fixContactDegeneracies ( RigidBody* aBody, const Contact& aContact );

  //  helpers
private:
  void limitVelocities( std::vector<RigidBody*>& aBodies );
  D3DXVECTOR3 pointVelocity( RigidBody* aBody, D3DXVECTOR3 aPoint );

  //  DEBUG
public:  
  
  int   mDebugFrame;
  void printPos( int i);


private:
  enum  eContactResult{
    eContactMovingAway,
    eContactRestingContact,
    eContactInterpenetration
  };


};


#endif