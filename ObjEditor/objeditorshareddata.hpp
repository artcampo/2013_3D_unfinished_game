#ifndef OBJEDITORSHAREDDATA_HPP
#define OBJEDITORSHAREDDATA_HPP

#include "dx_misc.hpp"
#include <string>
#include <vector>




#include "ObjEditor/Mesh/HalfEdge/HE_vert.hpp"
#include "Geometry/Intersections.hpp"

namespace ObjEd{

  //class HE_vert;

enum eOpExtMode{
  eOpExtModeOffset,
  eOpExtModeInner,
  eOpExtModeMatrix
};

struct MeshInfo{
  int nv;
  int ne;
  int nf;
  int lastKnowSize;
};

enum eMode{
  eModeObject,
  eModeVertex,
  eModeEdge,
  eModeFace
};

enum eEntityEV{
  eVertex,
  eEdge
};

enum eModeExtension{
  eModeNoEntity
};

enum eOpMode{
  eOpModeSelection,
  eOpModeRotation,
  eOpModeScale,
  eOpModeExtrude,
  eOpModeBevel,
  eOpModeKniveLine,
  eOpModeNormalScale,
  eOpModeTexture
};

enum eRenderMode{
  eTransparentMode,
  ePhongMode,
  eProcTexMode,
  eColTexMode
};

namespace OpId{

const int gsNumOpIds = 10;

enum eOpId{
  ePrimCube,

  eTranslate,
  eScale,

  eExtrude,
  eExtrudeInner,
  eExtrudeMatrix,

  eBevelV,
  eBevelE,

  eKniveLine,
  eNormalScale
};

};

struct Selection{
  int idComponent;
  int typeEntity;   //pertains to eMode
  int idEntity;
};

struct EdgeIntersection{
  int idEntity;     // Id of intersected Edge
  D3DXVECTOR3 p;    // point of cut
};

struct ColourMapping{
  D3DXCOLOR  obj, objSel, objHigh;
  D3DXCOLOR  ver, verSel, verHigh;
  D3DXCOLOR  edg, edgSel, edgHigh;
  D3DXCOLOR  fac, facSel, facHigh;
  D3DXCOLOR  gizmo[3], gizmoSel[3], gizmoHigh[3];
};

class ProcTex{

public:
  ProcTex(){};
  ProcTex( const ProcTex& rhs){
    wcscpy_s ( mPath, rhs.mPath );
    name = rhs.name;
    scale = rhs.scale;
    col = rhs.col;
  };
  ProcTex& operator= ( const ProcTex& rhs )
  {
    wcscpy_s ( mPath, rhs.mPath );
    name = rhs.name;
    scale = rhs.scale;
    col = rhs.col;
    return *this;};


  wchar_t       mPath[2048];
  std::string   name;
  float         scale;
  D3DXCOLOR     col;
};


extern const std::string gsModeNames[];

};

#endif // OBJEDITORSHAREDDATA_HPP
