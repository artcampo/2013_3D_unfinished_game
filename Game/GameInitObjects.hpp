#ifndef _GAME_INIT_OBJECTS_HPP_
#define _GAME_INIT_OBJECTS_HPP_

class DeferredEngine;
class Camera;
class Camera3rdPersonView;
class Game;

class GameInitObjects
{ 
public:
  GameInitObjects(  Game&                 aGame,
                    DeferredEngine&       aDeferredEngine,
                    Camera&               aCam,
                    Camera3rdPersonView&  amCam3dPerson )
    : mGame           ( aGame ),
      mDeferredEngine ( aDeferredEngine ),
      mCam            ( aCam ),
      mCam3dPerson    ( amCam3dPerson )
  {};

  Game&                   mGame;
  DeferredEngine&         mDeferredEngine;
  Camera&                 mCam;
  Camera3rdPersonView&    mCam3dPerson; 
private:
  GameInitObjects();
};

#endif // #define _GAME_INIT_OBJECTS_HPP_