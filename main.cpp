#define _HAS_EXCEPTIONS 0
#define _HAS_ITERATOR_DEBUGGING 0

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
extern "C" int _fltused = 0;

#include  <windowsx.h>

#include  "dx_misc.hpp"
#include  "App/GameApp.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{  

#ifdef _WINDOWS_VISUAL_STUDIO_MEAM_LEAK_ENABLE
  #if defined(DEBUG) | defined(_DEBUG)
	  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  #endif
#endif

  bool fullmWindowsScreen = false;
  bool showFPS = true;
  std::wstring wTitle;

  wTitle =  L"Underground Sea 96k";  
  GameApp my(hInstance, mWindowsScreen_W, mWindowsScreen_H, fullmWindowsScreen, wTitle, showFPS);  

  my.main();
}