#include "Segment.h"
#include <algorithm>

using std::map;
using std::pair;
using std::vector;

int Segment::n_segment = 0;

Segment::Segment() {
	serial_no = n_segment;
	map<float, Point> contains_point = map<float, Point>();
	n_segment++;
}

Segment::Segment(Point p0, Point q0) {
	p = p0;
	q = q0;
	serial_no = n_segment;
	map<float, Point> contains_point = map<float, Point>();
	n_segment++;
}

Segment::Segment(float x1, float y1, float x2, float y2) {
	Segment(Point(x1, y1), Point(x2, y2));
	serial_no = n_segment;
	map<float, Point> contains_point = map<float, Point>();
	n_segment++;
}

void Segment::setCoordinates(Point p0, Point q0) {
	p = p0;
	q = q0;
}

Point Segment::pointOnSegment(float t) {
	return Point(p.x + t * (q.x - p.x), p.y + t * (q.y - p.y));
}

vector<pair<float, Point>> Segment::sortedListOfPoints() {
	vector<pair<float, Point>> A;
	for (auto& it : contains_point) {
		A.push_back(it);
	}
	sort(A.begin(), A.end(), Segment::cmp);
	return A;
}

bool operator< (const Segment& s1, const Segment& s2) {
	return s1.serial_no < s2.serial_no;
}

bool Segment::cmp(pair<float, Point>& a, pair<float, Point>& b) {
	return a.first < b.first;
}