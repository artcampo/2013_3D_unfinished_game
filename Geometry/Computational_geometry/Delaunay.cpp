#include	"Geometry\Computational_geometry\Delaunay.hpp"
#include  "Geometry\Computational_geometry\DelaunayHelpers.hpp"

using namespace DelaunayHelpers;

void IncDelaunay::DeleteEnclosingTriangle()
{


	for (  int i = 0; i < 3  ; ++i)
		DeleteEdgesOfVertex( i );

}

//	Deletes edges && faces outside of the
//	boundary, (int vi, int vi1, int vi2) being three
//	consecutive vertices of the boundary in counter clockwise order
void	IncDelaunay::DeleteEdgesAndFacesFromBoundary ( int vi, int vi1, int vi2 )
{

	std::vector<int> e;
	std::vector<int> f;

	Vertex_ListEdgesAndFaces ( vi1, e, f );

	bool deleteedges = false;
	int first_boundary_segment_index;
	int second_boundary_segment_index;

	for ( unsigned  int i = 0; (i < e.size())  ; ++i)
		{
		if (edgeContainsVertices( e[i], vi, vi1 ))
			first_boundary_segment_index = i;

		if (edgeContainsVertices( e[i], vi1, vi2 ))
			second_boundary_segment_index = i;
		}


	for (  int i = first_boundary_segment_index ; i != second_boundary_segment_index  ;)
		{
		if ( i != first_boundary_segment_index && i!= second_boundary_segment_index )
			{
		
			DeleteEdge( e[i] );	
			}
		i =  (++i)%e.size();
		}

  for (  int i = 0; i < 3  ; ++i)
		DeleteFace ( f[i] );
}

void	IncDelaunay::DeleteEdgesOfVertex ( int v )
{
	std::vector<int> e;
	std::vector<int> f;

	Vertex_ListEdgesAndFaces ( v, e, f );

	for ( unsigned int i = 0; (i < e.size())  ; ++i)
		DeleteEdge( e[i] );

	for ( unsigned int i = 0; (i < f.size())  ; ++i)
		DeleteFace( f[i] );
}

void	IncDelaunay::DeleteEdge		( int e )
{
	DeleteFace( Edges[e].fl );
	DeleteFace( Edges[e].fr );

	EdgeIsValid[e] = false;
}

bool	IncDelaunay::IsEdgeValid( int e )
{
	return EdgeIsValid[e];
}

void	IncDelaunay::Vertex_ListEdgesAndFaces	( int v, std::vector<int> &e , std::vector<int> &f )
{
	e.clear();
	f.clear();

	int edge = Edge_of_Vertex[v];
	int initial_edge = edge;

	do
		{
		e.push_back(edge);
		if ( Edges[edge].vb == v )
			{
			f.push_back( Edges[edge].fl );
			edge = Edges[edge].ep;
			}
		else
			{
			f.push_back( Edges[edge].fr );
			edge = Edges[edge].en;
			}
		}while (initial_edge != edge);
}


void	IncDelaunay::Vertex_ListEdgesAndFaces_Voronoi	( int v, std::vector<int> &e , std::vector<int> &f )
{
	e.clear();
	f.clear();

	int edge = Edge_of_Vertex[v];
	int initial_edge = edge;

	do
		{
		e.push_back(edge);
		if ( Edges[edge].vb == v )
			{
			f.push_back( Edges[edge].fl );
			edge = Edges[edge].ep;
			}
		else
			{
			f.push_back( Edges[edge].fr );
			edge = Edges[edge].en;
			}
		}while (initial_edge != edge);
}

void	IncDelaunay::Compute_Edge_of_Vertex ()
{
	for ( int i = 1; (i < number_of_edges)  ; ++i)
		{
		Edge_of_Vertex[Edges[i].vb] = i;
		Edge_of_Vertex[Edges[i].ve] = i;
		}
}


