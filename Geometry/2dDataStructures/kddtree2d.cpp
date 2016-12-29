#include "Geometry/2dDataStructures/KddTree2d.hpp"
#include "Geometry/2dDataStructures/KddTree2dSpecializations.hpp"



template <class Point>
int  KddTree2d<Point>::numPoints() const
{
  return mNumPoints;
}

template <class Point>
KddTree2d<Point>::KddTree2d ( std::vector<Point*> aPoints )
{
  /*
  //  Forces to define member functions for each instantiated type
  float fa = aPoints[0]->x();
  float fb = aPoints[1]->y();
  

  // this is how to force a non-member template function
  float fa = kddX<Point>(aPoints[0]);
  float fb = kddY<Point>(aPoints[1]);*/

  //  Insert one by one and sort
  std::vector<Point*> points ( aPoints.size() );
  for ( int pointi = 0; pointi < aPoints.size() ; ++pointi )
  {
    points[pointi] = aPoints[pointi];  
  }
  mNumPoints = points.size();

  //*Global::mFileDebugKdTree << "N: " << points.size() << std::endl;

  //Global::kd_iter = 0;
  //  Now partition all the points
  mRoot = partitionPoints( points, true, 0, points.size() );
}

template <class Point>
bool kdtSortX (const Point* a, const Point* b) 
{ return ( kddX( a ) < kddX( b )); }

template <class Point>
bool kdtSortY (const Point* a, const Point* b) 
{ return ( kddY( a ) < kddY( b )); }


template <class Point>
typename KddTree2d<Point>::KddTree2dNode*
KddTree2d<Point>::partitionPoints (  std::vector<Point*>& aPoints, 
                                          bool aOrder, 
                                          int aBeg, 
                                          int aEnd )
{
  
  if ( (aEnd - aBeg) < 1 )
    return NULL;

  typedef bool (*comparer_t)(const Point*, const Point*);
  comparer_t cmp;
  if (aOrder)
    cmp = &kdtSortX;
  else
    cmp = &kdtSortY;

  //  Sort by coordinate
  std::sort( aPoints.begin() + aBeg, aPoints.begin() + aEnd, cmp );
   
  //  Select pivot  
  int   indexPivot = (aEnd - aBeg )/2 + aBeg;
  float px = kddX( aPoints[indexPivot] );
  float py = kddY( aPoints[indexPivot] );


  KddTree2dNode* node = new KddTree2dNode( px, py, aPoints[indexPivot] );

  node->setLeftNode ( partitionPoints( aPoints, !aOrder, aBeg, indexPivot  ) ); 
  node->setRightNode( partitionPoints( aPoints, !aOrder, indexPivot + 1, aEnd ) ); 

  return node; 
}

template <class Point>
std::vector<Point*>
KddTree2d<Point>::neighbours( const float aX, 
                                    const float aY,
                                    const float aEpsilon )
{
  std::vector<Point*> points;

  neighboursRec( mRoot, true, aX, aY, aEpsilon, points );

  return points;
}

template <class Point>
void
KddTree2d<Point>::neighboursRec( typename KddTree2d<Point>::KddTree2dNode* aNode,
                    bool aOrder, 
                    const float aX, 
                    const float aY,
                    const float aEpsilon,
                    std::vector<Point*>& aPoints )
{
  bool nearLine = false;
  bool isInLeftBranch;
  if ( aOrder )
  {
    if ( fabs( aX - aNode->px() ) < aEpsilon )
      nearLine = true;

    isInLeftBranch = ( aX < aNode->px() );
  }
  else
  {
    if ( fabs( aY - aNode->py() ) < aEpsilon )
      nearLine = true;
    isInLeftBranch = ( aY < aNode->py() );
  }

  //  Check if point has to be added
  if ( nearLine )
  {
    float x     = aX - aNode->px();
    float y     = aY - aNode->py();
    float dist  = sqrt( x*x + y*y );
    if ( dist < aEpsilon )
      aPoints.push_back( aNode->point() );
  }

  //  Recurse 
  if( ( nearLine || isInLeftBranch ) && aNode->hasLeftNode() )
      neighboursRec( aNode->leftNode(), !aOrder, aX, aY, aEpsilon, aPoints ); 

  if( ( nearLine || !isInLeftBranch ) && aNode->hasRightNode() )
      neighboursRec( aNode->rightNode(), !aOrder, aX, aY, aEpsilon, aPoints ); 

}