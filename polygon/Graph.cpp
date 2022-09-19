#include <iostream>
#include <stack>
#include <set>
#include "Graph.h"
#include "Segment.h"

using std::map;
using std::pair;
using std::set;
using std::stack;
using std::vector;

using std::cout;
using std::endl;

Graph::Graph(): n_nodes(0), n_edges(0) {
	nodes = set<int>();
	edges = set<pair<int, int>>();
	adjacency = map<int, set<int>>();

	edge_available = map<pair<int, int>, bool>();
	node_available = map<int, bool>();
}

void Graph::addEdge(int u, int v) {
	if (nodes.find(u) == nodes.end()) {
		nodes.insert(u);
		adjacency.insert({ u, set<int>() });
		adjacency.find(u)->second = set<int>();
		n_nodes++;
	}
	if (nodes.find(v) == nodes.end()) {
		nodes.insert(v);
		adjacency.insert({ v, set<int>() });
		adjacency.find(v)->second = set<int>();
		n_nodes++;
	}
	if (!hasEdge(u, v)) {
		edges.insert({ u, v });
		edge_available.insert({ {u, v}, true });
		adjacency.find(u)->second.insert(v);
		n_edges++;
	}
}

bool Graph::hasEdge(int u, int v) {
	for (set<pair<int, int>>::iterator edge = edges.begin(); edge != edges.end(); edge++) {
		if (edge->first == u && edge->second == v) return true;
	}
	return false;
}

void Graph::printEdges() {
	for (set<pair<int, int>>::iterator edge = edges.begin(); edge != edges.end(); edge++) {
		cout << edge->first << " " << edge->second;
		if (edgeAvailable(edge->first, edge->second)) cout << "*";
		cout << endl;
	}
}

bool Graph::edgeAvailable(int u, int v) {
	return edge_available.find({ u, v })->second;
}

bool Graph::edgesAvailable() {
	for (map<pair<int, int>, bool>::iterator edge = edge_available.begin(); edge != edge_available.end(); edge++) {
		if (edge->second) return true;
	}
	return false;
}

bool Graph::nodeAvailable(int u) {
	if (node_available.find(u) != node_available.end()) return node_available.find(u)->second;
	for (set<int>::iterator iter_v = adjacency.find(u)->second.begin(); iter_v != adjacency.find(u)->second.end(); iter_v++) {
		if (edgeAvailable(u, *iter_v)) return true;
	}
	node_available.insert({ u, false });
	return false;
}

void Graph::disableEdge(int u, int v) {
	edge_available.find({ u, v })->second = false;
}

bool Graph::hasLeaves() {
	int value = 0;
	for (map<int, set<int>>::iterator iter = adjacency.begin(); iter != adjacency.end(); iter++) {
		if (iter->second.size() == 1) return true;
	}
	return false;
}

void Graph::removeNode(int u) {
	for (set<int>::iterator iter_v = nodes.begin(); iter_v != nodes.end(); iter_v++) {
		if (hasEdge(u, *iter_v)){
			edges.erase({ u, *iter_v });
			n_edges--;
		}
		if (hasEdge(*iter_v, u)) {
			edges.erase({ *iter_v, u });
			n_edges--;
			adjacency.find(*iter_v)->second.erase(u);
		}
	}
	nodes.erase(u);
	adjacency.erase(u);
	n_nodes--;
}

void Graph::trimGraph() {
	while (hasLeaves()) {
		stack<int> leaves = stack<int>();
		for (set<int>::iterator iter_u = nodes.begin(); iter_u != nodes.end(); iter_u++) {
			if (adjacency.find(*iter_u)->second.size() == 1) leaves.push(*iter_u);
		}
		while (!leaves.empty()) {
			int u = leaves.top();
			removeNode(u);
			leaves.pop();
		}
	}
	stack<int> isolates = stack<int>();
	for (set<int>::iterator iter_u = nodes.begin(); iter_u != nodes.end(); iter_u++) {
		if (adjacency.find(*iter_u)->second.size() == 0) isolates.push(*iter_u);
	}
	while (!isolates.empty()) {
		int u = isolates.top();
		removeNode(u);
		isolates.pop();
	}
}

