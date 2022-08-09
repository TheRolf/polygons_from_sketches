#pragma once
#include <map>
#include <set>
#include <vector>
#include "Polygon.h"

using std::map;
using std::pair;
using std::set;
using std::vector;

class Graph
{
public:
	int n_nodes, n_edges;
	set<int> nodes;
	set<pair<int, int>> edges;
	map<int, set<int>> adjacency;

	map<pair<int, int>, bool> edge_available;
	map<int, bool> node_available;
	
	Graph();
	void addEdge(int, int);
	bool hasEdge(int, int);
	void removeNode(int);

	bool hasLeaves();

	/** Node/edge availability depends on whether it has been visited by findCycles(); more comments there
	*/
	bool nodeAvailable(int);
	bool edgeAvailable(int, int);
	bool edgesAvailable();
	void disableEdge(int, int);
	
	void printEdges();

	void trimGraph();
	
	/** Returns the connected components.
	* Approach: simple DFS.
	*/
	vector<vector<int>> findComponents();

	/** Returns the minimal cycles of the graph ~ minimal faces defined by the segments.
	* @param points contains the coordinates of the nodes
	*/
	vector<vector<int>> findCycles(set<Point>* points);

	/** Gets the point from 'points' with serial_no = x. */
	static Point getPoint(set<Point>* points, int x);
};

