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

#include "TestDataSource.h"

void TestDataSource::initTrain(int train_id, const stop_arr_t& stops)
{
	if (stops.size() < 1) {
		return;
	}
	for (unsigned int i = 0; i < stops.size() - 1; i++) {
		createTrain(train_id, stops[i].first, stops[i + 1].first, stops[i].second, stops[i + 1].second);
	}
}

stop_t TestDataSource::makeStop(int station_id, const string& time_str) const
{
	return stop_t(getStationById(station_id), Utils::parseTime(time_str));
}

void TestDataSource::initStations()
{
	createStation(100, "stat_a");
	createStation(200, "stat_b");
	createStation(300, "stat_c");
	createStation(400, "stat_d");
	createStation(500, "stat_e");
	createStation(600, "stat_f");
}

void TestDataSource::initTrains()
{
	switch (test) {
	case 1:
		// run with -t 1 100 10:00:00 600
		initTrain(3, stop_arr_t{
			  makeStop(500, "12:01:00"), makeStop(600, "12:30:00")
	});
		initTrain(1, stop_arr_t{
			makeStop(100, "10:00:00"), makeStop(200, "10:30:00"), makeStop(300, "11:00:00"), makeStop(400, "11:30:00"), makeStop(500, "12:00:00"), makeStop(600, "12:30:00")
		});
		initTrain(2, stop_arr_t{
			makeStop(200, "10:31:00"), makeStop(400, "10:32:00")
		});
		break;
	case 2:
		// run with -t 2 100 10:00:00 500
		initTrain(1, stop_arr_t{
			  makeStop(100, "10:00:00"), makeStop(200, "10:30:00"), makeStop(300, "11:00:00"), makeStop(400, "11:30:00")
	});
		initTrain(2, stop_arr_t{
			makeStop(400, "12:00:00"), makeStop(500, "12:30:00")
		});
		initTrain(3, stop_arr_t{
			makeStop(300, "11:01:00"), makeStop(400, "11:02:00")
		});
		break;
	case 3:
		// run with -t 3 100 10:00:00 300
		initTrain(1, stop_arr_t{
			  makeStop(100, "10:00:00"), makeStop(200, "10:30:00"), makeStop(300, "11:00:00"), makeStop(400, "11:30:00")
	});
		initTrain(2, stop_arr_t{
			makeStop(100, "10:00:00"), makeStop(400, "10:30:00")
		});
		initTrain(3, stop_arr_t{
			makeStop(400, "10:30:00"), makeStop(300, "10:40:00")
		});
		break;
	default:
		throw HaException("Test case not implemented");
	}
}
