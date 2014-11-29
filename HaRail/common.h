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

#ifndef __COMMON_H__
#define __COMMON_H__

#define STATIC_CLASS(x) private: x() = delete;
#define UNCOPYABLE_CLASS(x) private: x(const x&) = delete; x& operator=(const x&) = delete;

#ifdef _DEBUG
static const char *DATA_ROOT = "C:\\irw_gtfs";
#else // _DEBUG
static const char *DATA_ROOT = "./irw_gtfs";
#endif // _DEBUG

static const int SWITCH_COST = 60;

using namespace std;

class Station;
class Train;
class Node;
class Edge;
class IDataSource;
class TestDataSource;
class GTFSDataSource;
class HaException;
class ArgumentParser;
class Utils;
class Graph;
class StringTokenizer;

#endif //__COMMON_H__
