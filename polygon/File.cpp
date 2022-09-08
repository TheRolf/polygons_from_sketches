#include <fstream>
#include <iostream>
#include "Polygon.h"
#include "Segment.h"
#include "File.h"

vector<Segment> File::readLines(string filename) {
	int n_lines;
	float x1, y1, x2, y2;
	std::ifstream infile;
	infile.open(filename);

	infile >> n_lines;
	vector<Segment> lines(n_lines);
	for (int i = 0; i < n_lines; i++) {
		infile >> x1;
		infile >> y1;
		infile >> x2;
		infile >> y2;
		lines[i].setCoordinates(Point(x1, y1), Point(x2, y2));
	}
	infile.close();
	
	return lines;
}

void File::writePolygons(string filename, vector<Polygon> polygons) {
	std::fstream outfile;
	outfile.open(filename);
	outfile << polygons.size() << std::endl;
	for (vector<Polygon>::iterator polygon = polygons.begin(); polygon != polygons.end(); polygon++) {
		outfile << polygon->n_sides << std::endl;
		for (int j = 0; j < polygon->n_sides; j++) {
			Point point = polygon->coordinates[j];
			outfile << point.x << " " << point.y << std::endl;
		}
	}
	outfile.close();
	return;
}