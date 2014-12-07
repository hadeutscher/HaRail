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
				throw HaException("Could not initialize data source", HaException::CONVERSION_ERROR);
			}
		}
		else if (argp.isArgumentExists("date")) {
			return new GTFSDataSource(DATA_ROOT, argp.getArgument("date"));
		}
		else {
			return new GTFSDataSource(DATA_ROOT, Utils::getCurrentDate());
		}
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
				cout << "Error: start_station or dest_station not a number" << endl;
				throw HaException("Invalid station", HaException::CONVERSION_ERROR);
			}
			int start_time = Utils::parseTime(argp.getArgument("start_time"));
			ds->initTrains();
			vector<Train *> shortest_route;
			vector<Train *> best_route;
			Graph::getBestRoutes(ds, start_station, start_time, dest_station, shortest_route, best_route);
			Graph::printBestRoutes(shortest_route, best_route, cout);
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