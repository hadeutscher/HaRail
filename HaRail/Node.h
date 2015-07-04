/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __NODE_H__
#define __NODE_H__

#include "common.h"
#include <climits>

namespace HaRail {
	class Node {
	public:
		// Class Methods
		Node(Station *station, int time, int train_id)
			: station(station),
			station_time(time),
			train_id(train_id),
			edges(),
			visited(false),
			best_cost(UNEXPLORED_COST),
			best_source(nullptr),
			best_dest(nullptr) {}
		virtual ~Node() {}

		// Property Accessors
		Station *getStation() const { return station; }
		int getStationTime() const { return station_time; }
		int getTrainId() const { return train_id; }
		vector<Edge *>& getEdges() { return edges; }
		const vector<Edge *>& getEdges() const { return edges; }
		bool getVisited() const { return visited; }
		void setVisited(bool visited) { this->visited = visited; }
		int getBestCost() const { return best_cost; }
		void setBestCost(int best_cost) { this->best_cost = best_cost; }
		Edge *getBestSource() const { return best_source; }
		void setBestSource(Edge *best_source) { this->best_source = best_source; }
		Edge *getBestDest() const { return best_dest; }
		void setBestDest(Edge *best_dest) { this->best_dest = best_dest; }

		// Constants
		static const int UNEXPLORED_COST = INT_MAX;

	protected:
		// Fields
		Station *station;
		int station_time;
		int train_id;
		vector<Edge *> edges;
		bool visited;
		int best_cost;
		Edge *best_source;
		Edge *best_dest;

		UNCOPYABLE_CLASS(Node);
	};
}
#endif //__NODE_H__
