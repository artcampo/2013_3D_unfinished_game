#include  "Game/State/GameContext.hpp"
#include  "Game/World/GameStateWorld.hpp"
#include  "Game/GameInitObjects.hpp"

GameContext::GameContext( GameInitObjects& aInitObjs ) :
  mStateWorld( new GameStateWorld( aInitObjs ) )
{
  mCurrentState = mStateWorld.get();
}

GameContext::~GameContext()
{

}

void GameContext::_keyboard( const std::vector<bool>& aKeyPressed, const Mouse& aMouseState, const float aLapsedTime )
{
  mCurrentState->_keyboard(  aKeyPressed, aMouseState, aLapsedTime );
}

void GameContext::_update( const float aLapsedTime)
{
  mCurrentState->_update( aLapsedTime );
}
void GameContext::_drawScene()
{
  mCurrentState->_drawScene();
}

void GameContext::currentState( GameState* aState )
{
}