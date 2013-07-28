//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTSERV_BAN_H__
#define __OTSERV_BAN_H__

#include <boost/thread/recursive_mutex.hpp>
#include <list>
#include "player.h"

struct BanInfo {
	std::string bannedBy;
	std::string reason;
	time_t expiresAt;
};

struct ConnectBlock {
	uint64_t lastAttempt;
	uint64_t blockTime;
	uint32_t count;
};

typedef std::map<uint32_t, ConnectBlock > IpConnectMap;

class Ban
{
	public:
		bool acceptConnection(uint32_t clientip);

	protected:
		IpConnectMap ipConnectMap;

		mutable boost::recursive_mutex banLock;
};

class IOBan
{
	public:
		static IOBan* getInstance() {
			static IOBan instance;
			return &instance;
		}

		bool isAccountBanned(uint32_t accountId, BanInfo& banInfo);
		bool isIpBanned(uint32_t ip, BanInfo& banInfo);
		bool isPlayerNamelocked(uint32_t playerId);
};

#endif
