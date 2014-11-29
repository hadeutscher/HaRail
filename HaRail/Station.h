#ifndef __STATION_H__
#define __STATION_H__

#include "common.h"
#include <vector>

class Station {
public:
	Station(int station_id, string station_name)
		: station_id(station_id),
		station_name(station_name)
		//trains()
	{}
	virtual ~Station() {}

	int getStationId() const { return station_id; }
	const string& getStationName() const { return station_name; }
	/*vector<Train *>& getTrains() { return trains; }
	const vector<Train *>& getTrains() const { return trains; }*/

protected:
	int station_id;
	string station_name;
	//vector<Train *> trains;

	UNCOPYABLE_CLASS(Station);
};

#endif //__STATION_H__