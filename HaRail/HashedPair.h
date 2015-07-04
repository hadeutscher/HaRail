/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __HASHED_PAIR_H__
#define __HASHED_PAIR_H__

#include "common.h"
namespace HaRail {
	class HashedPair {
	public:
		template <typename T, typename U>
		std::size_t operator()(const std::pair<T, U> &x) const
		{
			return 3 * std::hash<T>()(x.first) + std::hash<U>()(x.second);
		}
	};
}
#endif //__HASHED_PAIR_H__
