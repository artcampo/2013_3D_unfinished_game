#ifndef _WORLD_TERRAIN_HPP_
#define _WORLD_TERRAIN_HPP_


#include  <vector>
#include	"dx_misc.hpp"
#include	"Noise/RandomNumberGenerator.hpp"

#include  "Project_Dependant/LogConditionalCompilation.hpp"
#ifdef _LOG_ENABLE_
  class Log;
#endif

#ifdef _LOG_ENABLE_BMP_
  //#include  "Exterior/Libbmp/bmpfile.h"
#endif

struct VertPNT;
template <class T> class DxMeshModelBatch;
template <class T1> class LineListModel;

class Terrain;
class HeightFunction;
class HeightZone;
class ConvexHull2d;

class WorldTerrain
{
public:
  WorldTerrain ();
  ~WorldTerrain (){};
  void  init();
  std::auto_ptr< DxMeshModelBatch<VertPNT> >  compute ();

private:
  std::auto_ptr<HeightFunction> mHeightFunc;
public:
  std::auto_ptr<Terrain>        mTerrain;  
  std::auto_ptr<ConvexHull2d>   mFrontier;  
  std::auto_ptr<Rand::RandGen>  mRandGen;

  std::vector<LineListModel<D3DXVECTOR3>*> mConvexHullsModels;

  void displayConvexHulls();
private:
  void  initCreateFrontiers();
  void  initCreatePlains();
  void  initCreateMountains();

#ifdef _LOG_ENABLE_
  Log*                  mLog;
#endif

#ifdef _LOG_BMP_MOUNTAIN_STEPS_
  void  writeHeightFunction( const char *aFilename ) const;
#endif
};


#endif  //#ifndef _WORLD_TERRAIN_HPP_