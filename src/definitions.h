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

#ifndef __OTSERV_DEFINITIONS_H__
#define __OTSERV_DEFINITIONS_H__

#define STATUS_SERVER_NAME "The Forgotten Server"
#define STATUS_SERVER_PROTOCOL "10.1"
#define STATUS_SERVER_VERSION "1.0"
#define STATUS_SERVER_DEVELOPERS "Mark Samman"

#define CLIENT_VERSION_MIN 1010
#define CLIENT_VERSION_MAX 1010
#define CLIENT_VERSION_STR "10.1"

#ifndef __FUNCTION__
#define	__FUNCTION__ __func__
#endif

#include <stdint.h>
#include <assert.h>
#include <string.h>

#ifndef WIN32
#ifdef _WIN32
#define WIN32
#endif
#endif

#include <list>
#include <vector>
#include <algorithm>
#include <sys/timeb.h>

typedef std::vector< std::pair<uint32_t, uint32_t> > IPList;

#ifdef WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif

#define WIN32_LEAN_AND_MEAN

#ifdef _MSC_VER
#ifdef NDEBUG
#define _SECURE_SCL 0
#define HAS_ITERATOR_DEBUGGING 0
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#define ATOI64 _atoi64

#pragma warning(disable:4100) // unused parameters
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4244) // 'argument' : conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4250) // 'class1' : inherits 'class2::member' via dominance
#pragma warning(disable:4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

//Windows 2000	0x0500
//Windows XP	0x0501
//Windows 2003	0x0502
//Windows Vista	0x0600
#define _WIN32_WINNT 0x0501
#endif

inline int64_t OTSYS_TIME()
{
	timeb t;
	ftime(&t);
	return ((int64_t)t.millitm) + ((int64_t)t.time) * 1000;
}

#ifdef __GNUC__
#define ATOI64 atoll
#endif

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#endif
