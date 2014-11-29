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

#include "common.h"
#include "Utils.h"
#include "HaException.h"
#include "ArgumentParser.h"
#include "IDataSource.h"
#include "GTFSDataSource.h"
#include "TestDataSource.h"
#include "Station.h"
#include "Train.h"
#include "Node.h"
#include "Edge.h"
#include "Graph.h"

namespace HaRail {
	IDataSource *initializeDataSource(ArgumentParser& argp)
	{
		if (argp.isArgumentExists("test_source")) {
			try {
				return new TestDataSource(Utils::str2int(argp.getArgument("test_source")));
			}
			catch (boost::bad_lexical_cast) {
				cout << "Error: Invalid argument test_source" << endl;
				throw HaException("Could not initialize data source");
			}
		}
		else if (argp.isArgumentExists("date")) {
			return new GTFSDataSource(DATA_ROOT, argp.getArgument("date"));
		}
		else {
			return new GTFSDataSource(DATA_ROOT, Utils::getCurrentDate());
		}
	}

	void printRoute(vector<Train *>& route)
	{
		Train *last_train = nullptr;
		for (Train *train : route) {
			if (!last_train || train->getTrainId() != last_train->getTrainId()) {
				// Boarding new train
				if (last_train) {
					cout << " to " << last_train->getDest()->getStationName() << " (" << Utils::makeTime(last_train->getDestTime(), true) << ")" << endl;
				}
				cout << "Train #" << train->getTrainId() << " from " << train->getSource()->getStationName() << " (" << Utils::makeTime(train->getSourceTime(), true) << ")";
			}
			last_train = train;
		}
		cout << " to " << last_train->getDest()->getStationName() << " (" << Utils::makeTime(last_train->getDestTime(), true) << ")" << endl;
	}

	int getRouteEndTime(const vector<Train *>& route)
	{
		return route[route.size() - 1]->getDestTime();
	}

	int countTrainSwitches(const vector<Train *>& route)
	{
		int result = 0;
		int last_tid = -1;
		for (Train *train : route)
		{
			if (train->getTrainId() != last_tid) {
				if (last_tid != -1) {
					result++;
				}
				last_tid = train->getTrainId();
			}
		}
		return result;
	}

	int main(int argc, const char *argv[])
	{
		IDataSource *ds = nullptr;
		try {
			ArgumentParser argp(argc, argv);
			argp.parseArguments();
			ds = initializeDataSource(argp);
			ds->initStations();
			if (argp.isArgumentExists("list_stations")) {
				ds->listStations();
				return 0;
			}
			if (!argp.isArgumentExists("start_station") || !argp.isArgumentExists("start_time") || !argp.isArgumentExists("dest_station")) {
				argp.showHelp();
				return 0;
			}

			Station *start_station, *dest_station;

			try {
				start_station = ds->getStationById(Utils::str2int(argp.getArgument("start_station")));
				dest_station = ds->getStationById(Utils::str2int(argp.getArgument("dest_station")));
			}
			catch (boost::bad_lexical_cast) {
				cout << "Error: start_station or dest_station not a number";
				throw HaException("Invalid station");
			}
			int start_time = Utils::parseTime(argp.getArgument("start_time"));
			ds->initTrains();
			Graph g(ds, start_station, start_time);
			g.dijkstra(dest_station);
			vector<Train *> shortest_route = g.backtraceRoute();
			vector<Train *> best_route = shortest_route;
			while (true) {
				// Try to obtain a route with a later train, that still ends at the same time
				int best_route_start = best_route[0]->getSourceTime();
				
				Graph g2(ds, start_station, best_route_start + 1);
				try {
					g2.dijkstra(dest_station);
				}
				catch (HaException) {
					break;
				}
				vector<Train *> alt_route = g2.backtraceRoute();
				if (getRouteEndTime(alt_route) > getRouteEndTime(best_route)) {
					break;
				}
				// We found a route that starts later, and ends at the same time.
				if (getRouteEndTime(alt_route) != getRouteEndTime(best_route)) {
					// This should not be possible
					cout << "BUG DETECTED, please report this" << endl;
				}
				best_route = alt_route;

				// Perhaps it doesn't even cost more train switches, in which case its simply better?
				if (countTrainSwitches(shortest_route) >= countTrainSwitches(alt_route)) {
					if (countTrainSwitches(shortest_route) != countTrainSwitches(alt_route)) {
						// This should not be possible
						cout << "BUG DETECTED, please report this" << endl;
					}
					shortest_route = alt_route;
				}
			}
			if (shortest_route == best_route) {
				printRoute(best_route);
			}
			else {
				cout << "Best route (train switches most important):" << endl;
				printRoute(shortest_route);
				cout << endl;
				cout << "Best route (delayed leaving most important):" << endl;
				printRoute(best_route);
			}
		}
		catch (HaException e) {
			cout << "Error: " << e.what() << endl;
		}

		if (ds) {
			delete ds;
			ds = nullptr;
		}
		return 0;
	}
}

// Global Namespace
int main(int argc, const char *argv[])
{
	return HaRail::main(argc, argv);
}