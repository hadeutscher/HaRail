﻿/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "IDataSource.h"

namespace HaRail {
	IDataSource::~IDataSource()
	{
		for (Station *station : stations) {
			delete station;
		}

		for (Train *train : trains) {
			delete train;
		}
	}

	Station *IDataSource::createStation(int station_id, const string& station_name)
	{
		Station *station = new Station(station_id, station_name);
		stations.push_back(station);
		stations_by_id.emplace(station_id, station);
		stations_by_name.emplace(station_name, station);
		return station;
	}

	Train *IDataSource::createTrain(int train_id, Station *source, Station *dest, int source_time, int dest_time)
	{
		Train *train = new Train(train_id, source, dest, source_time, dest_time);
		trains.push_back(train);
		return train;
	}

	void IDataSource::listStations()
	{
		for (Station *station : stations) {
			cout << station->getStationId() << " : " << station->getStationName().c_str() << endl;
		}
	}
}
