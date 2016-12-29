#ifndef _Game_State_World_HPP_
#define _Game_State_World_HPP_

#include  "Game/State/GameState.hpp"
#include  <vector>

class Game;
class GameContext;
class ShaderManager;
class DeferredEngine;
class DeferredMeshModel;
class Camera;
class Camera3rdPersonView;
class GameInitObjects;
class Mouse;

class GameStateWorld : public GameState {
public:  
  
  GameStateWorld(  GameInitObjects& aInitObjs );  
  ~GameStateWorld();

  virtual void _keyboard( const std::vector<bool>& aKeyPressed, const Mouse& aMouseState, const float aLapsedTime  );
  virtual void _update( const float aLapsedTime );
  virtual void _drawScene();  
  
private:  
  Game&                             mGame;
  DeferredEngine&                   mDeferredEngine;  
  Camera&                           mCam;
  Camera3rdPersonView&              mCam3dPerson;
  ShaderManager*                    mShaderManager;

  std::vector<DeferredMeshModel*>   mArchModel;

};


#endif  //_Game_State_World_HPP_