IncDelaunay::IncDelaunay ()
{
}


void IncDelaunay::RecomputeDrawing()
{
/*
	for ( int i = 0; (i < number_of_edges)  ; ++i)
		{
		if ( IsEdgeValid(i) )
			{
			LineSegment a;
			a.begin = Vertexes[Edges[i].vb];
			a.end 	= Vertexes[Edges[i].ve];
			a.r = 0.0f;
			a.g = 0.0f;
			a.b = 0.0f;
			addSegment ( a );
			}

		if ( !IsEdgeValid(i) )
			{
			LineSegment a;
			a.begin = Vertexes[Edges[i].vb];
			a.end 	= Vertexes[Edges[i].ve];
			a.r 	= 1.0f;
			a.g 	= 0.0f;	
			a.b 	= 0.0f;
			addSegment ( a );
			}

		}


	for ( int i = 0; (i < number_of_vertexes)  ; ++i)
		{
		addPoint (Vertexes[i]);
		}

	for ( int i = 0; (i < number_of_faces)  ; ++i)
		{
		if ( IsFaceValid(i) )
			{
			int e[3],v[3];
			Face_ListEdgesAndVertices ( i, e, v );
			if ( signed_area( Vertexes[v[0]], Vertexes[v[1]], Vertexes[v[2]] ) > 0)
				{
				int temp = v[0];
				v[0] = v[2];
				v[2] = temp;
				}
				
			//Triangle t (v[0],v[1],v[2]);
			}
		}

*/
}

int 	IncDelaunay::LocatePointInFace	( D3DXVECTOR3 p )
{
	// Test against the three edges

	return LocatePointInFace_Recursive ( p , start_search_face , start_search_point, 0 );



}

//	Searches for D3DXVECTOR3 p, in "face" where sp is a D3DXVECTOR3
//	inside that face
int 	IncDelaunay::LocatePointInFace_Recursive	( D3DXVECTOR3 p , int face, D3DXVECTOR3 sp , int traversed_edge )
{
	int face_where_point_is_located = face;
	
	int edges[3], vertices[3];
	Face_ListEdgesAndVertices ( face, edges, vertices );
	// Test against the three edges

	
	//addPoint (p);
	//addPoint (sp);

	bool found = false;	


	for ( int i = 0; (i < 3) && (! found) ; ++i)
		{

		if ( edges[i] != traversed_edge &&
			 Segment_intersection ( p, sp , Vertexes[Edges[ edges[i] ].vb], Vertexes[Edges[ edges[i] ].ve] ) )
			{

			int next_search_face;
			if ( Edges[ edges[i] ].fl == face)
				next_search_face = Edges[ edges[i] ].fr;
			else
				next_search_face = Edges[ edges[i] ].fl;


			D3DXVECTOR3 barycenter_face = FaceBarycenter ( next_search_face );

			face_where_point_is_located = 
				LocatePointInFace_Recursive ( p, next_search_face, barycenter_face, edges[i] );
			found = true;
			}
		}
	
	return face_where_point_is_located;
}

D3DXVECTOR3	IncDelaunay::FaceBarycenter ( int f )
{
	int edges[3], vertices[3];
	Face_ListEdgesAndVertices ( f, edges, vertices );

	D3DXVECTOR3 result = (Vertexes[vertices[0]] + Vertexes[vertices[1]] + Vertexes[vertices[2]]) / 3.0f;
	
	return result;
}

//	Returns edges && vertices of Face f in counter-clockwise order
void 	IncDelaunay::Face_ListEdgesAndVertices	( int f , int e [3], int v [3] )
{
	int	edge = Edge_of_Face[f];

	for ( int i = 0; i < 3 ; ++i)
		{
		e[i] = edge;
		if ( Edges[edge].fl == f )
			{
			v[i] 	= Edges[edge].vb;
			edge	= Edges[edge].ep;
			}
		else
			{
			v[i] 	= Edges[edge].ve;
			edge	= Edges[edge].en;
			}
		}

	//Return edges && vertices in counter-clockwise
	int temp;

	temp = v[0]; v[0] = v[2]; v[2] = temp;		//three points: swap
	temp = e[0]; e[0] = e[2]; e[2] = temp;		//first && last


}

