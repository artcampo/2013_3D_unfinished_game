#ifndef _NOISE3D_HPP_
#define _NOISE3D_HPP_

#include  <cstdlib>
#include  <vector>
#include  "dx_misc.hpp"

namespace Noise3d{

class Noise3d {

public: 
  Noise3d();

  double  noise ( const D3DXVECTOR3& aVector, const D3DXVECTOR3& aW );
  double  noise ( const D3DXVECTOR3& aVector );
  double  fade  ( double t );
  double  lerp  ( double t, double a, double b );
  double  grad  ( int hash, double x, double y, double z );  

private:
  static bool mInitialized;
  static std::vector<int> mP;
  static const int        mPermutation[];

};

}

#endif  //_NOISE3D_HPP_