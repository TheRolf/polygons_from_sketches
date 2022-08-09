#pragma once
#include <map>
#include <vector>
#include "Point.h"

using std::map;
using std::pair;
using std::vector;

class Segment
{
private:
	static int n_segment;
public:
	int serial_no;
	Point p, q;
	map<float, Point> contains_point;

	Segment();
	Segment(Point, Point);
	Segment(float, float, float, float);
	void setCoordinates(Point, Point);
	Point pointOnSegment(float);

	vector<pair<float, Point>> sortedListOfPoints();
	friend bool operator< (const Segment&, const Segment&);
	static bool cmp(pair<float, Point>& a, pair<float, Point>& b);

};