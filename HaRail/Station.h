/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __STATION_H__
#define __STATION_H__

#include "common.h"
#include <string>
#include <vector>

namespace HaRail {
	class Station {
	public:
		// Class Methods
		Station(int station_id, string station_name)
			: station_id(station_id),
			station_name(station_name)
		{}
		virtual ~Station() {}

		// Property Accessors
		int getStationId() const { return station_id; }
		const string& getStationName() const { return station_name; }

	protected:
		// Fields
		int station_id;
		string station_name;

		UNCOPYABLE_CLASS(Station);
	};
}
#endif //__STATION_H__
