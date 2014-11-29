#ifndef __IDS_H__
#define __IDS_H__

#include "common.h"
#include "Station.h"
#include "Train.h"
#include <unordered_map>
#include <iostream>

class IDataSource {
public:
	// Class Methods
	IDataSource() 
		: stations(),
		stations_by_id(),
		stations_by_name() {}
	virtual ~IDataSource();
	virtual void initStations() = 0;
	virtual void initTrains() = 0;

	// Methods
	Station *createStation(int station_id, const string& station_name);
	Train *createTrain(int train_id, Station *source, Station *dest, int source_time, int dest_time);
	void listStations();

	// Property Accessors
	vector<Train *>& getTrains() { return trains; }
	const vector<Train *>& getTrains() const { return trains; }
	vector<Station *>& getStations() { return stations; }
	const vector<Station *>& getStations() const { return stations; }
	Station *getStationById(int id) const { return stations_by_id.at(id); }
	Station *getStationByName(const string& name) const { return stations_by_name.at(name); }

protected:
	// Fields
	vector<Station *> stations;
	unordered_map<int, Station *> stations_by_id;
	unordered_map<string, Station *> stations_by_name;
	vector<Train *> trains;

};

#endif //__IDS_H__
