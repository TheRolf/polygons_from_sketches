#include "Algorithms.h"
#include "Instance.h"
#include <tuple>

void Instance::calculateIntersectionPoints() {
	for (vector<Segment>::iterator s1 = segments.begin(); s1 != segments.end(); ++s1) {
		for (vector<Segment>::iterator s2 = s1 + 1; s2 != segments.end(); ++s2) {
			Intersection intersection = s1->intersectParams(&*s2);
			if (intersection.existsQ) {
				float t = intersection.t;
				float u = intersection.u;


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
	// Get rid of degree-1 and degree-0 nodes, they don't contribute in any cycles in G, or faces in the end
	G.trimGraph();
}

void Instance::findMinimalFaces() {
	cycles = G.findCycles(&intersection_points);
}

void Instance::buildPolygons(bool removeLargestPolygonQ=true, bool sortPolygonsDecreasingQ=true) {
	for (vector<vector<int>>::iterator cycle = cycles.begin(); cycle != cycles.end(); cycle++) {
		Polygon polygon = Algorithms::cycleToPolygon(&intersection_points, *cycle);
		if (polygon.n_sides > 0) polygons.push_back(polygon);
	}

	// For each connected component, it removes the largest (outer) face
	if (removeLargestPolygonQ) {
		Graph H = Graph();
		for (int i = 0; i < polygons.size(); i++) {
			for (int j = 0; j < i; j++) {
				if (Algorithms::polygonsIntersect(&polygons[i], &polygons[j])) {
					H.addEdge(i, j);
					H.addEdge(j, i);
				}
			}
		}
		vector<vector<int>> componentsOfH = H.findComponents();
		Algorithms::removeLargestPolygon(&polygons, &componentsOfH);
	}

	// Sorts the faces from largest area to smallest; makes the contained faces appear when drawing in that order 
	if (sortPolygonsDecreasingQ) {
		sort(polygons.begin(), polygons.end(), Polygon::cmpPolygon);
	}
}

