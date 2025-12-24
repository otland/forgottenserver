#include "../../otpch.h"

#include "../../player.h"

#include "../../depotchest.h"
#include "../../game.h"
#include "../../inbox.h"
#include "../../iologindata.h"
#include "../../spells.h"
#include "../../storeinbox.h"
#include "../../vocation.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Chat* g_chat;
extern Game g_game;
extern Spells* g_spells;
extern Vocations g_vocations;

namespace {

int luaDoPlayerAddItem(lua_State* L)
{
	// doPlayerAddItem(cid, itemid, <optional: default: 1> count/subtype, <optional: default: 1> canDropOnMap)
	// doPlayerAddItem(cid, itemid, <optional: default: 1> count, <optional: default: 1> canDropOnMap, <optional:
	// default: 1>subtype)
	const auto& player = tfs::lua::getPlayer(L, 1);
	if (!player) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	uint16_t itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	int32_t count = tfs::lua::getNumber<int32_t>(L, 3, 1);
	bool canDropOnMap = tfs::lua::getBoolean(L, 4, true);
	uint16_t subType = tfs::lua::getNumber<uint16_t>(L, 5, 1);

	const ItemType& it = Item::items[itemId];
	int32_t itemCount;

	auto parameters = lua_gettop(L);
	if (parameters > 4) {
		// subtype already supplied, count then is the amount
		itemCount = std::max<int32_t>(1, count);
	} else if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = static_cast<int32_t>(std::ceil(static_cast<float>(count) / ITEM_STACK_SIZE));
		} else {
			itemCount = 1;
		}
		subType = count;
	} else {
		itemCount = std::max<int32_t>(1, count);
	}

	while (itemCount > 0) {
		uint16_t stackCount = subType;
		if (it.stackable && stackCount > ITEM_STACK_SIZE) {
			stackCount = ITEM_STACK_SIZE;
		}

		const auto& newItem = Item::CreateItem(itemId, stackCount);
		if (!newItem) {
			tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_ITEM_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		if (it.stackable) {
			subType -= stackCount;
		}

		ReturnValue ret = g_game.internalPlayerAddItem(player, newItem, canDropOnMap);
		if (ret != RETURNVALUE_NOERROR) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		if (--itemCount == 0) {
			if (newItem->hasParent()) {
				uint32_t uid = tfs::lua::getScriptEnv()->addThing(newItem);
				tfs::lua::pushNumber(L, uid);
				return 1;
			} else {
				// stackable item stacked with existing object, newItem will be released
				tfs::lua::pushBoolean(L, false);
				return 1;
			}
		}
	}

	tfs::lua::pushBoolean(L, false);
	return 1;
}

