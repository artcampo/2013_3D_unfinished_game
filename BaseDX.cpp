#include  "BaseDX.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"
#include  "Dx/DeferredEngine.hpp"
#include  "Global.hpp"

#include  "Project_Dependant/LogConditionalCompilation.hpp"

#ifdef _LOG_ENABLE_
  #include  "Misc/Log.hpp"
#endif

//Extern call
LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BaseDX* app = 0;

	switch( msg )
	{
		case WM_CREATE:{			
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			app = (BaseDX*)cs->lpCreateParams;
			return 0;
		}
	}

	if( app )
		return app->getMessage(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
  }


//Creator
BaseDX::BaseDX(HINSTANCE& handler, int width, int height, bool isFullmWindowsScreen, 
               std::wstring wTitle, bool showFPS):
               mWindowsScreen         (0),               
               mAppIcon               (0),
               mHInstance             (handler),               
               mPtsBegin              (0.0f,0.0f,0.0f),
               mPtsEnd                (0.0f,0.0f,0.0f),               
               mCurrentWidth          (width),
               mCurrentHeight         (height),
               mIsFullmWindowsScreen  (isFullmWindowsScreen),               
               mSecondsCounter        (0.0f),
               mNumFrame              (0),
               mPrevTime              (0),
               mBaseTime              (0),
               mDeltaTime             (0.0),
               mSecondsPerCount       (0.0),
               mShowFPS               (showFPS),
               mWindowTitle           (wTitle),
               mLastFramePerSecondCount (0),
               mKeybWaitCount           (0),
               mDeferredEngine          (0),
               mKeyPressed              (256),
               mMouseCenterBias         (0.3f)
{
  
  keybCleanAll();

  //Set mSecondsPerCount
  __int64 countsPerSec;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  mSecondsPerCount = 1.0 / (double)countsPerSec; 
  

  initWindowsApp();
  initDX();
  //captureMouse();

  mDeferredEngine = new DeferredEngine( mDxDevice, mSwapChain, mCurrentWidth, mCurrentHeight );
  
#ifdef _LOG_ENABLE_
  mLog = new Log();
#endif

}

//Destructor
BaseDX::~BaseDX()
{ 
  //releaseMouse();
	DestroyIcon(mAppIcon);
  delete mDeferredEngine;
  delete mLog;
}

bool BaseDX::initDX()
{
  #if defined(DEBUG) | defined(_DEBUG)
	  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  #endif
    
	DXGI_SWAP_CHAIN_DESC swapchain;
	swapchain.BufferDesc.Width                    = mCurrentWidth;
	swapchain.BufferDesc.Height                   = mCurrentHeight;
	swapchain.BufferDesc.RefreshRate.Numerator    = 60;
	swapchain.BufferDesc.RefreshRate.Denominator  = 1;
	swapchain.BufferDesc.Format                   = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain.BufferDesc.ScanlineOrdering         = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchain.BufferDesc.Scaling                  = DXGI_MODE_SCALING_UNSPECIFIED;
	// No multisampling.
	swapchain.SampleDesc.Count   = 1;
	swapchain.SampleDesc.Quality = 0;

	swapchain.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain.BufferCount  = 1;
	swapchain.OutputWindow = mWindowsScreen;  
	swapchain.Windowed     = !mIsFullmWindowsScreen;
	swapchain.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	swapchain.Flags        = 0;

                    
  D3D10_DRIVER_TYPE md3dDriverType  = D3D10_DRIVER_TYPE_HARDWARE;	

	UINT createDeviceFlags = 0;
  #if defined(DEBUG) || defined(_DEBUG)  
      createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
  #endif 
  IDXGIFactory1 *pFactory;
  HR(CreateDXGIFactory1(__uuidof(IDXGIFactory),(void**)(&pFactory)));
  IDXGIAdapter1 *pAdapter;
  pFactory->EnumAdapters1(0,&pAdapter);

  D3D10CreateDevice(pAdapter,md3dDriverType,0,createDeviceFlags,D3D10_SDK_VERSION,&mDxDevice);
  pFactory->CreateSwapChain(mDxDevice,&swapchain,&mSwapChain);  
  ReleaseCOM(pAdapter);
  ReleaseCOM(pFactory);

  Global::md3dDevice = mDxDevice;

  return true;
}

