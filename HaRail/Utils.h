#ifndef __UTILS_H__
#define __UTILS_H__

#include "common.h"
#include "HaException.h"
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>

class Utils {
public:
	static int str2int(const string& str) { return boost::lexical_cast<int, string>(str); }
	static string int2str(int i) { return boost::lexical_cast<string, int>(i); }
	static int parseTime(const string& time);
	static string padWithZeroes(const string& data, unsigned int target_len);
	static string makeTime(int time, bool short_form);
	static void readFile(const string& path, char **out_buf);
	static string getCurrentDate();

	STATIC_CLASS(Utils);
	UNCOPYABLE_CLASS(Utils);
};

#endif //__UTILS_H__
