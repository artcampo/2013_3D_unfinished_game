#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <iostream>
#include <fstream>
#include "Misc/Timing_windows_x64.hpp"

class Log{

public:
  Log ()
  {
    if ( !mInitialized )
    {
      initLog();   
      mInitialized = true;
    }
    ++mReferences;
  }
  ~Log ()
  {
    --mReferences;
    if ( mReferences == 0 )
    {
      mFileShipPlayer->flush();
      mFileShipPlayer->close();

      mFileWorldCreation->flush();
      mFileWorldCreation->close();
    
      /*
      mFileFps->flush();
      mFileFps->close();      
      mPhysicsComplexity->flush();
      mPhysicsComplexity->close();          
      delete mProf;
      delete mProfGeom;
      */
    }
  }
public:
  static std::ofstream* mFileShipPlayer;
  static std::ofstream* mFileWorldCreation;

  /*
  static std::ofstream* mFileFps;  
  static ProfilerDp*    mProf;
  static ProfilerDp*    mProfGeom;
  */

private:
  static bool mInitialized;
  static int  mReferences;
  
private:
  void initLog()
  {
    /*
    mProf     = new ProfilerDp( std::string("General") );
    mProfGeom = new ProfilerDp( std::string("Branches") );
*/
    mFileShipPlayer = new std::ofstream ( "./Log/Ships/ShipPlayer.txt", std::fstream::out );  
    mFileShipPlayer->setf(std::ios::fixed,std::ios::floatfield); 
    mFileShipPlayer->precision(4);

    mFileWorldCreation = new std::ofstream ( "./Log/World/creation.txt", std::fstream::out );  
    mFileWorldCreation->setf(std::ios::fixed,std::ios::floatfield); 
    mFileWorldCreation->precision(0);

    
/*
    mFileFps = new std::ofstream ( "./log/log_Fps_lights.txt", std::fstream::out );  
    mFileFps->setf(std::ios::fixed,std::ios::floatfield); 
    mFileFps->precision(0);
*/
    
    
  }

};
   
#endif  //  _STL_HELPERS_HPP_
