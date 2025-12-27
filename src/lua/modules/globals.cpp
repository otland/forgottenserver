#include "../../otpch.h"

#include "../../configmanager.h"
#include "../../const.h"
#include "../../enums.h"
#include "../../globalevent.h"
#include "../../script.h"
#include "../../tools.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

namespace {

int luaIsType(lua_State* L)
{
	// isType(derived, base)
	lua_getmetatable(L, -2);
	lua_getmetatable(L, -2);

	lua_rawgeti(L, -2, 'p');
	uint_fast8_t parentsB = tfs::lua::getNumber<uint_fast8_t>(L, 1);

	lua_rawgeti(L, -3, 'h');
	size_t hashB = tfs::lua::getNumber<size_t>(L, 1);

	lua_rawgeti(L, -3, 'p');
	uint_fast8_t parentsA = tfs::lua::getNumber<uint_fast8_t>(L, 1);
	for (uint_fast8_t i = parentsA; i < parentsB; ++i) {
		lua_getfield(L, -3, "__index");
		lua_replace(L, -4);
	}

	lua_rawgeti(L, -4, 'h');
	size_t hashA = tfs::lua::getNumber<size_t>(L, 1);

	tfs::lua::pushBoolean(L, hashA == hashB);
	return 1;
}

int luaRawGetMetatable(lua_State* L)
{
	// rawgetmetatable(metatableName)
	luaL_getmetatable(L, tfs::lua::getString(L, 1).data());
	return 1;
}

} // namespace

