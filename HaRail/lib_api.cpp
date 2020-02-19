/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "lib_api.h"

namespace HaRail {

	GTFSDataSource *HaRailAPI::gds = nullptr;
	HaException HaRailAPI::last_err("", HaException::CRITICAL_ERROR);

	bool HaRailAPI::loadData(int date, int start_time, string data_root)
	{
		try {
			if (gds) {
				delete gds;
				gds = nullptr;
			}
			gds = new GTFSDataSource(data_root, Utils::padWithZeroes(Utils::int2str(date), 6));
			gds->initStations();
			gds->initTrains();
			return true;
		}
		catch (HaException e)  {
			if (gds) {
				delete gds;
				gds = nullptr;
			}
			last_err = e;
			return false;
		}
		catch (...) {
			last_err = HaException("Unknown Error", HaException::CRITICAL_ERROR);
			return false;
		}
	}

	string HaRailAPI::getLastError()
	{
		return last_err.what();
	}

    vector<int> HaRailAPI::getAllFollowingRoutes(int start_time, int start_station_id, int dest_station_id)
	{
		vector<int> result{ 1 };
		bool found_some_route = false;
		try {
			while (true) {
				vector<Train *> route{};
				Graph::getBestRouteSimple(gds, gds->getStationById(start_station_id), start_time, gds->getStationById(dest_station_id), route);
				encodeRouteSimple(route, result);
				start_time = Graph::getRouteStartTime(route) + 1;
				found_some_route = true;
			}
		}
		catch (HaException e) {
		    if (e.getType() == HaException::INVALID_ROUTE_ERROR && found_some_route) {
		        // This is fine, we just reached end of day
		        return result;
		    }
			last_err = e;
			return vector<int> { 0 };
		}
		catch (...) {
			last_err = HaException("Unknown Error", HaException::CRITICAL_ERROR);
			return vector<int> { 0 };
		}
	}

	vector<int> HaRailAPI::getRoutes(int start_time, int start_station_id, int dest_station_id)
	{
		try {
			vector<Train *> shortest_route;
			vector<Train *> best_route;
			Graph::getBestRoutes(gds, gds->getStationById(start_station_id), start_time, gds->getStationById(dest_station_id), shortest_route, best_route);
			if (shortest_route == best_route) {
				vector<int> result{ 1 };
				encodeRoute(best_route, result);
				return result;
			}
			else {
				vector<int> result{ 2 };
				encodeRoute(shortest_route, result);
				encodeRoute(best_route, result);
				return result;
			}
		}
		catch (HaException e) {
			last_err = e;
			return vector<int> { 0 };
		}
		catch (...) {
			last_err = HaException("Unknown Error", HaException::CRITICAL_ERROR);
			return vector<int> { 0 };
		}
	}

	void HaRailAPI::encodeRoute(const vector<Train *>& route, vector<int>& out_vec)
	{
		vector<int> temp_vec;
		Train *last_train = nullptr;
		int trains = 0;
		for (Train *train : route) {
			if (!last_train || train->getTrainId() != last_train->getTrainId()) {
				// Boarding new train
				if (last_train) {
					temp_vec.push_back(last_train->getDest()->getStationId());
					temp_vec.push_back(last_train->getDestTime());
				}
				++trains;
				temp_vec.push_back(train->getTrainId());
				temp_vec.push_back(train->getSource()->getStationId());
				temp_vec.push_back(train->getSourceTime());
			}
			last_train = train;
		}
		temp_vec.push_back(last_train->getDest()->getStationId());
		temp_vec.push_back(last_train->getDestTime());
		
		out_vec.reserve(out_vec.size() + 1 + temp_vec.size());
		out_vec.push_back(trains);
		for (int curr : temp_vec) {
			out_vec.push_back(curr);
		}
	}

    void HaRailAPI::encodeRouteSimple(const vector<Train *>& route, vector<int>& out_vec)
    {
        out_vec.push_back(Graph::getRouteStartTime(route));
        out_vec.push_back(Graph::getRouteEndTime(route));
        out_vec.push_back(Graph::countTrainSwitches(route));
    }

    string HaRailAPI::getRoutesStr(int start_time, int start_station_id, int dest_station_id)
	{
		try {
			vector<Train *> shortest_route;
			vector<Train *> best_route;
			Graph::getBestRoutes(gds, gds->getStationById(start_station_id), start_time, gds->getStationById(dest_station_id), shortest_route, best_route);
			stringstream ss;
			Graph::printBestRoutes(shortest_route, best_route, ss);
			return ss.str();
		}
		catch (HaException e) {
			return e.what();
		}
		catch (...) {
			return "Unknown Error";
		}
	}

	vector<int> HaRailAPI::getWholeTrainPath(int train_id)
	{
		try {
			int trains = 0;
			vector<int> result{ 1, trains };
			for (Train * train : gds->getTrains()) {
				if (train->getTrainId() == train_id) {
					++trains;
					result.push_back(train->getSource()->getStationId());
					result.push_back(train->getSourceTime());
					result.push_back(train->getDest()->getStationId());
					result.push_back(train->getDestTime());
				}
			}
			result[1] = trains;
			return result;
		}
		catch (HaException e) {
			last_err = e;
			return vector<int> { 0 };
		}
		catch (...) {
			last_err = HaException("Unknown Error", HaException::CRITICAL_ERROR);
			return vector<int> { 0 };
		}
	}
}
