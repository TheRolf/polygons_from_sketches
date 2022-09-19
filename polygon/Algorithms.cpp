#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#include "Algorithms.h"
#include "Graph.h"
#include "Polygon.h"
#include "Instance.h"

using std::map;
using std::pair;
using std::set;
using std::tuple;
using std::vector;

float Algorithms::EPS = (float) 0.0001;

Polygon Algorithms::cycleToPolygon(set<Point>* points, vector<int> cycle) {
	Polygon polygon = Polygon();
	for (int i = 0; i < cycle.size()-1; i++) {
		Point p = Graph::getPoint(points, cycle[i]);
		polygon.addPoint(p);
	}
	return polygon;
}

bool Algorithms::polygonsIntersect(Polygon* polygon1, Polygon* polygon2) {
	for (vector<Point>::iterator iter_p1 = polygon1->coordinates.begin(); iter_p1 != polygon1->coordinates.end(); iter_p1++) {
		for (vector<Point>::iterator iter_p2 = polygon2->coordinates.begin(); iter_p2 != polygon2->coordinates.end(); iter_p2++) {
			if (iter_p1->serial_no == iter_p2->serial_no) return true;
		}
	}
	return false;
}

void Algorithms::removeLargestPolygon(vector<Polygon>* polygons, vector<vector<int>>* components) {
	for (vector<vector<int>>::iterator iter_comp = components->begin(); iter_comp != components->end(); iter_comp++) {
		float largest_area = 0.0;
		int largest_area_idx = -1;
		for (vector<int>::iterator iter_idx = iter_comp->begin(); iter_idx != iter_comp->end(); iter_idx++) {
			float area = (*polygons)[*iter_idx].area();
			if (area > largest_area) {
				largest_area_idx = *iter_idx;
				largest_area = area;
			}
		}
		polygons->erase(polygons->begin() + largest_area_idx);
	}
}

vector<Polygon> Algorithms::algorithm(vector<Segment>* segments, Instance* instance) {
	bool removeLargestPolygonQ = false;
	bool sortPolygonsDecreasingQ = false;
	
	set<Point> intersection_points;
	vector<vector<int>> cycles;
	Graph G = Graph();

	// Intersection points of segments
	instance->calculateIntersectionPoints();

	for (vector<Segment>::iterator s1 = segments->begin(); s1 != segments->end(); ++s1) {
		for (vector<Segment>::iterator s2 = s1+1; s2 != segments->end(); ++s2) {
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

	// Sort the intersection points on each segment, according to their positions
	// Building the graph G of intersection points;
	// edge between two points if they are next to each other on a segment in *both* directions
	instance->buildGraphOfSegments();

	for (vector<Segment>::iterator s1 = segments->begin(); s1 != segments->end(); ++s1) {
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

	// Finding the minimal faces
	instance->findMinimalFaces();
	cycles = G.findCycles(&intersection_points);

	// Building the polygons of the faces from position data
	instance->buildPolygons(true, true);
	vector<Polygon> polygons = vector<Polygon>();
	for (vector<vector<int>>::iterator cycle = cycles.begin(); cycle != cycles.end(); cycle++) {
		Polygon polygon = cycleToPolygon(&intersection_points, *cycle);
		if(polygon.n_sides > 0) polygons.push_back(polygon);
	}
	
	// For each connected component, it removes the largest (outer) face
	if (removeLargestPolygonQ) {
		Graph H = Graph();
		for (int i = 0; i < polygons.size(); i++) {
			for (int j = 0; j < i; j++) {
				if (polygonsIntersect(&polygons[i], &polygons[j])) {
					H.addEdge(i, j);
					H.addEdge(j, i);
				}
			}
		}
		vector<vector<int>> componentsOfH = H.findComponents();
		removeLargestPolygon(&polygons, &componentsOfH);
	}
	
	// Sorts the faces from largest area to smallest; makes the contained faces appear when drawing in that order 
	if (sortPolygonsDecreasingQ) {
		sort(polygons.begin(), polygons.end(), Polygon::cmpPolygon);
	}

	return polygons; // instance->polygons;
}