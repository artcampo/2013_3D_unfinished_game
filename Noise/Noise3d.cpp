#include	"Noise/Noise3d.hpp"

#include  <cmath>

namespace Noise3d{  

  double Noise3d::noise( const D3DXVECTOR3& aVector, const D3DXVECTOR3& aW )
  {    
    D3DXVECTOR3 p ( aVector.x*aW.x,
                    aVector.y*aW.y,
                    aVector.z*aW.z );
    return noise( p );
  }

  double Noise3d::noise( const D3DXVECTOR3& aVector )
  {
    D3DXVECTOR3 vector = aVector;
    int X = (int)floor(vector.x) & 255,                  // FIND UNIT CUBE THAT
        Y = (int)floor(vector.y) & 255,                  // CONTAINS POINT.
        Z = (int)floor(vector.z) & 255;

    vector.x -= floor(vector.x);                                // FIND RELATIVE X,Y,Z
    vector.y -= floor(vector.y);                                // OF POINT IN CUBE.
    vector.z -= floor(vector.z);

    double u = fade(vector.x),                                // COMPUTE FADE CURVES
           v = fade(vector.y),                                // FOR EACH OF X,Y,Z.
           w = fade(vector.z);
    int A = mP[X  ]+Y, AA = mP[A]+Z, AB = mP[A+1]+Z,      // HASH COORDINATES OF
        B = mP[X+1]+Y, BA = mP[B]+Z, BB = mP[B+1]+Z;      // THE 8 CUBE CORNERS,

    return lerp(w, lerp(v, lerp(u, grad(mP[AA  ], vector.x  , vector.y  , vector.z   ),  // AND ADD
                                   grad(mP[BA  ], vector.x-1, vector.y  , vector.z   )), // BLENDED
                           lerp(u, grad(mP[AB  ], vector.x  , vector.y-1, vector.z   ),  // RESULTS
                                   grad(mP[BB  ], vector.x-1, vector.y-1, vector.z   ))),// FROM  8
                   lerp(v, lerp(u, grad(mP[AA+1], vector.x  , vector.y  , vector.z-1 ),  // CORNERS
                                   grad(mP[BA+1], vector.x-1, vector.y  , vector.z-1 )), // OF CUBE
                           lerp(u, grad(mP[AB+1], vector.x  , vector.y-1, vector.z-1 ),
                                   grad(mP[BB+1], vector.x-1, vector.y-1, vector.z-1 ))));
  }

  double Noise3d::fade(double t) 
  { 
    return t * t * t * (t * (t * 6 - 15) + 10); 
  }

  double Noise3d::lerp(double t, double a, double b) 
  { 
    return a + t * (b - a);
  }

  Noise3d::Noise3d()
  {
    if ( !mInitialized )
    {
      mP.resize( 512 );
      for ( int i = 0; i < 256 ; i++ ) 
        mP[256+i] = mP[i] = mPermutation[i];
      mInitialized = true;
    }
  }

  double Noise3d::grad(int hash, double x, double y, double z) 
  {
    int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
    double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
           v = h<4 ? y : h==12||h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
  }

  bool Noise3d::mInitialized = false;
  std::vector<int> Noise3d::mP;

  const int Noise3d::mPermutation[256] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
   };

};