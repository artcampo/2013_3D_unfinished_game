#include  "../BaseDX.hpp"
#include <vector>

class Game;
class ShaderManager;
#include  "Shader/ShaderManager.hpp"



class GameApp : public BaseDX
{ 
public:
	GameApp(HINSTANCE& handler, int width, int height, bool isFullmWindowsScreen, 
               std::wstring wTitle, bool showFPS);
	~GameApp();

  //  Main Game
  int   main();	
	void  onResize();
	void  drawScene();    

private:
  std::auto_ptr<Game>     mGame;
  ShaderManager           mShaderManager;

  double  mLastTime;
  bool    mTimeInitalized;


private:

  double  getLapsedTime();
  void    displayFPSandDebugInfo();

};
