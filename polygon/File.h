#pragma once
#include <string>
#include <vector>
#include "Segment.h"
#include "Polygon.h"

using std::string;
using std::vector;

class File {
public:
	static vector<Segment> readLines(string filename);
	static void writePolygons(string filename, vector<Polygon> polygons);
};
