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

#include "paths.h"
#include "gtfs.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <sstream>

int str2int(string& str)
{
	return boost::lexical_cast<int, string>(str);
}

string int2str(int i)
{
	return boost::lexical_cast<string, int>(i);
}

int convertTime(string& time)
{
	if (time.substr(2, 1) != ":" || time.substr(5, 1) != ":") {
		throw new exception("bad time");
	}
	return str2int(time.substr(0, 2)) * 3600 + str2int(time.substr(3, 2)) * 60 + str2int(time.substr(6, 2));
}

string padWithZeroes(string data, unsigned int target_len)
{
	while (data.length() < target_len) {
		data = "0" + data;
	}
	return data;
}

string convertToTime(int time)
{
	string hours = padWithZeroes(int2str(time / 3600), 2);
	time %= 3600;
	string mins = padWithZeroes(int2str(time / 60), 2);
	string secs = padWithZeroes(int2str(time % 60), 2);
	return hours + ":" + mins + ":" + secs;
}

string convertToUserTime(int time)
{
	string hours = padWithZeroes(int2str(time / 3600), 2);
	time %= 3600;
	string mins = padWithZeroes(int2str(time / 60), 2);
	return hours + ":" + mins;
}

Station *getStationById(int id)
{
	return g_stations_by_id->at(id);
}

Station *getStationByName(string& name)
{
	return g_stations_by_name->at(name);
}

Stop::Stop(int id, string& time)
: time(convertTime(time)),
station(getStationById(id))
{
}

Edge::Edge(int train_id, Station *source, Station *dest, int source_time, int dest_time)
: train_id(train_id),
source(source),
dest(dest),
source_time(source_time),
dest_time(dest_time)
{
}

int Edge::getCost(int curr_time)
{
	return this->getDestTime() - curr_time;
}

bool Edge::isAvailable(int curr_time)
{
	return this->getSourceTime() >= curr_time;
}

Station *Station::createStation(int station_id, string station_name)
{
	Station *station = new Station(station_id, station_name);
	g_stations->push_back(station);
	g_stations_by_id->emplace(station_id, station);
	g_stations_by_name->emplace(station_name, station);
	return station;
}

Station::Station(int station_id, string station_name)
: station_id(station_id),
station_name(station_name),
edges(new vector<Edge *>()),
visited(false),
best_cost(UNEXPLORED_COST),
best_source(nullptr),
best_dest(nullptr)
{
}

Station::~Station()
{
	delete this->edges;
}

TestDataSource::TestDataSource()
{
}

TestDataSource::~TestDataSource()
{
}

void TestDataSource::initStations()
{
	Station::createStation(100, "stat_a");
	Station::createStation(200, "stat_b");
	Station::createStation(300, "stat_c");
	Station::createStation(400, "stat_d");
	Station::createStation(500, "stat_e");
	Station::createStation(600, "stat_f");
}

void TestDataSource::initTrain(int train_id, vector<Stop>& stops)
{
	if (stops.size() < 1) {
		return;
	}
	for (unsigned int i = 0; i < stops.size() - 1; i++) {
		stops[i].getStation()->getEdges()->push_back(new Edge(train_id, stops[i].getStation(), stops[i + 1].getStation(), stops[i].getTime(), stops[i + 1].getTime()));
	}
}

#define TEST_NONE

void TestDataSource::initTrains()
{
#ifdef TEST1
	// run with -t 100 10:00:00 600
	initTrain(3, vector<Stop> {
		Stop(500, "12:01:00"), Stop(600, "12:30:00")
	});
	initTrain(1, vector<Stop> {
		Stop(100, "10:00:00"), Stop(200, "10:30:00"), Stop(300, "11:00:00"), Stop(400, "11:30:00"), Stop(500, "12:00:00"), Stop(600, "12:30:00")
	});
	initTrain(2, vector<Stop> {
		Stop(200, "10:31:00"), Stop(400, "10:32:00")
	});
#endif
#ifdef TEST2
	// run with -t 100 10:00:00 500
	initTrain(1, vector<Stop> {
		Stop(100, "10:00:00"), Stop(200, "10:30:00"), Stop(300, "11:00:00"), Stop(400, "11:30:00")
	});
	initTrain(2, vector<Stop> {
		Stop(400, "12:00:00"), Stop(500, "12:30:00")
	});
	initTrain(3, vector<Stop> {
		Stop(300, "11:01:00"), Stop(400, "11:02:00")
	});
#endif
}

