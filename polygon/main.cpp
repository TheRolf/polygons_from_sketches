#include <chrono>
#include <iostream>
#include "File.h"
#include "Segment.h"
#include "Polygon.h"
#include "Algorithms.h"
#include "Graph.h"

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
	string inputfile = ".\\..\\res\\points.txt";
	string outputfile = ".\\..\\res\\polygons.txt";
	
	if (argc > 1) {
		inputfile = argv[1];
		outputfile = argv[2];
	}

	vector<Segment> segments;
	vector<Polygon> polygons;

	segments = File::readLines(inputfile);
	cout << "n_segments: " << segments.size() << endl;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	polygons = Algorithms::algorithm(&segments);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	cout << "n_polygons: " << polygons.size() << endl;
	cout << "TOTAL TIME: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000.0 << " s" << std::endl;
	
	File::writePolygons(outputfile, polygons);
	
	return 0;
}