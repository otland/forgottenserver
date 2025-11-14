// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SPECTATORS_H
#define FS_SPECTATORS_H

class Creature;

class SpectatorVec
{
public:
	SpectatorVec() = default;

	void addSpectators(const SpectatorVec& spectators)
	{
		for (const auto& spectator : spectators.vec) {
			vec.emplace(spectator);
		}
	}

	void erase(const std::shared_ptr<Creature>& spectator) { vec.erase(spectator); }

	size_t size() const { return vec.size(); }
	bool empty() const { return vec.empty(); }
	auto begin() { return vec.begin(); }
	const auto begin() const { return vec.begin(); }
	auto end() { return vec.end(); }
	const auto end() const { return vec.end(); }
	void emplace(const std::shared_ptr<Creature>& creature) { vec.emplace(creature); }

private:
	boost::container::flat_set<std::shared_ptr<Creature>, std::owner_less<std::shared_ptr<Creature>>> vec;
};

#endif // FS_SPECTATORS_H
