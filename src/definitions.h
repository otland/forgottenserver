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
#define STATUS_SERVER_PROTOCOL "10.0"
#define STATUS_SERVER_VERSION "1.0"
#define STATUS_SERVER_DEVELOPERS "Mark Samman"

#define CLIENT_VERSION_MIN 972
#define CLIENT_VERSION_MAX 981
#define CLIENT_VERSION_STR "10.0"

enum passwordType_t {
	PASSWORD_TYPE_PLAIN = 0,
	PASSWORD_TYPE_MD5,
	PASSWORD_TYPE_SHA1
};

#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#endif

#ifndef __FUNCTION__
#define	__FUNCTION__ __func__
#endif

#ifdef XML_GCC_FREE
#define xmlFreeOTSERV(s)	free(s)
#else
#define xmlFreeOTSERV(s)	xmlFree(s)
#endif

#if defined(WIN32) || defined(_WIN32)
#if !defined(WINDOWS)
#define WINDOWS
#endif
#endif

#include <stdint.h>
#include <assert.h>
#include <string.h>

#ifdef WINDOWS
#include <io.h>
#include <process.h>
#include <direct.h>

#if defined(_MSC_VER) && defined(NDEBUG)
#define _SECURE_SCL 0
#define HAS_ITERATOR_DEBUGGING 0
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

//Windows 2000	0x0500
//Windows XP	0x0501
//Windows 2003	0x0502
//Windows Vista	0x0600
#define _WIN32_WINNT 0x0501

#ifdef __GNUC__
#if GCC_VERSION >= 40700
#include <unordered_map>
#include <unordered_set>
#define OTSERV_HASH_MAP std::unordered_map
#define OTSERV_HASH_SET std::unordered_set
#elif GCC_VERSION >= 40000
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#define OTSERV_HASH_MAP std::tr1::unordered_map
#define OTSERV_HASH_SET std::tr1::unordered_set
#else
#include <ext/hash_map>
#include <ext/hash_set>
#define OTSERV_HASH_MAP __gnu_cxx::hash_map
#define OTSERV_HASH_SET __gnu_cxx::hash_set
#endif
#include <time.h>

#define ATOI64 atoll

inline int strcasecmp(const char* s1, const char* s2)
{
	return ::_stricmp(s1, s2);
}

inline int strncasecmp(const char* s1, const char* s2, size_t n)
{
	return ::_strnicmp(s1, s2, n);
}
#else
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <unordered_map>
#include <unordered_set>
#include <limits>

inline int strcasecmp(const char* s1, const char* s2)
{
	return ::_stricmp(s1, s2);
}

inline int strncasecmp(const char* s1, const char* s2, size_t n)
{
	return ::_strnicmp(s1, s2, n);
}

#define OTSERV_HASH_MAP std::unordered_map
#define OTSERV_HASH_SET std::unordered_set
#define ATOI64 _atoi64

#pragma warning(disable:4786) // msvc too long debug names in stl
#pragma warning(disable:4250) // 'class1' : inherits 'class2::member' via dominance
#pragma warning(disable:4244) //'argument' : conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4267) //'var' : conversion from 'size_t' to 'type', possible loss of data
#endif
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#include <unordered_map>
#include <unordered_set>
#define OTSERV_HASH_MAP std::unordered_map
#define OTSERV_HASH_SET std::unordered_set
#elif GCC_VERSION >= 40000
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#define OTSERV_HASH_MAP std::tr1::unordered_map
#define OTSERV_HASH_SET std::tr1::unordered_set
#else
#include <ext/hash_map>
#include <ext/hash_set>
#define OTSERV_HASH_MAP __gnu_cxx::hash_map
#define OTSERV_HASH_SET __gnu_cxx::hash_set
#endif

#define ATOI64 atoll
#endif

#endif
