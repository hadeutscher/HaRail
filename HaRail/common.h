/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __COMMON_H__
#define __COMMON_H__

#define STATIC_CLASS(x) private: x() = delete;
#define UNCOPYABLE_CLASS(x) private: x(const x&) = delete; x& operator=(const x&) = delete;

#include <iostream>
using namespace std;

namespace HaRail {
#ifdef _DEBUG
#ifdef _WIN32
	static const char *DATA_ROOT = "C:/irw_gtfs2/";
#else
	static const char *DATA_ROOT = "~/irw_gtfs2/";
#endif
#else // _DEBUG
#ifdef ANDROID
	static const char *DATA_ROOT = "/sdcard/irw_gtfs2/";
#else // ANDROID
	static const char *DATA_ROOT = "./irw_gtfs2/";
#endif // ANDROID
#endif // _DEBUG

	static const unsigned int SWITCH_COST = 60; // This minimizes train switches
	static const unsigned int MOVEMENT_COST = 1; // This minimizes train movements, to prevent e.g. going a->b->c->d->c->b instead of a->b->c->b, if they have the same dest time

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
	class HaRailAPI;
	class GTFSReader;

	// HashedPair is not included here because VC is stupid
}
#endif //__COMMON_H__
