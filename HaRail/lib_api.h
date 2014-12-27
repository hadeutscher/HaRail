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
		static vector<int> getRoutes(int start_time, int start_station_id, int dest_station_id);
		static string getRoutesStr(int start_time, int start_station_id, int dest_station_id);
		static vector<int> getWholeTrainPath(int train_id);

	private:
		static void encodeRoute(const vector<Train *>& route, vector<int>& out_vec);

		static GTFSDataSource *gds;
		static HaException last_err;
	};
}

#endif //__LIB_API_H__
