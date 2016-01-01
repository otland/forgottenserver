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

#ifndef FS_PUGICAST_H_07810DF7954D411EB14A16C3ED2A7548
#define FS_PUGICAST_H_07810DF7954D411EB14A16C3ED2A7548

#include <boost/lexical_cast.hpp>

namespace pugi {
	template<typename T>
	T cast(const pugi::char_t* str)
	{
		T value;
		try {
			value = boost::lexical_cast<T>(str);
		} catch (boost::bad_lexical_cast&) {
			value = T();
		}
		return value;
	}
}

#endif
