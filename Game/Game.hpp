#ifndef _GAME_HPP_
#define _GAME_HPP_

#include  "BaseDX.hpp"
//#include <vector>

#include  "Project_Dependant/LogConditionalCompilation.hpp"
#ifdef _LOG_ENABLE_
  class Log;
#endif

class DeferredMeshModel;
class ShaderManager;
class GameContext;
class DeferredEngine;
class Camera;
class Camera3rdPersonView;
class WorldTerrain;
template <class T> class DxMeshModelBatch;
class VertPNT;
class Physics;
class Ship;
class Mouse;


//test
class MeshCol;
//test END

class Game
{ 
public:
	Game( DeferredEngine& aDeferredEngine );
	~Game();

  //  Main Game
  
	void  drawScene();  
  void  keyboard( const std::vector<bool>& aKeyPressed, const Mouse& aMouseState, const float aLapsedTime );
  void  update( const float aLapsedTime );

  void  keyboardShipPlayer  ( const std::vector<bool>& aKeyPressed, const float aLapsedTime  );
  void  mouseShipPlayer     ( const Mouse& aMouseState, const float aLapsedTime  );

  int   numTriangles() const;

private:
  void  startGame();

private:

  std::auto_ptr<Camera>               mCam;
  std::auto_ptr<Camera3rdPersonView>  mCam3dPerson;
  DeferredEngine&                     mDeferredEngine;
  std::auto_ptr<GameContext>          mGameContext;  
  ShaderManager*                      mShaderManager;
#ifdef _LOG_ENABLE_
  Log*                  mLog;
#endif

public:
  std::auto_ptr<Physics>              mPhysics;
  public:
  std::auto_ptr<WorldTerrain>         mWorldTerrain;
private:
  //  In-game dependant classes
  std::auto_ptr<Ship>       mShipPlayer;

  //  not in ctor
  std::auto_ptr< DxMeshModelBatch<VertPNT> >  mTerrainModel;


  //  Configuration of player
  static const float mTorquesRenormalize;
  static const float mStrafeRenormalize;


  //test
  MeshCol* mMeshColTest;
  //test END

  friend class GameStateWorld;

  //debug
public:
  void displayConvexHulls() const;
};

#endif