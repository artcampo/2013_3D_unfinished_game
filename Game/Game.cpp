#include  "Project_Dependant/LogConditionalCompilation.hpp"

#include  "Game/Game.hpp"
#include  "Game/State/GameState.hpp"
#include  "Game/State/GameContext.hpp"
#include  "Game/GameInitObjects.hpp"

//  render models
#include  "RenderModels/Compounded/DeferredMeshModel.h"
#include  "ObjEditor/Mesh/mesh.hpp"
#include  "ObjEditor/MeshLoad.hpp"

#include  "Dx/DeferredEngine.hpp"
#include  "Dx/DefVertexLayouts.hpp"
#include  "Dx/Camera/Camera.hpp"
#include  "Dx/Camera/CameraThidPerson.hpp"

#include  "Global.hpp"
#include  "Shader/ShaderManager.hpp"



#include  "Game/Terrain/Terrain.hpp"
#include  "Game/WorldTerrain/WorldTerrain.hpp"
#include  "Game/WorldTerrain/HeightFunction.hpp"
#include  "Game/Entities/Ship/Ship.hpp"

#include  "Physics/Physics.hpp"
#include  "Geometry/ConvexHull2d/ConvexHull2d.hpp"

const float Game::mTorquesRenormalize = 400.0f;
const float Game::mStrafeRenormalize  = 400.0f;

#ifdef _LOG_ENABLE_
  #include  "Misc/Log.hpp"
  #include	"Physics/RigidBody.hpp"
#endif

//test only
#include  "Physics/MeshCol.hpp"
//test only END


Game::Game( DeferredEngine& aDeferredEngine ) :
  mCam            ( new Camera() ),
  mCam3dPerson    ( new Camera3rdPersonView( mCam.get() ) ),
  mDeferredEngine ( aDeferredEngine ),
  mGameContext    ( new GameContext ( GameInitObjects( *this, aDeferredEngine, *mCam, *mCam3dPerson ) ) ),
  mShaderManager  ( new ShaderManager( Global::md3dDevice ) ),
  mWorldTerrain   ( new WorldTerrain() ),
  mPhysics        ( new Physics )
{  
  
  mWorldTerrain->init();
  mTerrainModel = mWorldTerrain->compute();

  mPhysics->setParameters( 0, 0.01f );

  std::vector<VertexPosNor> v;
  v.push_back( VertexPosNor( D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,1,0) ));
  v.push_back( VertexPosNor( D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,1,0) ));
  v.push_back( VertexPosNor( D3DXVECTOR3(0,0,1), D3DXVECTOR3(0,1,0) ));
  
  mMeshColTest = new MeshCol( v );
  mPhysics->createStaticRigidBody( mMeshColTest );
  mPhysics->initStaticStructures();

  startGame();

#ifdef _LOG_ENABLE_
  mLog = new Log();
#endif
}

Game::~Game()
{
  delete mLog;
}



void  Game::drawScene()
{
  mGameContext->_drawScene();  
}

	
void  Game::keyboard( const std::vector<bool>& aKeyPressed, const Mouse& aMouseState, const float aLapsedTime )
{
  mGameContext->_keyboard( aKeyPressed, aMouseState, aLapsedTime );  
  
  
#ifdef _LOG_ENABLE_MOUSE_INCREMENT  
  *mLog->mFileShipPlayer<< "Mouse: " 
                        << aMouseState.position.x 
                        << ", "
                        << aMouseState.position.y
                        << ", "
                        << aMouseState.position.z 
                        << std::endl;  
#endif

}

void  Game::update( const float aLapsedTime )
{
  mGameContext->_update(aLapsedTime);  

#ifdef _LOG_ENABLE_
  *mLog->mFileShipPlayer << std::endl;  
#endif

#ifdef _LOG_ENABLE_PHYSICS_PLAYER_POS
  D3DXVECTOR3 shipPosition = mShipPlayer->mBody->getxWS();
  *mLog->mFileShipPlayer<< "Pos: " 
                        << shipPosition.x 
                        << ", "
                        << shipPosition.y
                        << ", "
                        << shipPosition.z 
                        << std::endl;  
#endif

#ifdef _LOG_ENABLE_LAPSED_TIME
  *mLog->mFileShipPlayer<< "Lapsed: " 
                        << aLapsedTime
                        << std::endl;  
#endif

  
#ifdef _LOG_ENABLE_PHYSICS_PLAYER_POINTING_VECTOR
  D3DXVECTOR3 shipPointing = mShipPlayer->pointingUnit();
  *mLog->mFileShipPlayer<< "Pointing: " 
                        << shipPointing.x 
                        << ", "
                        << shipPointing.y
                        << ", "
                        << shipPointing.z 
                        << std::endl;  
#endif
}

void  Game::startGame()
{
  ShipDescription desc;
  desc.bckModule    = 8;
  desc.fwdModule    = 8;
  desc.sidModule    = 8;
  desc.pitchModule  = 0.1;
  desc.yawModule    = 0.1;
  mShipPlayer = std::auto_ptr<Ship>( new Ship ( *mPhysics, D3DXVECTOR3(500,0.0f,500), desc ) );
}

void  Game::keyboardShipPlayer( const std::vector<bool>& aKeyPressed, const float aLapsedTime  )
{    
  mShipPlayer->moveFwd( aKeyPressed[VK_UP],     aLapsedTime*mStrafeRenormalize );
  mShipPlayer->moveBck( aKeyPressed[VK_DOWN],   aLapsedTime*mStrafeRenormalize );
  mShipPlayer->moveLft( aKeyPressed[VK_LEFT],   aLapsedTime*mStrafeRenormalize );
  mShipPlayer->moveRgt( aKeyPressed[VK_RIGHT],  aLapsedTime*mStrafeRenormalize );  
}

void  Game::mouseShipPlayer( const Mouse& aMouseState, const float aLapsedTime  )
{
  mShipPlayer->pitchUp  ( aMouseState.position.y > 0, aLapsedTime*mTorquesRenormalize );
  mShipPlayer->pitchDown( aMouseState.position.y < 0, aLapsedTime*mTorquesRenormalize );
  mShipPlayer->turnRight( aMouseState.position.x > 0, aLapsedTime*mTorquesRenormalize );
  mShipPlayer->turnLeft ( aMouseState.position.x < 0, aLapsedTime*mTorquesRenormalize );
}



int   Game::numTriangles() const
{
  return mTerrainModel->mNumVertices;
}

void   Game::displayConvexHulls() const
{
  mWorldTerrain->displayConvexHulls();
}
