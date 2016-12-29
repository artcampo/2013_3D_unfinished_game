#include  "Game/Game.hpp"
#include  "Game/GameInitObjects.hpp"
#include  "Game/World/GameStateWorld.hpp"
#include  "Global.hpp"

//  render models
#include  "RenderModels/Compounded/DeferredMeshModel.h"
#include  "ObjEditor/Mesh/mesh.hpp"
#include  "ObjEditor/MeshLoad.hpp"
#include  "Dx/DeferredEngine.hpp"
#include  "Shader/ShaderManager.hpp"

#include  "Dx/Camera/Camera.hpp"
#include  "Dx/Camera/CameraThidPerson.hpp"

#include  "Game/Entities/Ship/Ship.hpp"
#include	"Physics/RigidBody.hpp"
#include  "Physics/Physics.hpp"


GameStateWorld::GameStateWorld( GameInitObjects& aInitObjs ) :
  GameState       ( aInitObjs.mDeferredEngine),
  mGame           ( aInitObjs.mGame ),
  mDeferredEngine ( aInitObjs.mDeferredEngine ),
  mCam            ( aInitObjs.mCam ),
  mCam3dPerson    ( aInitObjs.mCam3dPerson )
{
  Mesh* archMesh;
  archMesh =  MeshFile::loadMesh( "./DummyCargo.nbo" );

  ColourMapping   colours;
  mArchModel = archMesh->convertToDeferred( Global::md3dDevice, 
                        D3DXVECTOR3( 10,0,0),
                        &colours);

  mShaderManager = new ShaderManager( Global::md3dDevice );
  //mDeferredEngine = deferredEngineInstance();
}

GameStateWorld::~GameStateWorld()
{
  delete mArchModel[0];
}


void GameStateWorld::_keyboard( const std::vector<bool>& aKeyPressed, const Mouse& aMouseState, const float aLapsedTime )
{
  mGame.keyboardShipPlayer( aKeyPressed, aLapsedTime );
  mGame.mouseShipPlayer(aMouseState, aLapsedTime);
}

void GameStateWorld::_update( const float aLapsedTime)
{
  mGame.mPhysics->advanceSimulation ( aLapsedTime );

  mGame.mShipPlayer->update(aLapsedTime);

  mGame.mCam3dPerson->updateTargetParameters( mGame.mShipPlayer->mBody->getxWS(), 
                                        mGame.mShipPlayer->pointingUnit(), 
                                        mGame.mShipPlayer->pointingUnit(), 
                                        aLapsedTime );  
  mGame.mCam3dPerson->updateCamera();
  
  
}

void  GameStateWorld::_drawScene()
{
  //D3DXMatrixIdentity  ( &mDeferredEngine.mWorld );
  D3DXVECTOR3 shipPosition = mGame.mShipPlayer->mBody->getxWS();

  D3DXMatrixTranslation (   &mDeferredEngine.mWorld, 
                            shipPosition.x, 
                            shipPosition.y,
                            shipPosition.z );

  D3DXMATRIX matrixRotation;
  D3DXMatrixRotationQuaternion( &matrixRotation, &mGame.mShipPlayer->mBody->qWS );
  mDeferredEngine.mWorld = matrixRotation*mDeferredEngine.mWorld;
  

  mCam.updateCamera   ( &mDeferredEngine.mView );

  ID3D10EffectPass* pass;
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  mShaderManager->mShaderTable[indexShadDefGeomV_PNT_Arch].SetMatrix( &(std::string("World")), &mDeferredEngine.mWorld );
  mShaderManager->mShaderTable[indexShadDefGeomV_PNT_Arch].SetMatrix( &(std::string("View")), &mDeferredEngine.mView );
  mShaderManager->mShaderTable[indexShadDefGeomV_PNT_Arch].SetMatrix( &(std::string("Projection")), &mDeferredEngine.mProjection );
  mShaderManager->mShaderTable[indexShadDefGeomV_PNT_Arch].SetResource( &(std::string("PermTexture")), mDeferredEngine.mPermTextureSRV ); 
  Global::md3dDevice->IASetInputLayout( mShaderManager->mShaderTable[indexShadDefGeomV_PNT_Arch].mVertexLayout );  
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );  
  pass = mShaderManager->mShaderTable[indexShadDefGeomV_PNT_Arch].mTech->GetPassByIndex(0);
  pass->Apply(0);    

  for ( int j = 0; j < mArchModel.size() ; ++j)  
    mArchModel[j]->displayTriangles(); 


  D3DXMatrixTranslation ( &mDeferredEngine.mWorld, -10, -10, -10 ); 
  mShaderManager->mShaderTable[indexShadDefGeomV_PNT_Arch].SetMatrix( &(std::string("World")), &mDeferredEngine.mWorld );
  mShaderManager->mShaderTable[indexShadDefGeomV_PNT_Arch].SetMatrix( &(std::string("View")), &mDeferredEngine.mView );
  pass->Apply(0);  

  mGame.mTerrainModel->display();
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP );  
  pass->Apply(0);  

  mGame.displayConvexHulls();
}