bool BaseDX::initWindowsApp()
{
  //Icon
  mAppIcon =  ExtractIcon(mHInstance,L"img\\icon.bmp",0);
  //Window class
  WNDCLASSEX wc;    
  wc.cbSize         = sizeof(WNDCLASSEX);
  wc.style          = CS_HREDRAW | CS_VREDRAW; 
  wc.lpfnWndProc    = WndProc;
  wc.cbClsExtra     = 0; 
  wc.cbWndExtra     = 0; 
  wc.hInstance      = mHInstance;
  wc.hIcon          = mAppIcon;
  wc.hCursor        = LoadCursor(NULL,  IDC_ARROW); 
  wc.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName   = 0;
  wc.lpszClassName  = L"IconWndClassEx"; 
  wc.hIconSm        = mAppIcon;

  //Registramos la instancia WNDCLASS en windows para que podamos crear ventanas basados en ella.
  if(!RegisterClassEx(&wc)){
    MessageBox(0, L"RegisterClass failed", 0, 0);
    return false;
  }

  RECT R = { 0, 0, mCurrentWidth, mCurrentHeight};
  AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
  unsigned int width  = R.right - R.left;
	unsigned int height = R.bottom - R.top;

  mWindowsScreen = CreateWindow(
    L"IconWndClassEx",
    mWindowTitle.c_str(), 
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, //Top left (0,0)
    CW_USEDEFAULT, 
    width, 
    height,
    0,
    0,
    mHInstance,
    this);

  if(mWindowsScreen == NULL){
   MessageBox(0, L"CreateWindow failed", 0, 0);
   return false;
  }
  
  ShowWindow(mWindowsScreen, SW_SHOW);
  UpdateWindow(mWindowsScreen);
  //captureMouse();


  return true;
}



//Extern, it's called by the SO
LRESULT BaseDX::getMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{  
  switch(msg){
     
    case WM_LBUTTONDOWN: 
    case WM_LBUTTONUP: 
    case WM_RBUTTONDOWN: 
    case WM_RBUTTONUP: 
    case WM_MOUSEMOVE: 
        
      //captureMouse();
      mPtsEnd.x = (float)GET_X_LPARAM(lParam); 
      mPtsEnd.y = (float)GET_Y_LPARAM(lParam);       
#ifdef _LOG_ENABLE_MOUSE_INCREMENT  
  *mLog->mFileShipPlayer<< "MEnd: " 
                        << mPtsEnd.x 
                        << ", "
                        << mPtsEnd.y
                        << ", "
                        << mPtsEnd.z 
                        << std::endl;  
#endif
      mMouseState.position = mPtsEnd - D3DXVECTOR3(mCurrentWidth>>1,mCurrentHeight>>1,0);          
      mMouseState.position.x /= float(mCurrentWidth >>1);
      mMouseState.position.y /= float(mCurrentHeight>>1);

#ifdef _LOG_ENABLE_MOUSE_INCREMENT  
  *mLog->mFileShipPlayer<< "Normalized: " 
                        << mMouseState.position.x 
                        << ", "
                        << mMouseState.position.y
                        << ", "
                        << mMouseState.position.z 
                        << std::endl;  
#endif

      if ( mMouseState.position.x < mMouseCenterBias ) mMouseState.position.x = 0.0f;
      else mMouseState.position.x -= mMouseCenterBias;

#ifdef _LOG_ENABLE_MOUSE_INCREMENT  
  *mLog->mFileShipPlayer<< "sensivity: " 
                        << mMouseState.position.x 
                        << ", "
                        << mMouseState.position.y
                        << ", "
                        << mMouseState.position.z 
                        << std::endl;  
#endif

      if ( mMouseState.position.y < mMouseCenterBias ) mMouseState.position.y = 0.0f;
      else mMouseState.position.y -= mMouseCenterBias;
                        
      mMouseState.rightButton = wParam & MK_RBUTTON;
      mMouseState.leftButton  = wParam & MK_LBUTTON;
      //releaseMouse();
      return 0; 
 
  
    case WM_KEYDOWN:     
      mKeyPressed[wParam] = true;  
     return 0;

    case WM_KEYUP:
      mKeyPressed[wParam] = false;       
      return 0;    

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

  	case WM_SYSCOMMAND:
			return 0;			
				
    case WM_SIZE:       
	    mCurrentWidth  = LOWORD(lParam);
	    mCurrentHeight = HIWORD(lParam);      
		  if( wParam == SIZE_RESTORED ){  				
				  onResize();
			}      
      return 0;

    default:
      return DefWindowProc(mWindowsScreen, msg, wParam, lParam);
  }  
  
}

