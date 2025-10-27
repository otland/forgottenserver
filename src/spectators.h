// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SPECTATORS_H
#define FS_SPECTATORS_H

class Creature;

class SpectatorVec
{
	using Vec = std::vector<std::shared_ptr<Creature>>;
	using Iterator = Vec::iterator;
	using ConstIterator = Vec::const_iterator;

public:
	SpectatorVec() { vec.reserve(32); }

	void addSpectators(const SpectatorVec& spectators)
	{
		for (const auto& spectator : spectators.vec) {
			auto it = std::find(vec.begin(), vec.end(), spectator);
			if (it != end()) {
				continue;
			}
			vec.emplace_back(spectator);
		}
	}

	void erase(const std::shared_ptr<Creature>& spectator)
	{
		auto it = std::find(vec.begin(), vec.end(), spectator);
		if (it == end()) {
			return;
		}
		std::iter_swap(it, end() - 1);
		vec.pop_back();
	}

	size_t size() const { return vec.size(); }
	bool empty() const { return vec.empty(); }
	Iterator begin() { return vec.begin(); }
	ConstIterator begin() const { return vec.begin(); }
	Iterator end() { return vec.end(); }
	ConstIterator end() const { return vec.end(); }
	void emplace_back(std::shared_ptr<Creature> c) { vec.emplace_back(std::move(c)); }

private:
	Vec vec;
};

#endif // FS_SPECTATORS_H
