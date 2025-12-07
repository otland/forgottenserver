#ifndef FS_LUAVARIANT_H
#define FS_LUAVARIANT_H

#include "position.h"

enum LuaVariantType_t
{
	VARIANT_NUMBER = 0,
	VARIANT_POSITION = 1,
	VARIANT_TARGETPOSITION = 2,
	VARIANT_STRING = 3,

	VARIANT_NONE = std::variant_npos,
};

class LuaVariant
{
public:
	uint32_t getNumber() const { return std::get<VARIANT_NUMBER>(variant); }
	const Position& getPosition() const { return std::get<VARIANT_POSITION>(variant); }
	const Position& getTargetPosition() const { return std::get<VARIANT_TARGETPOSITION>(variant); }
	const std::string& getString() const { return std::get<VARIANT_STRING>(variant); }

	bool isNumber() const { return variant.index() == VARIANT_NUMBER; }
	bool isPosition() const { return variant.index() == VARIANT_POSITION; }
	bool isTargetPosition() const { return variant.index() == VARIANT_TARGETPOSITION; }
	bool isString() const { return variant.index() == VARIANT_STRING; }

	void setNumber(uint32_t value) { variant.emplace<VARIANT_NUMBER>(value); }
	void setPosition(const Position& value) { variant.emplace<VARIANT_POSITION>(value); }
	void setTargetPosition(const Position& value) { variant.emplace<VARIANT_TARGETPOSITION>(value); }
	void setString(const std::string& value) { variant.emplace<VARIANT_STRING>(value); }

	auto type() const { return static_cast<LuaVariantType_t>(variant.index()); }

private:
	std::variant<uint32_t, Position, Position, std::string> variant;
};

#endif // FS_LUAVARIANT_H
