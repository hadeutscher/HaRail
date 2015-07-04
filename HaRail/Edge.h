/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __EDGE_H__
#define __EDGE_H__

#include "common.h"

namespace HaRail {
	class Edge {
	public:
		// Class Methods
		Edge(Train *train, Node *source, Node *dest, int cost)
			: train(train),
			source(source),
			dest(dest),
			cost(cost) {}
		virtual ~Edge() {};

		// Property Accessors
		Train *getTrain() const { return train; }
		Node *getSource() const { return source; }
		Node *getDest() const { return dest; }
		int getCost() const { return cost; }

	protected:
		// Fields
		Train *train;
		Node *source;
		Node *dest;
		int cost;

		UNCOPYABLE_CLASS(Edge);
	};
}
#endif //__EDGE_H__