bool IncDelaunay::edgeContainsVertices ( int e, int v1, int v2 )
{
	return ((Edges[e].vb == v1 && Edges[e].ve == v2 )
		||
		(Edges[e].ve == v1 && Edges[e].vb == v2 ));
}



void IncDelaunay::updateEdge_FaceAndPointer ( int e, int old_face, int new_face,  int pointed_e, int old_pointer )
{
	
	if ( Edges[e].fl == old_face )
		{
		
		Edges[e].fl = new_face;
		Edges[e].ep = pointed_e;
		
		}
	else
		{
		Edges[e].fr = new_face;
		Edges[e].en = pointed_e;
		}
}

void IncDelaunay::FlipDisplayCircles ( int p, int f, int e, int index )
{


}

// Checks the delaunay propierty of e, that belongs
// to f, in respect to D3DXVECTOR3 p.
// If it doesn't hold, it flip it, && testes
// the newly created edges.
//
// sets "start_search_face" to last face created.
void IncDelaunay::FlipTest ( int p, int f, int e, int index )
{

	// Check if the edge is in the exterior
	if ( (Edges[e].fl == 0) || (Edges[e].fr == 0) )
		return;

	// Obtain third vertex opposing to e
	int face_opposing;
	int opposing_vertex;
	

	if ( Edges[e].fl == f )
		face_opposing = Edges[e].fr;
	else
		face_opposing = Edges[e].fl;

	
	int edges_opposing[3], vertices_opposing[3];
	Face_ListEdgesAndVertices ( face_opposing, edges_opposing, vertices_opposing );

	
	for ( int i = 0; i < 3 ; ++i)
		{
		if ((vertices_opposing[i] != Edges[e].vb) && (vertices_opposing[i] != Edges[e].ve))
			opposing_vertex = vertices_opposing[i];
		
		}
	//cout << "Opposite vertex: " << opposing_vertex << endl;


	bool given_in_counterclockwise;
	given_in_counterclockwise = ( signed_area(Vertexes[p],
		Vertexes[Edges[e].vb],
		Vertexes[Edges[e].ve]) > 0 );
	float determinant;


	if (given_in_counterclockwise)
		{

		determinant = point_to_circumference_alignment(
				Vertexes[p],
				Vertexes[Edges[e].vb],
				Vertexes[Edges[e].ve],
				Vertexes[opposing_vertex]);

		}
	else
		{

		determinant = point_to_circumference_alignment(
				Vertexes[Edges[e].ve],
				Vertexes[Edges[e].vb],
				Vertexes[p],
				Vertexes[opposing_vertex]);

		}

	if ( determinant < 0 )
		{

		// Flip edge
		// Edge e is updated with new information.
		// Likwise no face is removed, just updated,
		// being we're just modifying the interior
		// of two adjacent triangles (no external changes
		// to the dcel structure

		// Fix an order of quadriteral: p,b,opp,a (counterclockwise)
		
		int b;
		int a;
		
		if ( signed_area(Vertexes[p], Vertexes[opposing_vertex], Vertexes[Edges[e].ve]) < 0 )
			{
			a = Edges[e].ve;
			b = Edges[e].vb;	//vertices
			}
		else
			{
			a = Edges[e].vb;
			b = Edges[e].ve;	//vertices
			}
			



		int pa, pb, ao, bo;	//edges
		
		int edges[3], vertices[3];
		Face_ListEdgesAndVertices ( f, edges, vertices );

		for ( int i = 0; i < 3 ; ++i)
			{
			if ( edgeContainsVertices( edges[i], p, a ) )
				pa = edges[i];
			else
			if ( edgeContainsVertices( edges[i], p, b ) )
				pb = edges[i];
			//else //if (edges[i] != e) { cout << "PANIK -A "<<endl;exit(1);}
			}

		for ( int i = 0; i < 3 ; ++i)
			{
			if ( edgeContainsVertices( edges_opposing[i], opposing_vertex, a ) )
				ao = edges_opposing[i];
			else
			if ( edgeContainsVertices( edges_opposing[i], opposing_vertex, b ) )
				bo = edges_opposing[i];
			//else //if (edges_opposing[i] != e) { cout << "PANIK -B"<<endl;exit(1);}
			}


		//exit(1);


		Edges[e].vb = p;
		Edges[e].ve = opposing_vertex;

		Edges[e].fl = face_opposing;
		Edges[e].fr = f;

		Edges[e].ep = pb;
		Edges[e].en = ao;



		//Update ep,en of edges
		updateEdge_FaceAndPointer ( pa, f, 		f, 		e , pb);
		updateEdge_FaceAndPointer ( pb, f, 		face_opposing,	bo, e );
		updateEdge_FaceAndPointer ( ao, face_opposing, 	f,		pa, e );
		updateEdge_FaceAndPointer ( bo, face_opposing, 	face_opposing,	e , ao);


		Edge_of_Face[f] = e;
		Edge_of_Face[face_opposing] = e;

		
		//Update incident edge of changed faces
		//Edge_of_Face[f] = e;
		//Edge_of_Face[face_opposing] = e;

		//start_search_face = f;

		//cout << "redo flip"<<endl;
		// Test new two edges

		FlipTest ( p, f, ao, index );
		FlipTest ( p, face_opposing, bo, index );

		}
	
	
}


