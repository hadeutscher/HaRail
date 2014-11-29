#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "common.h"
#include "IDataSource.h"
#include "Station.h"
#include "Train.h"
#include "Node.h"
#include "Edge.h"
#include <algorithm>
#include <queue>
#include <unordered_map>

class Graph {
public:
	Graph(const IDataSource *ids);
	virtual ~Graph();
	
	void dijkstra(Station *source_station, int start_time, Station *dest_station);
	vector<Train *> backtraceRoute();
	void resetGraph();

protected:
	Node *getNodeOrAdd(Station *station, int time);
	int getCurrentTrain(Node *node) const;

	vector<Node *> nodes;
	unordered_map<Station *, unordered_map<int, Node *>> nodesByStation;
	vector<Edge *> edges;
	Node *start_node;
	Node *end_node;

	UNCOPYABLE_CLASS(Graph);
};

#endif //__GRAPH_H__
