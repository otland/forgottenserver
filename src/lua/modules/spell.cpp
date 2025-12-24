#include "../../otpch.h"

#include "../../spells.h"
#include "../../vocation.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Spells* g_spells;
extern Vocations g_vocations;

namespace {

int luaSpellCreate(lua_State* L)
{
	// Spell(words, name or id) to get an existing spell
	// Spell(type) ex: Spell(SPELL_INSTANT) or Spell(SPELL_RUNE) to create a new spell
	if (lua_gettop(L) == 1) {
		std::cout << "[Error - Spell::luaSpellCreate] There is no parameter set!\n";
		lua_pushnil(L);
		return 1;
	}

	SpellType_t spellType = SPELL_UNDEFINED;

	if (tfs::lua::isNumber(L, 2)) {
		int32_t id = tfs::lua::getNumber<int32_t>(L, 2);
		RuneSpell* rune = g_spells->getRuneSpell(id);

		if (rune) {
			tfs::lua::pushUserdata<Spell>(L, rune);
			tfs::lua::setMetatable(L, -1, "Spell");
			return 1;
		}

		spellType = static_cast<SpellType_t>(id);
	} else if (lua_isstring(L, 2)) {
		std::string arg = tfs::lua::getString(L, 2);
		InstantSpell* instant = g_spells->getInstantSpellByName(arg);
		if (instant) {
			tfs::lua::pushUserdata<Spell>(L, instant);
			tfs::lua::setMetatable(L, -1, "Spell");
			return 1;
		}
		instant = g_spells->getInstantSpell(arg);
		if (instant) {
			tfs::lua::pushUserdata<Spell>(L, instant);
			tfs::lua::setMetatable(L, -1, "Spell");
			return 1;
		}
		RuneSpell* rune = g_spells->getRuneSpellByName(arg);
		if (rune) {
			tfs::lua::pushUserdata<Spell>(L, rune);
			tfs::lua::setMetatable(L, -1, "Spell");
			return 1;
		}

		std::string tmp = boost::algorithm::to_lower_copy(arg);
		if (tmp == "instant") {
			spellType = SPELL_INSTANT;
		} else if (tmp == "rune") {
			spellType = SPELL_RUNE;
		}
	}

	if (spellType == SPELL_INSTANT) {
		InstantSpell* spell = new InstantSpell(tfs::lua::getScriptEnv()->getScriptInterface());
		spell->fromLua = true;
		tfs::lua::pushUserdata<Spell>(L, spell);
		tfs::lua::setMetatable(L, -1, "Spell");
		spell->spellType = SPELL_INSTANT;
		return 1;
	} else if (spellType == SPELL_RUNE) {
		RuneSpell* spell = new RuneSpell(tfs::lua::getScriptEnv()->getScriptInterface());
		spell->fromLua = true;
		tfs::lua::pushUserdata<Spell>(L, spell);
		tfs::lua::setMetatable(L, -1, "Spell");
		spell->spellType = SPELL_RUNE;
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

int luaSpellOnCastSpell(lua_State* L)
{
	// spell:onCastSpell(callback)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (spell->spellType == SPELL_INSTANT) {
			InstantSpell* instant = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
			if (!instant->loadCallback()) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
			instant->scripted = true;
			tfs::lua::pushBoolean(L, true);
		} else if (spell->spellType == SPELL_RUNE) {
			RuneSpell* rune = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
			if (!rune->loadCallback()) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
			rune->scripted = true;
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellRegister(lua_State* L)
{
	// spell:register()
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (spell->spellType == SPELL_INSTANT) {
			InstantSpell* instant = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
			if (!instant->isScripted()) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
			tfs::lua::pushBoolean(L, g_spells->registerInstantLuaEvent(instant));
		} else if (spell->spellType == SPELL_RUNE) {
			RuneSpell* rune = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
			if (rune->getMagicLevel() != 0 || rune->getLevel() != 0) {
				// Change information in the ItemType to get accurate description
				ItemType& iType = Item::items.getItemType(rune->getRuneItemId());
				iType.name = rune->getName();
				iType.runeMagLevel = rune->getMagicLevel();
				iType.runeLevel = rune->getLevel();
				iType.charges = rune->getCharges();
			}
			if (!rune->isScripted()) {
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
			tfs::lua::pushBoolean(L, g_spells->registerRuneLuaEvent(rune));
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellName(lua_State* L)
{
	// spell:name(name)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushString(L, spell->getName());
		} else {
			spell->setName(tfs::lua::getString(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellId(lua_State* L)
{
	// spell:id(id)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getId());
		} else {
			spell->setId(tfs::lua::getNumber<uint8_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellGroup(lua_State* L)
{
	// spell:group(primaryGroup[, secondaryGroup])
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getGroup());
			tfs::lua::pushNumber(L, spell->getSecondaryGroup());
			return 2;
		} else if (lua_gettop(L) == 2) {
			SpellGroup_t group = tfs::lua::getNumber<SpellGroup_t>(L, 2);
			if (group) {
				spell->setGroup(group);
				tfs::lua::pushBoolean(L, true);
			} else if (lua_isstring(L, 2)) {
				group = stringToSpellGroup(tfs::lua::getString(L, 2));
				if (group != SPELLGROUP_NONE) {
					spell->setGroup(group);
				} else {
					std::cout << "[Warning - Spell::group] Unknown group: " << tfs::lua::getString(L, 2) << '\n';
					tfs::lua::pushBoolean(L, false);
					return 1;
				}
				tfs::lua::pushBoolean(L, true);
			} else {
				std::cout << "[Warning - Spell::group] Unknown group: " << tfs::lua::getString(L, 2) << '\n';
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
		} else {
			SpellGroup_t primaryGroup = tfs::lua::getNumber<SpellGroup_t>(L, 2);
			SpellGroup_t secondaryGroup = tfs::lua::getNumber<SpellGroup_t>(L, 3);
			if (primaryGroup && secondaryGroup) {
				spell->setGroup(primaryGroup);
				spell->setSecondaryGroup(secondaryGroup);
				tfs::lua::pushBoolean(L, true);
			} else if (lua_isstring(L, 2) && lua_isstring(L, 3)) {
				primaryGroup = stringToSpellGroup(tfs::lua::getString(L, 2));
				if (primaryGroup != SPELLGROUP_NONE) {
					spell->setGroup(primaryGroup);
				} else {
					std::cout << "[Warning - Spell::group] Unknown primaryGroup: " << tfs::lua::getString(L, 2) << '\n';
					tfs::lua::pushBoolean(L, false);
					return 1;
				}
				secondaryGroup = stringToSpellGroup(tfs::lua::getString(L, 3));
				if (secondaryGroup != SPELLGROUP_NONE) {
					spell->setSecondaryGroup(secondaryGroup);
				} else {
					std::cout << "[Warning - Spell::group] Unknown secondaryGroup: " << tfs::lua::getString(L, 3)
					          << '\n';
					tfs::lua::pushBoolean(L, false);
					return 1;
				}
				tfs::lua::pushBoolean(L, true);
			} else {
				std::cout << "[Warning - Spell::group] Unknown primaryGroup: " << tfs::lua::getString(L, 2)
				          << " or secondaryGroup: " << tfs::lua::getString(L, 3) << '\n';
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellCooldown(lua_State* L)
{
	// spell:cooldown(cooldown)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getCooldown());
		} else {
			spell->setCooldown(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellGroupCooldown(lua_State* L)
{
	// spell:groupCooldown(primaryGroupCd[, secondaryGroupCd])
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getGroupCooldown());
			tfs::lua::pushNumber(L, spell->getSecondaryCooldown());
			return 2;
		} else if (lua_gettop(L) == 2) {
			spell->setGroupCooldown(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		} else {
			spell->setGroupCooldown(tfs::lua::getNumber<uint32_t>(L, 2));
			spell->setSecondaryCooldown(tfs::lua::getNumber<uint32_t>(L, 3));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellLevel(lua_State* L)
{
	// spell:level(lvl)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getLevel());
		} else {
			spell->setLevel(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellMagicLevel(lua_State* L)
{
	// spell:magicLevel(lvl)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getMagicLevel());
		} else {
			spell->setMagicLevel(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellMana(lua_State* L)
{
	// spell:mana(mana)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getMana());
		} else {
			spell->setMana(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellManaPercent(lua_State* L)
{
	// spell:manaPercent(percent)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getManaPercent());
		} else {
			spell->setManaPercent(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellSoul(lua_State* L)
{
	// spell:soul(soul)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getSoulCost());
		} else {
			spell->setSoulCost(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellRange(lua_State* L)
{
	// spell:range(range)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getRange());
		} else {
			spell->setRange(tfs::lua::getNumber<int32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellPremium(lua_State* L)
{
	// spell:isPremium(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->isPremium());
		} else {
			spell->setPremium(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellEnabled(lua_State* L)
{
	// spell:isEnabled(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->isEnabled());
		} else {
			spell->setEnabled(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellNeedTarget(lua_State* L)
{
	// spell:needTarget(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedTarget());
		} else {
			spell->setNeedTarget(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellNeedWeapon(lua_State* L)
{
	// spell:needWeapon(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedWeapon());
		} else {
			spell->setNeedWeapon(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellNeedLearn(lua_State* L)
{
	// spell:needLearn(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedLearn());
		} else {
			spell->setNeedLearn(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellSelfTarget(lua_State* L)
{
	// spell:isSelfTarget(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getSelfTarget());
		} else {
			spell->setSelfTarget(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellBlocking(lua_State* L)
{
	// spell:isBlocking(blockingSolid, blockingCreature)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getBlockingSolid());
			tfs::lua::pushBoolean(L, spell->getBlockingCreature());
			return 2;
		} else {
			spell->setBlockingSolid(tfs::lua::getBoolean(L, 2));
			spell->setBlockingCreature(tfs::lua::getBoolean(L, 3));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellAggressive(lua_State* L)
{
	// spell:isAggressive(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getAggressive());
		} else {
			spell->setAggressive(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellPzLock(lua_State* L)
{
	// spell:isPzLock(bool)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (spell) {
		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getPzLock());
		} else {
			spell->setPzLock(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaSpellVocation(lua_State* L)
{
	// spell:vocation(vocation)
	Spell* spell = tfs::lua::getUserdata<Spell>(L, 1);
	if (!spell) {
		lua_pushnil(L);
		return 1;
	}

	if (lua_gettop(L) == 1) {
		lua_createtable(L, 0, 0);
		int i = 0;
		for (auto&& vocation : spell->getVocationSpellMap() | std::views::keys | std::views::as_const) {
			const std::string& name = g_vocations.getVocation(vocation)->getVocName();
			tfs::lua::pushString(L, name);
			lua_rawseti(L, -2, ++i);
		}
	} else {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		for (int i = 0; i < parameters; ++i) {
			std::string vocStr = tfs::lua::getString(L, 2 + i);
			auto vocList = explodeString(vocStr, ";");
			spell->addVocationSpellMap(vocList[0], vocList.size() > 1 ? booleanString(vocList[1]) : false);
		}
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

// only for InstantSpells
int luaSpellWords(lua_State* L)
{
	// spell:words(words[, separator = ""])
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushString(L, spell->getWords());
			tfs::lua::pushString(L, spell->getSeparator());
			return 2;
		} else {
			std::string sep = "";
			if (lua_gettop(L) == 3) {
				sep = tfs::lua::getString(L, 3);
			}
			spell->setWords(tfs::lua::getString(L, 2));
			spell->setSeparator(sep);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int luaSpellNeedDirection(lua_State* L)
{
	// spell:needDirection(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedDirection());
		} else {
			spell->setNeedDirection(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int luaSpellHasParams(lua_State* L)
{
	// spell:hasParams(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getHasParam());
		} else {
			spell->setHasParam(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int luaSpellHasPlayerNameParam(lua_State* L)
{
	// spell:hasPlayerNameParam(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getHasPlayerNameParam());
		} else {
			spell->setHasPlayerNameParam(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int luaSpellNeedCasterTargetOrDirection(lua_State* L)
{
	// spell:needCasterTargetOrDirection(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getNeedCasterTargetOrDirection());
		} else {
			spell->setNeedCasterTargetOrDirection(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for InstantSpells
int luaSpellIsBlockingWalls(lua_State* L)
{
	// spell:blockWalls(bool)
	InstantSpell* spell = dynamic_cast<InstantSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_INSTANT, it means that this actually is no InstantSpell, so we return nil
		if (spell->spellType != SPELL_INSTANT) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getBlockWalls());
		} else {
			spell->setBlockWalls(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int luaSpellRuneLevel(lua_State* L)
{
	// spell:runeLevel(level)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	int32_t level = tfs::lua::getNumber<int32_t>(L, 2);
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getLevel());
		} else {
			spell->setLevel(level);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int luaSpellRuneMagicLevel(lua_State* L)
{
	// spell:runeMagicLevel(magLevel)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	int32_t magLevel = tfs::lua::getNumber<int32_t>(L, 2);
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getMagicLevel());
		} else {
			spell->setMagicLevel(magLevel);
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int luaSpellRuneId(lua_State* L)
{
	// spell:runeId(id)
	RuneSpell* rune = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (rune) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (rune->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, rune->getRuneItemId());
		} else {
			rune->setRuneItemId(tfs::lua::getNumber<uint16_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int luaSpellCharges(lua_State* L)
{
	// spell:charges(charges)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushNumber(L, spell->getCharges());
		} else {
			spell->setCharges(tfs::lua::getNumber<uint32_t>(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int luaSpellAllowFarUse(lua_State* L)
{
	// spell:allowFarUse(bool)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getAllowFarUse());
		} else {
			spell->setAllowFarUse(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int luaSpellBlockWalls(lua_State* L)
{
	// spell:blockWalls(bool)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getCheckLineOfSight());
		} else {
			spell->setCheckLineOfSight(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// only for RuneSpells
int luaSpellCheckFloor(lua_State* L)
{
	// spell:checkFloor(bool)
	RuneSpell* spell = dynamic_cast<RuneSpell*>(tfs::lua::getUserdata<Spell>(L, 1));
	if (spell) {
		// if spell != SPELL_RUNE, it means that this actually is no RuneSpell, so we return nil
		if (spell->spellType != SPELL_RUNE) {
			lua_pushnil(L);
			return 1;
		}

		if (lua_gettop(L) == 1) {
			tfs::lua::pushBoolean(L, spell->getCheckFloor());
		} else {
			spell->setCheckFloor(tfs::lua::getBoolean(L, 2));
			tfs::lua::pushBoolean(L, true);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerSpell(LuaScriptInterface& lsi)
{
	registerEnum(lsi, SPELL_INSTANT);
	registerEnum(lsi, SPELL_RUNE);

	lsi.registerClass("Spell", "", luaSpellCreate);
	lsi.registerMetaMethod("Spell", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("Spell", "onCastSpell", luaSpellOnCastSpell);
	lsi.registerMethod("Spell", "register", luaSpellRegister);
	lsi.registerMethod("Spell", "name", luaSpellName);
	lsi.registerMethod("Spell", "id", luaSpellId);
	lsi.registerMethod("Spell", "group", luaSpellGroup);
	lsi.registerMethod("Spell", "cooldown", luaSpellCooldown);
	lsi.registerMethod("Spell", "groupCooldown", luaSpellGroupCooldown);
	lsi.registerMethod("Spell", "level", luaSpellLevel);
	lsi.registerMethod("Spell", "magicLevel", luaSpellMagicLevel);
	lsi.registerMethod("Spell", "mana", luaSpellMana);
	lsi.registerMethod("Spell", "manaPercent", luaSpellManaPercent);
	lsi.registerMethod("Spell", "soul", luaSpellSoul);
	lsi.registerMethod("Spell", "range", luaSpellRange);
	lsi.registerMethod("Spell", "isPremium", luaSpellPremium);
	lsi.registerMethod("Spell", "isEnabled", luaSpellEnabled);
	lsi.registerMethod("Spell", "needTarget", luaSpellNeedTarget);
	lsi.registerMethod("Spell", "needWeapon", luaSpellNeedWeapon);
	lsi.registerMethod("Spell", "needLearn", luaSpellNeedLearn);
	lsi.registerMethod("Spell", "isSelfTarget", luaSpellSelfTarget);
	lsi.registerMethod("Spell", "isBlocking", luaSpellBlocking);
	lsi.registerMethod("Spell", "isAggressive", luaSpellAggressive);
	lsi.registerMethod("Spell", "isPzLock", luaSpellPzLock);
	lsi.registerMethod("Spell", "vocation", luaSpellVocation);

	// only for InstantSpell
	lsi.registerMethod("Spell", "words", luaSpellWords);
	lsi.registerMethod("Spell", "needDirection", luaSpellNeedDirection);
	lsi.registerMethod("Spell", "hasParams", luaSpellHasParams);
	lsi.registerMethod("Spell", "hasPlayerNameParam", luaSpellHasPlayerNameParam);
	lsi.registerMethod("Spell", "needCasterTargetOrDirection", luaSpellNeedCasterTargetOrDirection);
	lsi.registerMethod("Spell", "isBlockingWalls", luaSpellIsBlockingWalls);

	// only for RuneSpells
	lsi.registerMethod("Spell", "runeLevel", luaSpellRuneLevel);
	lsi.registerMethod("Spell", "runeMagicLevel", luaSpellRuneMagicLevel);
	lsi.registerMethod("Spell", "runeId", luaSpellRuneId);
	lsi.registerMethod("Spell", "charges", luaSpellCharges);
	lsi.registerMethod("Spell", "allowFarUse", luaSpellAllowFarUse);
	lsi.registerMethod("Spell", "blockWalls", luaSpellBlockWalls);
	lsi.registerMethod("Spell", "checkFloor", luaSpellCheckFloor);
}
