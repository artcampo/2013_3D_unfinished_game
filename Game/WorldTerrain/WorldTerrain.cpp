#include  <memory>
#include  "Game/Terrain/Terrain.hpp"
#include  "Game/WorldTerrain/WorldTerrain.hpp"
#include  "Game/WorldTerrain/HeightFunction.hpp"
#include  "Game/WorldTerrain/HeightZone.hpp"
#include	"Noise/Noise3d.hpp"
#include  "Geometry/ConvexHull2d/ConvexHull2d.hpp"
#include  "Geometry/ConvexHull2d/ConvexHull2dHelpers.hpp"
#include  "RenderModels\Basics\linelistmodel.h"
#include  "Dx/DefVertexLayouts.hpp"

#include  "Project_Dependant/LogConditionalCompilation.hpp"
#ifdef _LOG_ENABLE_
  #include  "Misc/Log.hpp"
#endif


WorldTerrain::WorldTerrain() :
  mRandGen( new Rand::RandGen( 42 ) )
{

}


void  WorldTerrain::init()
{
  mHeightFunc = std::auto_ptr<HeightFunction> ( new HeightFunction() );

  initCreateFrontiers();
  //initCreatePlains();
  initCreateMountains();

  mTerrain = std::auto_ptr<Terrain>( new Terrain( ) );

  for ( int i = 0 ; i < mHeightFunc->mZones.size() ; ++i )
  {
    mConvexHullsModels.push_back( new LineListModel<D3DXVECTOR3>( mHeightFunc->mZones[i]->mOuterHull->mPoints ) ) ;
  }

  void  pointsSeaBase ( const std::vector<D3DXVECTOR3>& aPoints );
  int N = 10000;
  std::vector<D3DXVECTOR3> points ( N );
  for ( int i = 0 ; i < N; ++i )
  {
    D3DXVECTOR3 p = mFrontier->getRandomPoint( *mRandGen );

    float height = mHeightFunc->height( p );
    p.y = height;
    points[i] = p;
  }

  mTerrain->configure();
  mTerrain->pointsSeaBase( points );
 
  mHeightFunc->height( D3DXVECTOR3(500,0,500) );

}

std::auto_ptr< DxMeshModelBatch<VertPNT> >  WorldTerrain::compute ()
{
  return mTerrain->compute();
}

void WorldTerrain::initCreateFrontiers()
{
  std::vector<D3DXVECTOR3> pointsHull;
  pointsHull.push_back( D3DXVECTOR3(0,0,0)  );
  pointsHull.push_back( D3DXVECTOR3(1000,0,0)  );
  pointsHull.push_back( D3DXVECTOR3(1000,0,1000)  );
  pointsHull.push_back( D3DXVECTOR3(0,0,1000)  );  
  
  mFrontier = std::auto_ptr<ConvexHull2d> ( new ConvexHull2d ( pointsHull ) );

  HeightZone* zFrontier = new HeightZone( new ConvexHull2d ( pointsHull ) , -100, 50 );
 
  mHeightFunc->add ( zFrontier );
}

void WorldTerrain::displayConvexHulls()
{
  for ( int i = 0 ; i < mConvexHullsModels.size(); ++i ) 
    mConvexHullsModels[i]->display();
}

void WorldTerrain::initCreatePlains()
{
  
  for ( int i = 0 ; i < 3; ++i ) 
  {
    D3DXVECTOR3 center = mFrontier->getRandomPoint( *mRandGen );
    //float height = 50 + 50*mRandGen->randFloat_0_1();
    float height = 20 + 5*mRandGen->randFloat_0_1();
    float radius = 200 + 50*mRandGen->randFloat_0_1();
    int numLevels = 4;
    for ( int j = 0 ; j < numLevels; ++j ) 
    {
      
      height += 20;
      radius -= 20;
      ConvexHull2d* c = ConvexHull2dHelpers::randomConvexHull( center, radius, 2 + 2*(numLevels - j + 1) + mRandGen->randInt(3), *mRandGen );

      HeightZone* zFrontier = new HeightZone( c, height , 60 );
     
      mHeightFunc->add ( zFrontier );
    }
  }  
  
}


void WorldTerrain::initCreateMountains()
{
  
  for ( int i = 0 ; i < 1; ++i ) 
  {
    //D3DXVECTOR3 center = mFrontier->getRandomPoint( *mRandGen );
    D3DXVECTOR3 center  = D3DXVECTOR3(500,0,500);
    D3DXVECTOR3 u       = D3DXVECTOR3(1,0,0);
    D3DXVECTOR3 v       = D3DXVECTOR3(0,0,2.5);
    float length        = 40;
    float growth        = 1.2;
    int numPoints       = 20;
    int numSlopes       = 3;
    float height        = 80;    

    std::vector<D3DXVECTOR3> pointsHull;
    for ( int slope = 0 ; slope < numSlopes; ++slope ) 
    {
      for ( int j = 0 ; j < numPoints; ++j ) 
      {
        D3DXVECTOR3 p = center 
                        + length*u*mRandGen->randFloat_minus1_1()
                        + length*v*mRandGen->randFloat_minus1_1();

        pointsHull.push_back( p );


#ifdef _LOG_ENABLE_WORLD_POINTS_IN_MOUNTAIN
  *mLog->mFileWorldCreation<< "Point at: " 
                        << p.x 
                        << ", "
                        << p.y
                        << ", "
                        << p.z 
                        << std::endl;  
#endif
      }


      ConvexHull2d* c = ConvexHull2dHelpers::convexHull( pointsHull );
      D3DXVECTOR3 center  = c->center();
      float       area    = c->aproxArea();
      HeightZone* mountainLevel = new HeightZone( c, height , 30 );
      mHeightFunc->add ( mountainLevel );

#ifdef _LOG_ENABLE_WORLD_MOUNTAIN
  *mLog->mFileWorldCreation<< "Mountain at: " 
                        << center.x
                        << ", "
                        << center.y
                        << ", "
                        << center.z
                        << " AproxArea: "
                        << area
                        << " Height: "
                        << height
                        << std::endl;  
#endif

      u *= (growth + growth*0.5f*mRandGen->randFloat_0_1() );
      v *= (growth + growth*0.5f*mRandGen->randFloat_0_1() );
      //numPoints *= 2;
      //height        -= 20 + 5*mRandGen->randFloat_0_1();
      //height        -= 30;

    }
    
  }  
  
}


void  WorldTerrain::writeHeightFunction( const char *aFilename  ) const
{
  

  const int widthBmp  = 1000;
  const int heightBmp = 1000;
  std::vector< std::vector<float> >  depth( heightBmp );

  for ( int i = 0; i < depth.size(); ++i )
    depth[i].resize( widthBmp );

  for ( int x = 0; x < widthBmp; ++x )
    for ( int y = 0; y < widthBmp; ++y )
    {

    }


}