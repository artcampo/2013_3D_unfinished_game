#ifndef _INC_TRIANGULATION_H
#define _INC_TRIANGULATION_H


#include	<vector>
#include  "dx_misc.hpp"




class DcelEdge {

public:
	int		id;
	int		vb, ve;	
	int		fl, fr;
	int		ep, en;

};

class DisplayEdge {
public:
	int				      id;			//id of edge
	D3DXVECTOR3			vb;
	D3DXVECTOR3			ve;
};


class IncDelaunay{
public:
  int			number_of_faces;
  std::vector <D3DXVECTOR3>		Vertexes;
  void 	Face_ListEdgesAndVertices	( int f, int e [3] , int v [3] );
  bool	IsFaceValid			( int f );
private:
	//	Size = n
	
	std::vector <int>		Edge_of_Vertex;
	int			number_of_vertexes;
	//std::vector <int>		Edge_of_Vertex;
	int max_vertices;

	//	Maximum size = 3n
	std::vector <DcelEdge>	Edges;
	std::vector <bool>		EdgeIsValid;
	int			number_of_edges;
	int 			max_edges;

	//	Maximum size = 2n
	std::vector <int>		Edge_of_Face;
	std::vector <bool>		FaceIsValid;
	
	int			max_faces;

	//	For searching a new D3DXVECTOR3
	int	start_search_face;
	D3DXVECTOR3	start_search_point;

	// for display
	int	selected_edge;
	int	selected_point;
	int	selected_face;

	////////////////////////////////////////
	//	functions

	//Related to primary data
	int 	AddVertex 	( D3DXVECTOR3 p );
	int	AddFace		( int edge );
	int	AddEdge		();

	void	InitEdge	( int e, int vb, int ve, int fl, int fr, int ep, int en );

	//Related to the algorithm
	int 	LocatePointInFace		( D3DXVECTOR3 p );
	int 	LocatePointInFace_Recursive	( D3DXVECTOR3 p , int face, D3DXVECTOR3 sp, int traversed_edge );
	D3DXVECTOR3	FaceBarycenter			( int f );

	
	
	//Specific of delaunay
	void	FlipTest 			( int p, int f, int e, int index );

	bool 	edgeContainsVertices 		( int e, int v1, int v2 );
	void	updateFace			( int e, int old_face, int new_face );
	void	updateEdgePointer		( int e, int f, int pointed_e );
	void	DeleteFace			( int f );
	
	

	void 	updateEdge_FaceAndPointer ( int e, int old_face, int new_face,  int pointed_e, int old_pointer );

	void 	FlipDisplayCircles ( int p, int f, int e, int index );

	//Newest, to deal with vertices
	
	void	DeleteEdge			( int e );
	bool	IsEdgeValid			( int e );
	

public:

  // targeted to voronoi extraction
  void Vertex_ListEdgesAndFaces_Voronoi	( int v, std::vector<int> &e , std::vector<int> &f );
  void	Vertex_ListEdgesAndFaces	( int v, std::vector<int> &e , std::vector<int> &f );
  DisplayEdge getVoronoiEdge ( int edgeI );
  D3DXVECTOR3 getVoronoiVertex ( int face );

  int   getNumVertices();

  //common
	IncDelaunay	 	();

	void 	Init 		 ( int num_vertexes , std::vector<D3DXVECTOR3> EnclosingTriangle );
	void 	AddPoint 	 ( D3DXVECTOR3 p );
	void	FlipPending	 ();
	void	RecomputeDrawing ();
	void	Compute_Edge_of_Vertex ();
	void	DeleteEdgesOfVertex ( int v );
	void	DeleteEdgesAndFacesFromBoundary ( int vi, int vi1, int vi2 );
	void	DeleteEnclosingTriangle();

	D3DXVECTOR3	CircumscribedCircle ( int f );

	///////////////////////////////
	//	Used for display of edges

	std::vector<DisplayEdge>		ReadAllEdges		( void );
	std::vector<DisplayEdge>		ReadAllVoronoiEdges ( void );

	///////////////////////////////
	//	Used for display of faces

};


std::vector<D3DXVECTOR3> DelaunayEnclosingTriangle ( float radius );

#endif
