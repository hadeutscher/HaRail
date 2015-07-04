/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "common.h"
#include "HaException.h"
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>

namespace HaRail {
	class Utils {
	public:
		static int str2int(const string& str) { return boost::lexical_cast<int, string>(str); }
		static string int2str(int i) { return boost::lexical_cast<string, int>(i); }
		static int parseTime(const string& time);
		static string padWithZeroes(const string& data, unsigned int target_len);
		static string makeTime(int time, bool short_form);
		static void readFile(const string& path, char **out_buf);
		static void readFilePart(const string& path, char **out_buf, unsigned int start, unsigned int length);
		static string getCurrentDate();
		template<typename T>
		static void writeObject(ofstream& ofs, T data) {
			ofs.write((const char *)&data, sizeof(T));
		}
		template<typename T>
		static T readObject(ifstream& ifs) {
			T result;
			ifs.read((char *)&result, sizeof(T));
			return result;
		}

		STATIC_CLASS(Utils);
		UNCOPYABLE_CLASS(Utils);
	};
}
#endif //__UTILS_H__
