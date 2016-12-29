#include "GameApp.hpp"

//  render models
#include  "Dx/DeferredEngine.hpp"
#include  "Game/Game.hpp"

// fps
#include <sstream>
#include <IOstream>
// /fps


GameApp::GameApp(HINSTANCE& handler, int width, int height, bool isFullmWindowsScreen, 
               std::wstring wTitle, bool showFPS): 
     BaseDX(mHInstance, width, height, isFullmWindowsScreen, wTitle, showFPS),
     mShaderManager( mDxDevice ),
     mGame ( new Game ( *mDeferredEngine ) ),
     mTimeInitalized ( false )
{  
}

GameApp::~GameApp()
{  
}


void GameApp::onResize()
{
	BaseDX::onResize();
}

int   GameApp::main()
{
  startTimer();
  MSG msg = {0}; 

  while(msg.message != WM_QUIT)
	{		
    
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE )) {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
		}		
    else {     

      if ( mKeyPressed[VK_ESCAPE] )
          return 0;

      double lapsedTime = getLapsedTime();
      lapsedTime *= 0.001f;
           
      
      mGame->keyboard ( mKeyPressed, mMouseState, lapsedTime );
      mGame->update   ( lapsedTime );

      mDeferredEngine->set();
      mDeferredEngine->startScene();
      drawScene();
      mDeferredEngine->composite();

      incrementFrameCounter();
      displayFPSandDebugInfo();

      mSwapChain->Present( 0, 0 );       
    }
  }

  return (int)msg.wParam;
}

void  GameApp::drawScene()
{
  mGame->drawScene();
}

double GameApp::getLapsedTime() 
{
  if (!mTimeInitalized)
  {
    mTimeInitalized = true;
    mLastTime = getCentiseconds();    
    return 0;
  }
  else
  {
    double time   = getCentiseconds();
    double lapsed = time - mLastTime;
    mLastTime = time;
    return lapsed;
  }
}

void  GameApp::displayFPSandDebugInfo()
{

  std::ostringstream textStream;   
 
  using namespace std;
  textStream.setf( ios_base::fixed, ios_base::floatfield );
  textStream.precision(4);

         
  textStream << "FPS: " << mLastFramePerSecondCount << "\n";
  //textStream << L"Seconds: "<< centiseconds/100.0f << L"\n";      
  textStream << "Triangles: "<< mGame->numTriangles() << "\n";

  std::string s = textStream.str();
  mDeferredEngine->drawText( s, false, D3DXCOLOR(1,1,1,1) );  
  
}