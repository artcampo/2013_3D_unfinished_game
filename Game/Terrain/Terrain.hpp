#ifndef _TERRAIN_HPP_
#define _TERRAIN_HPP_


#include <vector>
#include	"dx_misc.hpp"

class VertPNT;
template <class T> class DxMeshModelBatch;

//===========================================================================
/*
*/
//===========================================================================

struct MountainDesc{
  float       maxHeight;
  float       radius;
  D3DXVECTOR3 position;
};

//===========================================================================
/*!
      \class      Terrain 
      \brief      Class that handles dx10 shaders, hlsl 4.0
*/
//===========================================================================
class Terrain
{
public:
  Terrain (){};

  void  pointsSeaBase ( const std::vector<D3DXVECTOR3>& aPoints );

  std::auto_ptr< DxMeshModelBatch<VertPNT> >  compute ();
    void  configure ( );

private:

  void  computePointDistribution ();
  std::auto_ptr< DxMeshModelBatch<VertPNT> >  computeTriangulation ();

  void  pointsAdd ( D3DXVECTOR3& aPoint );
  
  

  
  std::vector<D3DXVECTOR3>  mPoints;  /*!< Array with points of the terrain */

  //  Configuration parameters
  float       mHeight, mHeightRand;
  
};


//---------------------------------------------
// EXTERN FUNCTIONS
//---------------------------------------------


#endif