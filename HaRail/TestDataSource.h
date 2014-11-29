#ifndef __TEST_SOURCE_H__
#define __TEST_SOURCE_H__

#include "common.h"
#include "Utils.h"
#include "IDataSource.h"
#include "Edge.h"
#include "Station.h"
#include "Train.h"
#include <vector>

typedef pair<Station *, int> stop_t;
typedef vector<stop_t> stop_arr_t;

class TestDataSource : public IDataSource {
public:
	TestDataSource(int test) : test(test) {}
	virtual ~TestDataSource() {}
	virtual void initStations();
	virtual void initTrains();

protected:
	void initTrain(int train_id, const vector<stop_t>& stops);
	stop_t makeStop(int station_id, const string& time_str) const;

	int test;

	UNCOPYABLE_CLASS(TestDataSource);
};

#endif //__TEST_SOURCE_H__