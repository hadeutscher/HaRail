#ifndef __EDGE_H__
#define __EDGE_H__

#include "common.h"

class Edge {
public:
	Edge(Train *train, Node *source, Node *dest, int cost)
		: train(train),
		source(source),
		dest(dest),
		cost(cost) {}
	virtual ~Edge() {};

	Train *getTrain() const { return train; }
	Node *getSource() const { return source; }
	Node *getDest() const { return dest; }
	int getCost() const { return cost; }

protected:
	Train *train;
	Node *source;
	Node *dest;
	int cost;

	UNCOPYABLE_CLASS(Edge);
};

#endif //__EDGE_H__