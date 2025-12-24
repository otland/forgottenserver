#include "../../otpch.h"

#include "../../talkaction.h"

#include "../../script.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Scripts* g_scripts;
extern TalkActions* g_talkActions;

namespace {

int luaCreateTalkaction(lua_State* L)
{
	// TalkAction(words)
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		tfs::lua::reportError(L, "TalkActions can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	TalkAction* talkAction = new TalkAction(tfs::lua::getScriptEnv()->getScriptInterface());
	for (int i = 2; i <= lua_gettop(L); i++) {
		talkAction->setWords(tfs::lua::getString(L, i));
	}
	talkAction->fromLua = true;
	tfs::lua::pushUserdata(L, talkAction);
	tfs::lua::setMetatable(L, -1, "TalkAction");
	return 1;
}

int luaTalkactionOnSay(lua_State* L)
{
	// talkAction:onSay(callback)
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		if (!talk->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTalkactionRegister(lua_State* L)
{
	// talkAction:register()
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		if (!talk->isScripted()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, g_talkActions->registerLuaEvent(talk));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTalkactionSeparator(lua_State* L)
{
	// talkAction:separator(sep)
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		talk->setSeparator(tfs::lua::getString(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTalkactionAccess(lua_State* L)
{
	// talkAction:access(needAccess = false)
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		talk->setNeedAccess(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTalkactionAccountType(lua_State* L)
{
	// talkAction:accountType(AccountType_t = ACCOUNT_TYPE_NORMAL)
	TalkAction* talk = tfs::lua::getUserdata<TalkAction>(L, 1);
	if (talk) {
		talk->setRequiredAccountType(tfs::lua::getNumber<AccountType_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerTalkAction(LuaScriptInterface& lsi)
{
	lsi.registerClass("TalkAction", "", luaCreateTalkaction);
	lsi.registerMethod("TalkAction", "onSay", luaTalkactionOnSay);
	lsi.registerMethod("TalkAction", "register", luaTalkactionRegister);
	lsi.registerMethod("TalkAction", "separator", luaTalkactionSeparator);
	lsi.registerMethod("TalkAction", "access", luaTalkactionAccess);
	lsi.registerMethod("TalkAction", "accountType", luaTalkactionAccountType);
}
