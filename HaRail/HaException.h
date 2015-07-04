/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __HAEXCEPT_H__
#define __HAEXCEPT_H__

#include "common.h"
#include <stdexcept>
#include <string>

namespace HaRail {
	class HaException : public runtime_error {
	public:
		enum type {
			CONVERSION_ERROR,
			FILE_NOT_FOUND_ERROR,
			MEMORY_ERROR,
			UNIMPLEMENTED_ERROR,
			CRITICAL_ERROR,
			INVALID_ROUTE_ERROR,
			DATABASE_FORMAT_ERROR
		};
		HaException(const string& message, type type)
			: runtime_error(message), type(type) {}

		type getType() const { return type; }

	protected:
		type type;
	};
}
#endif //__HAEXCEPT_H__
