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

#include "paths.h"

#include <boost/filesystem.hpp>

class GTFSDataSource : public IDataSource {
public:
	GTFSDataSource(const string& root, const string& date = getCurrentDate());
	virtual ~GTFSDataSource();
	virtual void initStations();
	virtual void initTrains();

private:
	boost::filesystem::path root_path;
	void loadTrainsForDate(char *start, int size);
	static void readFile(const string& path, char **out_buf, int *out_size);
	static string getCurrentDate();
	char *fasttrackToDate(char *buf, int size);

	string date;

	UNCOPYABLE_CLASS(GTFSDataSource);
};

#endif //__GTFS_H__