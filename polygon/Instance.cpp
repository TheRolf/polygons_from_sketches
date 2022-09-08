#include "Algorithms.h"
#include "Instance.h"
#include <tuple>

void Instance::calculateIntersectionPoints() {
	for (vector<Segment>::iterator s1 = segments.begin(); s1 != segments.end(); ++s1) {
		for (vector<Segment>::iterator s2 = s1 + 1; s2 != segments.end(); ++s2) {
			tuple<bool, float, float> t_and_u = Algorithms::segmentsIntersectParams(&*s1, &*s2);
			if (std::get<0>(t_and_u)) {
				float t = std::get<1>(t_and_u);
				float u = std::get<2>(t_and_u);

				Point p = s1->pointOnSegment(t);
				s1->contains_point.insert({ t, p });
				s2->contains_point.insert({ u, p });
				intersection_points.insert(p);
			}
		}
	}
}

void Instance::buildGraphOfSegments() {
	for (vector<Segment>::iterator s1 = segments.begin(); s1 != segments.end(); ++s1) {
		vector<pair<float, Point>> points;
		points = s1->sortedListOfPoints();
		if (points.size() > 0) {
			int i;
			for (i = 0; i < points.size() - 1; i++) {
				G.addEdge(points[i].second.serial_no, points[i+1].second.serial_no);
				G.addEdge(points[i+1].second.serial_no, points[i].second.serial_no);
			}
		}
	}
	G.trimGraph();
}
