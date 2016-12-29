#ifndef _KDD_TREE_2D_SPECIALIZATIONS_HPP
#define _KDD_TREE_2D_SPECIALIZATIONS_HPP

#include "Physics/StaticRigidBody.hpp"
#include "Geometry/2dDataStructures/KddTree2d.hpp"

////////////////////////////////////////////////////////////
//  StaticRigidBody

template<>
float inline kddX<StaticRigidBody>( const StaticRigidBody* aPoint )
{
  return aPoint->centerBoundingSphere().x;
}


template <>
float inline kddY<StaticRigidBody>( const StaticRigidBody* aPoint )
{
  return aPoint->centerBoundingSphere().z;
}

////////////////////////////////////////////////////////////
//  D3DXVECTOR3

template<>
float inline kddX<D3DXVECTOR3>( const D3DXVECTOR3* aPoint )
{
  return aPoint->x;
}


template <>
float inline kddY<D3DXVECTOR3>( const D3DXVECTOR3* aPoint )
{
  return aPoint->y;
}


#endif //_KDD_TREE_2D_SPECIALIZATIONS_HPP
