#ifndef FS_LUAVARIANT_H
#define FS_LUAVARIANT_H

#include <variant>

enum LuaVariantType_t {
	VARIANT_NUMBER = 0,
	VARIANT_POSITION = 1,
	VARIANT_TARGETPOSITION = 2,
	VARIANT_STRING = 3,

	VARIANT_NONE = std::variant_npos,
};

class LuaVariant {
public:
	uint32_t getNumber() const { return std::get<VARIANT_NUMBER>(v); }
	const Position& getPosition() const { return std::get<VARIANT_POSITION>(v); }
	const Position& getTargetPosition() const { return std::get<VARIANT_TARGETPOSITION>(v); }
	const std::string& getString() const { return std::get<VARIANT_STRING>(v); }

	bool isNumber() const { return v.index() == VARIANT_NUMBER; }
	bool isPosition() const { return v.index() == VARIANT_POSITION; }
	bool isTargetPosition() const { return v.index() == VARIANT_TARGETPOSITION; }
	bool isString() const { return v.index() == VARIANT_STRING; }

	void setNumber(uint32_t value) { v.emplace<VARIANT_NUMBER>(value); }
	void setPosition(const Position& value) { v.emplace<VARIANT_POSITION>(value); }
	void setTargetPosition(const Position& value) { v.emplace<VARIANT_TARGETPOSITION>(value); }
	void setString(const std::string& value) { v.emplace<VARIANT_STRING>(value); }

	auto type() const { return static_cast<LuaVariantType_t>(v.index()); }

private:
	std::variant<uint32_t, Position, Position, std::string> v;
};

#endif
