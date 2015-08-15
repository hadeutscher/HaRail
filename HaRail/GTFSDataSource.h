/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __GTFS_H__
#define __GTFS_H__

#include "common.h"
#include "Utils.h"
#include "IDataSource.h"
#include "Station.h"
#include "Train.h"
#include "StringTokenizer.h"
#include "HaException.h"
#include "GTFSReader.h"

#include <unordered_set>

namespace HaRail {
	class GTFSDataSource : public IDataSource {
	public:
		// Class Methods
		GTFSDataSource(const string& root, const string& date) : root_path(root), date(date) {}
		virtual ~GTFSDataSource() {}
		virtual void initStations();
		virtual void initTrains();

		// Constants
		static const unsigned int INDEXER_VERSION = 1;

	protected:
		// Private Methods
		unordered_set<string> loadServices() const;
		unordered_set<string> loadTrips(const unordered_set<string>& services) const;
		void loadStopTimes(const unordered_set<string>& trips);

		/*void loadTrainsForDate(char *start);
		pair<int, int> getDateIndex() const;
		void indexDatabase() const;*/

		// Fields
		string root_path;
		string date;
		
		// Constants
		const vector<string> dayStrings{ "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" };

		// Internal Classes
		typedef struct stopTime {
			int trainId;
			int arriveTime;
			int departTime;
			Station *station;
			int seq;
		} StopTime, *PStopTime;

		UNCOPYABLE_CLASS(GTFSDataSource);
	};
}
#endif //__GTFS_H__
