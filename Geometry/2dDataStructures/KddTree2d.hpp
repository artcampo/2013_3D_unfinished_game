#ifndef _KDD_TREE_2D_HPP
#define _KDD_TREE_2D_HPP

#pragma ONCE
//#include <algorithm>
#include <vector>
#include <cmath>

namespace Kdd2d{

///////////////////////////////////////////////////////////////
//  Template to be specialized by class to be instantiated
///////////////////////////////////////////////////////////////
  
template <typename T>
float kddX( const T* aPoint )
{
  return 0;
}


template <typename T>
float kddY( const T* aPoint )
{
  return 0;
}



///////////////////////////////////////////////////////////////
//  KddTree2d
///////////////////////////////////////////////////////////////
template <class Point>
class KddTree2d{

public:
  KddTree2d ( std::vector<Point*> aPoints );

  std::vector<Point*>   neighbours( const float aX, 
                                    const float aY,
                                    const float aEpsilon );

  int numPoints() const;

private:
  ///////////////////////////////////////////////////////////////
  //  KddTree2d Node
  ///////////////////////////////////////////////////////////////

  class KddTree2dNode;
  class KddTree2dNode{
  public:
    KddTree2dNode(  const float aX, 
                    const float aY,
                    Point* aPoint )
    {
      mX      = aX;
      mY      = aY;
      mPoint  = aPoint;
      mRight  = NULL;
      mLeft   = NULL;
    }
    void setLeftNode  (  KddTree2dNode* aLeft )
    {
      mLeft = aLeft;
    }
    void setRightNode (  KddTree2dNode* aRight )
    {
      mRight = aRight;
    }

    bool hasLeftNode () const
    {
      return (mLeft != NULL);
    }
    bool hasRightNode () const
    {
      return (mRight != NULL);
    }
    KddTree2dNode* leftNode () const
    {
      return mLeft;
    }
    KddTree2dNode* rightNode () const
    {
      return mRight;
    }

    float px() const
    {
      return mX;
    }
    float py() const
    {
      return mY;
    }

    Point* point() const
    {
      return mPoint;
    }

    
  private:
    float                   mX, mY;
    Point*                  mPoint;
    typename KddTree2d<Point>::KddTree2dNode*          mRight;
    typename KddTree2d<Point>::KddTree2dNode*          mLeft;
  };

///////////////////////////
private:
  KddTree2dNode*  mRoot;
  int             mNumPoints;
private:
  
  typename KddTree2d<Point>::KddTree2dNode*
  partitionPoints( std::vector<Point*>& aPoints, 
                              bool aOrder, 
                              int aBeg, 
                              int aEnd );

  void neighboursRec( typename KddTree2d<Point>::KddTree2dNode* aNode,
                      bool aOrder, 
                      const float aX, 
                      const float aY,
                      const float aEpsilon,
                      std::vector<Point*>& aPoints );
};


#include "KddTree2d.cpp"

};

#endif //_KDD_TREE_2D_HPP
