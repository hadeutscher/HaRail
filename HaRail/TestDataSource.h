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

#ifndef __TEST_SOURCE_H__
#define __TEST_SOURCE_H__

#include "common.h"
#include "Utils.h"
#include "IDataSource.h"
#include "Station.h"
#include "Train.h"
#include "HaException.h"
#include <vector>

namespace HaRail {
	typedef pair<Station *, int> stop_t;
	typedef vector<stop_t> stop_arr_t;

	class TestDataSource : public IDataSource {
	public:
		// Class Methods
		TestDataSource(int test) : test(test) {}
		virtual ~TestDataSource() {}
		virtual void initStations();
		virtual void initTrains();

	protected:
		// Private Methods
		void initTrain(int train_id, const vector<stop_t>& stops);
		stop_t makeStop(int station_id, const string& time_str) const;

		// Fields
		int test;

		UNCOPYABLE_CLASS(TestDataSource);
	};
}
#endif //__TEST_SOURCE_H__
