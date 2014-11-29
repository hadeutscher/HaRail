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


#include "GTFSDataSource.h"

void GTFSDataSource::initStations()
{
	boost::filesystem::path p = root_path / boost::filesystem::path("stops.txt");
	char *buf;

	Utils::readFile(p.string(), &buf);
	boost::char_separator<char> sep("\r\n");
	boost::tokenizer<boost::char_separator<char>> tokenizer(buf, sep);
	vector<string> line_split;

	for (string line : tokenizer) {
		boost::split(line_split, line, boost::is_any_of(","));
		createStation(Utils::str2int(line_split.at(0)), line_split.at(1));
	}

	delete[] buf;
}

void GTFSDataSource::initTrains()
{
	boost::filesystem::path p = root_path / boost::filesystem::path("stop_times.txt");
	char *buf;

	Utils::readFile(p.string(), &buf);
	char *start = fasttrackToDate(buf);
	loadTrainsForDate(start);

	delete[] buf;
}

void GTFSDataSource::loadTrainsForDate(char *buf)
{
	boost::char_separator<char> sep("\r\n");
	boost::tokenizer<boost::char_separator<char>> tokenizer(buf, sep);

	vector<string> line_split, id_split;
	int curr_train_id = -1;
	int curr_seq = -1;
	Station *last_station = nullptr;
	int last_time = -1;

	for (string line : tokenizer) {
		boost::split(line_split, line, boost::is_any_of(","));
		if (boost::split(id_split, line_split.at(0), boost::is_any_of("_")).at(0) == date) {
			int train_id = Utils::str2int(id_split.at(1));
			string time1 = line_split.at(1);
			string time2 = line_split.at(2);
			Station *station = getStationById(Utils::str2int(line_split.at(3)));
			int seq = Utils::str2int(line_split.at(4));
			int dw_time1 = Utils::parseTime(time1);
			int dw_time2 = Utils::parseTime(time2);
			if (curr_train_id != train_id) {
				// new train
				curr_train_id = train_id;
				curr_seq = seq;
				if (curr_seq != 1) {
					throw HaException("bad database format");
				}
			}
			else {
				if (seq != ++curr_seq) {
					throw HaException("bad database format");
				}
				createTrain(train_id, last_station, station, last_time, dw_time1);
				//last_station->getEdges().push_back(new Edge(train_id, last_station, station, last_time, dw_time1));
			}
			last_station = station;
			last_time = dw_time2;
		}
		else {
			return;
		}
	}
}

char *GTFSDataSource::fasttrackToDate(char *buf) const
{
	string to_find_str = string("\r\n") + date;
	const char *to_find = to_find_str.c_str();
	char *result = strstr(buf, to_find);
	if (!result) {
		throw HaException("Invalid date or database too old");
	}
	return result + 2;
}