void tfs::lua::registerGlobals(LuaScriptInterface& lsi)
{
	// Enums
	registerEnum(lsi, ACCOUNT_TYPE_NORMAL);
	registerEnum(lsi, ACCOUNT_TYPE_TUTOR);
	registerEnum(lsi, ACCOUNT_TYPE_SENIORTUTOR);
	registerEnum(lsi, ACCOUNT_TYPE_GAMEMASTER);
	registerEnum(lsi, ACCOUNT_TYPE_COMMUNITYMANAGER);
	registerEnum(lsi, ACCOUNT_TYPE_GOD);

	registerEnum(lsi, CALLBACK_PARAM_LEVELMAGICVALUE);
	registerEnum(lsi, CALLBACK_PARAM_SKILLVALUE);
	registerEnum(lsi, CALLBACK_PARAM_TARGETTILE);
	registerEnum(lsi, CALLBACK_PARAM_TARGETCREATURE);

	registerEnum(lsi, MESSAGE_STATUS_DEFAULT);
	registerEnum(lsi, MESSAGE_STATUS_WARNING);
	registerEnum(lsi, MESSAGE_EVENT_ADVANCE);
	registerEnum(lsi, MESSAGE_STATUS_WARNING2);
	registerEnum(lsi, MESSAGE_STATUS_SMALL);
	registerEnum(lsi, MESSAGE_INFO_DESCR);
	registerEnum(lsi, MESSAGE_DAMAGE_DEALT);
	registerEnum(lsi, MESSAGE_DAMAGE_RECEIVED);
	registerEnum(lsi, MESSAGE_HEALED);
	registerEnum(lsi, MESSAGE_EXPERIENCE);
	registerEnum(lsi, MESSAGE_DAMAGE_OTHERS);
	registerEnum(lsi, MESSAGE_HEALED_OTHERS);
	registerEnum(lsi, MESSAGE_EXPERIENCE_OTHERS);
	registerEnum(lsi, MESSAGE_EVENT_DEFAULT);
	registerEnum(lsi, MESSAGE_LOOT);
	registerEnum(lsi, MESSAGE_TRADE);
	registerEnum(lsi, MESSAGE_GUILD);
	registerEnum(lsi, MESSAGE_PARTY_MANAGEMENT);
	registerEnum(lsi, MESSAGE_PARTY);
	registerEnum(lsi, MESSAGE_REPORT);
	registerEnum(lsi, MESSAGE_HOTKEY_PRESSED);
	registerEnum(lsi, MESSAGE_MARKET);
	registerEnum(lsi, MESSAGE_BEYOND_LAST);
	registerEnum(lsi, MESSAGE_TOURNAMENT_INFO);
	registerEnum(lsi, MESSAGE_ATTENTION);
	registerEnum(lsi, MESSAGE_BOOSTED_CREATURE);
	registerEnum(lsi, MESSAGE_OFFLINE_TRAINING);
	registerEnum(lsi, MESSAGE_TRANSACTION);

	registerEnum(lsi, CREATURETYPE_PLAYER);
	registerEnum(lsi, CREATURETYPE_MONSTER);
	registerEnum(lsi, CREATURETYPE_NPC);
	registerEnum(lsi, CREATURETYPE_SUMMON_OWN);
	registerEnum(lsi, CREATURETYPE_SUMMON_OTHERS);

	registerEnum(lsi, CLIENTOS_LINUX);
	registerEnum(lsi, CLIENTOS_WINDOWS);
	registerEnum(lsi, CLIENTOS_FLASH);
	registerEnum(lsi, CLIENTOS_OTCLIENT_LINUX);
	registerEnum(lsi, CLIENTOS_OTCLIENT_WINDOWS);
	registerEnum(lsi, CLIENTOS_OTCLIENT_MAC);

	registerEnum(lsi, WIELDINFO_NONE);
	registerEnum(lsi, WIELDINFO_LEVEL);
	registerEnum(lsi, WIELDINFO_MAGLV);
	registerEnum(lsi, WIELDINFO_VOCREQ);
	registerEnum(lsi, WIELDINFO_PREMIUM);

	registerEnum(lsi, PlayerFlag_CannotUseCombat);
	registerEnum(lsi, PlayerFlag_CannotAttackPlayer);
	registerEnum(lsi, PlayerFlag_CannotAttackMonster);
	registerEnum(lsi, PlayerFlag_CannotBeAttacked);
	registerEnum(lsi, PlayerFlag_CanConvinceAll);
	registerEnum(lsi, PlayerFlag_CanSummonAll);
	registerEnum(lsi, PlayerFlag_CanIllusionAll);
	registerEnum(lsi, PlayerFlag_CanSenseInvisibility);
	registerEnum(lsi, PlayerFlag_IgnoredByMonsters);
	registerEnum(lsi, PlayerFlag_NotGainInFight);
	registerEnum(lsi, PlayerFlag_HasInfiniteMana);
	registerEnum(lsi, PlayerFlag_HasInfiniteSoul);
	registerEnum(lsi, PlayerFlag_HasNoExhaustion);
	registerEnum(lsi, PlayerFlag_CannotUseSpells);
	registerEnum(lsi, PlayerFlag_CannotPickupItem);
	registerEnum(lsi, PlayerFlag_CanAlwaysLogin);
	registerEnum(lsi, PlayerFlag_CanBroadcast);
	registerEnum(lsi, PlayerFlag_CanEditHouses);
	registerEnum(lsi, PlayerFlag_CannotBeBanned);
	registerEnum(lsi, PlayerFlag_CannotBePushed);
	registerEnum(lsi, PlayerFlag_HasInfiniteCapacity);
	registerEnum(lsi, PlayerFlag_CanPushAllCreatures);
	registerEnum(lsi, PlayerFlag_CanTalkRedPrivate);
	registerEnum(lsi, PlayerFlag_CanTalkRedChannel);
	registerEnum(lsi, PlayerFlag_TalkOrangeHelpChannel);
	registerEnum(lsi, PlayerFlag_NotGainExperience);
	registerEnum(lsi, PlayerFlag_NotGainMana);
	registerEnum(lsi, PlayerFlag_NotGainHealth);
	registerEnum(lsi, PlayerFlag_NotGainSkill);
	registerEnum(lsi, PlayerFlag_SetMaxSpeed);
	registerEnum(lsi, PlayerFlag_SpecialVIP);
	registerEnum(lsi, PlayerFlag_NotGenerateLoot);
	registerEnum(lsi, PlayerFlag_IgnoreProtectionZone);
	registerEnum(lsi, PlayerFlag_IgnoreSpellCheck);
	registerEnum(lsi, PlayerFlag_IgnoreWeaponCheck);
	registerEnum(lsi, PlayerFlag_CannotBeMuted);
	registerEnum(lsi, PlayerFlag_IsAlwaysPremium);
	registerEnum(lsi, PlayerFlag_IgnoreYellCheck);
	registerEnum(lsi, PlayerFlag_IgnoreSendPrivateCheck);

	registerEnum(lsi, PODIUM_SHOW_PLATFORM);
	registerEnum(lsi, PODIUM_SHOW_OUTFIT);
	registerEnum(lsi, PODIUM_SHOW_MOUNT);

	registerEnum(lsi, REPORT_REASON_NAMEINAPPROPRIATE);
	registerEnum(lsi, REPORT_REASON_NAMEPOORFORMATTED);
	registerEnum(lsi, REPORT_REASON_NAMEADVERTISING);
	registerEnum(lsi, REPORT_REASON_NAMEUNFITTING);
	registerEnum(lsi, REPORT_REASON_NAMERULEVIOLATION);
	registerEnum(lsi, REPORT_REASON_INSULTINGSTATEMENT);
	registerEnum(lsi, REPORT_REASON_SPAMMING);
	registerEnum(lsi, REPORT_REASON_ADVERTISINGSTATEMENT);
	registerEnum(lsi, REPORT_REASON_UNFITTINGSTATEMENT);
	registerEnum(lsi, REPORT_REASON_LANGUAGESTATEMENT);
	registerEnum(lsi, REPORT_REASON_DISCLOSURE);
	registerEnum(lsi, REPORT_REASON_RULEVIOLATION);
	registerEnum(lsi, REPORT_REASON_STATEMENT_BUGABUSE);
	registerEnum(lsi, REPORT_REASON_UNOFFICIALSOFTWARE);
	registerEnum(lsi, REPORT_REASON_PRETENDING);
	registerEnum(lsi, REPORT_REASON_HARASSINGOWNERS);
	registerEnum(lsi, REPORT_REASON_FALSEINFO);
	registerEnum(lsi, REPORT_REASON_ACCOUNTSHARING);
	registerEnum(lsi, REPORT_REASON_STEALINGDATA);
	registerEnum(lsi, REPORT_REASON_SERVICEATTACKING);
	registerEnum(lsi, REPORT_REASON_SERVICEAGREEMENT);

	registerEnum(lsi, REPORT_TYPE_NAME);
	registerEnum(lsi, REPORT_TYPE_STATEMENT);
	registerEnum(lsi, REPORT_TYPE_BOT);

	registerEnum(lsi, TALKTYPE_SAY);
	registerEnum(lsi, TALKTYPE_WHISPER);
	registerEnum(lsi, TALKTYPE_YELL);
	registerEnum(lsi, TALKTYPE_PRIVATE_FROM);
	registerEnum(lsi, TALKTYPE_PRIVATE_TO);
	registerEnum(lsi, TALKTYPE_CHANNEL_Y);
	registerEnum(lsi, TALKTYPE_CHANNEL_O);
	registerEnum(lsi, TALKTYPE_SPELL);
	registerEnum(lsi, TALKTYPE_PRIVATE_NP);
	registerEnum(lsi, TALKTYPE_PRIVATE_NP_CONSOLE);
	registerEnum(lsi, TALKTYPE_PRIVATE_PN);
	registerEnum(lsi, TALKTYPE_BROADCAST);
	registerEnum(lsi, TALKTYPE_CHANNEL_R1);
	registerEnum(lsi, TALKTYPE_PRIVATE_RED_FROM);
	registerEnum(lsi, TALKTYPE_PRIVATE_RED_TO);
	registerEnum(lsi, TALKTYPE_MONSTER_SAY);
	registerEnum(lsi, TALKTYPE_MONSTER_YELL);
	registerEnum(lsi, TALKTYPE_POTION);

	registerEnum(lsi, TEXTCOLOR_BLUE);
	registerEnum(lsi, TEXTCOLOR_LIGHTGREEN);
	registerEnum(lsi, TEXTCOLOR_LIGHTBLUE);
	registerEnum(lsi, TEXTCOLOR_MAYABLUE);
	registerEnum(lsi, TEXTCOLOR_DARKRED);
	registerEnum(lsi, TEXTCOLOR_LIGHTGREY);
	registerEnum(lsi, TEXTCOLOR_SKYBLUE);
	registerEnum(lsi, TEXTCOLOR_PURPLE);
	registerEnum(lsi, TEXTCOLOR_ELECTRICPURPLE);
	registerEnum(lsi, TEXTCOLOR_RED);
	registerEnum(lsi, TEXTCOLOR_PASTELRED);
	registerEnum(lsi, TEXTCOLOR_ORANGE);
	registerEnum(lsi, TEXTCOLOR_YELLOW);
	registerEnum(lsi, TEXTCOLOR_WHITE_EXP);
	registerEnum(lsi, TEXTCOLOR_NONE);

	// Use with player:addMapMark
	registerEnum(lsi, MAPMARK_TICK);
	registerEnum(lsi, MAPMARK_QUESTION);
	registerEnum(lsi, MAPMARK_EXCLAMATION);
	registerEnum(lsi, MAPMARK_STAR);
	registerEnum(lsi, MAPMARK_CROSS);
	registerEnum(lsi, MAPMARK_TEMPLE);
	registerEnum(lsi, MAPMARK_KISS);
	registerEnum(lsi, MAPMARK_SHOVEL);
	registerEnum(lsi, MAPMARK_SWORD);
	registerEnum(lsi, MAPMARK_FLAG);
	registerEnum(lsi, MAPMARK_LOCK);
	registerEnum(lsi, MAPMARK_BAG);
	registerEnum(lsi, MAPMARK_SKULL);
	registerEnum(lsi, MAPMARK_DOLLAR);
	registerEnum(lsi, MAPMARK_REDNORTH);
	registerEnum(lsi, MAPMARK_REDSOUTH);
	registerEnum(lsi, MAPMARK_REDEAST);
	registerEnum(lsi, MAPMARK_REDWEST);
	registerEnum(lsi, MAPMARK_GREENNORTH);
	registerEnum(lsi, MAPMARK_GREENSOUTH);

	// Use with Game.getReturnMessage
	registerEnum(lsi, RETURNVALUE_NOERROR);
	registerEnum(lsi, RETURNVALUE_NOTPOSSIBLE);
	registerEnum(lsi, RETURNVALUE_NOTENOUGHROOM);
	registerEnum(lsi, RETURNVALUE_PLAYERISPZLOCKED);
	registerEnum(lsi, RETURNVALUE_PLAYERISNOTINVITED);
	registerEnum(lsi, RETURNVALUE_CANNOTTHROW);
	registerEnum(lsi, RETURNVALUE_THEREISNOWAY);
	registerEnum(lsi, RETURNVALUE_DESTINATIONOUTOFREACH);
	registerEnum(lsi, RETURNVALUE_CREATUREBLOCK);
	registerEnum(lsi, RETURNVALUE_NOTMOVEABLE);
	registerEnum(lsi, RETURNVALUE_DROPTWOHANDEDITEM);
	registerEnum(lsi, RETURNVALUE_BOTHHANDSNEEDTOBEFREE);
	registerEnum(lsi, RETURNVALUE_CANONLYUSEONEWEAPON);
	registerEnum(lsi, RETURNVALUE_NEEDEXCHANGE);
	registerEnum(lsi, RETURNVALUE_CANNOTBEDRESSED);
	registerEnum(lsi, RETURNVALUE_PUTTHISOBJECTINYOURHAND);
	registerEnum(lsi, RETURNVALUE_PUTTHISOBJECTINBOTHHANDS);
	registerEnum(lsi, RETURNVALUE_TOOFARAWAY);
	registerEnum(lsi, RETURNVALUE_FIRSTGODOWNSTAIRS);
	registerEnum(lsi, RETURNVALUE_FIRSTGOUPSTAIRS);
	registerEnum(lsi, RETURNVALUE_CONTAINERNOTENOUGHROOM);
	registerEnum(lsi, RETURNVALUE_NOTENOUGHCAPACITY);
	registerEnum(lsi, RETURNVALUE_CANNOTPICKUP);
	registerEnum(lsi, RETURNVALUE_THISISIMPOSSIBLE);
	registerEnum(lsi, RETURNVALUE_DEPOTISFULL);
	registerEnum(lsi, RETURNVALUE_CREATUREDOESNOTEXIST);
	registerEnum(lsi, RETURNVALUE_CANNOTUSETHISOBJECT);
	registerEnum(lsi, RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE);
	registerEnum(lsi, RETURNVALUE_NOTREQUIREDLEVELTOUSERUNE);
	registerEnum(lsi, RETURNVALUE_YOUAREALREADYTRADING);
	registerEnum(lsi, RETURNVALUE_THISPLAYERISALREADYTRADING);
	registerEnum(lsi, RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT);
	registerEnum(lsi, RETURNVALUE_DIRECTPLAYERSHOOT);
	registerEnum(lsi, RETURNVALUE_NOTENOUGHLEVEL);
	registerEnum(lsi, RETURNVALUE_NOTENOUGHMAGICLEVEL);
	registerEnum(lsi, RETURNVALUE_NOTENOUGHMANA);
	registerEnum(lsi, RETURNVALUE_NOTENOUGHSOUL);
	registerEnum(lsi, RETURNVALUE_YOUAREEXHAUSTED);
	registerEnum(lsi, RETURNVALUE_YOUCANNOTUSEOBJECTSTHATFAST);
	registerEnum(lsi, RETURNVALUE_PLAYERISNOTREACHABLE);
	registerEnum(lsi, RETURNVALUE_CANONLYUSETHISRUNEONCREATURES);
	registerEnum(lsi, RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE);
	registerEnum(lsi, RETURNVALUE_YOUMAYNOTATTACKTHISPLAYER);
	registerEnum(lsi, RETURNVALUE_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE);
	registerEnum(lsi, RETURNVALUE_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE);
	registerEnum(lsi, RETURNVALUE_YOUMAYNOTATTACKTHISCREATURE);
	registerEnum(lsi, RETURNVALUE_YOUCANONLYUSEITONCREATURES);
	registerEnum(lsi, RETURNVALUE_CREATUREISNOTREACHABLE);
	registerEnum(lsi, RETURNVALUE_TURNSECUREMODETOATTACKUNMARKEDPLAYERS);
	registerEnum(lsi, RETURNVALUE_YOUNEEDPREMIUMACCOUNT);
	registerEnum(lsi, RETURNVALUE_YOUNEEDTOLEARNTHISSPELL);
	registerEnum(lsi, RETURNVALUE_YOURVOCATIONCANNOTUSETHISSPELL);
	registerEnum(lsi, RETURNVALUE_YOUNEEDAWEAPONTOUSETHISSPELL);
	registerEnum(lsi, RETURNVALUE_PLAYERISPZLOCKEDLEAVEPVPZONE);
	registerEnum(lsi, RETURNVALUE_PLAYERISPZLOCKEDENTERPVPZONE);
	registerEnum(lsi, RETURNVALUE_ACTIONNOTPERMITTEDINANOPVPZONE);
	registerEnum(lsi, RETURNVALUE_YOUCANNOTLOGOUTHERE);
	registerEnum(lsi, RETURNVALUE_YOUNEEDAMAGICITEMTOCASTSPELL);
	registerEnum(lsi, RETURNVALUE_NAMEISTOOAMBIGUOUS);
	registerEnum(lsi, RETURNVALUE_CANONLYUSEONESHIELD);
	registerEnum(lsi, RETURNVALUE_NOPARTYMEMBERSINRANGE);
	registerEnum(lsi, RETURNVALUE_YOUARENOTTHEOWNER);
	registerEnum(lsi, RETURNVALUE_TRADEPLAYERFARAWAY);
	registerEnum(lsi, RETURNVALUE_YOUDONTOWNTHISHOUSE);
	registerEnum(lsi, RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE);
	registerEnum(lsi, RETURNVALUE_TRADEPLAYERHIGHESTBIDDER);
	registerEnum(lsi, RETURNVALUE_YOUCANNOTTRADETHISHOUSE);
	registerEnum(lsi, RETURNVALUE_YOUDONTHAVEREQUIREDPROFESSION);
	registerEnum(lsi, RETURNVALUE_YOUCANNOTUSETHISBED);

	registerEnum(lsi, RELOAD_TYPE_ALL);
	registerEnum(lsi, RELOAD_TYPE_ACTIONS);
	registerEnum(lsi, RELOAD_TYPE_CHAT);
	registerEnum(lsi, RELOAD_TYPE_CONFIG);
	registerEnum(lsi, RELOAD_TYPE_EVENTS);
	registerEnum(lsi, RELOAD_TYPE_GLOBAL);
	registerEnum(lsi, RELOAD_TYPE_GLOBALEVENTS);
	registerEnum(lsi, RELOAD_TYPE_ITEMS);
	registerEnum(lsi, RELOAD_TYPE_MONSTERS);
	registerEnum(lsi, RELOAD_TYPE_MOUNTS);
	registerEnum(lsi, RELOAD_TYPE_MOVEMENTS);
	registerEnum(lsi, RELOAD_TYPE_NPCS);
	registerEnum(lsi, RELOAD_TYPE_QUESTS);
	registerEnum(lsi, RELOAD_TYPE_SCRIPTS);
	registerEnum(lsi, RELOAD_TYPE_SPELLS);
	registerEnum(lsi, RELOAD_TYPE_TALKACTIONS);
	registerEnum(lsi, RELOAD_TYPE_WEAPONS);

	lsi.registerGlobalBoolean("VIRTUAL_PARENT", true);

	lsi.registerGlobalMethod("isType", luaIsType);
	lsi.registerGlobalMethod("rawgetmetatable", luaRawGetMetatable);
}
