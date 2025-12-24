#include "../../otpch.h"

#include "../../combat.h"

#include "../../creature.h"
#include "../../game.h"
#include "../../matrixarea.h"
#include "../../player.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;
extern LuaEnvironment g_luaEnvironment;

namespace {

int luaCombatCreate(lua_State* L)
{
	// Combat()
	tfs::lua::pushSharedPtr(L, g_luaEnvironment.createCombatObject(tfs::lua::getScriptEnv()->getScriptInterface()));
	tfs::lua::setMetatable(L, -1, "Combat");
	return 1;
}

int luaCombatDelete(lua_State* L)
{
	Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (combat) {
		combat.reset();
	}
	return 0;
}

int luaCombatSetParameter(lua_State* L)
{
	// combat:setParameter(key, value)
	const Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (!combat) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	CombatParam_t key = tfs::lua::getNumber<CombatParam_t>(L, 2);
	uint32_t value;
	if (lua_isboolean(L, 3)) {
		value = tfs::lua::getBoolean(L, 3) ? 1 : 0;
	} else {
		value = tfs::lua::getNumber<uint32_t>(L, 3);
	}
	combat->setParam(key, value);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCombatGetParameter(lua_State* L)
{
	// combat:getParameter(key)
	const Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (!combat) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	int32_t value = combat->getParam(tfs::lua::getNumber<CombatParam_t>(L, 2));
	if (value == std::numeric_limits<int32_t>().max()) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushNumber(L, value);
	return 1;
}

int luaCombatSetFormula(lua_State* L)
{
	// combat:setFormula(type, mina, minb, maxa, maxb)
	const Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (!combat) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	formulaType_t type = tfs::lua::getNumber<formulaType_t>(L, 2);
	double mina = tfs::lua::getNumber<double>(L, 3);
	double minb = tfs::lua::getNumber<double>(L, 4);
	double maxa = tfs::lua::getNumber<double>(L, 5);
	double maxb = tfs::lua::getNumber<double>(L, 6);
	combat->setPlayerCombatValues(type, mina, minb, maxa, maxb);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCombatSetArea(lua_State* L)
{
	// combat:setArea(area)
	if (tfs::lua::getScriptEnv()->getScriptId() != EVENT_ID_LOADING) {
		tfs::lua::reportError(L, "This function can only be used while loading the script.");
		lua_pushnil(L);
		return 1;
	}

	const AreaCombat* area = g_luaEnvironment.getAreaObject(tfs::lua::getNumber<uint32_t>(L, 2));
	if (!area) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_AREA_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	const Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (!combat) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	combat->setArea(new AreaCombat(*area));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCombatAddCondition(lua_State* L)
{
	// combat:addCondition(condition)
	const Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (!combat) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	Condition* condition = tfs::lua::getUserdata<Condition>(L, 2);
	if (condition) {
		combat->addCondition(condition->clone());
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCombatClearConditions(lua_State* L)
{
	// combat:clearConditions()
	const Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (!combat) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	combat->clearConditions();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCombatSetCallback(lua_State* L)
{
	// combat:setCallback(key, function)
	const Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (!combat) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	CallBackParam_t key = tfs::lua::getNumber<CallBackParam_t>(L, 2);
	if (!combat->setCallback(key)) {
		lua_pushnil(L);
		return 1;
	}

	CallBack* callback = combat->getCallback(key);
	if (!callback) {
		lua_pushnil(L);
		return 1;
	}

	const std::string& function = tfs::lua::getString(L, 3);
	tfs::lua::pushBoolean(L, callback->loadCallBack(tfs::lua::getScriptEnv()->getScriptInterface(), function));
	return 1;
}

int luaCombatSetOrigin(lua_State* L)
{
	// combat:setOrigin(origin)
	const Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (!combat) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	combat->setOrigin(tfs::lua::getNumber<CombatOrigin>(L, 2));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCombatExecute(lua_State* L)
{
	// combat:execute(creature, variant)
	const Combat_ptr& combat = tfs::lua::getSharedPtr<Combat>(L, 1);
	if (!combat) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_COMBAT_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	if (lua_isuserdata(L, 2)) {
		const auto type = tfs::lua::getUserdataType(L, 2);
		if (type != tfs::lua::LuaData_Player && type != tfs::lua::LuaData_Monster && type != tfs::lua::LuaData_Npc) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
	}

	const auto& creature = tfs::lua::getCreature(L, 2);

	const LuaVariant& variant = tfs::lua::getVariant(L, 3);
	switch (variant.type()) {
		case VARIANT_NUMBER: {
			const auto& target = g_game.getCreatureByID(variant.getNumber());
			if (!target) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}

			if (combat->hasArea()) {
				combat->doCombat(creature, target->getPosition());
			} else {
				combat->doCombat(creature, target);
			}
			break;
		}

		case VARIANT_POSITION: {
			combat->doCombat(creature, variant.getPosition());
			break;
		}

		case VARIANT_TARGETPOSITION: {
			if (combat->hasArea()) {
				combat->doCombat(creature, variant.getTargetPosition());
			} else {
				combat->postCombatEffects(creature, variant.getTargetPosition());
				g_game.addMagicEffect(variant.getTargetPosition(), CONST_ME_POFF);
			}
			break;
		}

		case VARIANT_STRING: {
			const auto& target = g_game.getPlayerByName(variant.getString());
			if (!target) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}

			combat->doCombat(creature, target);
			break;
		}

		case VARIANT_NONE: {
			tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_VARIANT_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		default: {
			break;
		}
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

} // namespace

void tfs::lua::registerCombat(LuaScriptInterface& lsi)
{
	registerEnum(lsi, CONDITION_NONE);
	registerEnum(lsi, CONDITION_POISON);
	registerEnum(lsi, CONDITION_FIRE);
	registerEnum(lsi, CONDITION_ENERGY);
	registerEnum(lsi, CONDITION_BLEEDING);
	registerEnum(lsi, CONDITION_HASTE);
	registerEnum(lsi, CONDITION_PARALYZE);
	registerEnum(lsi, CONDITION_OUTFIT);
	registerEnum(lsi, CONDITION_INVISIBLE);
	registerEnum(lsi, CONDITION_LIGHT);
	registerEnum(lsi, CONDITION_MANASHIELD);
	registerEnum(lsi, CONDITION_MANASHIELD_BREAKABLE);
	registerEnum(lsi, CONDITION_INFIGHT);
	registerEnum(lsi, CONDITION_DRUNK);
	registerEnum(lsi, CONDITION_EXHAUST_WEAPON);
	registerEnum(lsi, CONDITION_REGENERATION);
	registerEnum(lsi, CONDITION_SOUL);
	registerEnum(lsi, CONDITION_DROWN);
	registerEnum(lsi, CONDITION_MUTED);
	registerEnum(lsi, CONDITION_CHANNELMUTEDTICKS);
	registerEnum(lsi, CONDITION_YELLTICKS);
	registerEnum(lsi, CONDITION_ATTRIBUTES);
	registerEnum(lsi, CONDITION_FREEZING);
	registerEnum(lsi, CONDITION_DAZZLED);
	registerEnum(lsi, CONDITION_CURSED);
	registerEnum(lsi, CONDITION_EXHAUST_COMBAT);
	registerEnum(lsi, CONDITION_EXHAUST_HEAL);
	registerEnum(lsi, CONDITION_PACIFIED);
	registerEnum(lsi, CONDITION_SPELLCOOLDOWN);
	registerEnum(lsi, CONDITION_SPELLGROUPCOOLDOWN);
	registerEnum(lsi, CONDITION_ROOT);

	registerEnum(lsi, CONDITIONID_DEFAULT);
	registerEnum(lsi, CONDITIONID_COMBAT);
	registerEnum(lsi, CONDITIONID_HEAD);
	registerEnum(lsi, CONDITIONID_NECKLACE);
	registerEnum(lsi, CONDITIONID_BACKPACK);
	registerEnum(lsi, CONDITIONID_ARMOR);
	registerEnum(lsi, CONDITIONID_RIGHT);
	registerEnum(lsi, CONDITIONID_LEFT);
	registerEnum(lsi, CONDITIONID_LEGS);
	registerEnum(lsi, CONDITIONID_FEET);
	registerEnum(lsi, CONDITIONID_RING);
	registerEnum(lsi, CONDITIONID_AMMO);

	registerEnum(lsi, CONDITION_PARAM_OWNER);
	registerEnum(lsi, CONDITION_PARAM_TICKS);
	registerEnum(lsi, CONDITION_PARAM_DRUNKENNESS);
	registerEnum(lsi, CONDITION_PARAM_HEALTHGAIN);
	registerEnum(lsi, CONDITION_PARAM_HEALTHTICKS);
	registerEnum(lsi, CONDITION_PARAM_MANAGAIN);
	registerEnum(lsi, CONDITION_PARAM_MANATICKS);
	registerEnum(lsi, CONDITION_PARAM_DELAYED);
	registerEnum(lsi, CONDITION_PARAM_SPEED);
	registerEnum(lsi, CONDITION_PARAM_LIGHT_LEVEL);
	registerEnum(lsi, CONDITION_PARAM_LIGHT_COLOR);
	registerEnum(lsi, CONDITION_PARAM_SOULGAIN);
	registerEnum(lsi, CONDITION_PARAM_SOULTICKS);
	registerEnum(lsi, CONDITION_PARAM_MINVALUE);
	registerEnum(lsi, CONDITION_PARAM_MAXVALUE);
	registerEnum(lsi, CONDITION_PARAM_STARTVALUE);
	registerEnum(lsi, CONDITION_PARAM_TICKINTERVAL);
	registerEnum(lsi, CONDITION_PARAM_FORCEUPDATE);
	registerEnum(lsi, CONDITION_PARAM_SKILL_MELEE);
	registerEnum(lsi, CONDITION_PARAM_SKILL_FIST);
	registerEnum(lsi, CONDITION_PARAM_SKILL_CLUB);
	registerEnum(lsi, CONDITION_PARAM_SKILL_SWORD);
	registerEnum(lsi, CONDITION_PARAM_SKILL_AXE);
	registerEnum(lsi, CONDITION_PARAM_SKILL_DISTANCE);
	registerEnum(lsi, CONDITION_PARAM_SKILL_SHIELD);
	registerEnum(lsi, CONDITION_PARAM_SKILL_FISHING);
	registerEnum(lsi, CONDITION_PARAM_STAT_MAXHITPOINTS);
	registerEnum(lsi, CONDITION_PARAM_STAT_MAXMANAPOINTS);
	registerEnum(lsi, CONDITION_PARAM_STAT_MAGICPOINTS);
	registerEnum(lsi, CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT);
	registerEnum(lsi, CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT);
	registerEnum(lsi, CONDITION_PARAM_STAT_MAGICPOINTSPERCENT);
	registerEnum(lsi, CONDITION_PARAM_PERIODICDAMAGE);
	registerEnum(lsi, CONDITION_PARAM_SKILL_MELEEPERCENT);
	registerEnum(lsi, CONDITION_PARAM_SKILL_FISTPERCENT);
	registerEnum(lsi, CONDITION_PARAM_SKILL_CLUBPERCENT);
	registerEnum(lsi, CONDITION_PARAM_SKILL_SWORDPERCENT);
	registerEnum(lsi, CONDITION_PARAM_SKILL_AXEPERCENT);
	registerEnum(lsi, CONDITION_PARAM_SKILL_DISTANCEPERCENT);
	registerEnum(lsi, CONDITION_PARAM_SKILL_SHIELDPERCENT);
	registerEnum(lsi, CONDITION_PARAM_SKILL_FISHINGPERCENT);
	registerEnum(lsi, CONDITION_PARAM_BUFF_SPELL);
	registerEnum(lsi, CONDITION_PARAM_SUBID);
	registerEnum(lsi, CONDITION_PARAM_FIELD);
	registerEnum(lsi, CONDITION_PARAM_DISABLE_DEFENSE);
	registerEnum(lsi, CONDITION_PARAM_MANASHIELD_BREAKABLE);
	registerEnum(lsi, CONDITION_PARAM_SPECIALSKILL_CRITICALHITCHANCE);
	registerEnum(lsi, CONDITION_PARAM_SPECIALSKILL_CRITICALHITAMOUNT);
	registerEnum(lsi, CONDITION_PARAM_SPECIALSKILL_LIFELEECHCHANCE);
	registerEnum(lsi, CONDITION_PARAM_SPECIALSKILL_LIFELEECHAMOUNT);
	registerEnum(lsi, CONDITION_PARAM_SPECIALSKILL_MANALEECHCHANCE);
	registerEnum(lsi, CONDITION_PARAM_SPECIALSKILL_MANALEECHAMOUNT);
	registerEnum(lsi, CONDITION_PARAM_AGGRESSIVE);

	registerEnum(lsi, CONST_ME_NONE);
	registerEnum(lsi, CONST_ME_DRAWBLOOD);
	registerEnum(lsi, CONST_ME_LOSEENERGY);
	registerEnum(lsi, CONST_ME_POFF);
	registerEnum(lsi, CONST_ME_BLOCKHIT);
	registerEnum(lsi, CONST_ME_EXPLOSIONAREA);
	registerEnum(lsi, CONST_ME_EXPLOSIONHIT);
	registerEnum(lsi, CONST_ME_FIREAREA);
	registerEnum(lsi, CONST_ME_YELLOW_RINGS);
	registerEnum(lsi, CONST_ME_GREEN_RINGS);
	registerEnum(lsi, CONST_ME_HITAREA);
	registerEnum(lsi, CONST_ME_TELEPORT);
	registerEnum(lsi, CONST_ME_ENERGYHIT);
	registerEnum(lsi, CONST_ME_MAGIC_BLUE);
	registerEnum(lsi, CONST_ME_MAGIC_RED);
	registerEnum(lsi, CONST_ME_MAGIC_GREEN);
	registerEnum(lsi, CONST_ME_HITBYFIRE);
	registerEnum(lsi, CONST_ME_HITBYPOISON);
	registerEnum(lsi, CONST_ME_MORTAREA);
	registerEnum(lsi, CONST_ME_SOUND_GREEN);
	registerEnum(lsi, CONST_ME_SOUND_RED);
	registerEnum(lsi, CONST_ME_POISONAREA);
	registerEnum(lsi, CONST_ME_SOUND_YELLOW);
	registerEnum(lsi, CONST_ME_SOUND_PURPLE);
	registerEnum(lsi, CONST_ME_SOUND_BLUE);
	registerEnum(lsi, CONST_ME_SOUND_WHITE);
	registerEnum(lsi, CONST_ME_BUBBLES);
	registerEnum(lsi, CONST_ME_CRAPS);
	registerEnum(lsi, CONST_ME_GIFT_WRAPS);
	registerEnum(lsi, CONST_ME_FIREWORK_YELLOW);
	registerEnum(lsi, CONST_ME_FIREWORK_RED);
	registerEnum(lsi, CONST_ME_FIREWORK_BLUE);
	registerEnum(lsi, CONST_ME_STUN);
	registerEnum(lsi, CONST_ME_SLEEP);
	registerEnum(lsi, CONST_ME_WATERCREATURE);
	registerEnum(lsi, CONST_ME_GROUNDSHAKER);
	registerEnum(lsi, CONST_ME_HEARTS);
	registerEnum(lsi, CONST_ME_FIREATTACK);
	registerEnum(lsi, CONST_ME_ENERGYAREA);
	registerEnum(lsi, CONST_ME_SMALLCLOUDS);
	registerEnum(lsi, CONST_ME_HOLYDAMAGE);
	registerEnum(lsi, CONST_ME_BIGCLOUDS);
	registerEnum(lsi, CONST_ME_ICEAREA);
	registerEnum(lsi, CONST_ME_ICETORNADO);
	registerEnum(lsi, CONST_ME_ICEATTACK);
	registerEnum(lsi, CONST_ME_STONES);
	registerEnum(lsi, CONST_ME_SMALLPLANTS);
	registerEnum(lsi, CONST_ME_CARNIPHILA);
	registerEnum(lsi, CONST_ME_PURPLEENERGY);
	registerEnum(lsi, CONST_ME_YELLOWENERGY);
	registerEnum(lsi, CONST_ME_HOLYAREA);
	registerEnum(lsi, CONST_ME_BIGPLANTS);
	registerEnum(lsi, CONST_ME_CAKE);
	registerEnum(lsi, CONST_ME_GIANTICE);
	registerEnum(lsi, CONST_ME_WATERSPLASH);
	registerEnum(lsi, CONST_ME_PLANTATTACK);
	registerEnum(lsi, CONST_ME_TUTORIALARROW);
	registerEnum(lsi, CONST_ME_TUTORIALSQUARE);
	registerEnum(lsi, CONST_ME_MIRRORHORIZONTAL);
	registerEnum(lsi, CONST_ME_MIRRORVERTICAL);
	registerEnum(lsi, CONST_ME_SKULLHORIZONTAL);
	registerEnum(lsi, CONST_ME_SKULLVERTICAL);
	registerEnum(lsi, CONST_ME_ASSASSIN);
	registerEnum(lsi, CONST_ME_STEPSHORIZONTAL);
	registerEnum(lsi, CONST_ME_BLOODYSTEPS);
	registerEnum(lsi, CONST_ME_STEPSVERTICAL);
	registerEnum(lsi, CONST_ME_YALAHARIGHOST);
	registerEnum(lsi, CONST_ME_BATS);
	registerEnum(lsi, CONST_ME_SMOKE);
	registerEnum(lsi, CONST_ME_INSECTS);
	registerEnum(lsi, CONST_ME_DRAGONHEAD);
	registerEnum(lsi, CONST_ME_ORCSHAMAN);
	registerEnum(lsi, CONST_ME_ORCSHAMAN_FIRE);
	registerEnum(lsi, CONST_ME_THUNDER);
	registerEnum(lsi, CONST_ME_FERUMBRAS);
	registerEnum(lsi, CONST_ME_CONFETTI_HORIZONTAL);
	registerEnum(lsi, CONST_ME_CONFETTI_VERTICAL);
	registerEnum(lsi, CONST_ME_BLACKSMOKE);
	registerEnum(lsi, CONST_ME_REDSMOKE);
	registerEnum(lsi, CONST_ME_YELLOWSMOKE);
	registerEnum(lsi, CONST_ME_GREENSMOKE);
	registerEnum(lsi, CONST_ME_PURPLESMOKE);
	registerEnum(lsi, CONST_ME_EARLY_THUNDER);
	registerEnum(lsi, CONST_ME_RAGIAZ_BONECAPSULE);
	registerEnum(lsi, CONST_ME_CRITICAL_DAMAGE);
	registerEnum(lsi, CONST_ME_PLUNGING_FISH);
	registerEnum(lsi, CONST_ME_BLUECHAIN);
	registerEnum(lsi, CONST_ME_ORANGECHAIN);
	registerEnum(lsi, CONST_ME_GREENCHAIN);
	registerEnum(lsi, CONST_ME_PURPLECHAIN);
	registerEnum(lsi, CONST_ME_GREYCHAIN);
	registerEnum(lsi, CONST_ME_YELLOWCHAIN);
	registerEnum(lsi, CONST_ME_YELLOWSPARKLES);
	registerEnum(lsi, CONST_ME_FAEEXPLOSION);
	registerEnum(lsi, CONST_ME_FAECOMING);
	registerEnum(lsi, CONST_ME_FAEGOING);
	registerEnum(lsi, CONST_ME_BIGCLOUDSSINGLESPACE);
	registerEnum(lsi, CONST_ME_STONESSINGLESPACE);
	registerEnum(lsi, CONST_ME_BLUEGHOST);
	registerEnum(lsi, CONST_ME_POINTOFINTEREST);
	registerEnum(lsi, CONST_ME_MAPEFFECT);
	registerEnum(lsi, CONST_ME_PINKSPARK);
	registerEnum(lsi, CONST_ME_FIREWORK_GREEN);
	registerEnum(lsi, CONST_ME_FIREWORK_ORANGE);
	registerEnum(lsi, CONST_ME_FIREWORK_PURPLE);
	registerEnum(lsi, CONST_ME_FIREWORK_TURQUOISE);
	registerEnum(lsi, CONST_ME_THECUBE);
	registerEnum(lsi, CONST_ME_DRAWINK);
	registerEnum(lsi, CONST_ME_PRISMATICSPARKLES);
	registerEnum(lsi, CONST_ME_THAIAN);
	registerEnum(lsi, CONST_ME_THAIANGHOST);
	registerEnum(lsi, CONST_ME_GHOSTSMOKE);
	registerEnum(lsi, CONST_ME_FLOATINGBLOCK);
	registerEnum(lsi, CONST_ME_BLOCK);
	registerEnum(lsi, CONST_ME_ROOTING);
	registerEnum(lsi, CONST_ME_GHOSTLYSCRATCH);
	registerEnum(lsi, CONST_ME_GHOSTLYBITE);
	registerEnum(lsi, CONST_ME_BIGSCRATCHING);
	registerEnum(lsi, CONST_ME_SLASH);
	registerEnum(lsi, CONST_ME_BITE);
	registerEnum(lsi, CONST_ME_CHIVALRIOUSCHALLENGE);
	registerEnum(lsi, CONST_ME_DIVINEDAZZLE);
	registerEnum(lsi, CONST_ME_ELECTRICALSPARK);
	registerEnum(lsi, CONST_ME_PURPLETELEPORT);
	registerEnum(lsi, CONST_ME_REDTELEPORT);
	registerEnum(lsi, CONST_ME_ORANGETELEPORT);
	registerEnum(lsi, CONST_ME_GREYTELEPORT);
	registerEnum(lsi, CONST_ME_LIGHTBLUETELEPORT);
	registerEnum(lsi, CONST_ME_FATAL);
	registerEnum(lsi, CONST_ME_DODGE);
	registerEnum(lsi, CONST_ME_HOURGLASS);
	registerEnum(lsi, CONST_ME_FIREWORKSSTAR);
	registerEnum(lsi, CONST_ME_FIREWORKSCIRCLE);
	registerEnum(lsi, CONST_ME_FERUMBRAS_1);
	registerEnum(lsi, CONST_ME_GAZHARAGOTH);
	registerEnum(lsi, CONST_ME_MAD_MAGE);
	registerEnum(lsi, CONST_ME_HORESTIS);
	registerEnum(lsi, CONST_ME_DEVOVORGA);
	registerEnum(lsi, CONST_ME_FERUMBRAS_2);
	registerEnum(lsi, CONST_ME_FOAM);

	registerEnum(lsi, CONST_ANI_NONE);
	registerEnum(lsi, CONST_ANI_SPEAR);
	registerEnum(lsi, CONST_ANI_BOLT);
	registerEnum(lsi, CONST_ANI_ARROW);
	registerEnum(lsi, CONST_ANI_FIRE);
	registerEnum(lsi, CONST_ANI_ENERGY);
	registerEnum(lsi, CONST_ANI_POISONARROW);
	registerEnum(lsi, CONST_ANI_BURSTARROW);
	registerEnum(lsi, CONST_ANI_THROWINGSTAR);
	registerEnum(lsi, CONST_ANI_THROWINGKNIFE);
	registerEnum(lsi, CONST_ANI_SMALLSTONE);
	registerEnum(lsi, CONST_ANI_DEATH);
	registerEnum(lsi, CONST_ANI_LARGEROCK);
	registerEnum(lsi, CONST_ANI_SNOWBALL);
	registerEnum(lsi, CONST_ANI_POWERBOLT);
	registerEnum(lsi, CONST_ANI_POISON);
	registerEnum(lsi, CONST_ANI_INFERNALBOLT);
	registerEnum(lsi, CONST_ANI_HUNTINGSPEAR);
	registerEnum(lsi, CONST_ANI_ENCHANTEDSPEAR);
	registerEnum(lsi, CONST_ANI_REDSTAR);
	registerEnum(lsi, CONST_ANI_GREENSTAR);
	registerEnum(lsi, CONST_ANI_ROYALSPEAR);
	registerEnum(lsi, CONST_ANI_SNIPERARROW);
	registerEnum(lsi, CONST_ANI_ONYXARROW);
	registerEnum(lsi, CONST_ANI_PIERCINGBOLT);
	registerEnum(lsi, CONST_ANI_WHIRLWINDSWORD);
	registerEnum(lsi, CONST_ANI_WHIRLWINDAXE);
	registerEnum(lsi, CONST_ANI_WHIRLWINDCLUB);
	registerEnum(lsi, CONST_ANI_ETHEREALSPEAR);
	registerEnum(lsi, CONST_ANI_ICE);
	registerEnum(lsi, CONST_ANI_EARTH);
	registerEnum(lsi, CONST_ANI_HOLY);
	registerEnum(lsi, CONST_ANI_SUDDENDEATH);
	registerEnum(lsi, CONST_ANI_FLASHARROW);
	registerEnum(lsi, CONST_ANI_FLAMMINGARROW);
	registerEnum(lsi, CONST_ANI_SHIVERARROW);
	registerEnum(lsi, CONST_ANI_ENERGYBALL);
	registerEnum(lsi, CONST_ANI_SMALLICE);
	registerEnum(lsi, CONST_ANI_SMALLHOLY);
	registerEnum(lsi, CONST_ANI_SMALLEARTH);
	registerEnum(lsi, CONST_ANI_EARTHARROW);
	registerEnum(lsi, CONST_ANI_EXPLOSION);
	registerEnum(lsi, CONST_ANI_CAKE);
	registerEnum(lsi, CONST_ANI_TARSALARROW);
	registerEnum(lsi, CONST_ANI_VORTEXBOLT);
	registerEnum(lsi, CONST_ANI_PRISMATICBOLT);
	registerEnum(lsi, CONST_ANI_CRYSTALLINEARROW);
	registerEnum(lsi, CONST_ANI_DRILLBOLT);
	registerEnum(lsi, CONST_ANI_ENVENOMEDARROW);
	registerEnum(lsi, CONST_ANI_GLOOTHSPEAR);
	registerEnum(lsi, CONST_ANI_SIMPLEARROW);
	registerEnum(lsi, CONST_ANI_LEAFSTAR);
	registerEnum(lsi, CONST_ANI_DIAMONDARROW);
	registerEnum(lsi, CONST_ANI_SPECTRALBOLT);
	registerEnum(lsi, CONST_ANI_ROYALSTAR);
	registerEnum(lsi, CONST_ANI_WEAPONTYPE);

	registerEnum(lsi, ORIGIN_NONE);
	registerEnum(lsi, ORIGIN_CONDITION);
	registerEnum(lsi, ORIGIN_SPELL);
	registerEnum(lsi, ORIGIN_MELEE);
	registerEnum(lsi, ORIGIN_RANGED);
	registerEnum(lsi, ORIGIN_WAND);

	registerEnum(lsi, COMBAT_FORMULA_UNDEFINED);
	registerEnum(lsi, COMBAT_FORMULA_LEVELMAGIC);
	registerEnum(lsi, COMBAT_FORMULA_SKILL);
	registerEnum(lsi, COMBAT_FORMULA_DAMAGE);

	registerEnum(lsi, COMBAT_NONE);
	registerEnum(lsi, COMBAT_PHYSICALDAMAGE);
	registerEnum(lsi, COMBAT_ENERGYDAMAGE);
	registerEnum(lsi, COMBAT_EARTHDAMAGE);
	registerEnum(lsi, COMBAT_FIREDAMAGE);
	registerEnum(lsi, COMBAT_UNDEFINEDDAMAGE);
	registerEnum(lsi, COMBAT_LIFEDRAIN);
	registerEnum(lsi, COMBAT_MANADRAIN);
	registerEnum(lsi, COMBAT_HEALING);
	registerEnum(lsi, COMBAT_DROWNDAMAGE);
	registerEnum(lsi, COMBAT_ICEDAMAGE);
	registerEnum(lsi, COMBAT_HOLYDAMAGE);
	registerEnum(lsi, COMBAT_DEATHDAMAGE);

	registerEnum(lsi, COMBAT_PARAM_TYPE);
	registerEnum(lsi, COMBAT_PARAM_EFFECT);
	registerEnum(lsi, COMBAT_PARAM_DISTANCEEFFECT);
	registerEnum(lsi, COMBAT_PARAM_BLOCKSHIELD);
	registerEnum(lsi, COMBAT_PARAM_BLOCKARMOR);
	registerEnum(lsi, COMBAT_PARAM_TARGETCASTERORTOPMOST);
	registerEnum(lsi, COMBAT_PARAM_CREATEITEM);
	registerEnum(lsi, COMBAT_PARAM_AGGRESSIVE);
	registerEnum(lsi, COMBAT_PARAM_DISPEL);
	registerEnum(lsi, COMBAT_PARAM_USECHARGES);

	lsi.registerClass("Combat", "", luaCombatCreate);
	lsi.registerMetaMethod("Combat", "__eq", tfs::lua::luaUserdataCompare);
	lsi.registerMetaMethod("Combat", "__gc", luaCombatDelete);
	lsi.registerMethod("Combat", "delete", luaCombatDelete);

	lsi.registerMethod("Combat", "setParameter", luaCombatSetParameter);
	lsi.registerMethod("Combat", "getParameter", luaCombatGetParameter);

	lsi.registerMethod("Combat", "setFormula", luaCombatSetFormula);

	lsi.registerMethod("Combat", "setArea", luaCombatSetArea);
	lsi.registerMethod("Combat", "addCondition", luaCombatAddCondition);
	lsi.registerMethod("Combat", "clearConditions", luaCombatClearConditions);
	lsi.registerMethod("Combat", "setCallback", luaCombatSetCallback);
	lsi.registerMethod("Combat", "setOrigin", luaCombatSetOrigin);

	lsi.registerMethod("Combat", "execute", luaCombatExecute);
}