int face_edges 		[3];
int vn;
int newFaces [3];

void IncDelaunay::FlipPending()
{

	FlipTest	( vn, newFaces [0], face_edges[0] , 0);

	FlipTest	( vn, newFaces [1], face_edges[1] , 1);

	FlipTest	( vn, newFaces [2], face_edges[2] , 2);


	start_search_face = newFaces[0];
	start_search_point	= FaceBarycenter ( start_search_face );


}


// Locate D3DXVECTOR3
// List edges of face
// Insert D3DXVECTOR3


void IncDelaunay::AddPoint ( D3DXVECTOR3 p )
{
	//cout << "D3DXVECTOR3 " << p <<endl;
	// Locate D3DXVECTOR3
	int face;		//where to insert the D3DXVECTOR3
	face = LocatePointInFace ( p );
	//cout << " -> in face: " << face << endl;

	// List edges of face
//	int face_edges 		[3];
	int face_vertices 	[3];

	Face_ListEdgesAndVertices ( face , face_edges , face_vertices );

	// Insert D3DXVECTOR3
	vn = AddVertex ( p );
	int newEdges [3];
	

	newEdges [0] = AddEdge ();
	newEdges [1] = AddEdge ();
	newEdges [2] = AddEdge ();

	newFaces [0] = AddFace ( newEdges [0] );	
	newFaces [1] = AddFace ( newEdges [1] );	
	newFaces [2] = AddFace ( newEdges [2] );	


	//Create new edges
	//nitEdge	( int e, int vb, int ve, int fl, int fr, int ep, int en )

	InitEdge	( newEdges [0], face_vertices[0], vn, newFaces[2], newFaces[0], face_edges[2], newEdges [1] );
	InitEdge	( newEdges [1], face_vertices[1], vn, newFaces[0], newFaces[1], face_edges[0], newEdges [2] );
	InitEdge	( newEdges [2], face_vertices[2], vn, newFaces[1], newFaces[2], face_edges[1], newEdges [0] );


	//Update edges of initial face

	updateEdge_FaceAndPointer ( face_edges[0], face, newFaces[0], newEdges[0] , face_edges[2]);
	updateEdge_FaceAndPointer ( face_edges[1], face, newFaces[1], newEdges[1] , face_edges[0]);
	updateEdge_FaceAndPointer ( face_edges[2], face, newFaces[2], newEdges[2] , face_edges[1]);

	DeleteFace ( face );
	start_search_face = newFaces[0];

}

