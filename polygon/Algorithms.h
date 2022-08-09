#pragma once
#include <set>
#include <tuple>
#include <vector>
#include "Point.h"
#include "Segment.h"
#include "Polygon.h"
#include "Graph.h"

using std::set;
using std::tuple;
using std::vector; 

class Algorithms
{
public:
	static float EPS;

	/** Returns a tuple, regarding the intersection of two segments.
	* If they intersect, returns 'true' and the parameters of the intersection in the equation of the segments.
	* If they don't, returns 'false' and two zeros.
	*/
	static tuple<bool, float, float> segmentsIntersectParams(Segment*, Segment*);
	
	static Polygon cycleToPolygon(set<Point>* points, vector<int>);
	static bool polygonsIntersect(Polygon*, Polygon*);
	static void removeLargestPolygon(vector<Polygon>*, vector<vector<int>>*);

	static vector<Polygon> algorithm(vector<Segment>*);


};

