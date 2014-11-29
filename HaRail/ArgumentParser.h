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

#ifndef __ARGS_H__
#define __ARGS_H__

#include "common.h"
#include "IDataSource.h"
#include "Station.h"
#include <unordered_map>
#include <iostream>

class ArgumentParser {
public:
	// Class Methods
	ArgumentParser(int argc, const char *argv[]) : argc(argc), argv(argv), args_map() {}
	virtual ~ArgumentParser() {}

	// Methods
	void parseArguments();
	template<typename T> T getArgument(const string &name) const { throw HaException("unimplemented"); }
	template<> string getArgument<string>(const string& name) const { return args_map.at(name); }
	template<> bool getArgument<bool>(const string& name) const { return args_map.count(name) > 0; }
	static void showHelp();

	// Property Accessors
	int getArgc() const { return argc; }
	const char **getArgv() const { return argv; }

protected:
	// Fields
	unordered_map<string, string> args_map;
	int argc;
	const char **argv;

	UNCOPYABLE_CLASS(ArgumentParser);
};

#endif //__ARGS_H__