//	Face 0 is outer space
void IncDelaunay::Init ( int num_vertexes , std::vector<D3DXVECTOR3> EnclosingTriangle )
{
	Vertexes 	= std::vector <D3DXVECTOR3>	( (num_vertexes + 4) );
	Edge_of_Vertex	= std::vector <int>		( (num_vertexes + 4) );
	number_of_vertexes = 0;
	max_vertices = (num_vertexes + 4);
	//Edge_of_Vertex	= std::vector <int>		( (num_vertexes + 3) );

	Edges 		= std::vector <DcelEdge>	( 3*(num_vertexes + 3) );
	EdgeIsValid	= std::vector <bool>		( 3*(num_vertexes + 3) );
	number_of_edges = 1;
	max_edges = 3*(num_vertexes + 3);

	Edge_of_Face	= std::vector <int>		( 6*(num_vertexes + 3) );
	FaceIsValid	= std::vector <bool>		( 6*(num_vertexes + 3) );
	max_faces	= 6*(num_vertexes + 3);
	number_of_faces = 1;			

	

	// Create enclosing triangle
	int v1 	= AddVertex 	( EnclosingTriangle[2] );
	int v2 	= AddVertex 	( EnclosingTriangle[1] );
	int v3 	= AddVertex 	( EnclosingTriangle[0] );

	
	int e1	= AddEdge ();
	int e2	= AddEdge ();
	int e3	= AddEdge ();

	int f  	= AddFace ( e1 );

	InitEdge	( e1, v1, v2, f, 0, e3, e2 );
	InitEdge	( e2, v2, v3, f, 0, e1, e3 );
	InitEdge	( e3, v3, v1, f, 0, e2, e1 );



	// TODO: Init search structure
	start_search_face = f;
	start_search_point = FaceBarycenter ( f );
	//start_search_point = barycenter( EnclosingTriangle[0], EnclosingTriangle[0], EnclosingTriangle[0] );

}

void	IncDelaunay::InitEdge	( int e, int vb, int ve, int fl, int fr, int ep, int en )
{


	//cout << "edge: (vb,ve)" << e <<" : "<<vb <<","<<ve<<" (fl,fr) "<<fl<<","<<fr<<endl;

	Edges[e].vb 	= vb;
	Edges[e].ve 	= ve;
	Edges[e].fl 	= fl;
	Edges[e].fr 	= fr;
	Edges[e].ep 	= ep;
	Edges[e].en 	= en;
	
}

int	IncDelaunay::AddFace		( int edge )
{
	Edge_of_Face[number_of_faces] = edge;
	FaceIsValid[number_of_faces] = true;
	++number_of_faces;

	

	return (number_of_faces - 1);	
}


void	IncDelaunay::DeleteFace		( int f )
{
	FaceIsValid[f] = false;
}

bool	IncDelaunay::IsFaceValid( int f )
{
	return FaceIsValid[f];
}

int	IncDelaunay::AddEdge ()
{
	EdgeIsValid[number_of_edges] = true;
	++number_of_edges;
	
	return (number_of_edges - 1);
}



//	Creates a new vertex, returns its identifier (=> position in array!)
int IncDelaunay::AddVertex ( D3DXVECTOR3 p )
{

	
	Vertexes[number_of_vertexes] = p;
	++number_of_vertexes;
	
	return (number_of_vertexes - 1);
}



