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

#ifndef __OTSERV_OTTHREAD_H__
#define __OTSERV_OTTHREAD_H__

#include <stdint.h>
#include "logger.h"

#include <list>
#include <vector>
#include <algorithm>

typedef std::vector< std::pair<uint32_t, uint32_t> > IPList;

#ifdef WIN32
#ifdef __WIN_LOW_FRAG_HEAP__
#define _WIN32_WINNT 0x0501
#endif
#include <winsock2.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/timeb.h>

#define WIN32_LEAN_AND_MEAN

inline int64_t OTSYS_TIME()
{
	_timeb t;
	_ftime(&t);
	return int64_t(t.millitm) + int64_t(t.time) * 1000;
}

#else
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

inline int64_t OTSYS_TIME()
{
	timeb t;
	ftime(&t);
	return ((int64_t)t.millitm) + ((int64_t)t.time) * 1000;
}

#endif
#endif // #ifndef __OTSYSTEM_H__
