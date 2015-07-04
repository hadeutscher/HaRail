/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __TRAIN_H__
#define __TRAIN_H__

#include "common.h"

namespace HaRail {
	class Train {
	public:
		// Class Methods
		Train(int train_id, Station *source, Station *dest, int source_time, int dest_time)
			: train_id(train_id),
			source(source),
			dest(dest),
			source_time(source_time),
			dest_time(dest_time) {}
		virtual ~Train() {};

		// Methods
		int getCost() const { return dest_time - source_time; }

		// Property Accessors
		int getTrainId() const { return train_id; }
		Station *getSource() const { return source; }
		Station *getDest() const { return dest; }
		int getSourceTime() const { return source_time; }
		int getDestTime() const { return dest_time; }

	protected:
		// Fields
		int train_id;
		Station *source;
		Station *dest;
		int source_time;
		int dest_time;

		UNCOPYABLE_CLASS(Train);
	};
}
#endif //__TRAIN_H__
