#ifndef _SPLINELENGTHMAPPED_HPP
#define _SPLINELENGTHMAPPED_HPP

#include  <vector>

#include  "dx_misc.hpp"
#include  "Project_Dependant/StoredSplines.hpp"

class SplineLengthMapped
{
public:

	//SplineLengthMapped( float* aData, int aNumNodes );
  SplineLengthMapped( StoredSplineDesc *aDesc );
	~SplineLengthMapped();


  float getPoint ( float t );

private:

  int                 mNumNodes;       /*!< Number of total nodes */
  std::vector<D3DXVECTOR2>  mNodes;          /*!< Spline nodes */

  std::vector<float>  mAccLength;      /*!< Accumulated length of node ith, undefined for i=0, i=mNumNodes-1.
                                            mAccLength[i=mNumNodes-2] contains total length */
  //std::vector<float>  mLength;         /*!< Number of strips, undefined for i=0, i=mNumNodes-1  */
  float               mTotalLength;

  void    computeLengths();
};


#endif 
