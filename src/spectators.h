// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SPECTATORS_H
#define FS_SPECTATORS_H

class Creature;

class Spectators final
{
public:
	Spectators() { spectators.reserve(32); }

	void insert(Creature* spectator)
	{
		auto it = std::find(spectators.begin(), spectators.end(), spectator);
		if (it == spectators.end()) {
			spectators.insert(it, spectator);
		}
	}

	void insert(Spectators& range)
	{
		for (Creature* spectator : range.spectators) {
			insert(spectator);
		}
	}
	void insert(const Spectators& range)
	{
		for (Creature* spectator : range.spectators) {
			insert(spectator);
		}
	}

	void erase(Creature* spectator)
	{
		auto it = std::find(spectators.begin(), spectators.end(), spectator);
		if (it == spectators.end()) {
			return;
		}

		std::iter_swap(it, spectators.end() - 1);
		spectators.pop_back();
	}

	size_t size() const { return spectators.size(); }

	bool empty() const { return spectators.empty(); }

	void emplace_back(Creature* c) { spectators.emplace_back(c); }

	decltype(auto) begin() { return spectators.begin(); }
	decltype(auto) begin() const { return spectators.begin(); }

	decltype(auto) end() { return spectators.end(); }
	decltype(auto) end() const { return spectators.end(); }

private:
	std::vector<Creature*> spectators;
};

#endif // FS_SPECTATORS_H
