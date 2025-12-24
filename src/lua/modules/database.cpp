#include "../../otpch.h"

#include "../../databasemanager.h"
#include "../../databasetasks.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern DatabaseTasks g_databaseTasks;
extern LuaEnvironment g_luaEnvironment;

namespace {

int luaDBInsertCreate(lua_State* L)
{
	// DBInsert(query)
	if (lua_isstring(L, 2)) {
		tfs::lua::pushUserdata(L, new DBInsert(tfs::lua::getString(L, 2)));
		tfs::lua::setMetatable(L, -1, "DBInsert");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaDBInsertAddRow(lua_State* L)
{
	// insert:addRow(row)
	DBInsert* insert = tfs::lua::getUserdata<DBInsert>(L, 1);
	if (insert) {
		tfs::lua::pushBoolean(L, insert->addRow(tfs::lua::getString(L, 2)));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaDBInsertExecute(lua_State* L)
{
	// insert:execute()
	DBInsert* insert = tfs::lua::getUserdata<DBInsert>(L, 1);
	if (insert) {
		tfs::lua::pushBoolean(L, insert->execute());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaDBInsertDelete(lua_State* L)
{
	DBInsert** insertPtr = tfs::lua::getRawUserdata<DBInsert>(L, 1);
	if (insertPtr && *insertPtr) {
		delete *insertPtr;
		*insertPtr = nullptr;
	}
	return 0;
}

int luaDBTransactionCreate(lua_State* L)
{
	// DBTransaction()
	tfs::lua::pushUserdata(L, new DBTransaction);
	tfs::lua::setMetatable(L, -1, "DBTransaction");
	return 1;
}

int luaDBTransactionBegin(lua_State* L)
{
	// transaction:begin()
	DBTransaction* transaction = tfs::lua::getUserdata<DBTransaction>(L, 1);
	if (transaction) {
		tfs::lua::pushBoolean(L, transaction->begin());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaDBTransactionCommit(lua_State* L)
{
	// transaction:commit()
	DBTransaction* transaction = tfs::lua::getUserdata<DBTransaction>(L, 1);
	if (transaction) {
		tfs::lua::pushBoolean(L, transaction->commit());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaDBTransactionDelete(lua_State* L)
{
	DBTransaction** transactionPtr = tfs::lua::getRawUserdata<DBTransaction>(L, 1);
	if (transactionPtr && *transactionPtr) {
		delete *transactionPtr;
		*transactionPtr = nullptr;
	}
	return 0;
}

int luaDatabaseExecute(lua_State* L)
{
	// db.query(query)
	tfs::lua::pushBoolean(L, Database::getInstance().executeQuery(tfs::lua::getString(L, -1)));
	return 1;
}

int luaDatabaseAsyncExecute(lua_State* L)
{
	// db.asyncQuery(query, callback)
	std::function<void(const std::shared_ptr<DBResult>&, bool)> callback;
	if (lua_gettop(L) > 1) {
		int32_t ref = luaL_ref(L, LUA_REGISTRYINDEX);
		auto scriptId = tfs::lua::getScriptEnv()->getScriptId();
		callback = [ref, scriptId](const std::shared_ptr<DBResult>&, bool success) {
			lua_State* L = g_luaEnvironment.getLuaState();
			if (!L) {
				return;
			}

			if (!tfs::lua::reserveScriptEnv()) {
				luaL_unref(L, LUA_REGISTRYINDEX, ref);
				return;
			}

			lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
			tfs::lua::pushBoolean(L, success);
			auto env = tfs::lua::getScriptEnv();
			env->setScriptId(scriptId, &g_luaEnvironment);
			g_luaEnvironment.callFunction(1);

			luaL_unref(L, LUA_REGISTRYINDEX, ref);
		};
	}
	g_databaseTasks.addTask(tfs::lua::getString(L, -1), callback);
	return 0;
}

int luaDatabaseStoreQuery(lua_State* L)
{
	// db.storeQuery(query)
	if (const auto& result = Database::getInstance().storeQuery(tfs::lua::getString(L, -1))) {
		tfs::lua::pushNumber(L, tfs::lua::addResult(result));
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int luaDatabaseAsyncStoreQuery(lua_State* L)
{
	// db.asyncStoreQuery(query, callback)
	std::function<void(const std::shared_ptr<DBResult>&, bool)> callback;
	if (lua_gettop(L) > 1) {
		int32_t ref = luaL_ref(L, LUA_REGISTRYINDEX);
		auto scriptId = tfs::lua::getScriptEnv()->getScriptId();
		callback = [ref, scriptId](const std::shared_ptr<DBResult>& result, bool) {
			lua_State* L = g_luaEnvironment.getLuaState();
			if (!L) {
				return;
			}

			if (!tfs::lua::reserveScriptEnv()) {
				luaL_unref(L, LUA_REGISTRYINDEX, ref);
				return;
			}

			lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
			if (result) {
				tfs::lua::pushNumber(L, tfs::lua::addResult(result));
			} else {
				tfs::lua::pushBoolean(L, false);
			}
			auto env = tfs::lua::getScriptEnv();
			env->setScriptId(scriptId, &g_luaEnvironment);
			g_luaEnvironment.callFunction(1);

			luaL_unref(L, LUA_REGISTRYINDEX, ref);
		};
	}
	g_databaseTasks.addTask(tfs::lua::getString(L, -1), callback, true);
	return 0;
}

int luaDatabaseEscapeString(lua_State* L)
{
	// db.escapeString(s)
	tfs::lua::pushString(L, Database::getInstance().escapeString(tfs::lua::getString(L, -1)));
	return 1;
}

int luaDatabaseEscapeBlob(lua_State* L)
{
	// db.escapeBlob(s, length)
	uint32_t length = tfs::lua::getNumber<uint32_t>(L, 2);
	tfs::lua::pushString(L, Database::getInstance().escapeBlob(tfs::lua::getString(L, 1).data(), length));
	return 1;
}

int luaDatabaseLastInsertId(lua_State* L)
{
	// db.lastInsertId()
	tfs::lua::pushNumber(L, Database::getInstance().getLastInsertId());
	return 1;
}

int luaDatabaseTableExists(lua_State* L)
{
	// db.tableExists(tableName)
	tfs::lua::pushBoolean(L, DatabaseManager::tableExists(tfs::lua::getString(L, -1)));
	return 1;
}

static const luaL_Reg luaDatabaseTable[] = {{"query", luaDatabaseExecute},
                                            {"asyncQuery", luaDatabaseAsyncExecute},
                                            {"storeQuery", luaDatabaseStoreQuery},
                                            {"asyncStoreQuery", luaDatabaseAsyncStoreQuery},
                                            {"escapeString", luaDatabaseEscapeString},
                                            {"escapeBlob", luaDatabaseEscapeBlob},
                                            {"lastInsertId", luaDatabaseLastInsertId},
                                            {"tableExists", luaDatabaseTableExists},
                                            {nullptr, nullptr}};

int luaResultGetNumber(lua_State* L)
{
	const auto& result = tfs::lua::getResultByID(tfs::lua::getNumber<uint32_t>(L, 1));
	if (!result) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const std::string& s = tfs::lua::getString(L, 2);
	tfs::lua::pushNumber(L, result->getNumber<int64_t>(s));
	return 1;
}

int luaResultGetString(lua_State* L)
{
	const auto& result = tfs::lua::getResultByID(tfs::lua::getNumber<uint32_t>(L, 1));
	if (!result) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const std::string& s = tfs::lua::getString(L, 2);
	tfs::lua::pushString(L, result->getString(s));
	return 1;
}

int luaResultGetStream(lua_State* L)
{
	const auto& result = tfs::lua::getResultByID(tfs::lua::getNumber<uint32_t>(L, 1));
	if (!result) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	auto stream = result->getString(tfs::lua::getString(L, 2));
	lua_pushlstring(L, stream.data(), stream.size());
	tfs::lua::pushNumber(L, stream.size());
	return 2;
}

int luaResultNext(lua_State* L)
{
	const auto& result = tfs::lua::getResultByID(tfs::lua::getNumber<uint32_t>(L, -1));
	if (!result) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	tfs::lua::pushBoolean(L, result->next());
	return 1;
}

int luaResultFree(lua_State* L)
{
	tfs::lua::pushBoolean(L, tfs::lua::removeResult(tfs::lua::getNumber<uint32_t>(L, -1)));
	return 1;
}

static const luaL_Reg luaResultTable[] = {{"getNumber", luaResultGetNumber}, {"getString", luaResultGetString},
                                          {"getStream", luaResultGetStream}, {"next", luaResultNext},
                                          {"free", luaResultFree},           {nullptr, nullptr}};

} // namespace

void tfs::lua::registerDatabase(LuaScriptInterface& lsi)
{
	// db table
	luaL_newlib(lsi.getLuaState(), luaDatabaseTable);
	lua_pushvalue(lsi.getLuaState(), -1);
	lua_setglobal(lsi.getLuaState(), "db");
	lua_pop(lsi.getLuaState(), 1);

	// result table
	luaL_newlib(lsi.getLuaState(), luaResultTable);
	lua_pushvalue(lsi.getLuaState(), -1);
	lua_setglobal(lsi.getLuaState(), "result");
	lua_pop(lsi.getLuaState(), 1);

	lsi.registerClass("DBInsert", "", luaDBInsertCreate);
	lsi.registerMetaMethod("DBInsert", "__gc", luaDBInsertDelete);

	lsi.registerMethod("DBInsert", "addRow", luaDBInsertAddRow);
	lsi.registerMethod("DBInsert", "execute", luaDBInsertExecute);

	lsi.registerClass("DBTransaction", "", luaDBTransactionCreate);
	lsi.registerMetaMethod("DBTransaction", "__eq", tfs::lua::luaUserdataCompare);
	lsi.registerMetaMethod("DBTransaction", "__gc", luaDBTransactionDelete);

	lsi.registerMethod("DBTransaction", "begin", luaDBTransactionBegin);
	lsi.registerMethod("DBTransaction", "commit", luaDBTransactionCommit);
	lsi.registerMethod("DBTransaction", "rollback", luaDBTransactionDelete);
}
