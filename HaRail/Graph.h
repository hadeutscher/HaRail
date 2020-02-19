/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "common.h"
#include "IDataSource.h"
#include "Station.h"
#include "Train.h"
#include "Node.h"
#include "Edge.h"
#include "HashedPair.h"
#include "Utils.h"
#include <algorithm>
#include <queue>
#include <unordered_map>

namespace HaRail {
	class Graph {
	public:
		// Class Methods
		Graph(const IDataSource *ids, Station *source_station, int start_time);
		virtual ~Graph();

		// Methods
		void dijkstra(Station *dest_station);
		vector<Train *> backtraceRoute();
		void resetGraph();

		// Static Methods
		static void getBestRouteSimple(IDataSource *ds, Station *start_station, int start_time, Station *dest_station, vector<Train *>& best_route);
		static void getBestRoutes(IDataSource *ds, Station *start_station, int start_time, Station *dest_station, vector<Train *>& shortest_route, vector<Train *>& best_route);
		static void printRoute(vector<Train *>& route, ostream& out);
		static void printBestRoutes(vector<Train *>& shortest_route, vector<Train *>& best_route, ostream& out);

		static int getRouteStartTime(const vector<Train *>& route);
		static int getRouteEndTime(const vector<Train *>& route);
		static int countTrainSwitches(const vector<Train *>& route);

	protected:
		// Private Methods
		Node *getNodeOrAdd(Station *station, int time, int train_id);
		Edge *createEdge(Train *train, Node *source, Node *dest, int cost);
		int getCurrentTrain(Node *node) const;

		// Fields
		vector<Node *> nodes;
		unordered_map<Station *, unordered_map<int, Node *>> generalNodesByStation;
		unordered_map<Station *, unordered_map<pair<int, int>, Node *, HashedPair>> trainNodesByStation;
		vector<Edge *> edges;
		Node *start_node;
		Node *end_node;

		UNCOPYABLE_CLASS(Graph);
	};
}
#endif //__GRAPH_H__