//Modifiers
void BaseDX::enableFps	()
{
	mShowFPS = true;
}

void BaseDX::disableFps()
{
	mShowFPS = false;
}

//Time
void BaseDX::startTimer()
{
	__int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  mPrevTime = mBaseTime = currTime;
  mNumFrame   = 0;
}
void BaseDX::calculateTime()
{
  __int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	
	mDeltaTime = (currTime - mPrevTime)*mSecondsPerCount;
	mPrevTime  = currTime;  
}

double BaseDX::getSeconds()
{   
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	

  return (currTime - mBaseTime) * mSecondsPerCount;
}


float BaseDX::getmDeltaTime() const
{   
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	
  
  return ((currTime - mBaseTime) * (float)mSecondsPerCount) - (float)mSecondsCounter;
}

float BaseDX::getCentiseconds()
{
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	

  return (currTime - mBaseTime) * (float)mSecondsPerCount * 100.0f;
}



void BaseDX::keybCleanAll()
{
  for (int i = 0; i < 256; i++ )
    mKeyPressed[i] = false;  
}


void BaseDX::incrementFrameCounter()
{
  ++mNumFrame;

  if( ( getmDeltaTime()) >= 1.0f )
  {         
    //_storeFps( int(mNumFrame) );
	  // Reset
    mLastFramePerSecondCount = mNumFrame;
	  mNumFrame = 0;
	  mSecondsCounter  += 1.0f;
  }

  float centiseconds = getCentiseconds();
 
}

DeferredEngine* BaseDX::deferredEngineInstance() const
{
  return mDeferredEngine;
}

void BaseDX::onResize()
{  
  if ( mDeferredEngine != NULL )
  {
    //releaseMouse();
    mDeferredEngine->resize( mCurrentWidth, mCurrentHeight );
	  mDeferredEngine->onResize();    
    //captureMouse();
  }
}

void BaseDX::releaseMouse()
{
  ClipCursor(0); 
  ReleaseCapture(); 
}

void BaseDX::captureMouse()
{
  // Capture mouse input.  
  SetCapture(mWindowsScreen);  
  // Retrieve the screen coordinates of the client area, 
  // and convert them into client coordinates. 
  RECT rcClient;                  // client area rectangle     
  POINT ptClientUL;             // client upper left corner 
  POINT ptClientLR;              // client lower right corner
  GetClientRect(mWindowsScreen, &rcClient); 
  ptClientUL.x = rcClient.left; 
  ptClientUL.y = rcClient.top;  
  // Add one to the right and bottom sides, because the 
  // coordinates retrieved by GetClientRect do not 
  // include the far left and lowermost pixels.  
  ptClientLR.x = rcClient.right + 1; 
  ptClientLR.y = rcClient.bottom + 1; 
  ClientToScreen(mWindowsScreen, &ptClientUL); 
  ClientToScreen(mWindowsScreen, &ptClientLR);  
  // Copy the client coordinates of the client area 
  // to the rcClient structure. Confine the mouse cursor 
  // to the client area by passing the rcClient structure 
  // to the ClipCursor function. 
  SetRect(&rcClient, ptClientUL.x, ptClientUL.y, 
          ptClientLR.x, ptClientLR.y); 
  ClipCursor(&rcClient); 
}