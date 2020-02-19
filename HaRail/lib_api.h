/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __LIB_API_H__
#define __LIB_API_H__

#include "common.h"
#include "Utils.h"
#include "HaException.h"
#include "IDataSource.h"
#include "GTFSDataSource.h"
#include "Station.h"
#include "Train.h"
#include "Node.h"
#include "Edge.h"
#include "Graph.h"
#include <sstream>

namespace HaRail {
	class HaRailAPI {
	public:
		static bool loadData(int date, int start_time, string data_root);
		static string getLastError();
        static vector<int> getAllFollowingRoutes(int start_time, int start_station_id, int dest_station_id);
		static vector<int> getRoutes(int start_time, int start_station_id, int dest_station_id);
		static string getRoutesStr(int start_time, int start_station_id, int dest_station_id);
		static vector<int> getWholeTrainPath(int train_id);

	private:
		static void encodeRoute(const vector<Train *>& route, vector<int>& out_vec);
        static void encodeRouteSimple(const vector<Train *>& route, vector<int>& out_vec);

		static GTFSDataSource *gds;
		static HaException last_err;
	};
}

#endif //__LIB_API_H__
