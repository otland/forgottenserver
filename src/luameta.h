#ifndef TFS_LUAMETA
#define TFS_LUAMETA

class Creature;
class Item;

namespace tfs::lua {

void setMetatable(lua_State* L, int32_t index, const std::string& name);
void setWeakMetatable(lua_State* L, int32_t index, const std::string& name);

void setItemMetatable(lua_State* L, int32_t index, const Item* item);
void setCreatureMetatable(lua_State* L, int32_t index, const Creature* creature);

} // namespace tfs::lua

#endif
