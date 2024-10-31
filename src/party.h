// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PARTY_H
#define FS_PARTY_H

class Player;

class Party
{
public:
	explicit Party(Player* leader) : leader(leader) { leader->setParty(this); }

	Player* leader;

	std::vector<Player*> members;
	std::vector<Player*> invitees;

	struct
	{
		bool active = false;
		bool enabled = false;
		std::map<uint32_t, int64_t> attackTicks;
	} sharedExperience;
};

#endif // FS_PARTY_H
