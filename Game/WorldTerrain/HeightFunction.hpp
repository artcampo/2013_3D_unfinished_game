#ifndef _HEIGHT_FUNCTION_HPP_
#define _HEIGHT_FUNCTION_HPP_

#include  <vector>
#include	"dx_misc.hpp"

class HeightZone;

class HeightFunction
{
public:
  HeightFunction ();
  ~HeightFunction();

  void    add ( HeightZone* aZone );
  float   height ( const D3DXVECTOR3& aPoint );

private:
public:
  
  std::vector< HeightZone* > mZones;

};



#endif  //_HEIGHT_FUNCTION_HPP_
