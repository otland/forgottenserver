/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_SPECTATORS_H_D78A7CCB7080406E8CAA6B1D31D3DA71
#define FS_SPECTATORS_H_D78A7CCB7080406E8CAA6B1D31D3DA71

#include <vector>

class Creature;

class SpectatorVec
{
	using Vec = std::vector<Creature*>;
	using Iterator = Vec::iterator;
	using ConstIterator = Vec::const_iterator;
public:
	SpectatorVec() {
		vec.reserve(32);
	}

	void addSpectators(const SpectatorVec& spectators) {
		for (Creature* spectator : spectators.vec) {
			auto it = std::find(begin(), end(), spectator);
			if (it != end()) {
				continue;
			}
			vec.emplace_back(spectator);
		}
	}

	void erase(Creature* spectator) {
		auto it = std::find(begin(), end(), spectator);
		if (it == end()) {
			return;
		}
		std::iter_swap(it, end());
		vec.pop_back();
	}

	inline size_t size() const { return vec.size(); }
	inline bool empty() const { return vec.empty(); }
	inline Iterator begin() { return vec.begin(); }
	inline ConstIterator begin() const { return vec.begin(); }
	inline ConstIterator cbegin() const { return vec.cbegin(); }
	inline Iterator end() { return vec.end(); }
	inline ConstIterator end() const { return vec.end(); }
	inline ConstIterator cend() const { return vec.cend(); }
	inline void emplace_back(Creature* c) { vec.emplace_back(c); }

	template<class InputIterator>
	inline void insert(Iterator pos, InputIterator first, InputIterator last) { vec.insert(pos, first, last); }

private:
	Vec vec;
};

#endif
