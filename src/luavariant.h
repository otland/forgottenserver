#ifndef FS_LUAVARIANT_H
#define FS_LUAVARIANT_H

#include <variant>

using LuaVariant = std::variant<uint32_t, Position, Position, std::string>;

enum LuaVariantType_t {
	VARIANT_NUMBER = 0,
	VARIANT_POSITION = 1,
	VARIANT_TARGETPOSITION = 2,
	VARIANT_STRING = 3,

	VARIANT_NONE = std::variant_npos,
};

#endif
