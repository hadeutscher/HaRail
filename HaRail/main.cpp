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

IDataSource *initializeDataSource(ArgumentParser& argp)
{
	if (argp.getArgument<bool>("test_source")) {
		try {
			return new TestDataSource(Utils::str2int(argp.getArgument<string>("test_source")));
		}
		catch (boost::bad_lexical_cast) {
			cout << "Error: Invalid argument test_source" << endl;
			throw HaException("Could not initialize data source");
		}
	}
	else if (argp.getArgument<bool>("date")) {
		return new GTFSDataSource(DATA_ROOT, argp.getArgument<string>("date"));
	}
	else {
		return new GTFSDataSource(DATA_ROOT, Utils::getCurrentDate());
	}
}

void printRoute(vector<Train *>& route)
{
	Train *last_train = nullptr;
	for (Train *train : route) {
		if (last_train && train->getTrainId() != last_train->getTrainId()) {
			cout << " to " << last_train->getDest()->getStationName() << " (" << Utils::makeTime(last_train->getDestTime(), true) << ")" << endl;
		}
		else {
			cout << "Train #" << train->getTrainId() << " from " << train->getSource()->getStationName() << " (" << Utils::makeTime(train->getSourceTime(), true) << ")";
		}
		last_train = train;
	}
	cout << " to " << last_train->getDest()->getStationName() << " (" << Utils::makeTime(last_train->getDestTime(), true) << ")" << endl;
}

int main(int argc, const char *argv[])
{
	IDataSource *ds = nullptr;
	try {
		ArgumentParser argp(argc, argv);
		argp.parseArguments();
		ds = initializeDataSource(argp);
		if (argp.getArgument<bool>("list_stations")) {
			ds->listStations();
			return 0;
		}
		if (!argp.getArgument<bool>("start_station") || !argp.getArgument<bool>("start_time") || !argp.getArgument<bool>("dest_station")) {
			argp.showHelp();
			return 0;
		}

		Station *start_station, *dest_station;

		try {
			start_station = ds->getStationById(Utils::str2int(argp.getArgument<string>("start_station")));
			dest_station = ds->getStationById(Utils::str2int(argp.getArgument<string>("dest_station")));
		}
		catch (boost::bad_lexical_cast) {
			cout << "Error: start_station or dest_station not a number";
			throw HaException("Invalid station");
		}
		int start_time = Utils::parseTime(argp.getArgument<string>("start_time"));
		Graph g(ds);
		g.dijkstra(start_station, start_time, dest_station);
		vector<Train *> route = g.backtraceRoute();
		printRoute(route);
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