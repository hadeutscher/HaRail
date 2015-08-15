/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __GTFS_READER__
#define __GTFS_READER__

#include "common.h"
#include "Utils.h"
#include "StringTokenizer.h"
#include <string>
#include <unordered_map>

namespace HaRail {
	class GTFSReader {
	public:
		// Class Methods
		GTFSReader(const string& file);
		virtual ~GTFSReader();
		int getHeaderIndex(const string& name) const;
		StringTokenizer getLines() const;

		// Static Methods
		static vector<string> splitLine(string line);

	private:
		// Fields
		char *buf;
		char *fileStart;
		unordered_map<string, int> headers;

		// Constants
		const char *linebreak = "\r\n";

		UNCOPYABLE_CLASS(GTFSReader);
	};
}

#endif //__GTFS_READER__
