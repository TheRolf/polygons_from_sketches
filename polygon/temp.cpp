/*
* 	float leftmost, rightmost, topmost, bottommost;

* 	float getLeftmost();
	float getRightmost();
	float getTopmost();
	float getBottommost();

		leftmost = -INFINITY;
	rightmost = INFINITY;
	topmost = INFINITY;
	bottommost = -INFINITY;

float Polygon::getLeftmost() {
	if (leftmost > -INFINITY) return leftmost;
	float value = INFINITY;
	for (vector<Point>::iterator iter_p = coordinates.begin(); iter_p != coordinates.end(); iter_p++) {
		if (iter_p->x < value) value = iter_p->x;
	}
	leftmost = value;
	return value;
}

float Polygon::getRightmost() {
	if (rightmost < INFINITY) return rightmost;
	float value = -INFINITY;
	for (vector<Point>::iterator iter_p = coordinates.begin(); iter_p != coordinates.end(); iter_p++) {
		if (iter_p->x > value) value = iter_p->x;
	}
	rightmost = value;
	return value;
}

float Polygon::getTopmost() {
	if (topmost < INFINITY) return topmost;
	float value = -INFINITY;
	for (vector<Point>::iterator iter_p = coordinates.begin(); iter_p != coordinates.end(); iter_p++) {
		if (iter_p->y > value) value = iter_p->y;
	}
	topmost = value;
	return value;
}

float Polygon::getBottommost() {
	if (bottommost > -INFINITY) return bottommost;
	float value = INFINITY;
	for (vector<Point>::iterator iter_p = coordinates.begin(); iter_p != coordinates.end(); iter_p++) {
		if (iter_p->y < value) value = iter_p->y;
	}
	bottommost = value;
	return value;
}

	if (polygon1->getLeftmost() > polygon2->getRightmost() ||
		polygon1->getRightmost() < polygon2->getLeftmost() ||
		polygon1->getTopmost() < polygon2->getBottommost() ||
		polygon1->getBottommost() > polygon2->getTopmost()) {
		return false;
	}
 
pair <Graph, set<pair<int, int>>> Graph::spanningForest() {
	if (n_vertices == 0) return make_pair(Graph(), set<pair<int, int>>());
	Graph F = Graph();
	set<pair<int, int>> back_edges = set<pair<int, int>>();
	stack<int> stack;
	map<int, bool> visited = map<int, bool>();
	for (set<int>::iterator v = vertices.begin(); v != vertices.end(); v++) visited.insert({ *v, false });
	int u = *(vertices.begin());
	stack.push(u);
	while (!stack.empty()) {
		//cout << u << " ";
		visited.find(u)->second = true;
		int next = -1;
		for (set<int>::iterator w = adjacency.find(u)->second.begin(); w != adjacency.find(u)->second.end(); w++) {
			if (visited.find(*w)->second) {
				if (back_edges.find({ u, *w }) == back_edges.end() && back_edges.find({ *w, u }) == back_edges.end() && !F.hasEdge(u, *w)) {
					back_edges.insert({ u, *w });
				}
			}
			else {
				next = *w;
				F.addEdge(u, *w);
				stack.push(u);
				break;
			}
		}
		if (next == -1) {
			u = stack.top();
			stack.pop();
		}
		else {
			u = next;
		}
	}
	//cout << endl;
	return make_pair(F, back_edges);
}

set<vector<int>> Graph::findCycles3(set<pair<int, int>> back_edges) {
	set<vector<int>> cycles = set<vector<int>>();
	for (set<pair<int, int>>::iterator back_edge = back_edges.begin(); back_edge != back_edges.end(); back_edge++) {
		stack<int> stack;
		map<int, bool> visited = map<int, bool>();
		for (set<int>::iterator v = vertices.begin(); v != vertices.end(); v++) visited.insert({ *v, false });
		int u = back_edge->first;
		int v = back_edge->second;
		cout << endl << u << " " << v << " |";
		stack.push(u);
		visited.find(u)->second = true;
		while (!stack.empty()) {
			visited.find(v)->second = true;
			int next = -1;
			for (set<int>::iterator w = adjacency.find(v)->second.begin(); w != adjacency.find(v)->second.end(); w++) {
				cout << " " << *w << "(" << v << "," << stack.top() << ")";
				if (*w == u) {
					stack.push(v);
					next = -2;
					break;
				}

				if (!visited.find(*w)->second) {
					stack.push(v);
					next = *w;
					break;
				}


			}
			if (next == -2) break;
			if (next == -1) {
				v = stack.top();
				stack.pop();
			}
			else {
				v = next;
			}
		}
		cout << endl;

		int cycle_size = (int)stack.size();
		vector<int> cycle = vector<int>(cycle_size);
		while (!stack.empty()) {
			cycle[--cycle_size] = stack.top();
			stack.pop();
		}


		cycles.insert(cycle);
		for (int i = 0; i < cycle.size(); i++) {
			cout << " " << cycle[i];
		}
		cout << endl;
	}

	return cycles;
}

set<vector<int>> Graph::findCycles2(set<pair<int, int>> back_edges) {
	set<vector<int>> cycles = set<vector<int>>();
	for (set<pair<int, int>>::iterator back_edge = back_edges.begin(); back_edge != back_edges.end(); back_edge++) {
		stack<int> stack;
		map<int, bool> visited = map<int, bool>();
		Point prevprev, previous;
		for (set<int>::iterator v = vertices.begin(); v != vertices.end(); v++) visited.insert({ *v, false });
		int u = back_edge->first;
		int v = back_edge->second;
		cout << endl << u << " " << v << " |";
		stack.push(u);
		//visited.find(u)->second = true;
		float angle;
		Segment first = Segment::getSegment(u);
		cout << " first(" << first.p.x << ") ";
		bool firstQ = true;
		previous = Point((first.p.x + first.q.x) / 2, (first.p.y + first.q.y) / 2);
		int prev_segment = u;
		while (!stack.empty()) {
			prevprev = previous;
			previous = Point::point_map_find(prev_segment, v);
			visited.find(v)->second = true;
			int next = -1;
			float min_dist = FLT_MAX;
			for (set<int>::iterator w = adjacency.find(v)->second.begin(); w != adjacency.find(v)->second.end(); w++) {
				if (firstQ) {
					if (!visited.find(*w)->second && *w != prev_segment) {
						Point current = Point::point_map_find(v, *w);
						cout << " " << *w << "(" << v << "," << stack.top() << "," << current.distance(previous) << ") ";
						if (current.distance(previous) < min_dist) {
							min_dist = current.distance(previous);
							next = *w;
							angle = current.angle(prevprev, previous);
							cout << "angle(" << angle << ") ";
						}
					}

				}

				else {
					if (!visited.find(*w)->second && *w != prev_segment) {
						Point current = Point::point_map_find(v, *w);
						cout << " " << *w << "(" << v << "," << stack.top() << "," << previous.distance(current) << "," << current.angle(prevprev, previous) << ")";
						if (angle * current.angle(prevprev, previous) > 0 && current.distance(previous) < min_dist) {
							min_dist = current.distance(previous);
							next = *w;
						}
					}
				}
			}
			// cout << " " << *w << "(" << v << "," << stack.top() << ")";
			if (next == u) {
				stack.push(v);
				next = -2;
				break;
			}

			if (next == -2) break;
			if (next == -1) {
				v = stack.top();
				stack.pop();
				if (!stack.empty()) prev_segment = stack.top();
			}
			else {
				stack.push(v);
				prev_segment = v;
				v = next;
				cout << endl;
				firstQ = false;
			}
		}
		cout << endl;

		int cycle_size = (int)stack.size();
		vector<int> cycle = vector<int>(cycle_size);
		while (!stack.empty()) {
			cycle[--cycle_size] = stack.top();
			stack.pop();
		}


		cycles.insert(cycle);
		for (int i = 0; i < cycle.size(); i++) {
			cout << " " << cycle[i];
		}
		cout << endl;
	}

	return cycles;
}

set<vector<int>> Graph::findCycles1(set<pair<int, int>> back_edges) {
	set<vector<int>> cycles = set<vector<int>>();
	for (set<pair<int, int>>::iterator back_edge = back_edges.begin(); back_edge != back_edges.end(); back_edge++) {
		stack<int> stack;
		map<int, bool> visited = map<int, bool>();
		for (set<int>::iterator v = vertices.begin(); v != vertices.end(); v++) visited.insert({ *v, false });
		int u = back_edge->first;
		int v = back_edge->second;
		cout << endl << u << " " << v << " |";
		stack.push(u);
		visited.find(u)->second = true;
		while (!stack.empty()) {
			visited.find(v)->second = true;
			int next = -1;
			for (set<int>::iterator w = adjacency.find(v)->second.begin(); w != adjacency.find(v)->second.end(); w++) {
				cout << " " << *w << "(" << v << "," << stack.top() << ")";
				if (*w == u) {
					stack.push(v);
					next = -2;
					break;
				}

				if (!visited.find(*w)->second) {
					stack.push(v);
					next = *w;
					break;
				}


			}
			if (next == -2) break;
			if (next == -1) {
				v = stack.top();
				stack.pop();
			}
			else {
				v = next;
			}
		}
		cout << endl;

		int cycle_size = (int)stack.size();
		vector<int> cycle = vector<int>(cycle_size);
		while (!stack.empty()) {
			cycle[--cycle_size] = stack.top();
			stack.pop();
		}


		cycles.insert(cycle);
		for (int i = 0; i < cycle.size(); i++) {
			cout << " " << cycle[i];
		}
		cout << endl;
	}

	return cycles;
}
*/