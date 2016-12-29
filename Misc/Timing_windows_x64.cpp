#include	"Timing_windows_x64.hpp"

using namespace std;

ProfilerDp::ProfilerDp( std::string& aTitle )
{
  mTitle = aTitle;
}



void ProfilerDp::start ( std::string& aName )
{
  mBlockNames.push_back( aName );
  mStart = ReadTSC();
}


void ProfilerDp::end ( )
{
  mEnd = ReadTSC();
  Measurement m;
  m.totalTime = time();
  mTimings.push_back ( m );
}

long long ProfilerDp::time()
{
  return mEnd-mStart;
}

void ProfilerDp::dumpToFile(const char* aFileName)
{
  std::ofstream  file( aFileName, std::fstream::out ); 
  file.setf(std::ios::fixed,std::ios::floatfield); 
  file.precision(0);

  double sum = 0;
  for (unsigned int j = 0; j < mTimings.size() ; ++j)
    sum += mTimings[j].totalTime;


  file << "------------------------------------------------------------" << endl;
  file << "Init times: "<< mTitle << endl;
  file << "------------------------------------------------------------" << endl;
  for (unsigned int j = 0; j < mTimings.size() ; ++j)
  {
    file  << (mTimings[j].totalTime*100)/sum
      << " [" << (mTimings[j].totalTime/1000000)<< "]: " 
          << mBlockNames[j] <<endl;
  }
  file << "Total time:" << (sum/1000000)<< endl; 


  file.close();
}

ProfilerLoopDp::ProfilerLoopDp( std::string& aTitle )
{
  mTitle = aTitle;
}



void ProfilerLoopDp::start ( std::string& aName )
{
  if ( mIds.find( aName ) == mIds.end() )
  {
    mCurrentId = mIds.size();
    mIds[aName] = mCurrentId;
    
    mTimings.resize( mCurrentId + 1 );
    mBlockNames.resize( mCurrentId + 1 );
    mBlockNames[mCurrentId] = aName;    
  }
  else
  {
    mCurrentId = mIds[aName];
  }
  mStart = ReadTSC();
}


void ProfilerLoopDp::end ( )
{
  mEnd = ReadTSC();
  Measurement m;
  m.totalTime = time();
  mTimings[mCurrentId].push_back ( m );
}

long long ProfilerLoopDp::time()
{
  return mEnd-mStart;
}

void ProfilerLoopDp::dumpToFile(const char* aFileName, const char* aFileNameExtended)
{
  std::ofstream  file( aFileName, std::fstream::out ); 
  file.setf(std::ios::fixed,std::ios::floatfield); 
  file.precision(0);

  double sum = 0;
  std::vector<double> mesT ( mTimings.size() );
  for (unsigned int j = 0; j < mTimings.size() ; ++j)
  {
    float sumT = 0.0f;
    for (unsigned int t = 0; t < mTimings[j].size() ; ++t)
      sumT += mTimings[j][t].totalTime;
    mesT[j] = sumT / double( mTimings[j].size() );
    sum += mesT[j];
  }


  file << "------------------------------------------------------------" << endl;
  file << "Loop times: "<< mTitle << endl;
  file << "------------------------------------------------------------" << endl;
  for (unsigned int j = 0; j < mTimings.size() ; ++j)
  {
    file  << (mesT[j]*100)/sum
      << " [" << (mesT[j]/10000)<< "]: " 
          << mBlockNames[j] <<endl;
  }
  file << "Total time:" << (sum/1000000)<< endl; 


  file.flush();
  file.close();


  std::ofstream  fileExt( aFileNameExtended, std::fstream::out ); 
  fileExt.setf(std::ios::fixed,std::ios::floatfield); 
  fileExt.precision(0);

  fileExt << "------------------------------------------------------------" << endl;
  fileExt << "Loop times per frame: "<< mTitle << endl;
  fileExt << "------------------------------------------------------------" << endl;

  for (unsigned int time = 0; time < mBlockNames.size() ; ++time)  
    fileExt << time << " is : " << mBlockNames[time] << endl;  
  fileExt << endl;

  for (unsigned int time = 0; time < mTimings[0].size() ; ++time)
  {
    float sumT = 0.0f;
    for (unsigned int element = 0; element < mTimings.size() ; ++element)
      sumT += mTimings[element][time].totalTime;

    for (unsigned int element = 0; element < mBlockNames.size() ; ++element)
      fileExt        
          << element << ": "
          << 100.0f*mTimings[element][time].totalTime/sumT 
          << "  [" << mTimings[element][time].totalTime/1000000 << "]"
          << endl;

    fileExt << endl;
  }  


  fileExt.flush();
  fileExt.close();
}