int luaPlayerCreate(lua_State* L)
{
	// Player(id or guid or name or userdata)
	std::shared_ptr<Player> player = nullptr;
	if (tfs::lua::isNumber(L, 2)) {
		uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);
		if (id >= CREATURE_ID_MIN && id <= Player::playerIDLimit) {
			player = g_game.getPlayerByID(id);
		} else {
			player = g_game.getPlayerByGUID(id);
		}
	} else if (lua_isstring(L, 2)) {
		ReturnValue ret = g_game.getPlayerByNameWildcard(tfs::lua::getString(L, 2), player);
		if (ret != RETURNVALUE_NOERROR) {
			lua_pushnil(L);
			tfs::lua::pushNumber(L, ret);
			return 2;
		}
	} else if (lua_isuserdata(L, 2)) {
		if (tfs::lua::getUserdataType(L, 2) != tfs::lua::LuaData_Player) {
			lua_pushnil(L);
			return 1;
		}
		player = tfs::lua::getSharedPtr<Player>(L, 2);
	}

	if (player) {
		tfs::lua::pushSharedPtr(L, player);
		tfs::lua::setMetatable(L, -1, "Player");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerIsPlayer(lua_State* L)
{
	// player:isPlayer()
	if (const auto& creature = tfs::lua::getCreature(L, 1)) {
		tfs::lua::pushBoolean(L, creature->asPlayer() != nullptr);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetGuid(lua_State* L)
{
	// player:getGuid()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getGUID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetIp(lua_State* L)
{
	// player:getIp()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushString(L, player->getIP().to_string());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetAccountId(lua_State* L)
{
	// player:getAccountId()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getAccount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetLastLoginSaved(lua_State* L)
{
	// player:getLastLoginSaved()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getLastLoginSaved());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetLastLogout(lua_State* L)
{
	// player:getLastLogout()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getLastLogout());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetAccountType(lua_State* L)
{
	// player:getAccountType()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getAccountType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetAccountType(lua_State* L)
{
	// player:setAccountType(accountType)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setAccountType(tfs::lua::getNumber<AccountType_t>(L, 2));
		IOLoginData::setAccountType(player->getAccount(), player->getAccountType());
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetCapacity(lua_State* L)
{
	// player:getCapacity()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getCapacity());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetCapacity(lua_State* L)
{
	// player:setCapacity(capacity)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setCapacity(tfs::lua::getNumber<uint32_t>(L, 2));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetFreeCapacity(lua_State* L)
{
	// player:getFreeCapacity()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getFreeCapacity());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetDepotChest(lua_State* L)
{
	// player:getDepotChest(depotId[, autoCreate = false])
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t depotId = tfs::lua::getNumber<uint32_t>(L, 2);
	bool autoCreate = tfs::lua::getBoolean(L, 3, false);
	if (const auto& depotChest = player->getDepotChest(depotId, autoCreate)) {
		tfs::lua::pushSharedPtr(L, depotChest);
		tfs::lua::setItemMetatable(L, -1, depotChest);
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int luaPlayerGetInbox(lua_State* L)
{
	// player:getInbox()
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& inbox = player->getInbox()) {
		tfs::lua::pushSharedPtr(L, inbox);
		tfs::lua::setItemMetatable(L, -1, inbox);
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int luaPlayerGetSkullTime(lua_State* L)
{
	// player:getSkullTime()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getSkullTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetSkullTime(lua_State* L)
{
	// player:setSkullTime(skullTime)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setSkullTicks(tfs::lua::getNumber<int64_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetDeathPenalty(lua_State* L)
{
	// player:getDeathPenalty()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getLossPercent() * 100);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetExperience(lua_State* L)
{
	// player:getExperience()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getExperience());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddExperience(lua_State* L)
{
	// player:addExperience(experience[, sendText = false])
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		uint64_t experience = tfs::lua::getNumber<uint64_t>(L, 2);
		bool sendText = tfs::lua::getBoolean(L, 3, false);
		player->addExperience(nullptr, experience, sendText);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerRemoveExperience(lua_State* L)
{
	// player:removeExperience(experience[, sendText = false])
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		uint64_t experience = tfs::lua::getNumber<uint64_t>(L, 2);
		bool sendText = tfs::lua::getBoolean(L, 3, false);
		player->removeExperience(experience, sendText);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetLevel(lua_State* L)
{
	// player:getLevel()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetLevelPercent(lua_State* L)
{
	// player:getLevelPercent()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getLevelPercent());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetMagicLevel(lua_State* L)
{
	// player:getMagicLevel()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getMagicLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetMagicLevelPercent(lua_State* L)
{
	// player:getMagicLevelPercent()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getMagicLevelPercent());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetBaseMagicLevel(lua_State* L)
{
	// player:getBaseMagicLevel()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getBaseMagicLevel());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetMana(lua_State* L)
{
	// player:getMana()
	if (const auto& player = tfs::lua::getSharedPtr<const Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getMana());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddMana(lua_State* L)
{
	// player:addMana(manaChange[, animationOnLoss = false])
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	int32_t manaChange = tfs::lua::getNumber<int32_t>(L, 2);
	bool animationOnLoss = tfs::lua::getBoolean(L, 3, false);
	if (!animationOnLoss && manaChange < 0) {
		player->changeMana(manaChange);
	} else {
		CombatDamage damage;
		damage.primary.value = manaChange;
		damage.origin = ORIGIN_NONE;
		g_game.combatChangeMana(nullptr, player, damage);
	}
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerGetMaxMana(lua_State* L)
{
	// player:getMaxMana()
	if (const auto& player = tfs::lua::getSharedPtr<const Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getMaxMana());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetMaxMana(lua_State* L)
{
	// player:setMaxMana(maxMana)
	if (const auto& player = tfs::lua::getPlayer(L, 1)) {
		player->setMaxMana(tfs::lua::getNumber<int32_t>(L, 2));
		player->setMana(std::min<int32_t>(player->getMana(), player->getMaxMana()));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetManaShieldBar(lua_State* L)
{
	// player:setManaShieldBar(capacity, value)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setMaxManaShieldBar(tfs::lua::getNumber<uint16_t>(L, 2));
		player->setManaShieldBar(tfs::lua::getNumber<uint16_t>(L, 3));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetManaSpent(lua_State* L)
{
	// player:getManaSpent()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getSpentMana());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddManaSpent(lua_State* L)
{
	// player:addManaSpent(amount)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->addManaSpent(tfs::lua::getNumber<uint64_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerRemoveManaSpent(lua_State* L)
{
	// player:removeManaSpent(amount[, notify = true])
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->removeManaSpent(tfs::lua::getNumber<uint64_t>(L, 2), tfs::lua::getBoolean(L, 3, true));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetBaseMaxHealth(lua_State* L)
{
	// player:getBaseMaxHealth()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getMaxHealth());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetBaseMaxMana(lua_State* L)
{
	// player:getBaseMaxMana()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getMaxMana());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetSkillLevel(lua_State* L)
{
	// player:getSkillLevel(skillType)
	skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		tfs::lua::pushNumber(L, player->getSkillLevel(skillType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetEffectiveSkillLevel(lua_State* L)
{
	// player:getEffectiveSkillLevel(skillType)
	skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		tfs::lua::pushNumber(L, player->getSkillLevel(skillType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetSkillPercent(lua_State* L)
{
	// player:getSkillPercent(skillType)
	skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		tfs::lua::pushNumber(L, player->getSkillPercent(skillType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetSkillTries(lua_State* L)
{
	// player:getSkillTries(skillType)
	skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (player && skillType <= SKILL_LAST) {
		tfs::lua::pushNumber(L, player->getSkillTries(skillType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddSkillTries(lua_State* L)
{
	// player:addSkillTries(skillType, tries)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
		uint64_t tries = tfs::lua::getNumber<uint64_t>(L, 3);
		player->addSkillAdvance(skillType, tries);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerRemoveSkillTries(lua_State* L)
{
	// player:removeSkillTries(skillType, tries[, notify = true])
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
		uint64_t tries = tfs::lua::getNumber<uint64_t>(L, 3);
		player->removeSkillTries(skillType, tries, tfs::lua::getBoolean(L, 4, true));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetSpecialSkill(lua_State* L)
{
	// player:getSpecialSkill(specialSkillType)
	SpecialSkills_t specialSkillType = tfs::lua::getNumber<SpecialSkills_t>(L, 2);
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (player && specialSkillType <= SPECIALSKILL_LAST) {
		tfs::lua::pushNumber(L, player->getSpecialSkill(specialSkillType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddSpecialSkill(lua_State* L)
{
	// player:addSpecialSkill(specialSkillType, value)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	SpecialSkills_t specialSkillType = tfs::lua::getNumber<SpecialSkills_t>(L, 2);
	if (specialSkillType > SPECIALSKILL_LAST) {
		lua_pushnil(L);
		return 1;
	}

	player->setVarSpecialSkill(specialSkillType, tfs::lua::getNumber<int32_t>(L, 3));
	player->sendSkills();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerAddOfflineTrainingTime(lua_State* L)
{
	// player:addOfflineTrainingTime(time)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		int32_t time = tfs::lua::getNumber<int32_t>(L, 2);
		player->addOfflineTrainingTime(time);
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetOfflineTrainingTime(lua_State* L)
{
	// player:getOfflineTrainingTime()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getOfflineTrainingTime());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerRemoveOfflineTrainingTime(lua_State* L)
{
	// player:removeOfflineTrainingTime(time)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		int32_t time = tfs::lua::getNumber<int32_t>(L, 2);
		player->removeOfflineTrainingTime(time);
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddOfflineTrainingTries(lua_State* L)
{
	// player:addOfflineTrainingTries(skillType, tries)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
		uint64_t tries = tfs::lua::getNumber<uint64_t>(L, 3);
		tfs::lua::pushBoolean(L, player->addOfflineTrainingTries(skillType, tries));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetOfflineTrainingSkill(lua_State* L)
{
	// player:getOfflineTrainingSkill()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getOfflineTrainingSkill());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetOfflineTrainingSkill(lua_State* L)
{
	// player:setOfflineTrainingSkill(skillId)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		int32_t skillId = tfs::lua::getNumber<int32_t>(L, 2);
		player->setOfflineTrainingSkill(skillId);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetItemCount(lua_State* L)
{
	// player:getItemCount(itemId[, subType = -1])
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (tfs::lua::isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t subType = tfs::lua::getNumber<int32_t>(L, 3, -1);
	tfs::lua::pushNumber(L, player->getItemTypeCount(itemId, subType));
	return 1;
}

int luaPlayerGetItemById(lua_State* L)
{
	// player:getItemById(itemId, deepSearch[, subType = -1])
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (tfs::lua::isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}
	bool deepSearch = tfs::lua::getBoolean(L, 3);
	int32_t subType = tfs::lua::getNumber<int32_t>(L, 4, -1);

	if (auto item = g_game.findItemOfType(player, itemId, deepSearch, subType)) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetVocation(lua_State* L)
{
	// player:getVocation()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushUserdata(L, player->getVocation());
		tfs::lua::setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetVocation(lua_State* L)
{
	// player:setVocation(id or name or userdata)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Vocation* vocation = nullptr;
	if (tfs::lua::isNumber(L, 2)) {
		vocation = g_vocations.getVocation(tfs::lua::getNumber<uint16_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		vocation = g_vocations.getVocation(g_vocations.getVocationId(tfs::lua::getString(L, 2)));
	} else if (lua_isuserdata(L, 2)) {
		vocation = tfs::lua::getUserdata<Vocation>(L, 2);
	}

	if (!vocation) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->setVocation(vocation->getId());
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerGetSex(lua_State* L)
{
	// player:getSex()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getSex());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetSex(lua_State* L)
{
	// player:setSex(newSex)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		PlayerSex_t newSex = tfs::lua::getNumber<PlayerSex_t>(L, 2);
		player->setSex(newSex);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetTown(lua_State* L)
{
	// player:getTown()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushTown(L, *player->getTown());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetTown(lua_State* L)
{
	// player:setTown(town)
	if (!lua_istable(L, 2)) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const Town* town =
	    g_game.map.towns.getTown(tfs::lua::getField<uint32_t>(L, 2, "id", std::numeric_limits<uint32_t>::max()));
	if (!town) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setTown(town);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetGuild(lua_State* L)
{
	// player:getGuild()
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& guild = player->getGuild()) {
		tfs::lua::pushSharedPtr(L, guild);
		tfs::lua::setMetatable(L, -1, "Guild");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetGuild(lua_State* L)
{
	// player:setGuild(guild)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	player->setGuild(tfs::lua::getSharedPtr<Guild>(L, 2));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerGetGuildLevel(lua_State* L)
{
	// player:getGuildLevel()
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (player && player->getGuild()) {
		tfs::lua::pushNumber(L, player->getGuildRank()->level);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetGuildLevel(lua_State* L)
{
	// player:setGuildLevel(level)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const auto& guild = player->getGuild();
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t level = tfs::lua::getNumber<uint8_t>(L, 2);
	if (auto rank = guild->getRankByLevel(level)) {
		player->setGuildRank(rank);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetGuildNick(lua_State* L)
{
	// player:getGuildNick()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushString(L, player->getGuildNick());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetGuildNick(lua_State* L)
{
	// player:setGuildNick(nick)
	const std::string& nick = tfs::lua::getString(L, 2);
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setGuildNick(nick);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetGroup(lua_State* L)
{
	// player:getGroup()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushUserdata(L, player->getGroup());
		tfs::lua::setMetatable(L, -1, "Group");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetGroup(lua_State* L)
{
	// player:setGroup(group)
	Group* group = tfs::lua::getUserdata<Group>(L, 2);
	if (!group) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setGroup(group);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetStamina(lua_State* L)
{
	// player:getStamina()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getStaminaMinutes());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetStamina(lua_State* L)
{
	// player:setStamina(stamina)
	uint16_t stamina = tfs::lua::getNumber<uint16_t>(L, 2);
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setStaminaMinutes(stamina);
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetSoul(lua_State* L)
{
	// player:getSoul()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getSoul());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddSoul(lua_State* L)
{
	// player:addSoul(soulChange)
	int32_t soulChange = tfs::lua::getNumber<int32_t>(L, 2);
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->changeSoul(soulChange);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetMaxSoul(lua_State* L)
{
	// player:getMaxSoul()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		if (const auto& vocation = player->getVocation()) {
			tfs::lua::pushNumber(L, vocation->getSoulMax());
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

int luaPlayerGetBankBalance(lua_State* L)
{
	// player:getBankBalance()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getBankBalance());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetBankBalance(lua_State* L)
{
	// player:setBankBalance(bankBalance)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	int64_t balance = tfs::lua::getNumber<int64_t>(L, 2);
	if (balance < 0) {
		tfs::lua::reportError(L, "Invalid bank balance value.");
		lua_pushnil(L);
		return 1;
	}

	player->setBankBalance(balance);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerAddItem(lua_State* L)
{
	// player:addItem(itemId[, count = 1[, canDropOnMap = true[, subType = 1[, slot = CONST_SLOT_WHEREEVER]]]])
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	uint16_t itemId;
	if (tfs::lua::isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	int32_t count = tfs::lua::getNumber<int32_t>(L, 3, 1);
	int32_t subType = tfs::lua::getNumber<int32_t>(L, 5, 1);

	const ItemType& it = Item::items[itemId];

	int32_t itemCount = 1;
	int parameters = lua_gettop(L);
	if (parameters >= 5) {
		itemCount = std::max<int32_t>(1, count);
	} else if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = std::ceil(count / static_cast<float>(ITEM_STACK_SIZE));
		}

		subType = count;
	} else {
		itemCount = std::max<int32_t>(1, count);
	}

	bool hasTable = itemCount > 1;
	if (hasTable) {
		lua_newtable(L);
	} else if (itemCount == 0) {
		lua_pushnil(L);
		return 1;
	}

	bool canDropOnMap = tfs::lua::getBoolean(L, 4, true);
	slots_t slot = tfs::lua::getNumber<slots_t>(L, 6, CONST_SLOT_WHEREEVER);
	for (int32_t i = 1; i <= itemCount; ++i) {
		int32_t stackCount = subType;
		if (it.stackable) {
			stackCount = std::min<int32_t>(stackCount, ITEM_STACK_SIZE);
			subType -= stackCount;
		}

		const auto& item = Item::CreateItem(itemId, stackCount);
		if (!item) {
			if (!hasTable) {
				lua_pushnil(L);
			}
			return 1;
		}

		ReturnValue ret = g_game.internalPlayerAddItem(player, item, canDropOnMap, slot);
		if (ret != RETURNVALUE_NOERROR) {
			if (!hasTable) {
				lua_pushnil(L);
			}
			return 1;
		}

		if (hasTable) {
			tfs::lua::pushNumber(L, i);
			tfs::lua::pushSharedPtr(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
			lua_settable(L, -3);
		} else {
			tfs::lua::pushSharedPtr(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
		}
	}
	return 1;
}

int luaPlayerAddItemEx(lua_State* L)
{
	// player:addItemEx(item[, canDropOnMap = false[, index = INDEX_WHEREEVER[, flags = 0]]])
	// player:addItemEx(item[, canDropOnMap = true[, slot = CONST_SLOT_WHEREEVER]])
	const auto& item = tfs::lua::getSharedPtr<Item>(L, 2);
	if (!item) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_ITEM_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (item->hasParent()) {
		tfs::lua::reportError(L, "Item already has a parent");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	bool canDropOnMap = tfs::lua::getBoolean(L, 3, false);
	if (canDropOnMap) {
		slots_t slot = tfs::lua::getNumber<slots_t>(L, 4, CONST_SLOT_WHEREEVER);
		tfs::lua::pushNumber(L, g_game.internalPlayerAddItem(player, item, true, slot));
	} else {
		int32_t index = tfs::lua::getNumber<int32_t>(L, 4, INDEX_WHEREEVER);
		uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 5, 0);
		tfs::lua::pushNumber(L, g_game.internalAddItem(player, item, index, flags));
	}
	return 1;
}

int luaPlayerRemoveItem(lua_State* L)
{
	// player:removeItem(itemId, count[, subType = -1[, ignoreEquipped = false]])
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (tfs::lua::isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	uint32_t count = tfs::lua::getNumber<uint32_t>(L, 3);
	int32_t subType = tfs::lua::getNumber<int32_t>(L, 4, -1);
	bool ignoreEquipped = tfs::lua::getBoolean(L, 5, false);
	tfs::lua::pushBoolean(L, player->removeItemOfType(itemId, count, subType, ignoreEquipped));
	return 1;
}

int luaPlayerSendSupplyUsed(lua_State* L)
{
	// player:sendSupplyUsed(item)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const auto& item = tfs::lua::getSharedPtr<Item>(L, 2);
	if (!item) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_ITEM_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->sendSupplyUsed(item->getClientID());
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerGetMoney(lua_State* L)
{
	// player:getMoney()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getMoney());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddMoney(lua_State* L)
{
	// player:addMoney(money)
	uint64_t money = tfs::lua::getNumber<uint64_t>(L, 2);
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		g_game.addMoney(player, money);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerRemoveMoney(lua_State* L)
{
	// player:removeMoney(money)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		uint64_t money = tfs::lua::getNumber<uint64_t>(L, 2);
		tfs::lua::pushBoolean(L, g_game.removeMoney(player, money));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerShowTextDialog(lua_State* L)
{
	// player:showTextDialog(id or name or userdata[, text[, canWrite[, length]]])
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	int32_t length = tfs::lua::getNumber<int32_t>(L, 5, -1);
	bool canWrite = tfs::lua::getBoolean(L, 4, false);
	std::string text;

	int parameters = lua_gettop(L);
	if (parameters >= 3) {
		text = tfs::lua::getString(L, 3);
	}

	std::shared_ptr<Item> item = nullptr;
	if (tfs::lua::isNumber(L, 2)) {
		item = Item::CreateItem(tfs::lua::getNumber<uint16_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		item = Item::CreateItem(Item::items.getItemIdByName(tfs::lua::getString(L, 2)));
	} else if (lua_isuserdata(L, 2)) {
		if (tfs::lua::getUserdataType(L, 2) != tfs::lua::LuaData_Item) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		item = tfs::lua::getSharedPtr<Item>(L, 2);
	}

	if (!item) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_ITEM_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (length < 0) {
		length = Item::items[item->getID()].maxTextLen;
	}

	if (!text.empty()) {
		item->setText(text);
		length = std::max<int32_t>(text.size(), length);
	}

	item->setParent(player);
	tfs::lua::pushNumber(L, player->setWriteItem(item, length));
	player->sendTextWindow(item, length, canWrite);
	return 1;
}

int luaPlayerSendTextMessage(lua_State* L)
{
	// player:sendTextMessage(type, text[, position, primaryValue = 0, primaryColor = TEXTCOLOR_NONE[,
	// secondaryValue = 0, secondaryColor = TEXTCOLOR_NONE]]) player:sendTextMessage(type, text, channelId)

	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	int parameters = lua_gettop(L);

	TextMessage message(tfs::lua::getNumber<MessageClasses>(L, 2), tfs::lua::getString(L, 3));
	if (parameters == 4) {
		uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 4);
		ChatChannel* channel = g_chat->getChannel(player, channelId);
		if (!channel || !channel->hasUser(player)) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		message.channelId = channelId;
	} else {
		if (parameters >= 6) {
			message.position = tfs::lua::getPosition(L, 4);
			message.primary.value = tfs::lua::getNumber<int32_t>(L, 5);
			message.primary.color = tfs::lua::getNumber<TextColor_t>(L, 6);
		}

		if (parameters >= 8) {
			message.secondary.value = tfs::lua::getNumber<int32_t>(L, 7);
			message.secondary.color = tfs::lua::getNumber<TextColor_t>(L, 8);
		}
	}

	player->sendTextMessage(message);
	tfs::lua::pushBoolean(L, true);

	return 1;
}

int luaPlayerSendChannelMessage(lua_State* L)
{
	// player:sendChannelMessage(author, text, type, channelId)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 5);
	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 4);
	const std::string& text = tfs::lua::getString(L, 3);
	const std::string& author = tfs::lua::getString(L, 2);
	player->sendChannelMessage(author, text, type, channelId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerSendPrivateMessage(lua_State* L)
{
	// player:sendPrivateMessage(speaker, text[, type])
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const auto& speaker = tfs::lua::getSharedPtr<const Player>(L, 2);
	const std::string& text = tfs::lua::getString(L, 3);
	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 4, TALKTYPE_PRIVATE_FROM);
	player->sendPrivateMessage(speaker, type, text);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerChannelSay(lua_State* L)
{
	// player:channelSay(speaker, type, text, channelId)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	auto speaker = tfs::lua::getCreature(L, 2);
	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 3);
	const std::string& text = tfs::lua::getString(L, 4);
	uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 5);
	player->sendToChannel(speaker, type, text, channelId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerOpenChannel(lua_State* L)
{
	// player:openChannel(channelId)
	uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 2);
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		g_game.playerOpenChannel(player->getID(), channelId);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerLeaveChannel(lua_State* L)
{
	// player:leaveChannel(channelId)
	uint16_t channelId = tfs::lua::getNumber<uint16_t>(L, 2);
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		g_game.playerCloseChannel(player->getID(), channelId);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetSlotItem(lua_State* L)
{
	// player:getSlotItem(slot)
	const auto& player = tfs::lua::getSharedPtr<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t slot = tfs::lua::getNumber<uint32_t>(L, 2);
	const auto& thing = player->getThing(slot);
	if (!thing) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& item = thing->asItem()) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetParty(lua_State* L)
{
	// player:getParty()
	const auto& player = tfs::lua::getSharedPtr<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& party = player->getParty()) {
		tfs::lua::pushSharedPtr(L, party);
		tfs::lua::setMetatable(L, -1, "Party");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddOutfit(lua_State* L)
{
	// player:addOutfit(lookType)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->addOutfit(tfs::lua::getNumber<uint16_t>(L, 2), 0);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddOutfitAddon(lua_State* L)
{
	// player:addOutfitAddon(lookType, addon)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		uint8_t addon = tfs::lua::getNumber<uint8_t>(L, 3);
		player->addOutfit(lookType, addon);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerRemoveOutfit(lua_State* L)
{
	// player:removeOutfit(lookType)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		tfs::lua::pushBoolean(L, player->removeOutfit(lookType));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerRemoveOutfitAddon(lua_State* L)
{
	// player:removeOutfitAddon(lookType, addon)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		uint8_t addon = tfs::lua::getNumber<uint8_t>(L, 3);
		tfs::lua::pushBoolean(L, player->removeOutfitAddon(lookType, addon));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerHasOutfit(lua_State* L)
{
	// player:hasOutfit(lookType[, addon = 0])
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		uint8_t addon = tfs::lua::getNumber<uint8_t>(L, 3, 0);
		tfs::lua::pushBoolean(L, player->hasOutfit(lookType, addon));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerCanWearOutfit(lua_State* L)
{
	// player:canWearOutfit(lookType[, addon = 0])
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		uint16_t lookType = tfs::lua::getNumber<uint16_t>(L, 2);
		uint8_t addon = tfs::lua::getNumber<uint8_t>(L, 3, 0);
		tfs::lua::pushBoolean(L, player->canWear(lookType, addon));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSendOutfitWindow(lua_State* L)
{
	// player:sendOutfitWindow()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->sendOutfitWindow();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSendEditPodium(lua_State* L)
{
	// player:sendEditPodium(item)
	auto player = tfs::lua::getSharedPtr<Player>(L, 1);
	const auto& item = tfs::lua::getSharedPtr<Item>(L, 2);
	if (player && item) {
		player->sendPodiumWindow(item);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddMount(lua_State* L)
{
	// player:addMount(mountId or mountName)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t mountId;
	if (tfs::lua::isNumber(L, 2)) {
		mountId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		Mount* mount = g_game.mounts.getMountByName(tfs::lua::getString(L, 2));
		if (!mount) {
			lua_pushnil(L);
			return 1;
		}
		mountId = mount->id;
	}
	tfs::lua::pushBoolean(L, player->tameMount(mountId));
	return 1;
}

int luaPlayerRemoveMount(lua_State* L)
{
	// player:removeMount(mountId or mountName)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t mountId;
	if (tfs::lua::isNumber(L, 2)) {
		mountId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		Mount* mount = g_game.mounts.getMountByName(tfs::lua::getString(L, 2));
		if (!mount) {
			lua_pushnil(L);
			return 1;
		}
		mountId = mount->id;
	}
	tfs::lua::pushBoolean(L, player->untameMount(mountId));
	return 1;
}

int luaPlayerHasMount(lua_State* L)
{
	// player:hasMount(mountId or mountName)
	const auto& player = tfs::lua::getSharedPtr<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Mount* mount = nullptr;
	if (tfs::lua::isNumber(L, 2)) {
		mount = g_game.mounts.getMountByID(tfs::lua::getNumber<uint16_t>(L, 2));
	} else {
		mount = g_game.mounts.getMountByName(tfs::lua::getString(L, 2));
	}

	if (mount) {
		tfs::lua::pushBoolean(L, player->hasMount(mount));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerToggleMount(lua_State* L)
{
	// player:toggleMount(mount)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	bool mount = tfs::lua::getBoolean(L, 2);
	tfs::lua::pushBoolean(L, player->toggleMount(mount));
	return 1;
}

int luaPlayerGetPremiumEndsAt(lua_State* L)
{
	// player:getPremiumEndsAt()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getPremiumEndsAt());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetPremiumEndsAt(lua_State* L)
{
	// player:setPremiumEndsAt(timestamp)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	time_t timestamp = tfs::lua::getNumber<time_t>(L, 2);

	player->setPremiumTime(timestamp);
	IOLoginData::updatePremiumTime(player->getAccount(), timestamp);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerHasBlessing(lua_State* L)
{
	// player:hasBlessing(blessing)
	uint8_t blessing = tfs::lua::getNumber<uint8_t>(L, 2) - 1;
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushBoolean(L, player->hasBlessing(blessing));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddBlessing(lua_State* L)
{
	// player:addBlessing(blessing)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t blessing = tfs::lua::getNumber<uint8_t>(L, 2) - 1;
	if (player->hasBlessing(blessing)) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->addBlessing(blessing);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerRemoveBlessing(lua_State* L)
{
	// player:removeBlessing(blessing)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t blessing = tfs::lua::getNumber<uint8_t>(L, 2) - 1;
	if (!player->hasBlessing(blessing)) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->removeBlessing(blessing);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerCanLearnSpell(lua_State* L)
{
	// player:canLearnSpell(spellName)
	const auto& player = tfs::lua::getSharedPtr<const Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const std::string& spellName = tfs::lua::getString(L, 2);
	InstantSpell* spell = g_spells->getInstantSpellByName(spellName);
	if (!spell) {
		tfs::lua::reportError(L, "Spell \"" + spellName + "\" not found");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (player->hasFlag(PlayerFlag_IgnoreSpellCheck)) {
		tfs::lua::pushBoolean(L, true);
		return 1;
	}

	if (!spell->hasVocationSpellMap(player->getVocationId())) {
		tfs::lua::pushBoolean(L, false);
	} else if (player->getLevel() < spell->getLevel()) {
		tfs::lua::pushBoolean(L, false);
	} else if (player->getMagicLevel() < spell->getMagicLevel()) {
		tfs::lua::pushBoolean(L, false);
	} else {
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int luaPlayerLearnSpell(lua_State* L)
{
	// player:learnSpell(spellName)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		const std::string& spellName = tfs::lua::getString(L, 2);
		player->learnInstantSpell(spellName);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerForgetSpell(lua_State* L)
{
	// player:forgetSpell(spellName)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		const std::string& spellName = tfs::lua::getString(L, 2);
		player->forgetInstantSpell(spellName);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerHasLearnedSpell(lua_State* L)
{
	// player:hasLearnedSpell(spellName)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		const std::string& spellName = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, player->hasLearnedInstantSpell(spellName));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSendTutorial(lua_State* L)
{
	// player:sendTutorial(tutorialId)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		uint8_t tutorialId = tfs::lua::getNumber<uint8_t>(L, 2);
		player->sendTutorial(tutorialId);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerAddMapMark(lua_State* L)
{
	// player:addMapMark(position, type, description)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		const Position& position = tfs::lua::getPosition(L, 2);
		uint8_t type = tfs::lua::getNumber<uint8_t>(L, 3);
		const std::string& description = tfs::lua::getString(L, 4);
		player->sendAddMarker(position, type, description);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSave(lua_State* L)
{
	// player:save()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setLoginPosition(player->getPosition());
		tfs::lua::pushBoolean(L, IOLoginData::savePlayer(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerPopupFYI(lua_State* L)
{
	// player:popupFYI(message)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		const std::string& message = tfs::lua::getString(L, 2);
		player->sendFYIBox(message);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerIsPzLocked(lua_State* L)
{
	// player:isPzLocked()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushBoolean(L, player->isPzLocked());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetClient(lua_State* L)
{
	// player:getClient()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		lua_createtable(L, 0, 2);
		tfs::lua::setField(L, "version", player->getProtocolVersion());
		tfs::lua::setField(L, "os", player->getOperatingSystem());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetHouse(lua_State* L)
{
	// player:getHouse()
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const auto& house = g_game.getHouseByPlayerId(player->getGUID());
	if (house) {
		tfs::lua::pushSharedPtr(L, house);
		tfs::lua::setMetatable(L, -1, "House");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSendHouseWindow(lua_State* L)
{
	// player:sendHouseWindow(house, listId)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const auto& house = tfs::lua::getSharedPtr<House>(L, 2);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 3);
	player->sendHouseWindow(house, listId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerSetEditHouse(lua_State* L)
{
	// player:setEditHouse(house, listId)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const auto& house = tfs::lua::getSharedPtr<House>(L, 2);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 3);
	player->setEditHouse(house, listId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerSetGhostMode(lua_State* L)
{
	// player:setGhostMode(enabled[, magicEffect = CONST_ME_TELEPORT])
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	bool enabled = tfs::lua::getBoolean(L, 2);
	if (player->isInGhostMode() == enabled) {
		tfs::lua::pushBoolean(L, true);
		return 1;
	}

	MagicEffectClasses magicEffect = tfs::lua::getNumber<MagicEffectClasses>(L, 3, CONST_ME_TELEPORT);

	player->switchGhostMode();

	const auto& tile = player->getTile();
	const Position& position = player->getPosition();
	const bool isInvisible = player->isInvisible();

	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, position, true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->asPlayer() != nullptr);

		const auto& spectatorPlayer = std::static_pointer_cast<Player>(spectator);
		if (spectatorPlayer != player && !spectatorPlayer->isAccessPlayer()) {
			if (enabled) {
				spectatorPlayer->sendRemoveTileCreature(player, position,
				                                        tile->getClientIndexOfCreature(spectatorPlayer, player));
			} else {
				spectatorPlayer->sendAddCreature(player, position, magicEffect);
			}
		} else {
			if (isInvisible) {
				continue;
			}

			spectatorPlayer->sendCreatureChangeVisible(player, !enabled);
		}
	}

	if (player->isInGhostMode()) {
		for (const auto& onlinePlayer : g_game.getPlayers() | tfs::views::lock_weak_ptrs) {
			if (!onlinePlayer->isAccessPlayer()) {
				onlinePlayer->notifyStatusChange(player, VIPSTATUS_OFFLINE);
			}
		}
		IOLoginData::updateOnlineStatus(player->getGUID(), false);
	} else {
		for (const auto& onlinePlayer : g_game.getPlayers() | tfs::views::lock_weak_ptrs) {
			if (!onlinePlayer->isAccessPlayer()) {
				onlinePlayer->notifyStatusChange(player, VIPSTATUS_ONLINE);
			}
		}
		IOLoginData::updateOnlineStatus(player->getGUID(), true);
	}
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerGetContainerId(lua_State* L)
{
	// player:getContainerId(container)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& container = tfs::lua::getSharedPtr<Container>(L, 2)) {
		tfs::lua::pushNumber(L, player->getContainerID(container));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetContainerById(lua_State* L)
{
	// player:getContainerById(id)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& container = player->getContainerByID(tfs::lua::getNumber<uint8_t>(L, 2))) {
		tfs::lua::pushSharedPtr(L, container);
		tfs::lua::setMetatable(L, -1, "Container");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetContainerIndex(lua_State* L)
{
	// player:getContainerIndex(id)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getContainerIndex(tfs::lua::getNumber<uint8_t>(L, 2)));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetRuneSpells(lua_State* L)
{
	// player:getRuneSpells()
	Player* player = tfs::lua::getUserdata<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	auto runeSpells = g_spells->getRuneSpells();

	std::vector<RuneSpell*> spells;
	for (auto& spell : runeSpells | std::views::values) {
		if (spell.canUse(player)) {
			spells.push_back(&spell);
		}
	}

	lua_createtable(L, spells.size(), 0);

	int index = 0;
	for (auto& spell : spells) {
		tfs::lua::pushUserdata<Spell>(L, spell);
		tfs::lua::setMetatable(L, -1, "Spell");
		lua_rawseti(L, -2, ++index);
	}

	return 1;
}

int luaPlayerGetInstantSpells(lua_State* L)
{
	// player:getInstantSpells()
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	std::vector<const InstantSpell*> spells;
	for (auto&& spell : g_spells->getInstantSpells() | std::views::values | std::views::as_const) {
		if (spell.canCast(player)) {
			spells.push_back(&spell);
		}
	}

	lua_createtable(L, spells.size(), 0);

	int index = 0;
	for (auto&& spell : spells | std::views::as_const) {
		lua_createtable(L, 0, 7);

		tfs::lua::setField(L, "name", spell->getName());
		tfs::lua::setField(L, "words", spell->getWords());
		tfs::lua::setField(L, "level", spell->getLevel());
		tfs::lua::setField(L, "mlevel", spell->getMagicLevel());
		tfs::lua::setField(L, "mana", spell->getMana());
		tfs::lua::setField(L, "manapercent", spell->getManaPercent());
		tfs::lua::setField(L, "params", spell->getHasParam());

		tfs::lua::setMetatable(L, -1, "Spell");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaPlayerCanCast(lua_State* L)
{
	// player:canCast(spell)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	InstantSpell* spell = tfs::lua::getUserdata<InstantSpell>(L, 2);
	if (player && spell) {
		tfs::lua::pushBoolean(L, spell->canCast(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerHasChaseMode(lua_State* L)
{
	// player:hasChaseMode()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushBoolean(L, player->getChaseMode());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerHasSecureMode(lua_State* L)
{
	// player:hasSecureMode()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushBoolean(L, player->getSecureMode());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetFightMode(lua_State* L)
{
	// player:getFightMode()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getFightMode());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetStoreInbox(lua_State* L)
{
	// player:getStoreInbox()
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const auto& storeInbox = player->getStoreInbox();
	if (!storeInbox) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushSharedPtr(L, storeInbox);
	tfs::lua::setMetatable(L, -1, "Container");
	return 1;
}

int luaPlayerIsNearDepotBox(lua_State* L)
{
	// player:isNearDepotBox()
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, player->isNearDepotBox());
	return 1;
}

int luaPlayerGetIdleTime(lua_State* L)
{
	// player:getIdleTime()
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushNumber(L, player->getIdleTime());
	return 1;
}

int luaPlayerSetIdleTime(lua_State* L)
{
	// player:setIdleTime(ms)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	player->setIdleTime(tfs::lua::getNumber<uint32_t>(L, 2));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerResetIdleTime(lua_State* L)
{
	// player:resetIdleTime()
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	player->resetIdleTime();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerSendCreatureSquare(lua_State* L)
{
	// player:sendCreatureSquare(creature, color)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 1);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	const auto& creature = tfs::lua::getCreature(L, 2);
	if (!creature) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	player->sendCreatureSquare(creature, tfs::lua::getNumber<SquareColor_t>(L, 3));
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPlayerGetClientExpDisplay(lua_State* L)
{
	// player:getClientExpDisplay()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getClientExpDisplay());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetClientExpDisplay(lua_State* L)
{
	// player:setClientExpDisplay(value)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setClientExpDisplay(tfs::lua::getNumber<uint16_t>(L, 2));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetClientStaminaBonusDisplay(lua_State* L)
{
	// player:getClientStaminaBonusDisplay()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getClientStaminaBonusDisplay());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetClientStaminaBonusDisplay(lua_State* L)
{
	// player:setClientStaminaBonusDisplay(value)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setClientStaminaBonusDisplay(tfs::lua::getNumber<uint16_t>(L, 2));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerGetClientLowLevelBonusDisplay(lua_State* L)
{
	// player:getClientLowLevelBonusDisplay()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		tfs::lua::pushNumber(L, player->getClientLowLevelBonusDisplay());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSetClientLowLevelBonusDisplay(lua_State* L)
{
	// player:setClientLowLevelBonusDisplay(value)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->setClientLowLevelBonusDisplay(tfs::lua::getNumber<uint16_t>(L, 2));
		player->sendStats();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSendResourceBalance(lua_State* L)
{
	// player:sendResourceBalance(resource, amount)
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		const ResourceTypes_t resourceType = tfs::lua::getNumber<ResourceTypes_t>(L, 2);
		uint64_t amount = tfs::lua::getNumber<uint64_t>(L, 3);
		player->sendResourceBalance(resourceType, amount);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPlayerSendEnterMarket(lua_State* L)
{
	// player:sendEnterMarket()
	if (const auto& player = tfs::lua::getSharedPtr<Player>(L, 1)) {
		player->sendMarketEnter();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerPlayer(LuaScriptInterface& lsi)
{
	registerEnum(lsi, VOCATION_NONE);

	registerEnum(lsi, SKILL_FIST);
	registerEnum(lsi, SKILL_CLUB);
	registerEnum(lsi, SKILL_SWORD);
	registerEnum(lsi, SKILL_AXE);
	registerEnum(lsi, SKILL_DISTANCE);
	registerEnum(lsi, SKILL_SHIELD);
	registerEnum(lsi, SKILL_FISHING);
	registerEnum(lsi, SKILL_MAGLEVEL);
	registerEnum(lsi, SKILL_LEVEL);

	registerEnum(lsi, SPECIALSKILL_CRITICALHITCHANCE);
	registerEnum(lsi, SPECIALSKILL_CRITICALHITAMOUNT);
	registerEnum(lsi, SPECIALSKILL_LIFELEECHCHANCE);
	registerEnum(lsi, SPECIALSKILL_LIFELEECHAMOUNT);
	registerEnum(lsi, SPECIALSKILL_MANALEECHCHANCE);
	registerEnum(lsi, SPECIALSKILL_MANALEECHAMOUNT);

	registerEnum(lsi, STAT_MAXHITPOINTS);
	registerEnum(lsi, STAT_MAXMANAPOINTS);
	registerEnum(lsi, STAT_SOULPOINTS);
	registerEnum(lsi, STAT_MAGICPOINTS);

	registerEnum(lsi, PLAYERSEX_FEMALE);
	registerEnum(lsi, PLAYERSEX_MALE);

	registerEnum(lsi, FIGHTMODE_ATTACK);
	registerEnum(lsi, FIGHTMODE_BALANCED);
	registerEnum(lsi, FIGHTMODE_DEFENSE);

	registerEnum(lsi, RESOURCE_BANK_BALANCE);
	registerEnum(lsi, RESOURCE_GOLD_EQUIPPED);
	registerEnum(lsi, RESOURCE_PREY_WILDCARDS);
	registerEnum(lsi, RESOURCE_DAILYREWARD_STREAK);
	registerEnum(lsi, RESOURCE_DAILYREWARD_JOKERS);

	// doPlayerAddItem(uid, itemid, <optional: default: 1> count/subtype)
	// doPlayerAddItem(cid, itemid, <optional: default: 1> count, <optional: default: 1> canDropOnMap, <optional:
	// default: 1>subtype) Returns uid of the created item
	lsi.registerGlobalMethod("doPlayerAddItem", luaDoPlayerAddItem);

	lsi.registerClass("Player", "Creature", luaPlayerCreate);
	lsi.registerMetaMethod("Player", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("Player", "isPlayer", luaPlayerIsPlayer);

	lsi.registerMethod("Player", "getGuid", luaPlayerGetGuid);
	lsi.registerMethod("Player", "getIp", luaPlayerGetIp);
	lsi.registerMethod("Player", "getAccountId", luaPlayerGetAccountId);
	lsi.registerMethod("Player", "getLastLoginSaved", luaPlayerGetLastLoginSaved);
	lsi.registerMethod("Player", "getLastLogout", luaPlayerGetLastLogout);

	lsi.registerMethod("Player", "getAccountType", luaPlayerGetAccountType);
	lsi.registerMethod("Player", "setAccountType", luaPlayerSetAccountType);

	lsi.registerMethod("Player", "getCapacity", luaPlayerGetCapacity);
	lsi.registerMethod("Player", "setCapacity", luaPlayerSetCapacity);

	lsi.registerMethod("Player", "getFreeCapacity", luaPlayerGetFreeCapacity);

	lsi.registerMethod("Player", "getDepotChest", luaPlayerGetDepotChest);
	lsi.registerMethod("Player", "getInbox", luaPlayerGetInbox);

	lsi.registerMethod("Player", "getSkullTime", luaPlayerGetSkullTime);
	lsi.registerMethod("Player", "setSkullTime", luaPlayerSetSkullTime);
	lsi.registerMethod("Player", "getDeathPenalty", luaPlayerGetDeathPenalty);

	lsi.registerMethod("Player", "getExperience", luaPlayerGetExperience);
	lsi.registerMethod("Player", "addExperience", luaPlayerAddExperience);
	lsi.registerMethod("Player", "removeExperience", luaPlayerRemoveExperience);
	lsi.registerMethod("Player", "getLevel", luaPlayerGetLevel);
	lsi.registerMethod("Player", "getLevelPercent", luaPlayerGetLevelPercent);

	lsi.registerMethod("Player", "getMagicLevel", luaPlayerGetMagicLevel);
	lsi.registerMethod("Player", "getMagicLevelPercent", luaPlayerGetMagicLevelPercent);
	lsi.registerMethod("Player", "getBaseMagicLevel", luaPlayerGetBaseMagicLevel);
	lsi.registerMethod("Player", "getMana", luaPlayerGetMana);
	lsi.registerMethod("Player", "addMana", luaPlayerAddMana);
	lsi.registerMethod("Player", "getMaxMana", luaPlayerGetMaxMana);
	lsi.registerMethod("Player", "setMaxMana", luaPlayerSetMaxMana);
	lsi.registerMethod("Player", "setManaShieldBar", luaPlayerSetManaShieldBar);
	lsi.registerMethod("Player", "getManaSpent", luaPlayerGetManaSpent);
	lsi.registerMethod("Player", "addManaSpent", luaPlayerAddManaSpent);
	lsi.registerMethod("Player", "removeManaSpent", luaPlayerRemoveManaSpent);

	lsi.registerMethod("Player", "getBaseMaxHealth", luaPlayerGetBaseMaxHealth);
	lsi.registerMethod("Player", "getBaseMaxMana", luaPlayerGetBaseMaxMana);

	lsi.registerMethod("Player", "getSkillLevel", luaPlayerGetSkillLevel);
	lsi.registerMethod("Player", "getEffectiveSkillLevel", luaPlayerGetEffectiveSkillLevel);
	lsi.registerMethod("Player", "getSkillPercent", luaPlayerGetSkillPercent);
	lsi.registerMethod("Player", "getSkillTries", luaPlayerGetSkillTries);
	lsi.registerMethod("Player", "addSkillTries", luaPlayerAddSkillTries);
	lsi.registerMethod("Player", "removeSkillTries", luaPlayerRemoveSkillTries);
	lsi.registerMethod("Player", "getSpecialSkill", luaPlayerGetSpecialSkill);
	lsi.registerMethod("Player", "addSpecialSkill", luaPlayerAddSpecialSkill);

	lsi.registerMethod("Player", "addOfflineTrainingTime", luaPlayerAddOfflineTrainingTime);
	lsi.registerMethod("Player", "getOfflineTrainingTime", luaPlayerGetOfflineTrainingTime);
	lsi.registerMethod("Player", "removeOfflineTrainingTime", luaPlayerRemoveOfflineTrainingTime);

	lsi.registerMethod("Player", "addOfflineTrainingTries", luaPlayerAddOfflineTrainingTries);

	lsi.registerMethod("Player", "getOfflineTrainingSkill", luaPlayerGetOfflineTrainingSkill);
	lsi.registerMethod("Player", "setOfflineTrainingSkill", luaPlayerSetOfflineTrainingSkill);

	lsi.registerMethod("Player", "getItemCount", luaPlayerGetItemCount);
	lsi.registerMethod("Player", "getItemById", luaPlayerGetItemById);

	lsi.registerMethod("Player", "getVocation", luaPlayerGetVocation);
	lsi.registerMethod("Player", "setVocation", luaPlayerSetVocation);

	lsi.registerMethod("Player", "getSex", luaPlayerGetSex);
	lsi.registerMethod("Player", "setSex", luaPlayerSetSex);

	lsi.registerMethod("Player", "getTown", luaPlayerGetTown);
	lsi.registerMethod("Player", "setTown", luaPlayerSetTown);

	lsi.registerMethod("Player", "getGuild", luaPlayerGetGuild);
	lsi.registerMethod("Player", "setGuild", luaPlayerSetGuild);

	lsi.registerMethod("Player", "getGuildLevel", luaPlayerGetGuildLevel);
	lsi.registerMethod("Player", "setGuildLevel", luaPlayerSetGuildLevel);

	lsi.registerMethod("Player", "getGuildNick", luaPlayerGetGuildNick);
	lsi.registerMethod("Player", "setGuildNick", luaPlayerSetGuildNick);

	lsi.registerMethod("Player", "getGroup", luaPlayerGetGroup);
	lsi.registerMethod("Player", "setGroup", luaPlayerSetGroup);

	lsi.registerMethod("Player", "getStamina", luaPlayerGetStamina);
	lsi.registerMethod("Player", "setStamina", luaPlayerSetStamina);

	lsi.registerMethod("Player", "getSoul", luaPlayerGetSoul);
	lsi.registerMethod("Player", "addSoul", luaPlayerAddSoul);
	lsi.registerMethod("Player", "getMaxSoul", luaPlayerGetMaxSoul);

	lsi.registerMethod("Player", "getBankBalance", luaPlayerGetBankBalance);
	lsi.registerMethod("Player", "setBankBalance", luaPlayerSetBankBalance);

	lsi.registerMethod("Player", "addItem", luaPlayerAddItem);
	lsi.registerMethod("Player", "addItemEx", luaPlayerAddItemEx);
	lsi.registerMethod("Player", "removeItem", luaPlayerRemoveItem);
	lsi.registerMethod("Player", "sendSupplyUsed", luaPlayerSendSupplyUsed);

	lsi.registerMethod("Player", "getMoney", luaPlayerGetMoney);
	lsi.registerMethod("Player", "addMoney", luaPlayerAddMoney);
	lsi.registerMethod("Player", "removeMoney", luaPlayerRemoveMoney);

	lsi.registerMethod("Player", "showTextDialog", luaPlayerShowTextDialog);

	lsi.registerMethod("Player", "sendTextMessage", luaPlayerSendTextMessage);
	lsi.registerMethod("Player", "sendChannelMessage", luaPlayerSendChannelMessage);
	lsi.registerMethod("Player", "sendPrivateMessage", luaPlayerSendPrivateMessage);
	lsi.registerMethod("Player", "channelSay", luaPlayerChannelSay);
	lsi.registerMethod("Player", "openChannel", luaPlayerOpenChannel);
	lsi.registerMethod("Player", "leaveChannel", luaPlayerLeaveChannel);

	lsi.registerMethod("Player", "getSlotItem", luaPlayerGetSlotItem);

	lsi.registerMethod("Player", "getParty", luaPlayerGetParty);

	lsi.registerMethod("Player", "addOutfit", luaPlayerAddOutfit);
	lsi.registerMethod("Player", "addOutfitAddon", luaPlayerAddOutfitAddon);
	lsi.registerMethod("Player", "removeOutfit", luaPlayerRemoveOutfit);
	lsi.registerMethod("Player", "removeOutfitAddon", luaPlayerRemoveOutfitAddon);
	lsi.registerMethod("Player", "hasOutfit", luaPlayerHasOutfit);
	lsi.registerMethod("Player", "canWearOutfit", luaPlayerCanWearOutfit);
	lsi.registerMethod("Player", "sendOutfitWindow", luaPlayerSendOutfitWindow);

	lsi.registerMethod("Player", "sendEditPodium", luaPlayerSendEditPodium);

	lsi.registerMethod("Player", "addMount", luaPlayerAddMount);
	lsi.registerMethod("Player", "removeMount", luaPlayerRemoveMount);
	lsi.registerMethod("Player", "hasMount", luaPlayerHasMount);
	lsi.registerMethod("Player", "toggleMount", luaPlayerToggleMount);

	lsi.registerMethod("Player", "getPremiumEndsAt", luaPlayerGetPremiumEndsAt);
	lsi.registerMethod("Player", "setPremiumEndsAt", luaPlayerSetPremiumEndsAt);

	lsi.registerMethod("Player", "hasBlessing", luaPlayerHasBlessing);
	lsi.registerMethod("Player", "addBlessing", luaPlayerAddBlessing);
	lsi.registerMethod("Player", "removeBlessing", luaPlayerRemoveBlessing);

	lsi.registerMethod("Player", "canLearnSpell", luaPlayerCanLearnSpell);
	lsi.registerMethod("Player", "learnSpell", luaPlayerLearnSpell);
	lsi.registerMethod("Player", "forgetSpell", luaPlayerForgetSpell);
	lsi.registerMethod("Player", "hasLearnedSpell", luaPlayerHasLearnedSpell);

	lsi.registerMethod("Player", "sendTutorial", luaPlayerSendTutorial);
	lsi.registerMethod("Player", "addMapMark", luaPlayerAddMapMark);

	lsi.registerMethod("Player", "save", luaPlayerSave);
	lsi.registerMethod("Player", "popupFYI", luaPlayerPopupFYI);

	lsi.registerMethod("Player", "isPzLocked", luaPlayerIsPzLocked);

	lsi.registerMethod("Player", "getClient", luaPlayerGetClient);

	lsi.registerMethod("Player", "getHouse", luaPlayerGetHouse);
	lsi.registerMethod("Player", "sendHouseWindow", luaPlayerSendHouseWindow);
	lsi.registerMethod("Player", "setEditHouse", luaPlayerSetEditHouse);

	lsi.registerMethod("Player", "setGhostMode", luaPlayerSetGhostMode);

	lsi.registerMethod("Player", "getContainerId", luaPlayerGetContainerId);
	lsi.registerMethod("Player", "getContainerById", luaPlayerGetContainerById);
	lsi.registerMethod("Player", "getContainerIndex", luaPlayerGetContainerIndex);

	lsi.registerMethod("Player", "getRuneSpells", luaPlayerGetRuneSpells);
	lsi.registerMethod("Player", "getInstantSpells", luaPlayerGetInstantSpells);
	lsi.registerMethod("Player", "canCast", luaPlayerCanCast);

	lsi.registerMethod("Player", "hasChaseMode", luaPlayerHasChaseMode);
	lsi.registerMethod("Player", "hasSecureMode", luaPlayerHasSecureMode);
	lsi.registerMethod("Player", "getFightMode", luaPlayerGetFightMode);

	lsi.registerMethod("Player", "getStoreInbox", luaPlayerGetStoreInbox);

	lsi.registerMethod("Player", "isNearDepotBox", luaPlayerIsNearDepotBox);

	lsi.registerMethod("Player", "getIdleTime", luaPlayerGetIdleTime);
	lsi.registerMethod("Player", "setIdleTime", luaPlayerSetIdleTime);
	lsi.registerMethod("Player", "resetIdleTime", luaPlayerResetIdleTime);

	lsi.registerMethod("Player", "sendCreatureSquare", luaPlayerSendCreatureSquare);

	lsi.registerMethod("Player", "getClientExpDisplay", luaPlayerGetClientExpDisplay);
	lsi.registerMethod("Player", "setClientExpDisplay", luaPlayerSetClientExpDisplay);

	lsi.registerMethod("Player", "getClientStaminaBonusDisplay", luaPlayerGetClientStaminaBonusDisplay);
	lsi.registerMethod("Player", "setClientStaminaBonusDisplay", luaPlayerSetClientStaminaBonusDisplay);

	lsi.registerMethod("Player", "getClientLowLevelBonusDisplay", luaPlayerGetClientLowLevelBonusDisplay);
	lsi.registerMethod("Player", "setClientLowLevelBonusDisplay", luaPlayerSetClientLowLevelBonusDisplay);

	lsi.registerMethod("Player", "sendResourceBalance", luaPlayerSendResourceBalance);

	lsi.registerMethod("Player", "sendEnterMarket", luaPlayerSendEnterMarket);
}