vector<vector<int>> Graph::findComponents() {
	vector<vector<int>> components = vector<vector<int>>();
	map<int, bool> visited = map<int, bool>();
	stack<int> to_visit;
	for (set<int>::iterator iter_v = nodes.begin(); iter_v != nodes.end(); iter_v++) {
		visited.insert({ *iter_v, false });
		to_visit.push(*iter_v);
	}
	while (!to_visit.empty()) {
		int u = to_visit.top();
		to_visit.pop();
		vector<int> new_component = vector<int>();
		stack<int> stack;
		stack.push(u);
		while (!stack.empty()) {
			u = stack.top();
			stack.pop();
			if (!visited.find(u)->second) {
				new_component.push_back(u);
				visited.find(u)->second = true;
			}
			for (set<int>::iterator iter_v = adjacency.find(u)->second.begin(); iter_v != adjacency.find(u)->second.end(); iter_v++) {
				int v = *iter_v;
				if (!visited.find(v)->second) stack.push(v);
			}

		}
		if (new_component.size() > 0) components.push_back(new_component);
	}
	return components;
}

Point Graph::getPoint(set<Point>* points, int x) {
	for (set<Point>::iterator iter_p = points->begin(); iter_p != points->end(); iter_p++) {
		if (iter_p->serial_no == x) return *iter_p;
	}
	return *points->end();
}

vector<vector<int>> Graph::findCycles(set<Point>* points) {
	vector<vector<int>> cycles = vector<vector<int>>();
	// Until there is an edge not yet traversed AND node with an outgoing edge not yet traversed
	while (edgesAvailable()) {
		stack<int> stack;

		// First two nodes of the cycle: u, v
		int u = -1;
		for (set<int>::iterator iter_u = nodes.begin(); iter_u != nodes.end(); iter_u++) {
			if (nodeAvailable(*iter_u)) {
				u = *iter_u;
				break;
			}
		}
		if (u == -1) break;
		stack.push(u);
		int prev = u;

		int v = -1;
		for (set<int>::iterator iter_v = adjacency.find(u)->second.begin(); iter_v != adjacency.find(u)->second.end(); iter_v++) {
			if (edgeAvailable(u, *iter_v) && nodeAvailable(*iter_v)) {
				v = *iter_v;
				disableEdge(u, v);
				break;
			}
		}
		if (v == -1) {
			node_available.insert({ u, false });
			continue;
		}
		stack.push(v);

		int next = -1;

		// Until the starting node of the cycle is reached
		while (next != u) {
			next = -1;
			float next_angle = INFINITY;
			for (set<int>::iterator iter_next = adjacency.find(v)->second.begin(); iter_next != adjacency.find(v)->second.end(); iter_next++) {
				Point a = getPoint(points, prev);
				Point b = getPoint(points, v);
				
				// Chooses the next intersection point as 'right' as possible ~ smallest angle
				// Results in the vertices of the faces being traversed in a clockwise order
				if (*iter_next != prev && edgeAvailable(v, *iter_next) && (nodeAvailable(*iter_next) || *iter_next == u)) {
					Point c = getPoint(points, *iter_next);
					float angle = c.angle(a, b);
					if (angle < next_angle) {
						next = *iter_next;
						next_angle = angle;
					}
				}
			}

			if (next == -1 && stack.size() < 2) break;

			if (next == -1 && stack.size() >= 2) {
				stack.pop();
				v = stack.top();
			}
			else // next != -1, there is a feasible next node
			{
				// Traverse the edge
				disableEdge(v, next);
				stack.push(next);
				prev = v;
				v = next;
			}
		}
		
		if (!stack.empty()) {
			int cycle_size = (int)stack.size();
			vector<int> cycle = vector<int>(cycle_size);
			while (!stack.empty()) {
				cycle[--cycle_size] = stack.top();
				stack.pop();
			}

			if (cycle[0] == cycle[cycle.size() - 1] && cycle.size() > 3) {
				cycles.push_back(cycle);
			}
		}

	}
	return cycles;
}
