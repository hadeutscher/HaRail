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
