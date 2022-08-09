#include <cmath>
#include <map>
#include "Point.h"
#include "Segment.h"

using std::map;

int Point::n_point = 0;

Point::Point() {
	x = 0;
	y = 0;
	map<int, int> on_segment = map<int, int>();
	serial_no = n_point;
	n_point++;
}

Point::Point(float x0, float y0) {
	x = x0;
	y = y0;
	map<int, int> on_segment = map<int, int>();
	serial_no = n_point;
	n_point++;
}

float Point::distance(Point p) {
	return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
}

float Point::angle(Point pp, Point p) {
	float dot = (p.x - pp.x) * (x - p.x) + (p.y - pp.y) * (y - p.y);
	float det = (p.x - pp.x) * (y - p.y) - (p.y - pp.y) * (x - p.x);
	return atan2(det, dot);
}

bool operator< (const Point& p, const Point& q) {
	return p.serial_no < q.serial_no;
}
