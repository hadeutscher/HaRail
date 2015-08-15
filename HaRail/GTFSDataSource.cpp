/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GTFSDataSource.h"

namespace HaRail {
	void GTFSDataSource::initStations()
	{
		GTFSReader reader(root_path + "stops.txt");
		int stop_id_idx = reader.getHeaderIndex("stop_id");
		int stop_name_idx = reader.getHeaderIndex("stop_name");
		for (string line : reader.getLines()) {
			vector<string> lineSplit = reader.splitLine(line);
			createStation(Utils::str2int(lineSplit[stop_id_idx]), lineSplit[stop_name_idx]);
		}
	}

	void GTFSDataSource::initTrains()
	{
		unordered_set<string> services = loadServices();
		unordered_set<string> trips = loadTrips(services);
		loadStopTimes(trips);
	}

	unordered_set<string> GTFSDataSource::loadServices() const
	{
		GTFSReader reader(root_path + "calendar.txt");
		int dateInt = Utils::str2int(Utils::getReverseDate(date));
		int dayOfWeek = Utils::getDayOfWeek(date);
		int day_idx = reader.getHeaderIndex(dayStrings.at(dayOfWeek));
		int service_id_idx = reader.getHeaderIndex("service_id");
		int start_date_idx = reader.getHeaderIndex("start_date");
		int end_date_idx = reader.getHeaderIndex("end_date");
		unordered_set<string> result;
		for (string line : reader.getLines()) {
			vector<string> lineSplit = reader.splitLine(line);
			if (Utils::str2int(lineSplit[day_idx]) > 0 && Utils::str2int(lineSplit[start_date_idx]) <= dateInt && Utils::str2int(lineSplit[end_date_idx]) >= dateInt) { // It seems that both start and end are inclusive
				result.emplace(lineSplit[service_id_idx]);
			}
		}
		return result;
	}

	unordered_set<string> GTFSDataSource::loadTrips(const unordered_set<string>& services) const
	{
		GTFSReader reader(root_path + "trips.txt");
		int service_id_idx = reader.getHeaderIndex("service_id");
		int trip_id_idx = reader.getHeaderIndex("trip_id");
		unordered_set<string> result;
		auto notFound = services.end();
		for (string line : reader.getLines()) {
			vector<string> lineSplit = reader.splitLine(line);
			if (services.find(lineSplit[service_id_idx]) != notFound) {
				result.emplace(lineSplit[trip_id_idx]);
			}
		}
		return result;
	}

	void GTFSDataSource::loadStopTimes(const unordered_set<string>& trips)
	{
		GTFSReader reader(root_path + "stop_times.txt");
		int trip_id_idx = reader.getHeaderIndex("trip_id");
		int arrival_time_idx = reader.getHeaderIndex("arrival_time");
		int departure_time_idx = reader.getHeaderIndex("departure_time");
		int stop_id_idx = reader.getHeaderIndex("stop_id");
		int stop_sequence_idx = reader.getHeaderIndex("stop_sequence");
		auto notFound = trips.end();

		vector<StopTime> stopTimes;
		string currTripId;
		int currTrainId;
		bool tripIdValid = false;
		StopTime currStop;

		// Parse the file and extract all the relevant stop times
		for (string line : reader.getLines()) {
			vector<string> lineSplit = reader.splitLine(line);
			if (lineSplit[trip_id_idx] == currTripId) {
				if (!tripIdValid) {
					// If we already know the trip is invalid, just continue
					continue;
				}
			}
			else {
				// This is a new trip, validate it
				currTripId = lineSplit[trip_id_idx];
				tripIdValid = trips.find(currTripId) != notFound;
				if (!tripIdValid) {
					// If the new trip is invalid, just continue
					continue;
				}
				// If the new trip is valid, extract its train ID
				currTrainId = Utils::str2int(*(StringTokenizer(currTripId.c_str(), "_").begin()));
			}

			// Trip is valid, store the stop
			currStop.trainId = currTrainId;
			currStop.seq = Utils::str2int(lineSplit[stop_sequence_idx]);
			currStop.arriveTime = Utils::parseTime(lineSplit[arrival_time_idx]);
			currStop.departTime = Utils::parseTime(lineSplit[departure_time_idx]);
			currStop.station = getStationById(Utils::str2int(lineSplit[stop_id_idx]));
			stopTimes.push_back(currStop);
		}

		sort(stopTimes.begin(), stopTimes.end(),
			[](const StopTime& a, const StopTime& b) -> bool
		{
			if (a.trainId > b.trainId) {
				return false;
			}
			else if (a.trainId < b.trainId) {
				return true;
			}
			else {
				if (a.seq > b.seq) {
					return false;
				}
				else if (a.seq < b.seq) {
					return true;
				}
				else {
					throw HaException("bad database format: multiple stops with same SEQ", HaException::DATABASE_FORMAT_ERROR);
				}
			}
		});

		currTrainId = -1;
		int currSeq = -1;
		Station *lastStation = nullptr;
		int lastTime = -1;

		for (StopTime const& stop : stopTimes) {
			if (stop.trainId != currTrainId) {
				// New train
				if (stop.seq != (currSeq = 1)) {
					throw HaException("bad database format: missing SEQs", HaException::DATABASE_FORMAT_ERROR);
				}
				currTrainId = stop.trainId;
			}
			else {
				if (stop.seq != ++currSeq) {
					throw HaException("bad database format: missing SEQs", HaException::DATABASE_FORMAT_ERROR);
				}
				createTrain(currTrainId, lastStation, stop.station, lastTime, stop.arriveTime);
			}

			lastStation = stop.station;
			lastTime = stop.departTime;
		}
	}
}
