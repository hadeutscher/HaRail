#ifndef __TRAIN_H__
#define __TRAIN_H__

#include "common.h"

class Train {
public:
	Train(int train_id, Station *source, Station *dest, int source_time, int dest_time)
		: train_id(train_id),
		source(source),
		dest(dest),
		source_time(source_time),
		dest_time(dest_time) {}
	virtual ~Train() {};

	int getCost() const { return dest_time - source_time; }

	int getTrainId() const { return train_id; }
	Station *getSource() const { return source; }
	Station *getDest() const { return dest; }
	int getSourceTime() const { return source_time; }
	int getDestTime() const { return dest_time; }

protected:
	int train_id;
	Station *source;
	Station *dest;
	int source_time;
	int dest_time;

	UNCOPYABLE_CLASS(Train);
};

#endif //__TRAIN_H__