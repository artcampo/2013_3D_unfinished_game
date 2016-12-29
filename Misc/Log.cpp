#include  "Misc/Log.hpp"

bool  Log::mInitialized = false;
int   Log::mReferences  = 0;

std::ofstream* Log::mFileShipPlayer;
std::ofstream* Log::mFileWorldCreation;


/*
std::ofstream* Log::mFileFps;
ProfilerDp*    Log::mProf;
ProfilerDp*    Log::mProfGeom;
*/