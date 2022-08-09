#pragma once
#include <vector>
#include "Point.h"

using std::vector;

class Polygon
{
private:
	static int n_polygons;
public:
	int n_sides;
	vector<Point> coordinates;
	int serial_no;
	
	Polygon();
	void addPoint(Point);

	float area();
	static bool cmpPolygon(Polygon p, Polygon q);
	friend bool operator< (const Polygon&, const Polygon&);
};
