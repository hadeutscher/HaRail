/* Copyright (C) 2015 haha01haha01

* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __HASTRTOK_H__
#define __HASTRTOK_H__

#include "common.h"
#include <cstring>
#include <string>

namespace HaRail {
	class StringTokenizer {
	public:
		class iterator;

		// Class Methods
		StringTokenizer(const char *buf, const char *token) : buf(buf), token(token), token_len(strlen(token)), buf_end(buf + strlen(buf)) {}
		~StringTokenizer() {}

		// Methods
		const char *getNextToken(const char *curr) const;
		void advanceIterator(const char **curr, const char **next_tok) const;
		string buildString(const char *curr, const char *next_tok) const { return string(curr, next_tok); }

		// Iterator Methods
		StringTokenizer::iterator begin() { return StringTokenizer::iterator(buf, this); }
		StringTokenizer::iterator end() { return StringTokenizer::iterator(buf_end, this); }

		class iterator {
		public:
			iterator(const char *buf, StringTokenizer *parent) : pos(buf), parent(parent), next_tok(parent->getNextToken(pos)) {}

			StringTokenizer::iterator& operator++();
			StringTokenizer::iterator operator++(int unused);
			string operator*() const { return parent->buildString(pos, next_tok); }
			bool operator==(const StringTokenizer::iterator& second) { return this->parent == second.parent && this->pos == second.pos; }
			bool operator!=(const StringTokenizer::iterator& second) { return !this->operator==(second); }

			const char *getPosition() const { return pos; }

		protected:
			const char *pos;
			const char *next_tok;
			StringTokenizer *parent;
		};

	protected:
		// Fields
		const char *buf;
		const char *token;
		const char *buf_end;
		size_t token_len;
	};
}
#endif //__HASTRTOK_H__
