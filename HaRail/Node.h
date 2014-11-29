#ifndef __NODE_H__
#define __NODE_H__

#include "common.h"

class Node {
public:
	Node(Station *station, int time)
		: station(station),
		station_time(time),
		edges(),
		visited(false),
		best_cost(UNEXPLORED_COST),
		best_source(nullptr),
		best_dest(nullptr) {}
	virtual ~Node() {}

	Station *getStation() const { return station; }
	int getStationTime() const { return station_time; }
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

	static const int UNEXPLORED_COST = INT_MAX;

protected:
	Station *station;
	int station_time;
	vector<Edge *> edges;
	bool visited;
	int best_cost;
	Edge *best_source;
	Edge *best_dest;

	UNCOPYABLE_CLASS(Node);
};

#endif //__NODE_H__