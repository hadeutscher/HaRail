/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "StringTokenizer.h"

namespace HaRail {
	const char *StringTokenizer::getNextToken(const char *curr) const
	{
		const char *next = strstr(curr, token);
		return next ? next : buf_end;
	}

	void StringTokenizer::advanceIterator(const char **curr, const char **next_tok) const
	{
		if (*next_tok != buf_end) {
			*curr = *next_tok + token_len;
			*next_tok = getNextToken(*curr);
		}
		else {
			*curr = buf_end;
		}
	}

	StringTokenizer::iterator& StringTokenizer::iterator::operator++()
	{
		parent->advanceIterator(&pos, &next_tok);
		return *this;
	}

	StringTokenizer::iterator StringTokenizer::iterator::operator++(int unused)
	{
		StringTokenizer::iterator result = *this;
		parent->advanceIterator(&pos, &next_tok);
		return result;
	}
}
