#ifndef _DX_VERTEX_LAYOUTS_DEFERRED_HPP_
#define _DX_VERTEX_LAYOUTS_DEFERRED_HPP_

#include <d3dx10.h>
#include <dxerr.h>

//---------------------------------------------
// AUXILIAR STRUCTS
// Letter at the end
// W: World
// H: Homogeneous clip space
//---------------------------------------------

struct Vertex
{
  D3DXVECTOR3 pos;  // Position
};


struct VertexPos
{
  D3DXVECTOR3 pos;  // Position
};

struct VertexPosCol
{
  D3DXVECTOR3 pos;  // Position
  D3DXCOLOR   col;
};


class VertexPosNor
{
public:
  VertexPosNor (){};
  VertexPosNor ( D3DXVECTOR3 p, D3DXVECTOR3 n ) : pos(p), nor(n) {};
  D3DXVECTOR3 pos;  // Position
  D3DXVECTOR3 nor;  // Normal
};


struct VertexPosNorCol
{
  D3DXVECTOR3 pos;  // Position
  D3DXVECTOR3 nor;  // Normal
  D3DXCOLOR   col;
};



struct VertPNT
{
  D3DXVECTOR3 pos;  // Position
  D3DXVECTOR3 nor;  // Normal
  D3DXVECTOR3 tan;  // Tangent
};

struct VertPNTUV
{
  D3DXVECTOR3 pos;  // Position
  D3DXVECTOR3 nor;  // Normal
  D3DXVECTOR3 tan;  // Tangent
  D3DXVECTOR2 uv;   // Texture Coordinates
};


struct ColorVertex
{
  D3DXVECTOR3 pos;  // Position
  D3DXCOLOR   color;  
};

struct  PointLight{
    D3DXVECTOR3   center;
    float         radius;       //World coordinates
    D3DXVECTOR4   colour;
};

struct ScreenVertex
{
    D3DXVECTOR4 pos;
    D3DXVECTOR2 tex;
};

struct ParticleVertex
{
	D3DXVECTOR3 centerW;
	D3DXVECTOR2 sizeW;
  D3DXCOLOR   color;  
};

struct VertexPointPSC
{
  D3DXVECTOR3 centerW;
  D3DXVECTOR2 sizeW;
  D3DXCOLOR   color;
};

struct VertexPC
{
  D3DXVECTOR3 pos;  // Position
  D3DXCOLOR   col;
};

#endif //_DX_VERTEX_LAYOUTS_DEFERRED_HPP_