int dijkstra(Station *start, int start_time, Station *end)
{
	int time = start_time;
	start->setBestCost(0);
	Station *curr = start;
	while (curr != end) {
		for each (Edge *edge in *curr->getEdges()) {
			if (edge->isAvailable(time) && !edge->getDest()->getVisited()) {
				int cost = edge->getCost(start_time);
				if (edge->getDest()->getBestCost() > cost) {
					edge->getDest()->setBestCost(cost);
					edge->getDest()->setBestSource(edge);
				}
			}
		}
		curr->setVisited(true);
		
		int closest_station_cost = UNEXPLORED_COST;
		Station *closest_station = nullptr;
		for each (Station *station in *g_stations) {
			if (!station->getVisited() && station->getBestCost() < closest_station_cost) {
				closest_station_cost = station->getBestCost();
				closest_station = station;
			}
		}

		curr = closest_station;
		time = start_time + closest_station->getBestCost();
	}
	return time;
}

Edge *getEdgeWithTrainId(Station *s, int train_id)
{
	for each (Edge *edge in *s->getEdges()) {
		if (edge->getTrainId() == train_id) {
			return edge;
		}
	}
	return nullptr;
}

unordered_map<Station *, int> mapTrainArrivalTimes(Station *start, int train_id)
{
	unordered_map<Station *, int> result;
	Edge *next_arrive = getEdgeWithTrainId(start, train_id);
	while (next_arrive) {
		result.emplace(next_arrive->getDest(), next_arrive->getDestTime());
		next_arrive = getEdgeWithTrainId(next_arrive->getDest(), train_id);
	}
	return result;
}

void reroutTrainRoute(Station *start, Station *end, int train_id, int start_time)
{
	Station *curr = start;
	while (curr != end) {
		Edge *next_arrive = getEdgeWithTrainId(curr, train_id);
		Station *next_station = next_arrive->getDest();
		curr->setBestDest(next_arrive);
		next_station->setBestSource(next_arrive);
		next_station->setBestCost(next_arrive->getCost(start_time));
		curr = next_station;
	}
}

void attemptRemoveTrainSwitch(Station *start, Station *end, int old_train_id, int start_time)
{
	unordered_map<Station *, int> train_sched = mapTrainArrivalTimes(start, old_train_id);
	Station *curr = start;
	while (curr != end) {
		Station *next_stat = curr->getBestDest()->getDest();
		if (train_sched.count(next_stat) > 0) {
			int old_train_arrival_at_next_station = train_sched.at(next_stat);
			if (next_stat == end) {
				if (old_train_arrival_at_next_station == curr->getBestDest()->getDestTime()) {
					reroutTrainRoute(start, next_stat, old_train_id, start_time);
				}
				break;
			}
			int next_station_destination_leaving_time = next_stat->getBestDest()->getSourceTime();
			int next_station_destination_train_id = next_stat->getBestDest()->getTrainId();
			if (old_train_arrival_at_next_station < next_station_destination_leaving_time || next_station_destination_train_id == old_train_id) {
				reroutTrainRoute(start, next_stat, old_train_id, start_time);
				start = next_stat;
				if (next_station_destination_train_id == old_train_id) {
					break;
				}
			}
		}
		curr = next_stat;
	}
}

void minimizeSwitches(Station *start, int start_time, Station *end) 
{
	Station *curr = start;
	int last_train_id = -1;
	while (curr != end) {
		Edge* next = curr->getBestDest();
		if (next->getTrainId() != last_train_id) {
			if (last_train_id == -1) {
				last_train_id = next->getTrainId();
			}
			else {
				// Train switch
				attemptRemoveTrainSwitch(curr, end, last_train_id, start_time);
				// Restore running in case best dest was switched
				next = curr->getBestDest();
				last_train_id = next->getTrainId();
			}
		}
		curr = next->getDest();
	}
}

pair<string, int> backtraceRoute(Station *start, int start_time, Station *end, int end_time)
{
	Station *curr = end;
	while (curr != start) {
		Edge *best_edge = curr->getBestSource();
		best_edge->getSource()->setBestDest(best_edge);
		curr = best_edge->getSource();
	}

	if (g_optimize) {
		minimizeSwitches(start, start_time, end);
	}

	int train_count = 0;
	stringstream ss;
	bool boarded = false;
	int curr_train = -1;
	int curr_time = 0;

	while (curr != end) {
		Edge *edge = curr->getBestDest();
		if (edge->getTrainId() != curr_train) {
			if (boarded) {
				ss << " to " << curr->getStationName() << " (" << convertToUserTime(curr_time) << ")" << endl;
			}
			else {
				boarded = true;
			}
			curr_time = edge->getSourceTime();
			curr_train = edge->getTrainId();
			ss << "Train #" << curr_train << " from " << curr->getStationName() << " (" << convertToUserTime(curr_time) << ")";
			train_count++;
		}
		else {
			curr_time = edge->getSourceTime();
		}
		curr = edge->getDest();
		curr_time = edge->getDestTime();
	}

	if (curr_time != end_time) {
		cout << "Explode universe" << endl;
	}
	ss << " to " << end->getStationName() << " (" << convertToUserTime(end_time) << ")" << endl;
	return pair<string, int>(ss.str(), train_count);
}

