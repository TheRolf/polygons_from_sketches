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

Segment::Segment(Point p0, Point q0): p(p0), q(q0) {
	serial_no = n_segment;
	map<float, Point> contains_point = map<float, Point>();
	n_segment++;
}

Segment::Segment(float x1, float y1, float x2, float y2) {
	Segment(Point(x1, y1), Point(x2, y2));
}

void Segment::setCoordinates(Point p0, Point q0) {
	p = p0;
	q = q0;
}

Point Segment::pointOnSegment(float t) {
	return Point(p.x + t * (q.x - p.x), p.y + t * (q.y - p.y));
}

tuple<bool, float, float> Segment::intersectParams(Segment* s2) {
	float EPS = (float)0.0001;
	Segment* s1 = this;
	tuple<bool, float, float> null_tuple = { false, 0, 0 };
	
	float denominator = (s1->p.x - s1->q.x) * (s2->p.y - s2->q.y) - (s1->p.y - s1->q.y) * (s2->p.x - s2->q.x);
	// If denominator too close to zero.
	if (denominator < EPS && denominator > -EPS) return null_tuple;

	float u_numerator = (s1->p.x - s2->p.x) * (s1->p.y - s1->q.y) - (s1->p.y - s2->p.y) * (s1->p.x - s1->q.x);
	// If u > 1
	if (abs(u_numerator) > abs(denominator)) return null_tuple;
	// If u < 0 (u_num and den different sign)
	if ((u_numerator > 0 && denominator < 0) || (u_numerator < 0 && denominator > 0)) return null_tuple;

	float t_numerator = (s1->p.x - s2->p.x) * (s2->p.y - s2->q.y) - (s1->p.y - s2->p.y) * (s2->p.x - s2->q.x);
	// If t > 1
	if (abs(t_numerator) > abs(denominator)) return null_tuple;
	// If t < 0 (t_num and den different sign)
	if ((t_numerator > 0 && denominator < 0) || (t_numerator < 0 && denominator > 0)) return null_tuple;

	return { true, t_numerator/denominator, u_numerator/denominator };
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