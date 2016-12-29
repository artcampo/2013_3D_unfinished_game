#ifndef _Game_Context_HPP_
#define _Game_Context_HPP_

#include  <vector>
#include  <memory>
#include  "Game/State/GameState.hpp"

class Game;
class GameStateWorld;
class GameInitObjects;
class Mouse;

class GameContext {
public:

  GameContext( GameInitObjects& aInitObjs );  
  ~GameContext();

  virtual void _keyboard( const std::vector<bool>& aKeyPressed, const Mouse& aMouseState, const float aLapsedTime );
  virtual void _update( const float aLapsedTime);
  virtual void _drawScene();  

  virtual void currentState( GameState* aState );

private:
  GameState*                      mCurrentState;
  std::auto_ptr<GameStateWorld>   mStateWorld;
  
  friend class GameStateWorld;
};


#endif  //_Game_Context_HPP_
