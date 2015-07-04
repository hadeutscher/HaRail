/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __IDS_H__
#define __IDS_H__

#include "common.h"
#include "Station.h"
#include "Train.h"
#include "HaException.h"
#include <unordered_map>
#include <iostream>

namespace HaRail {
	class IDataSource {
	public:
		// Class Methods
		IDataSource()
			: stations(),
			stations_by_id(),
			stations_by_name(),
			trains()
			/*trains_by_id()*/ {}
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
		//const unordered_map<int, vector<Train *>>& getTrainsHashtable() const { return trains_by_id; }

	protected:
		// Fields
		vector<Station *> stations;
		unordered_map<int, Station *> stations_by_id;
		unordered_map<string, Station *> stations_by_name;
		vector<Train *> trains;
		//unordered_map<int, vector<Train *>> trains_by_id;

	};
}
#endif //__IDS_H__
