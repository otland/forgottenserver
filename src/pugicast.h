#ifndef __PUGICAST_H__
#define __PUGICAST_H__

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
