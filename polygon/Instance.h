#pragma once
#include <set>
#include <vector>
#include "Polygon.h"
#include "Segment.h"

class Instance {
private:
	vector<Segment> segments;
	set<Point> intersection_points;
	Graph G;
	vector<vector<int>> cycles;


public:
	void calculateIntersectionPoints();
	void buildGraphOfSegments();
	void findMinimalFaces();
	void buildPolygons(bool, bool);
	vector<Polygon> polygons;
};