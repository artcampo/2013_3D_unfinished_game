#include "Geometry/Spline/SplineLengthMapped.hpp"

//===========================================================================
/*!
    Init.

    \param      *aData    Pointer to vec4
    \param      aNumNodes Number of nodes

*/
//===========================================================================

SplineLengthMapped::SplineLengthMapped( StoredSplineDesc *aDesc )
{
  mNumNodes = aDesc->numElem;
  mNodes.resize( mNumNodes );
  mAccLength.resize( mNumNodes );
  //mLength.resize( mNumNodes );
  float* data = aDesc->data;

  for ( int i = 0; i < mNumNodes ; ++i)
  {
    mNodes[i].x = *(data + 4*i + 0);
    mNodes[i].y = *(data + 4*i + 1);
  }

  computeLengths();
}
 
SplineLengthMapped::~SplineLengthMapped()
{  	

}


void SplineLengthMapped::computeLengths()
{

 mAccLength[1] = 0.0f;

 for ( int i = 2; i < (mNumNodes - 1) ; ++i)
  {
    D3DXVECTOR2 difference = mNodes[i] - mNodes[i - 1];
    float distance = D3DXVec2Length( &difference );
    distance = mNodes[i].y - mNodes[i - 1].y;

    mAccLength[i] = mAccLength[i - 1] + distance;
  }

 mTotalLength = mAccLength[mNumNodes - 2];
}

//===========================================================================
/*!
    Init.

    \param      t       0<=t<=1
    \returns    point at length t

*/
//===========================================================================

float SplineLengthMapped::getPoint ( float t )
{
  bool  nodeFound = false;
  int   index     = 1;
  while (!nodeFound)
  {
    float currentParam = mAccLength[index] / mTotalLength;
    if ( t <= currentParam )
      nodeFound = true;
    else
      ++index;

  }

  --index;
  float currentParam = mAccLength[index] / mTotalLength;
  float targetT = mTotalLength * t;
  float archT;
  if ( t == 0.0f || ( targetT - mAccLength[index] == 0.0f))
    archT = 0.0f;
  else
  {
    float range = mAccLength[index + 1] - mAccLength[index];
    float interRangePos = targetT - mAccLength[index];
    archT = interRangePos / range;
  }
    
  if (t == 0.0f)
    return mNodes[1].x;
  if (t == 1.0f)
    return mNodes[mNumNodes - 2].x;
  else
  {
    D3DXVECTOR2 result;
    D3DXVec2CatmullRom (  &result, &mNodes[index - 1], &mNodes[index - 0],
                          &mNodes[index + 1], &mNodes[index + 2], archT);

    return result.x;
  }


}