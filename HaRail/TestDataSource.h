/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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
