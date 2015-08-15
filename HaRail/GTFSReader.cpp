/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GTFSReader.h"

namespace HaRail {
	GTFSReader::GTFSReader(const string& file)
		: buf(nullptr), 
		fileStart(nullptr),
		headers()
	{
		Utils::readFile(file, &buf);
		char *first_line = strstr(buf, linebreak);
		if (!first_line) {
			throw HaException("bad database format", HaException::DATABASE_FORMAT_ERROR);
		}
		fileStart = first_line + 2;
		vector<char> header;
		header.assign(buf, first_line);
		header.push_back(0);
		StringTokenizer headerTokens(header.data(), ",");
		int i = 0;
		for (string header : headerTokens) {
			headers.emplace(header, i++);
		}
	}

	GTFSReader::~GTFSReader()
	{
		if (buf != nullptr) {
			delete[] buf;
			buf = nullptr;
		}
	}

	int GTFSReader::getHeaderIndex(const string& name) const
	{
		return headers.at(name);
	}

	vector<string> GTFSReader::splitLine(string line)
	{
		StringTokenizer tokenizer(line.c_str(), ",");
		vector<string> result;
		for (string token : tokenizer) {
			result.push_back(token);
		}
		return result;
	}

	StringTokenizer GTFSReader::getLines() const
	{
		return StringTokenizer(fileStart, linebreak);
	}
}