std::vector<DisplayEdge>		IncDelaunay::ReadAllEdges ( void )
{
	std::vector<DisplayEdge> ret;

	for ( int edgeI = 0; (edgeI < number_of_edges)  ; ++edgeI)
	{
		if ( IsEdgeValid(edgeI) )
		{
			DisplayEdge d;
			d.id		=	edgeI;
			d.vb		=	Vertexes[ Edges[edgeI].vb ];
			d.ve		=	Vertexes[ Edges[edgeI].ve ];
			int vb = Edges[edgeI].vb;
			int ve = Edges[edgeI].ve;

			ret.push_back( d );
		}
	}

	return ret;
}

std::vector<DisplayEdge>		IncDelaunay::ReadAllVoronoiEdges ( void )
{
	std::vector<DisplayEdge> ret;

	for ( int edgeI = 0; (edgeI < number_of_edges)  ; ++edgeI)
	{
		if ( IsEdgeValid(edgeI) && IsFaceValid( Edges[edgeI].fl ) && IsFaceValid( Edges[edgeI].fr ) )
		{
			DisplayEdge d;
			d.id		=	edgeI;
			d.vb		=	CircumscribedCircle ( Edges[edgeI].fl );
			d.ve		=	CircumscribedCircle ( Edges[edgeI].fr );

			ret.push_back( d );
		}
	}

	return ret;
}


int   IncDelaunay::getNumVertices()
{
  return number_of_vertexes;
}
DisplayEdge		IncDelaunay::getVoronoiEdge ( int edgeI)
{
	DisplayEdge d;

  int fl = Edges[edgeI].fl ;
  int fr = Edges[edgeI].fr ;
  //if ( IsEdgeValid(edgeI) && IsFaceValid( Edges[edgeI].fl ) && IsFaceValid( Edges[edgeI].fr ) )
  if ( IsFaceValid( Edges[edgeI].fl ) && IsFaceValid( Edges[edgeI].fr ) )
		{
			
			d.id		=	edgeI;
			d.vb		=	CircumscribedCircle ( Edges[edgeI].fl );
			d.ve		=	CircumscribedCircle ( Edges[edgeI].fr );

		}

	return d;
}	

D3DXVECTOR3 IncDelaunay::getVoronoiVertex ( int face )
{
  return	CircumscribedCircle ( face );
}

D3DXVECTOR3 IncDelaunay::CircumscribedCircle ( int f )
{
	D3DXVECTOR3 ret;

	int v[3], e[3];
	Face_ListEdgesAndVertices ( f, e, v );

	D3DXVECTOR3 a = Vertexes [ v[0] ];
	D3DXVECTOR3 b = Vertexes [ v[1] ];
	D3DXVECTOR3 c = Vertexes [ v[2] ];

	float D= 2* (a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y) );
	float cartx = (a.x*a.x + a.y*a.y)*(b.y-c.y) + (b.y*b.y + b.x*b.x)*(c.y-a.y) + (c.y*c.y + c.x*c.x)*(a.y-b.y);
	float carty = (a.x*a.x + a.y*a.y)*(c.x-b.x) + (b.y*b.y + b.x*b.x)*(a.x-c.x) + (c.y*c.y + c.x*c.x)*(b.x-a.x);

	ret.x = cartx / D;
	ret.y = carty / D;
	ret.z = a.z;

	return ret;
}

std::vector<D3DXVECTOR3> DelaunayEnclosingTriangle ( float radius )
{
	std::vector<D3DXVECTOR3> enclosing_triangle;
	enclosing_triangle.resize (3);
	D3DXVECTOR3 center = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );

	for ( int i = 2 ; i >= 0; --i )
		{
		enclosing_triangle[i].x = float(center.x + (1.8f*radius)*sin(2.0f*3.121419f*float(i)/3.0));
		enclosing_triangle[i].y = float(center.y + (1.8f*radius)*cos(2.0f*3.121419f*float(i)/3.0));
		enclosing_triangle[i].z = 0.0f;
		}
	return enclosing_triangle;
}