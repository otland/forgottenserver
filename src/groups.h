/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_GROUPS_H_EE39438337D148E1983FB79D936DD8F3
#define FS_GROUPS_H_EE39438337D148E1983FB79D936DD8F3

struct Group {
	std::string name;
	uint64_t flags;
	uint32_t id;
	uint32_t maxDepotItems;
	uint32_t maxVipEntries;
	bool access;
};

class Groups {
	public:
		static Groups* getInstance() {
			static Groups instance;
			return &instance;
		}

		bool load();

		Group* getGroup(uint32_t id);

	private:
		std::vector<Group> groups;
};

#endif
