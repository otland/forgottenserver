// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SPECTATORS_H
#define FS_SPECTATORS_H

#ifdef EXPERIMENTAL_SPECTATOR_SET
#include <boost/unordered/unordered_flat_set.hpp>
#endif

class Creature;

class SpectatorVec
{
#ifdef EXPERIMENTAL_SPECTATOR_SET
	using Vec = boost::unordered_flat_set<Creature*>;
#else
	using Vec = std::vector<Creature*>;
#endif

public:
#ifdef EXPERIMENTAL_SPECTATOR_SET
	SpectatorVec() = default;
#else
	SpectatorVec() { vec.reserve(32); }
#endif

	size_t size() const { return vec.size(); }
	bool empty() const { return vec.empty(); }
	decltype(auto) begin() { return vec.begin(); }
	decltype(auto) begin() const { return vec.begin(); }
	decltype(auto) end() { return vec.end(); }
	decltype(auto) end() const { return vec.end(); }

	void addSpectators(const SpectatorVec& spectators)
	{
#ifdef EXPERIMENTAL_SPECTATOR_SET
		vec.insert(spectators.begin(), spectators.end());
#else
		for (Creature* spectator : spectators.vec) {
			auto it = std::find(vec.begin(), vec.end(), spectator);
			if (it != end()) {
				continue;
			}

			vec.push_back(spectator);
		}
#endif
	}

	void erase(Creature* spectator)
	{
#ifdef EXPERIMENTAL_SPECTATOR_SET
		vec.erase(spectator);
#else
		auto it = std::find(vec.begin(), vec.end(), spectator);
		if (it == end()) {
			return;
		}
		std::iter_swap(it, end() - 1);
		vec.pop_back();
#endif
	}

#ifdef EXPERIMENTAL_SPECTATOR_SET
	void add(Creature* c) { vec.insert(c); }
#else
	void add(Creature* c) { vec.push_back(c); }
#endif

private:
	Vec vec;
};

#endif // FS_SPECTATORS_H
