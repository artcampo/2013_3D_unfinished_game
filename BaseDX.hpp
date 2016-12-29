#ifndef _BASEDX_HPP_
#define _BASEDX_HPP_



#include	"dx_misc.hpp"
#include  "Project_Dependant/LogConditionalCompilation.hpp"

#include  <string>
#include  <memory>
#include  <vector>

class DeferredEngine;
class Log;



class BaseDX
{
public:

  BaseDX(HINSTANCE& handler, int width, int height, bool isFullmWindowsScreen, 
             std::wstring wTitle, bool showFPS);
  virtual ~BaseDX();

  bool initWindowsApp();
  bool initDX();
  
  virtual void onResize();
  virtual int main() = 0;    
  virtual LRESULT getMessage(UINT msg, WPARAM wParam, LPARAM lParam);

  void incrementFrameCounter();

  //Modifiers
  void    disableFps	();    
	void    enableFps	();  

  //Time
  void    startTimer();
  void    calculateTime();
  float   getmDeltaTime() const;    
  double  getSeconds();
  float   getCentiseconds();

  //  Keyb
  void keybCleanAll();    
      
  DeferredEngine* deferredEngineInstance() const;

private:
  void  releaseMouse();
  void  captureMouse();

protected:
  // Windows
  int           mCurrentWidth, mCurrentHeight;    
  HINSTANCE     mHInstance;
  HWND          mWindowsScreen;    
  bool          mIsFullmWindowsScreen;
  HICON         mAppIcon;
  std::wstring  mWindowTitle;    

  //Dx
  ID3D10Device*     mDxDevice;
  IDXGISwapChain*   mSwapChain;

  // Keyboard
  std::vector<bool>  mKeyPressed;  
    
  //Mouse
  Mouse       mMouseState;  
  D3DXVECTOR3 mPtsBegin, mPtsEnd;
  bool        mPtsStarted;
  float       mMouseCenterBias;

  //Time
	double    mSecondsPerCount;  
  double    mDeltaTime;
  __int64   mBaseTime;
  __int64   mPrevTime;
  double    mSecondsCounter;

  //FPS
  unsigned int  mLastFramePerSecondCount;
  unsigned int  mNumFrame; 
  int           mKeybWaitCount;
  bool		      mShowFPS;
  
  DeferredEngine*   mDeferredEngine;

#ifdef _LOG_ENABLE_
  Log*                  mLog;
#endif

};

#endif //_BASEDX_HPP_