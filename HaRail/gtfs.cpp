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

#include "gtfs.h"

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>

GTFSDataSource::GTFSDataSource(const string& root, const string& date)
: root_path(root),
date(date)
{
}

GTFSDataSource::~GTFSDataSource()
{
}

void GTFSDataSource::initStations()
{
	boost::filesystem::path p = root_path / boost::filesystem::path("stops.txt");
	ifstream ifs(p.c_str(), std::ios_base::in);
	string line;
	getline(ifs, line);
	vector<string> line_split;
	while (getline(ifs, line)) {
		boost::split(line_split, line, boost::is_any_of(","));
		Station::createStation(str2int(line_split.at(0)), line_split.at(1));
	}
}

void GTFSDataSource::loadTrainsForDate(char *start, int size)
{
	char *line_ptr = strtok(start, "\r\n");
	vector<string> line_split, id_split;
	int curr_train_id = -1;
	int curr_seq = -1;
	Station *last_station = nullptr;
	int last_time = -1;

	while (line_ptr) {
		string line(line_ptr);
		boost::split(line_split, line, boost::is_any_of(","));
		if (boost::split(id_split, line_split.at(0), boost::is_any_of("_")).at(0) == date) {
			int train_id = str2int(id_split.at(1));
			string time1 = line_split.at(1);
			string time2 = line_split.at(2);
			Station *station = getStationById(str2int(line_split.at(3)));
			int seq = str2int(line_split.at(4));
			int dw_time1 = convertTime(time1);
			int dw_time2 = convertTime(time2);
			if (curr_train_id != train_id) {
				// new train
				curr_train_id = train_id;
				curr_seq = seq;
				if (curr_seq != 1) {
					throw new exception("bad format");
				}
			}
			else {
				if (seq != ++curr_seq) {
					throw new exception("bad format");
				}
				last_station->getEdges()->push_back(new Edge(train_id, last_station, station, last_time, dw_time1));
			}
			last_station = station;
			last_time = dw_time2;
		}
		else {
			return;
		}
		line_ptr = strtok(nullptr, "\r\n");
	}
}

void GTFSDataSource::readFile(const string& path, char **out_buf, int *out_size)
{
	ifstream ifs(path, ios::in | ios::binary | ios::ate);
	if (!ifs.good()) {
		throw new exception("Could not access database");
	}
	int size = ifs.tellg();
	char *buf = new char[size];
	if (!buf) {
		throw new exception("Not enough memory");
	}
	ifs.seekg(0, ios_base::beg);
	ifs.read(buf, size);
	*out_buf = buf;
	*out_size = size;
}

char *GTFSDataSource::fasttrackToDate(char *buf, int size)
{
	string to_find_str = string("\r\n") + date;
	const char *to_find = to_find_str.c_str();
	char *result = strstr(buf, to_find);
	if (!result) {
		throw new exception("Invalid date or database too old");
	}
	return result + 2;
}

string GTFSDataSource::getCurrentDate()
{
	boost::posix_time::ptime pt = boost::posix_time::second_clock::local_time();
	boost::gregorian::date d = pt.date();
	string date = string(padWithZeroes(int2str(d.day().as_number()), 2) + padWithZeroes(int2str(d.month().as_number()), 2) + padWithZeroes(int2str(d.year() % 100), 2));
	return date;
}

void GTFSDataSource::initTrains()
{
	boost::filesystem::path p = root_path / boost::filesystem::path("stop_times.txt");
	char *buf;
	int size;
	readFile(p.string(), &buf, &size);
	char *start = fasttrackToDate(buf, size);
	loadTrainsForDate(start, size);
	delete[] buf;
}