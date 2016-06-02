/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_LEXICALCAST_H_F4679829FC8ADBB3CD686CE9D0284DC5
#define FS_LEXICALCAST_H_F4679829FC8ADBB3CD686CE9D0284DC5

/* fallback */
template<class T>
T lexical_cast(const char* str)
{
	static thread_local std::istringstream ss; /* reusing has severe (positive) impact on performance */
	T value;
	ss.str(str);
	ss >> value;
	ss.clear();
	return value;
}

/* trivial conversion */
template<>
inline std::string lexical_cast<std::string>(const char* str)
{
	return str;
}

/* conversions that exist in stl */
template<>
inline float lexical_cast<float>(const char* str)
{
	return std::strtof(str, nullptr);
}

template<>
inline long lexical_cast<long>(const char* str)
{
	return std::strtol(str, nullptr, 0);
}

template<>
inline long long lexical_cast<long long>(const char* str)
{
	return std::strtoll(str, nullptr, 0);
}

template<>
inline unsigned long lexical_cast<unsigned long>(const char* str)
{
	return std::strtoul(str, nullptr, 0);
}

template<>
inline unsigned long long lexical_cast<unsigned long long>(const char* str)
{
	return std::strtoull(str, nullptr, 0);
}

/* conversions that need to be truncated */
template<>
inline short lexical_cast<short>(const char* str)
{
	return static_cast<short>(lexical_cast<long>(str));
}

template<>
inline int lexical_cast<int>(const char* str)
{
	return static_cast<int>(lexical_cast<long>(str));
}

template<>
inline unsigned short lexical_cast<unsigned short>(const char* str)
{
	return static_cast<unsigned short>(lexical_cast<unsigned long>(str));
}

template<>
inline unsigned int lexical_cast<unsigned int>(const char* str)
{
	return static_cast<unsigned int>(lexical_cast<unsigned long>(str));
}

#endif
