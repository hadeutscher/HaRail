/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __ARGS_H__
#define __ARGS_H__

#include "common.h"
#include <unordered_map>
#include <iostream>

namespace HaRail {
	class ArgumentParser {
	public:
		// Class Methods
		ArgumentParser(int argc, const char *argv[]) : argc(argc), argv(argv), args_map() {}
		virtual ~ArgumentParser() {}

		// Methods
		void parseArguments();
		const string& getArgument(const string& name) const { return args_map.at(name); }
		bool isArgumentExists(const string& name) const { return args_map.count(name) > 0; }
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
}

#endif //__ARGS_H__