void listStations()
{
	for each (Station *station in *g_stations) {
		cout << station->getStationId() << " : " << station->getStationName() << endl;
	}
}

void printUsage()
{
	cout << "HaRail - Because Israel Railways Suck Dick (TM)" << endl << endl
		<< "Usage: HaRail [options] start_station start_time dest_station" << endl
		<< "  options:" << endl 
		<< "    -l\t\tList stations (ignores all other args)" << endl 
		<< "    -t\t\tUse test data source (for debugging)" << endl
		<< "    -d <date>\t Specify custom date. If this option is not present, today will be used." << endl
		<< "    -m\t\tDo not add cost to train switches" << endl << endl
		<< "  examples:" << endl
		<< "    Find route from Modiin to Binyamina, today, starting at 10 o'clock:" << endl << "      HaRail 300 10:00:00 2800" << endl
		<< "    Find the same route, at 22.05.13:" << endl << "      HaRail -d 220513 300 10:00:00 2800" << endl;
}

void resetEnvironment()
{
	for each (Station *station in *g_stations) {
		station->setBestSource(nullptr);
		station->setBestDest(nullptr);
		station->setBestCost(UNEXPLORED_COST);
		station->setVisited(false);
	}
}

int main(int argc, char *argv[])
{
#ifdef _DEBUG
	string data_path("C:\\irw_gtfs");
#else
	string data_path(".//irw_gtfs");
#endif

	bool list_stations = false;
	bool test_source = false;
	char *date_ptr = nullptr;
	char *start_station_ptr = nullptr;
	char *start_time_ptr = nullptr;
	char *dest_station_ptr = nullptr;

	for (int i = 1; i < argc; i++) {
		string arg_str(argv[i]);
		if (arg_str == "-l") {
			list_stations = true;
		}
		else if (arg_str == "-t") {
			test_source = true;
		}
		else if (arg_str == "-d") {
			date_ptr = argv[++i];
		}
		else if (arg_str == "-m") {
			g_optimize = false;
		}
		else {
			if (!start_station_ptr) {
				start_station_ptr = argv[i];
			}
			else if (!start_time_ptr) {
				start_time_ptr = argv[i];
			}
			else if (!dest_station_ptr) {
				dest_station_ptr = argv[i];
			}
		}
	}

	if (!list_stations && (!start_station_ptr || !start_time_ptr || !dest_station_ptr)) {
		printUsage();
		return 0;
	}

	IDataSource *ds;
	if (test_source) {
		ds = new TestDataSource();
	}
	else if (date_ptr) {
		ds = new GTFSDataSource(data_path, string(date_ptr));
	}
	else {
		ds = new GTFSDataSource(data_path);
	}
	ds->initStations();
	if (list_stations) {
		listStations();
		return 0;
	}
	try {
		ds->initTrains();
	}
	catch (exception *e) {
		string what(e->what());
		cout << "Error: " << what << endl;
		return 0;
	}
	Station *start_station;
	int start_time;
	Station *dest_station;

	try {
		start_station = getStationById(str2int(string(start_station_ptr)));
	}
	catch (...) {
		cout << "Error: Invalid start_station" << endl;
		return 0;
	}
	try {
		start_time = convertTime(string(start_time_ptr));
	}
	catch (...) {
		cout << "Error: Invalid start_time" << endl;
		return 0;
	}
	try {
		dest_station = getStationById(str2int(string(dest_station_ptr)));
	}
	catch (...) {
		cout << "Error: Invalid dest_station" << endl;
		return 0;
	}

	int dest_time = dijkstra(start_station, start_time, dest_station);
	pair<string, int> best_res = backtraceRoute(start_station, start_time, dest_station, dest_time);

	if (g_optimize) {
		while (true) {
			int first_rail_time = start_station->getBestDest()->getSourceTime();
			int new_start_time = first_rail_time + 1;
			resetEnvironment();
			if (dest_time == dijkstra(start_station, new_start_time, dest_station)) {
				//cout << endl <<  "BUT WAIT, THERES MORE!" << endl << "Rail switch optimization found a (possibly) better route:" << endl;
				pair<string, int> curr_res = backtraceRoute(start_station, start_time, dest_station, dest_time);
				if (curr_res.second <= best_res.second) {
					best_res = curr_res;
				}
			}
			else {
				break;
			}
		}
	}
	
	cout << best_res.first;
}