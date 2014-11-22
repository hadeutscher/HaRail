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

#ifndef __PATHS_H__
#define __PATHS_H__

#include <vector>
#include <unordered_map>

#define UNCOPYABLE_CLASS(x) private: x(const x&) = delete; x& operator=(const x&) = delete;
#define PROP_RO(type, varname, readname) public: type readname() { return varname; } protected: type varname;
#define PROP_RW(type, varname, readname, writename) public: type readname() { return varname; } void writename(type value) { varname = value; } protected: type varname;
static const int UNEXPLORED_COST = 0x7FFFFFFF;

using namespace std;

class Stop;
class Edge;
class Station;
class IDataSource;
class TestDataSource;

int str2int(string& str);
string int2str(int i);
string padWithZeroes(string data, unsigned int target_len);
int convertTime(string& time);
Station *getStationById(int id);
Station *getStationByName(string& name);

class Stop {
public:
	Stop(int id, const char *time) : Stop(id, string(time)) {};
	Stop(int id, string& time);
	virtual ~Stop() {};

	PROP_RO(Station *, station, getStation);
	PROP_RO(int, time, getTime);
};

class Edge {
public:
	Edge(int train_id, Station *source, Station *dest, int source_time, int dest_time);
	virtual ~Edge() {};

	int getCost(int curr_time);
	bool isAvailable(int curr_time);

	PROP_RO(int, train_id, getTrainId);
	PROP_RO(Station *, source, getSource);
	PROP_RO(Station *, dest, getDest);
	PROP_RO(int, source_time, getSourceTime);
	PROP_RO(int, dest_time, getDestTime);

	UNCOPYABLE_CLASS(Edge);
};

class Station {
public:
	static Station *createStation(int station_id, string station_name);
	Station(int station_id, string station_name);
	virtual ~Station();

	PROP_RO(int, station_id, getStationId);
	PROP_RO(string, station_name, getStationName);
	PROP_RO(vector<Edge *> *, edges, getEdges);
	PROP_RW(bool, visited, getVisited, setVisited);
	PROP_RW(int, best_cost, getBestCost, setBestCost);
	PROP_RW(Edge *, best_source, getBestSource, setBestSource);
	PROP_RW(Edge *, best_dest, getBestDest, setBestDest);

	UNCOPYABLE_CLASS(Station);
};

class IDataSource {
public:
	virtual ~IDataSource() {};
	virtual void initStations() = 0;
	virtual void initTrains() = 0;
};

class TestDataSource : public IDataSource {
public:
	TestDataSource();
	virtual ~TestDataSource();
	virtual void initStations();
	virtual void initTrains();

private:
	void initTrain(int train_id, vector<Stop>& stops);

	UNCOPYABLE_CLASS(TestDataSource);
};

static vector<Station *> *g_stations = new vector<Station *>();
static unordered_map<int, Station *> *g_stations_by_id = new unordered_map<int, Station *>();
static unordered_map<string, Station *> *g_stations_by_name = new unordered_map<string, Station *>();
static bool g_optimize = true;

#endif //__PATHS_H__