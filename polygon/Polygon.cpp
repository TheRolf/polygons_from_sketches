#include <map>
#include "Polygon.h"
#include "Point.h"
using std::map;
using std::pair;
using std::vector;

int Polygon::n_polygons = 0;

Polygon::Polygon(): n_sides(0) {
	coordinates = vector<Point>();
	serial_no = n_polygons;
	n_polygons++;
}

void Polygon::addPoint(Point p) {
	coordinates.push_back(p);
	n_sides++;
}

float Polygon::area() {
	float area = 0;
	int i = 1;
	for (vector<Point>::iterator iter_p = coordinates.begin(); iter_p != coordinates.end(); iter_p++) {
		Point p1, p2;
		if (i == n_sides) {
			p1 = *iter_p;
			p2 = *(coordinates.begin());
		}
		else {
			p1 = *iter_p;
			p2 = *(iter_p + 1);
		}
		area += (p1.x*p2.y - p1.y*p2.x);
		i++;
	}
	return area;
}

bool Polygon::cmpPolygon(Polygon p, Polygon q) {
	return p.area() < q.area();
}

bool operator< (const Polygon& p, const Polygon& q) {
	return p.serial_no < q.serial_no;
}
