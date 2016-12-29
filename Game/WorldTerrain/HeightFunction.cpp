#include  <memory>
#include  "Game/WorldTerrain/HeightFunction.hpp"
#include	"Noise/Noise3d.hpp"
#include  "Game/WorldTerrain/HeightZone.hpp"

HeightFunction::HeightFunction()
{

}

HeightFunction::~HeightFunction()
{
  for ( int i = 0; i < mZones.size(); ++i )
    delete mZones[i];
}

 
void HeightFunction::add ( HeightZone* aZone )
{
  mZones.push_back( aZone );
}

float   HeightFunction::height ( const D3DXVECTOR3& aPoint )
{
  float height = 0;
  for( int i = 0; i < mZones.size(); ++i )
  {

    height += mZones[i]->height( aPoint );
  }

  return height;
}