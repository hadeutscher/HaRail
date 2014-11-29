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

#ifndef __GTFS_H__
#define __GTFS_H__

#include "common.h"
#include "Utils.h"
#include "IDataSource.h"
#include "Station.h"
#include "Edge.h"
#include "StringTokenizer.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

class GTFSDataSource : public IDataSource {
public:
	// Class Methods
	GTFSDataSource(const string& root, const string& date) : root_path(root), date(date) {}
	virtual ~GTFSDataSource() {}
	virtual void initStations();
	virtual void initTrains();

protected:
	// Private Methods
	void loadTrainsForDate(char *start);
	char *fasttrackToDate(char *buf) const;
	
	// Fields
	boost::filesystem::path root_path;
	string date;

	UNCOPYABLE_CLASS(GTFSDataSource);
};

#endif //__GTFS_H__
