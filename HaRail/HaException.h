#ifndef __HAEXCEPT_H__
#define __HAEXCEPT_H__

#include "common.h"
#include <stdexcept>
#include <string>

class HaException : public runtime_error {
public:
	HaException(const string& message)
		: runtime_error(message) {}
};

#endif //__HAEXCEPT_H__
