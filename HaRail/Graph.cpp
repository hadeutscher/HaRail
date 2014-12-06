/* HaRail - Public transport fastest-route finder for Israel Railways
* Copyright(C) 2014  haha01haha01

* This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

* This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include "Graph.h"

namespace HaRail {
	Graph::Graph(const IDataSource *ids, Station *source_station, int start_time)
		: nodes(),
		generalNodesByStation(),
		trainNodesByStation(),
		edges(),
		start_node(nullptr),
		end_node(nullptr)
	{
		Train *last_train = nullptr;
		for (Train *train : ids->getTrains()) {
			if (last_train && last_train->getTrainId() == train->getTrainId())
			{
				if (last_train->getDest() != train->getSource()) {
					throw HaException("Bug detected, report this", HaException::CRITICAL_ERROR);
				}
				if (last_train->getDestTime() != train->getSourceTime()) {
					Node *wait_source_train = getNodeOrAdd(last_train->getDest(), last_train->getDestTime(), last_train->getTrainId());
					Node *wait_dest_train = getNodeOrAdd(train->getSource(), train->getSourceTime(), train->getTrainId());
					createEdge(nullptr, wait_source_train, wait_dest_train, train->getSourceTime() - last_train->getDestTime());
				}
			}
			Node *source_train = getNodeOrAdd(train->getSource(), train->getSourceTime(), train->getTrainId());
			Node *source_general = getNodeOrAdd(train->getSource(), train->getSourceTime(), -1);
			Node *dest_train = getNodeOrAdd(train->getDest(), train->getDestTime(), train->getTrainId());
			Node *dest_general = getNodeOrAdd(train->getDest(), train->getDestTime(), -1);
			
			// Train movement
			createEdge(train, source_train, dest_train, train->getCost() + MOVEMENT_COST);
			// Train boarding option
			createEdge(nullptr, source_general, source_train, SWITCH_COST);
			// Train unboarding option
			createEdge(nullptr, dest_train, dest_general, SWITCH_COST);

			last_train = train;
		}

		start_node = getNodeOrAdd(source_station, start_time, -1);

		for (pair<Station *, unordered_map<int, Node *>> p : generalNodesByStation) {
			vector<Node *> node_arr;
			node_arr.reserve(p.second.size());
			for (pair<int, Node *> p2 : p.second) {
				node_arr.push_back(p2.second);
			}
			sort(node_arr.begin(), node_arr.end(), [](Node *first, Node *second) -> bool { return first->getStationTime() < second->getStationTime(); });
			for (unsigned int i = 0; i < node_arr.size() - 1; i++) {
				Node *source_general = node_arr[i];
				Node *dest_general = node_arr[i + 1];
				createEdge(nullptr, source_general, dest_general, dest_general->getStationTime() - source_general->getStationTime());
			}
		}
	}

	Graph::~Graph()
	{
		for (Node *node : nodes) {
			delete node;
		}
		for (Edge *edge : edges) {
			delete edge;
		}
	}

	void Graph::dijkstra(Station *dest_station)
	{
		auto pr = [](const pair<Node *, int>& first, const pair<Node *, int>& second) { return first.second > second.second; };
		priority_queue<pair<Node *, int>, vector<pair<Node *, int>>, decltype(pr)> pq(pr);

		start_node->setBestCost(0);
		start_node->setVisited(true);
		Node *curr = start_node;
		while (curr->getStation() != dest_station) {
			for (Edge *edge : curr->getEdges()) {
				if (!edge->getDest()->getVisited()) {
					int cost = curr->getBestCost() + edge->getCost();
					if (edge->getDest()->getBestCost() > cost) {
						edge->getDest()->setBestCost(cost);
						edge->getDest()->setBestSource(edge);
						pq.push(pair<Node *, int>(edge->getDest(), cost));
					}
				}
			}
			curr->setVisited(true);

			int pair_cost;
			Node *pair_node;
			do {
				if (pq.size() == 0) {
					throw HaException("impossible route", HaException::INVALID_ROUTE_ERROR);
				}
				pair<Node *, int> p = pq.top();
				pq.pop();
				pair_node = p.first;
				pair_cost = p.second;
			} while (pair_node->getBestCost() != pair_cost);
			curr = pair_node;
		}
		end_node = curr;
	}

	vector<Train *> Graph::backtraceRoute()
	{
		Node *curr = end_node;
		while (curr != start_node) {
			Edge *best_edge = curr->getBestSource();
			best_edge->getSource()->setBestDest(best_edge);
			curr = best_edge->getSource();
		}

		vector<Train *> result;
		while (curr != end_node) {
			Edge *best_edge = curr->getBestDest();
			if (best_edge->getTrain()) {
				result.push_back(best_edge->getTrain());
			}
			curr = best_edge->getDest();
		}
		return result;
	}

	void Graph::resetGraph()
	{
		for (Node *node : nodes) {
			node->setBestSource(nullptr);
			node->setBestDest(nullptr);
			node->setBestCost(Node::UNEXPLORED_COST);
			node->setVisited(false);
		}
	}

	int Graph::getCurrentTrain(Node *node) const
	{
		Node *curr = node;
		Edge *edge = curr->getBestSource();
		while (edge && !edge->getTrain()) {
			curr = edge->getSource();
			edge = curr->getBestSource();
		}
		return edge ? edge->getTrain()->getTrainId() : -1;
	}

	Node *Graph::getNodeOrAdd(Station *station, int time, int train_id)
	{
		Node *& node_ref = train_id == -1 ? generalNodesByStation[station][time] : trainNodesByStation[station][pair<int, int>(time, train_id)];
		if (node_ref == nullptr) {
			Node *node = new Node(station, time, train_id);
			node_ref = node;
			nodes.push_back(node);
		}
		return node_ref;
	}

	Edge *Graph::createEdge(Train *train, Node *source, Node *dest, int cost)
	{
		Edge *edge = new Edge(train, source, dest, cost);
		edges.push_back(edge);
		source->getEdges().push_back(edge);
		return edge;
	}
}
