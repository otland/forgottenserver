#pragma once

class Creature;
class Item;

namespace tfs::lua {

void setMetatable(lua_State* L, int32_t index, std::string_view name);
void setWeakMetatable(lua_State* L, int32_t index, const std::string& name);

void setItemMetatable(lua_State* L, int32_t index, const std::shared_ptr<const Item>& item);
void setCreatureMetatable(lua_State* L, int32_t index, const std::shared_ptr<const Creature>& creature);

} // namespace tfs::lua
