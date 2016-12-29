#ifndef _Game_State_HPP_
#define _Game_State_HPP_

#include  <vector>

class Game;
class GameContext;
class DeferredEngine;
class Mouse;

class GameState {
public:  
  
  GameState( DeferredEngine& aDeferredEngine ){};  
  ~GameState(){};

  virtual void _keyboard( const std::vector<bool>& aKeyPressed, 
                          const Mouse& aMouseState, 
                          const float aLapsedTime  )              = 0;
  virtual void _update( const float aLapsedTime )                 = 0;
  virtual void _drawScene()                                       = 0;  
  
private:  

};


#endif  //_Game_State_HPP_
