/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "condition.h"
#include "game.h"

extern Game g_game;

bool Condition::setParam(ConditionParam_t param, int32_t value)
{
	switch (param) {
		case CONDITION_PARAM_TICKS: {
			ticks = value;
			return true;
		}

		case CONDITION_PARAM_BUFF_SPELL: {
			isBuff = (value != 0);
			return true;
		}

		case CONDITION_PARAM_SUBID: {
			subId = value;
			return true;
		}

		default: {
			return false;
		}
	}
}

bool Condition::unserialize(PropStream& propStream)
{
	uint8_t attr_type;
	while (propStream.read<uint8_t>(attr_type) && attr_type != CONDITIONATTR_END) {
		if (!unserializeProp(static_cast<ConditionAttr_t>(attr_type), propStream)) {
			return false;
		}
	}
	return true;
}

bool Condition::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	switch (attr) {
		case CONDITIONATTR_TYPE: {
			int32_t value;
			if (!propStream.read<int32_t>(value)) {
				return false;
			}

			conditionType = static_cast<ConditionType_t>(value);
			return true;
		}

		case CONDITIONATTR_ID: {
			int32_t value;
			if (!propStream.read<int32_t>(value)) {
				return false;
			}

			id = static_cast<ConditionId_t>(value);
			return true;
		}

		case CONDITIONATTR_TICKS: {
			return propStream.read<int32_t>(ticks);
		}

		case CONDITIONATTR_ISBUFF: {
			uint8_t value;
			if (!propStream.read<uint8_t>(value)) {
				return false;
			}

			isBuff = (value != 0);
			return true;
		}

		case CONDITIONATTR_SUBID: {
			return propStream.read<uint32_t>(subId);
		}

		case CONDITIONATTR_END:
			return true;

		default:
			return false;
	}
}

void Condition::serialize(PropWriteStream& propWriteStream)
{
	propWriteStream.write<uint8_t>(CONDITIONATTR_TYPE);
	propWriteStream.write<uint32_t>(conditionType);

	propWriteStream.write<uint8_t>(CONDITIONATTR_ID);
	propWriteStream.write<uint32_t>(id);

	propWriteStream.write<uint8_t>(CONDITIONATTR_TICKS);
	propWriteStream.write<uint32_t>(ticks);

	propWriteStream.write<uint8_t>(CONDITIONATTR_ISBUFF);
	propWriteStream.write<uint8_t>(isBuff);

	propWriteStream.write<uint8_t>(CONDITIONATTR_SUBID);
	propWriteStream.write<uint32_t>(subId);
}

void Condition::setTicks(int32_t newTicks)
{
	ticks = newTicks;
	endTime = ticks + OTSYS_TIME();
}

bool Condition::executeCondition(Creature*, int32_t interval)
{
	if (ticks == -1) {
		return true;
	}

	//Not using set ticks here since it would reset endTime
	ticks = std::max<int32_t>(0, ticks - interval);
	return getEndTime() >= OTSYS_TIME();
}

Condition* Condition::createCondition(ConditionId_t id, ConditionType_t type, int32_t ticks, int32_t param/* = 0*/, bool buff/* = false*/, uint32_t subId/* = 0*/)
{
	switch (type) {
		case CONDITION_POISON:
		case CONDITION_FIRE:
		case CONDITION_ENERGY:
		case CONDITION_DROWN:
		case CONDITION_FREEZING:
		case CONDITION_DAZZLED:
		case CONDITION_CURSED:
		case CONDITION_BLEEDING:
			return new ConditionDamage(id, type, buff, subId);

		case CONDITION_HASTE:
		case CONDITION_PARALYZE:
			return new ConditionSpeed(id, type, ticks, buff, subId, param);

		case CONDITION_INVISIBLE:
			return new ConditionInvisible(id, type, ticks, buff, subId);

		case CONDITION_OUTFIT:
			return new ConditionOutfit(id, type, ticks, buff, subId);

		case CONDITION_LIGHT:
			return new ConditionLight(id, type, ticks, buff, subId, param & 0xFF, (param & 0xFF00) >> 8);

		case CONDITION_REGENERATION:
			return new ConditionRegeneration(id, type, ticks, buff, subId);

		case CONDITION_SOUL:
			return new ConditionSoul(id, type, ticks, buff, subId);

		case CONDITION_ATTRIBUTES:
			return new ConditionAttributes(id, type, ticks, buff, subId);

		case CONDITION_SPELLCOOLDOWN:
			return new ConditionSpellCooldown(id, type, ticks, buff, subId);

		case CONDITION_SPELLGROUPCOOLDOWN:
			return new ConditionSpellGroupCooldown(id, type, ticks, buff, subId);

		case CONDITION_INFIGHT:
		case CONDITION_DRUNK:
		case CONDITION_EXHAUST_WEAPON:
		case CONDITION_EXHAUST_COMBAT:
		case CONDITION_EXHAUST_HEAL:
		case CONDITION_MUTED:
		case CONDITION_CHANNELMUTEDTICKS:
		case CONDITION_YELLTICKS:
		case CONDITION_PACIFIED:
		case CONDITION_MANASHIELD:
			return new ConditionGeneric(id, type, ticks, buff, subId);

		default:
			return nullptr;
	}
}

Condition* Condition::createCondition(PropStream& propStream)
{
	uint8_t attr;
	if (!propStream.read<uint8_t>(attr) || attr != CONDITIONATTR_TYPE) {
		return nullptr;
	}

	uint32_t type;
	if (!propStream.read<uint32_t>(type)) {
		return nullptr;
	}

	if (!propStream.read<uint8_t>(attr) || attr != CONDITIONATTR_ID) {
		return nullptr;
	}

	uint32_t id;
	if (!propStream.read<uint32_t>(id)) {
		return nullptr;
	}

	if (!propStream.read<uint8_t>(attr) || attr != CONDITIONATTR_TICKS) {
		return nullptr;
	}

	uint32_t ticks;
	if (!propStream.read<uint32_t>(ticks)) {
		return nullptr;
	}

	if (!propStream.read<uint8_t>(attr) || attr != CONDITIONATTR_ISBUFF) {
		return nullptr;
	}

	uint8_t buff;
	if (!propStream.read<uint8_t>(buff)) {
		return nullptr;
	}

	if (!propStream.read<uint8_t>(attr) || attr != CONDITIONATTR_SUBID) {
		return nullptr;
	}

	uint32_t subId;
	if (!propStream.read<uint32_t>(subId)) {
		return nullptr;
	}

	return createCondition(static_cast<ConditionId_t>(id), static_cast<ConditionType_t>(type), ticks, 0, buff != 0, subId);
}

bool Condition::startCondition(Creature*)
{
	if (ticks > 0) {
		endTime = ticks + OTSYS_TIME();
	}
	return true;
}

bool Condition::isPersistent() const
{
	if (ticks == -1) {
		return false;
	}

	if (!(id == CONDITIONID_DEFAULT || id == CONDITIONID_COMBAT || conditionType == CONDITION_MUTED)) {
		return false;
	}

	return true;
}

uint32_t Condition::getIcons() const
{
	return isBuff ? ICON_PARTY_BUFF : 0;
}

bool Condition::updateCondition(const Condition* addCondition)
{
	if (conditionType != addCondition->getType()) {
		return false;
	}

	if (ticks == -1 && addCondition->getTicks() > 0) {
		return false;
	}

	if (addCondition->getTicks() >= 0 && getEndTime() > (OTSYS_TIME() + addCondition->getTicks())) {
		return false;
	}

	return true;
}

bool ConditionGeneric::startCondition(Creature* creature)
{
	return Condition::startCondition(creature);
}

bool ConditionGeneric::executeCondition(Creature* creature, int32_t interval)
{
	return Condition::executeCondition(creature, interval);
}

void ConditionGeneric::endCondition(Creature*)
{
	//
}

void ConditionGeneric::addCondition(Creature*, const Condition* condition)
{
	if (updateCondition(condition)) {
		setTicks(condition->getTicks());
	}
}

uint32_t ConditionGeneric::getIcons() const
{
	uint32_t icons = Condition::getIcons();

	switch (conditionType) {
		case CONDITION_MANASHIELD:
			icons |= ICON_MANASHIELD;
			break;

		case CONDITION_INFIGHT:
			icons |= ICON_SWORDS;
			break;

		case CONDITION_DRUNK:
			icons |= ICON_DRUNK;
			break;

		default:
			break;
	}

	return icons;
}

void ConditionAttributes::addCondition(Creature* creature, const Condition* condition)
{
	if (updateCondition(condition)) {
		setTicks(condition->getTicks());

		const ConditionAttributes& conditionAttrs = static_cast<const ConditionAttributes&>(*condition);
		//Remove the old condition
		endCondition(creature);

		//Apply the new one
		memcpy(skills, conditionAttrs.skills, sizeof(skills));
		memcpy(specialSkills, conditionAttrs.specialSkills, sizeof(specialSkills));
		memcpy(skillsPercent, conditionAttrs.skillsPercent, sizeof(skillsPercent));
		memcpy(stats, conditionAttrs.stats, sizeof(stats));
		memcpy(statsPercent, conditionAttrs.statsPercent, sizeof(statsPercent));
		disableDefense = conditionAttrs.disableDefense;

		if (Player* player = creature->getPlayer()) {
			updatePercentSkills(player);
			updateSkills(player);
			updatePercentStats(player);
			updateStats(player);
		}
	}
}

bool ConditionAttributes::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_SKILLS) {
		return propStream.read<int32_t>(skills[currentSkill++]);
	} else if (attr == CONDITIONATTR_STATS) {
		return propStream.read<int32_t>(stats[currentStat++]);
	}
	return Condition::unserializeProp(attr, propStream);
}

void ConditionAttributes::serialize(PropWriteStream& propWriteStream)
{
	Condition::serialize(propWriteStream);

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		propWriteStream.write<uint8_t>(CONDITIONATTR_SKILLS);
		propWriteStream.write<int32_t>(skills[i]);
	}

	for (int32_t i = STAT_FIRST; i <= STAT_LAST; ++i) {
		propWriteStream.write<uint8_t>(CONDITIONATTR_STATS);
		propWriteStream.write<int32_t>(stats[i]);
	}
}

bool ConditionAttributes::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

	creature->setUseDefense(!disableDefense);

	if (Player* player = creature->getPlayer()) {
		updatePercentSkills(player);
		updateSkills(player);
		updatePercentStats(player);
		updateStats(player);
	}

	return true;
}

void ConditionAttributes::updatePercentStats(Player* player)
{
	for (int32_t i = STAT_FIRST; i <= STAT_LAST; ++i) {
		if (statsPercent[i] == 0) {
			continue;
		}

		switch (i) {
			case STAT_MAXHITPOINTS:
				stats[i] = static_cast<int32_t>(player->getMaxHealth() * ((statsPercent[i] - 100) / 100.f));
				break;

			case STAT_MAXMANAPOINTS:
				stats[i] = static_cast<int32_t>(player->getMaxMana() * ((statsPercent[i] - 100) / 100.f));
				break;

			case STAT_MAGICPOINTS:
				stats[i] = static_cast<int32_t>(player->getBaseMagicLevel() * ((statsPercent[i] - 100) / 100.f));
				break;
		}
	}
}

void ConditionAttributes::updateStats(Player* player)
{
	bool needUpdateStats = false;

	for (int32_t i = STAT_FIRST; i <= STAT_LAST; ++i) {
		if (stats[i]) {
			needUpdateStats = true;
			player->setVarStats(static_cast<stats_t>(i), stats[i]);
		}
	}

	if (needUpdateStats) {
		player->sendStats();
	}
}

void ConditionAttributes::updatePercentSkills(Player* player)
{
	for (uint8_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (skillsPercent[i] == 0) {
			continue;
		}

		int32_t unmodifiedSkill = player->getBaseSkill(i);
		skills[i] = static_cast<int32_t>(unmodifiedSkill * ((skillsPercent[i] - 100) / 100.f));
	}
}

void ConditionAttributes::updateSkills(Player* player)
{
	bool needUpdateSkills = false;

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (skills[i]) {
			needUpdateSkills = true;
			player->setVarSkill(static_cast<skills_t>(i), skills[i]);
		}
	}

	for (int32_t i = SPECIALSKILL_FIRST; i <= SPECIALSKILL_LAST; ++i) {
		if (specialSkills[i]) {
			needUpdateSkills = true;
			player->setVarSpecialSkill(static_cast<SpecialSkills_t>(i), specialSkills[i]);
		}
	}

	if (needUpdateSkills) {
		player->sendSkills();
	}
}

bool ConditionAttributes::executeCondition(Creature* creature, int32_t interval)
{
	return ConditionGeneric::executeCondition(creature, interval);
}

void ConditionAttributes::endCondition(Creature* creature)
{
	Player* player = creature->getPlayer();
	if (player) {
		bool needUpdateSkills = false;

		for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
			if (skills[i] || skillsPercent[i]) {
				needUpdateSkills = true;
				player->setVarSkill(static_cast<skills_t>(i), -skills[i]);
			}
		}

		for (int32_t i = SPECIALSKILL_FIRST; i <= SPECIALSKILL_LAST; ++i) {
			if (specialSkills[i]) {
				needUpdateSkills = true;
				player->setVarSpecialSkill(static_cast<SpecialSkills_t>(i), -specialSkills[i]);
			}
		}

		if (needUpdateSkills) {
			player->sendSkills();
		}

		bool needUpdateStats = false;

		for (int32_t i = STAT_FIRST; i <= STAT_LAST; ++i) {
			if (stats[i]) {
				needUpdateStats = true;
				player->setVarStats(static_cast<stats_t>(i), -stats[i]);
			}
		}

		if (needUpdateStats) {
			player->sendStats();
		}
	}

	if (disableDefense) {
		creature->setUseDefense(true);
	}
}

bool ConditionAttributes::setParam(ConditionParam_t param, int32_t value)
{
	bool ret = ConditionGeneric::setParam(param, value);

	switch (param) {
		case CONDITION_PARAM_SKILL_MELEE: {
			skills[SKILL_CLUB] = value;
			skills[SKILL_AXE] = value;
			skills[SKILL_SWORD] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_MELEEPERCENT: {
			skillsPercent[SKILL_CLUB] = value;
			skillsPercent[SKILL_AXE] = value;
			skillsPercent[SKILL_SWORD] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_FIST: {
			skills[SKILL_FIST] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_FISTPERCENT: {
			skillsPercent[SKILL_FIST] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_CLUB: {
			skills[SKILL_CLUB] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_CLUBPERCENT: {
			skillsPercent[SKILL_CLUB] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_SWORD: {
			skills[SKILL_SWORD] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_SWORDPERCENT: {
			skillsPercent[SKILL_SWORD] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_AXE: {
			skills[SKILL_AXE] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_AXEPERCENT: {
			skillsPercent[SKILL_AXE] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_DISTANCE: {
			skills[SKILL_DISTANCE] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_DISTANCEPERCENT: {
			skillsPercent[SKILL_DISTANCE] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_SHIELD: {
			skills[SKILL_SHIELD] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_SHIELDPERCENT: {
			skillsPercent[SKILL_SHIELD] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_FISHING: {
			skills[SKILL_FISHING] = value;
			return true;
		}

		case CONDITION_PARAM_SKILL_FISHINGPERCENT: {
			skillsPercent[SKILL_FISHING] = value;
			return true;
		}

		case CONDITION_PARAM_STAT_MAXHITPOINTS: {
			stats[STAT_MAXHITPOINTS] = value;
			return true;
		}

		case CONDITION_PARAM_STAT_MAXMANAPOINTS: {
			stats[STAT_MAXMANAPOINTS] = value;
			return true;
		}

		case CONDITION_PARAM_STAT_MAGICPOINTS: {
			stats[STAT_MAGICPOINTS] = value;
			return true;
		}

		case CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT: {
			statsPercent[STAT_MAXHITPOINTS] = std::max<int32_t>(0, value);
			return true;
		}

		case CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT: {
			statsPercent[STAT_MAXMANAPOINTS] = std::max<int32_t>(0, value);
			return true;
		}

		case CONDITION_PARAM_STAT_MAGICPOINTSPERCENT: {
			statsPercent[STAT_MAGICPOINTS] = std::max<int32_t>(0, value);
			return true;
		}

		case CONDITION_PARAM_DISABLE_DEFENSE: {
			disableDefense = (value != 0);
			return true;
		}

		case CONDITION_PARAM_SPECIALSKILL_CRITICALHITCHANCE: {
			specialSkills[SPECIALSKILL_CRITICALHITCHANCE] = value;
			return true;
		}

		case CONDITION_PARAM_SPECIALSKILL_CRITICALHITAMOUNT: {
			specialSkills[SPECIALSKILL_CRITICALHITAMOUNT] = value;
			return true;
		}

		case CONDITION_PARAM_SPECIALSKILL_HITPOINTSLEECHCHANCE: {
			specialSkills[SPECIALSKILL_HITPOINTSLEECHCHANCE] = value;
			return true;
		}

		case CONDITION_PARAM_SPECIALSKILL_HITPOINTSLEECHAMOUNT: {
			specialSkills[SPECIALSKILL_HITPOINTSLEECHAMOUNT] = value;
			return true;
		}

		case CONDITION_PARAM_SPECIALSKILL_MANAPOINTSLEECHCHANCE: {
			specialSkills[SPECIALSKILL_MANAPOINTSLEECHCHANCE] = value;
			return true;
		}

		case CONDITION_PARAM_SPECIALSKILL_MANAPOINTSLEECHAMOUNT: {
			specialSkills[SPECIALSKILL_MANAPOINTSLEECHAMOUNT] = value;
			return true;
		}

		default:
			return ret;
	}
}

void ConditionRegeneration::addCondition(Creature*, const Condition* condition)
{
	if (updateCondition(condition)) {
		setTicks(condition->getTicks());

		const ConditionRegeneration& conditionRegen = static_cast<const ConditionRegeneration&>(*condition);

		healthTicks = conditionRegen.healthTicks;
		manaTicks = conditionRegen.manaTicks;

		healthGain = conditionRegen.healthGain;
		manaGain = conditionRegen.manaGain;
	}
}

bool ConditionRegeneration::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_HEALTHTICKS) {
		return propStream.read<uint32_t>(healthTicks);
	} else if (attr == CONDITIONATTR_HEALTHGAIN) {
		return propStream.read<uint32_t>(healthGain);
	} else if (attr == CONDITIONATTR_MANATICKS) {
		return propStream.read<uint32_t>(manaTicks);
	} else if (attr == CONDITIONATTR_MANAGAIN) {
		return propStream.read<uint32_t>(manaGain);
	}
	return Condition::unserializeProp(attr, propStream);
}

void ConditionRegeneration::serialize(PropWriteStream& propWriteStream)
{
	Condition::serialize(propWriteStream);

	propWriteStream.write<uint8_t>(CONDITIONATTR_HEALTHTICKS);
	propWriteStream.write<uint32_t>(healthTicks);

	propWriteStream.write<uint8_t>(CONDITIONATTR_HEALTHGAIN);
	propWriteStream.write<uint32_t>(healthGain);

	propWriteStream.write<uint8_t>(CONDITIONATTR_MANATICKS);
	propWriteStream.write<uint32_t>(manaTicks);

	propWriteStream.write<uint8_t>(CONDITIONATTR_MANAGAIN);
	propWriteStream.write<uint32_t>(manaGain);
}

bool ConditionRegeneration::executeCondition(Creature* creature, int32_t interval)
{
	internalHealthTicks += interval;
	internalManaTicks += interval;

	if (creature->getZone() == ZONE_PROTECTION) {
		return ConditionGeneric::executeCondition(creature, interval);
	}

	if (internalHealthTicks >= healthTicks) {
		internalHealthTicks = 0;

		int32_t realHealthGain = creature->getHealth();
		creature->changeHealth(healthGain);
		realHealthGain = creature->getHealth() - realHealthGain;

		if (isBuff && realHealthGain > 0) {
			Player* player = creature->getPlayer();
			if (player) {
				std::string healString = std::to_string(realHealthGain) + (realHealthGain != 1 ? " hitpoints." : " hitpoint.");

				TextMessage message(MESSAGE_HEALED, "You were healed for " + healString);
				message.position = player->getPosition();
				message.primary.value = realHealthGain;
				message.primary.color = TEXTCOLOR_MAYABLUE;
				player->sendTextMessage(message);

				SpectatorHashSet spectators;
				g_game.map.getSpectators(spectators, player->getPosition(), false, true);
				spectators.erase(player);
				if (!spectators.empty()) {
					message.type = MESSAGE_HEALED_OTHERS;
					message.text = player->getName() + " was healed for " + healString;
					for (Creature* spectator : spectators) {
						spectator->getPlayer()->sendTextMessage(message);
					}
				}
			}
		}
	}

	if (internalManaTicks >= manaTicks) {
		internalManaTicks = 0;

		if (Player* player = creature->getPlayer()) {
			int32_t realManaGain = player->getMana();
			player->changeMana(manaGain);
			realManaGain = player->getMana() - realManaGain;

			if (isBuff && realManaGain > 0) {
				std::string manaGainString = std::to_string(realManaGain);

				TextMessage message(MESSAGE_HEALED, "You gained " + manaGainString + " mana.");
				message.position = player->getPosition();
				message.primary.value = realManaGain;
				message.primary.color = TEXTCOLOR_MAYABLUE;
				player->sendTextMessage(message);

				SpectatorHashSet spectators;
				g_game.map.getSpectators(spectators, player->getPosition(), false, true);
				spectators.erase(player);
				if (!spectators.empty()) {
					message.type = MESSAGE_HEALED_OTHERS;
					message.text = player->getName() + " gained " + manaGainString + " mana.";
					for (Creature* spectator : spectators) {
						spectator->getPlayer()->sendTextMessage(message);
					}
				}
			}
		}
	}

	return ConditionGeneric::executeCondition(creature, interval);
}

bool ConditionRegeneration::setParam(ConditionParam_t param, int32_t value)
{
	bool ret = ConditionGeneric::setParam(param, value);

	switch (param) {
		case CONDITION_PARAM_HEALTHGAIN:
			healthGain = value;
			return true;

		case CONDITION_PARAM_HEALTHTICKS:
			healthTicks = value;
			return true;

		case CONDITION_PARAM_MANAGAIN:
			manaGain = value;
			return true;

		case CONDITION_PARAM_MANATICKS:
			manaTicks = value;
			return true;

		default:
			return ret;
	}
}

void ConditionSoul::addCondition(Creature*, const Condition* condition)
{
	if (updateCondition(condition)) {
		setTicks(condition->getTicks());

		const ConditionSoul& conditionSoul = static_cast<const ConditionSoul&>(*condition);

		soulTicks = conditionSoul.soulTicks;
		soulGain = conditionSoul.soulGain;
	}
}

bool ConditionSoul::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_SOULGAIN) {
		return propStream.read<uint32_t>(soulGain);
	} else if (attr == CONDITIONATTR_SOULTICKS) {
		return propStream.read<uint32_t>(soulTicks);
	}
	return Condition::unserializeProp(attr, propStream);
}

void ConditionSoul::serialize(PropWriteStream& propWriteStream)
{
	Condition::serialize(propWriteStream);

	propWriteStream.write<uint8_t>(CONDITIONATTR_SOULGAIN);
	propWriteStream.write<uint32_t>(soulGain);

	propWriteStream.write<uint8_t>(CONDITIONATTR_SOULTICKS);
	propWriteStream.write<uint32_t>(soulTicks);
}

bool ConditionSoul::executeCondition(Creature* creature, int32_t interval)
{
	internalSoulTicks += interval;

	if (Player* player = creature->getPlayer()) {
		if (player->getZone() != ZONE_PROTECTION) {
			if (internalSoulTicks >= soulTicks) {
				internalSoulTicks = 0;
				player->changeSoul(soulGain);
			}
		}
	}

	return ConditionGeneric::executeCondition(creature, interval);
}

bool ConditionSoul::setParam(ConditionParam_t param, int32_t value)
{
	bool ret = ConditionGeneric::setParam(param, value);
	switch (param) {
		case CONDITION_PARAM_SOULGAIN:
			soulGain = value;
			return true;

		case CONDITION_PARAM_SOULTICKS:
			soulTicks = value;
			return true;

		default:
			return ret;
	}
}

bool ConditionDamage::setParam(ConditionParam_t param, int32_t value)
{
	bool ret = Condition::setParam(param, value);

	switch (param) {
		case CONDITION_PARAM_OWNER:
			owner = value;
			return true;

		case CONDITION_PARAM_FORCEUPDATE:
			forceUpdate = (value != 0);
			return true;

		case CONDITION_PARAM_DELAYED:
			delayed = (value != 0);
			return true;

		case CONDITION_PARAM_MAXVALUE:
			maxDamage = std::abs(value);
			break;

		case CONDITION_PARAM_MINVALUE:
			minDamage = std::abs(value);
			break;

		case CONDITION_PARAM_STARTVALUE:
			startDamage = std::abs(value);
			break;

		case CONDITION_PARAM_TICKINTERVAL:
			tickInterval = std::abs(value);
			break;

		case CONDITION_PARAM_PERIODICDAMAGE:
			periodDamage = value;
			break;

		case CONDITION_PARAM_FIELD:
			field = (value != 0);
			break;

		default:
			return false;
	}

	return ret;
}

bool ConditionDamage::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_DELAYED) {
		uint8_t value;
		if (!propStream.read<uint8_t>(value)) {
			return false;
		}

		delayed = (value != 0);
		return true;
	} else if (attr == CONDITIONATTR_PERIODDAMAGE) {
		return propStream.read<int32_t>(periodDamage);
	} else if (attr == CONDITIONATTR_OWNER) {
		return propStream.skip(4);
	} else if (attr == CONDITIONATTR_INTERVALDATA) {
		IntervalInfo damageInfo;
		if (!propStream.read<IntervalInfo>(damageInfo)) {
			return false;
		}

		damageList.push_back(damageInfo);
		if (ticks != -1) {
			setTicks(ticks + damageInfo.interval);
		}
		return true;
	}
	return Condition::unserializeProp(attr, propStream);
}

void ConditionDamage::serialize(PropWriteStream& propWriteStream)
{
	Condition::serialize(propWriteStream);

	propWriteStream.write<uint8_t>(CONDITIONATTR_DELAYED);
	propWriteStream.write<uint8_t>(delayed);

	propWriteStream.write<uint8_t>(CONDITIONATTR_PERIODDAMAGE);
	propWriteStream.write<int32_t>(periodDamage);

	for (const IntervalInfo& intervalInfo : damageList) {
		propWriteStream.write<uint8_t>(CONDITIONATTR_INTERVALDATA);
		propWriteStream.write<IntervalInfo>(intervalInfo);
	}
}

bool ConditionDamage::updateCondition(const Condition* addCondition)
{
	const ConditionDamage& conditionDamage = static_cast<const ConditionDamage&>(*addCondition);
	if (conditionDamage.doForceUpdate()) {
		return true;
	}

	if (ticks == -1 && conditionDamage.ticks > 0) {
		return false;
	}

	return conditionDamage.getTotalDamage() > getTotalDamage();
}

bool ConditionDamage::addDamage(int32_t rounds, int32_t time, int32_t value)
{
	time = std::max<int32_t>(time, EVENT_CREATURE_THINK_INTERVAL);
	if (rounds == -1) {
		//periodic damage
		periodDamage = value;
		setParam(CONDITION_PARAM_TICKINTERVAL, time);
		setParam(CONDITION_PARAM_TICKS, -1);
		return true;
	}

	if (periodDamage > 0) {
		return false;
	}

	//rounds, time, damage
	for (int32_t i = 0; i < rounds; ++i) {
		IntervalInfo damageInfo;
		damageInfo.interval = time;
		damageInfo.timeLeft = time;
		damageInfo.value = value;

		damageList.push_back(damageInfo);

		if (ticks != -1) {
			setTicks(ticks + damageInfo.interval);
		}
	}

	return true;
}

bool ConditionDamage::init()
{
	if (periodDamage != 0) {
		return true;
	}

	if (!damageList.empty()) {
		return true;
	}

	setTicks(0);

	int32_t amount = uniform_random(minDamage, maxDamage);
	if (amount != 0) {
		if (startDamage > maxDamage) {
			startDamage = maxDamage;
		} else if (startDamage == 0) {
			startDamage = std::max<int32_t>(1, std::ceil(amount / 20.0));
		}

		std::list<int32_t> list;
		ConditionDamage::generateDamageList(amount, startDamage, list);
		for (int32_t value : list) {
			addDamage(1, tickInterval, -value);
		}
	}
	return !damageList.empty();
}

bool ConditionDamage::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

	if (!init()) {
		return false;
	}

	if (!delayed) {
		int32_t damage;
		if (getNextDamage(damage)) {
			return doDamage(creature, damage);
		}
	}
	return true;
}

bool ConditionDamage::executeCondition(Creature* creature, int32_t interval)
{
	if (periodDamage != 0) {
		periodDamageTick += interval;

		if (periodDamageTick >= tickInterval) {
			periodDamageTick = 0;
			doDamage(creature, periodDamage);
		}
	} else if (!damageList.empty()) {
		IntervalInfo& damageInfo = damageList.front();

		bool bRemove = (ticks != -1);
		creature->onTickCondition(getType(), bRemove);
		damageInfo.timeLeft -= interval;

		if (damageInfo.timeLeft <= 0) {
			int32_t damage = damageInfo.value;

			if (bRemove) {
				damageList.pop_front();
			} else {
				damageInfo.timeLeft = damageInfo.interval;
			}

			doDamage(creature, damage);
		}

		if (!bRemove) {
			if (ticks > 0) {
				endTime += interval;
			}

			interval = 0;
		}
	}

	return Condition::executeCondition(creature, interval);
}

bool ConditionDamage::getNextDamage(int32_t& damage)
{
	if (periodDamage != 0) {
		damage = periodDamage;
		return true;
	} else if (!damageList.empty()) {
		IntervalInfo& damageInfo = damageList.front();
		damage = damageInfo.value;
		if (ticks != -1) {
			damageList.pop_front();
		}
		return true;
	}
	return false;
}

bool ConditionDamage::doDamage(Creature* creature, int32_t healthChange)
{
	if (creature->isSuppress(getType())) {
		return true;
	}

	CombatDamage damage;
	damage.origin = ORIGIN_CONDITION;
	damage.primary.value = healthChange;
	damage.primary.type = Combat::ConditionToDamageType(conditionType);

	Creature* attacker = g_game.getCreatureByID(owner);
	if (field && creature->getPlayer() && attacker && attacker->getPlayer()) {
		damage.primary.value = static_cast<int32_t>(std::round(damage.primary.value / 2.));
	}

	if (!creature->isAttackable() || Combat::canDoCombat(attacker, creature) != RETURNVALUE_NOERROR) {
		if (!creature->isInGhostMode()) {
			g_game.addMagicEffect(creature->getPosition(), CONST_ME_POFF);
		}
		return false;
	}

	if (g_game.combatBlockHit(damage, attacker, creature, false, false, field)) {
		return false;
	}
	return g_game.combatChangeHealth(attacker, creature, damage);
}

void ConditionDamage::endCondition(Creature*)
{
	//
}

void ConditionDamage::addCondition(Creature* creature, const Condition* condition)
{
	if (condition->getType() != conditionType) {
		return;
	}

	if (!updateCondition(condition)) {
		return;
	}

	const ConditionDamage& conditionDamage = static_cast<const ConditionDamage&>(*condition);

	setTicks(condition->getTicks());
	owner = conditionDamage.owner;
	maxDamage = conditionDamage.maxDamage;
	minDamage = conditionDamage.minDamage;
	startDamage = conditionDamage.startDamage;
	tickInterval = conditionDamage.tickInterval;
	periodDamage = conditionDamage.periodDamage;
	int32_t nextTimeLeft = tickInterval;

	if (!damageList.empty()) {
		//save previous timeLeft
		IntervalInfo& damageInfo = damageList.front();
		nextTimeLeft = damageInfo.timeLeft;
		damageList.clear();
	}

	damageList = conditionDamage.damageList;

	if (init()) {
		if (!damageList.empty()) {
			//restore last timeLeft
			IntervalInfo& damageInfo = damageList.front();
			damageInfo.timeLeft = nextTimeLeft;
		}

		if (!delayed) {
			int32_t damage;
			if (getNextDamage(damage)) {
				doDamage(creature, damage);
			}
		}
	}
}

int32_t ConditionDamage::getTotalDamage() const
{
	int32_t result;
	if (!damageList.empty()) {
		result = 0;
		for (const IntervalInfo& intervalInfo : damageList) {
			result += intervalInfo.value;
		}
	} else {
		result = minDamage + (maxDamage - minDamage) / 2;
	}
	return std::abs(result);
}

uint32_t ConditionDamage::getIcons() const
{
	uint32_t icons = Condition::getIcons();
	switch (conditionType) {
		case CONDITION_FIRE:
			icons |= ICON_BURN;
			break;

		case CONDITION_ENERGY:
			icons |= ICON_ENERGY;
			break;

		case CONDITION_DROWN:
			icons |= ICON_DROWNING;
			break;

		case CONDITION_POISON:
			icons |= ICON_POISON;
			break;

		case CONDITION_FREEZING:
			icons |= ICON_FREEZING;
			break;

		case CONDITION_DAZZLED:
			icons |= ICON_DAZZLED;
			break;

		case CONDITION_CURSED:
			icons |= ICON_CURSED;
			break;

		case CONDITION_BLEEDING:
			icons |= ICON_BLEEDING;
			break;

		default:
			break;
	}
	return icons;
}

void ConditionDamage::generateDamageList(int32_t amount, int32_t start, std::list<int32_t>& list)
{
	amount = std::abs(amount);
	int32_t sum = 0;
	double x1, x2;

	for (int32_t i = start; i > 0; --i) {
		int32_t n = start + 1 - i;
		int32_t med = (n * amount) / start;

		do {
			sum += i;
			list.push_back(i);

			x1 = std::fabs(1.0 - ((static_cast<float>(sum)) + i) / med);
			x2 = std::fabs(1.0 - (static_cast<float>(sum) / med));
		} while (x1 < x2);
	}
}

void ConditionSpeed::setFormulaVars(float mina, float minb, float maxa, float maxb)
{
	this->mina = mina;
	this->minb = minb;
	this->maxa = maxa;
	this->maxb = maxb;
}

void ConditionSpeed::getFormulaValues(int32_t var, int32_t& min, int32_t& max) const
{
	min = (var * mina) + minb;
	max = (var * maxa) + maxb;
}

bool ConditionSpeed::setParam(ConditionParam_t param, int32_t value)
{
	Condition::setParam(param, value);
	if (param != CONDITION_PARAM_SPEED) {
		return false;
	}

	speedDelta = value;

	if (value > 0) {
		conditionType = CONDITION_HASTE;
	} else {
		conditionType = CONDITION_PARALYZE;
	}
	return true;
}

bool ConditionSpeed::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_SPEEDDELTA) {
		return propStream.read<int32_t>(speedDelta);
	} else if (attr == CONDITIONATTR_FORMULA_MINA) {
		return propStream.read<float>(mina);
	} else if (attr == CONDITIONATTR_FORMULA_MINB) {
		return propStream.read<float>(minb);
	} else if (attr == CONDITIONATTR_FORMULA_MAXA) {
		return propStream.read<float>(maxa);
	} else if (attr == CONDITIONATTR_FORMULA_MAXB) {
		return propStream.read<float>(maxb);
	}
	return Condition::unserializeProp(attr, propStream);
}

void ConditionSpeed::serialize(PropWriteStream& propWriteStream)
{
	Condition::serialize(propWriteStream);

	propWriteStream.write<uint8_t>(CONDITIONATTR_SPEEDDELTA);
	propWriteStream.write<int32_t>(speedDelta);

	propWriteStream.write<uint8_t>(CONDITIONATTR_FORMULA_MINA);
	propWriteStream.write<float>(mina);

	propWriteStream.write<uint8_t>(CONDITIONATTR_FORMULA_MINB);
	propWriteStream.write<float>(minb);

	propWriteStream.write<uint8_t>(CONDITIONATTR_FORMULA_MAXA);
	propWriteStream.write<float>(maxa);

	propWriteStream.write<uint8_t>(CONDITIONATTR_FORMULA_MAXB);
	propWriteStream.write<float>(maxb);
}

bool ConditionSpeed::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

	if (speedDelta == 0) {
		int32_t min, max;
		getFormulaValues(creature->getBaseSpeed(), min, max);
		speedDelta = uniform_random(min, max);
	}

	g_game.changeSpeed(creature, speedDelta);
	return true;
}

bool ConditionSpeed::executeCondition(Creature* creature, int32_t interval)
{
	return Condition::executeCondition(creature, interval);
}

void ConditionSpeed::endCondition(Creature* creature)
{
	g_game.changeSpeed(creature, -speedDelta);
}

void ConditionSpeed::addCondition(Creature* creature, const Condition* condition)
{
	if (conditionType != condition->getType()) {
		return;
	}

	if (ticks == -1 && condition->getTicks() > 0) {
		return;
	}

	setTicks(condition->getTicks());

	const ConditionSpeed& conditionSpeed = static_cast<const ConditionSpeed&>(*condition);
	int32_t oldSpeedDelta = speedDelta;
	speedDelta = conditionSpeed.speedDelta;
	mina = conditionSpeed.mina;
	maxa = conditionSpeed.maxa;
	minb = conditionSpeed.minb;
	maxb = conditionSpeed.maxb;

	if (speedDelta == 0) {
		int32_t min;
		int32_t max;
		getFormulaValues(creature->getBaseSpeed(), min, max);
		speedDelta = uniform_random(min, max);
	}

	int32_t newSpeedChange = (speedDelta - oldSpeedDelta);
	if (newSpeedChange != 0) {
		g_game.changeSpeed(creature, newSpeedChange);
	}
}

uint32_t ConditionSpeed::getIcons() const
{
	uint32_t icons = Condition::getIcons();
	switch (conditionType) {
		case CONDITION_HASTE:
			icons |= ICON_HASTE;
			break;

		case CONDITION_PARALYZE:
			icons |= ICON_PARALYZE;
			break;

		default:
			break;
	}
	return icons;
}

bool ConditionInvisible::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

	g_game.internalCreatureChangeVisible(creature, false);
	return true;
}

void ConditionInvisible::endCondition(Creature* creature)
{
	if (!creature->isInvisible()) {
		g_game.internalCreatureChangeVisible(creature, true);
	}
}

void ConditionOutfit::setOutfit(const Outfit_t& outfit)
{
	this->outfit = outfit;
}

bool ConditionOutfit::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_OUTFIT) {
		return propStream.read<Outfit_t>(outfit);
	}
	return Condition::unserializeProp(attr, propStream);
}

void ConditionOutfit::serialize(PropWriteStream& propWriteStream)
{
	Condition::serialize(propWriteStream);

	propWriteStream.write<uint8_t>(CONDITIONATTR_OUTFIT);
	propWriteStream.write<Outfit_t>(outfit);
}

bool ConditionOutfit::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

	g_game.internalCreatureChangeOutfit(creature, outfit);
	return true;
}

bool ConditionOutfit::executeCondition(Creature* creature, int32_t interval)
{
	return Condition::executeCondition(creature, interval);
}

void ConditionOutfit::endCondition(Creature* creature)
{
	g_game.internalCreatureChangeOutfit(creature, creature->getDefaultOutfit());
}

void ConditionOutfit::addCondition(Creature* creature, const Condition* condition)
{
	if (updateCondition(condition)) {
		setTicks(condition->getTicks());

		const ConditionOutfit& conditionOutfit = static_cast<const ConditionOutfit&>(*condition);
		outfit = conditionOutfit.outfit;

		g_game.internalCreatureChangeOutfit(creature, outfit);
	}
}

bool ConditionLight::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

	internalLightTicks = 0;
	lightChangeInterval = ticks / lightInfo.level;
	creature->setCreatureLight(lightInfo);
	g_game.changeLight(creature);
	return true;
}

bool ConditionLight::executeCondition(Creature* creature, int32_t interval)
{
	internalLightTicks += interval;

	if (internalLightTicks >= lightChangeInterval) {
		internalLightTicks = 0;
		LightInfo lightInfo = creature->getCreatureLight();

		if (lightInfo.level > 0) {
			--lightInfo.level;
			creature->setCreatureLight(lightInfo);
			g_game.changeLight(creature);
		}
	}

	return Condition::executeCondition(creature, interval);
}

void ConditionLight::endCondition(Creature* creature)
{
	creature->setNormalCreatureLight();
	g_game.changeLight(creature);
}

void ConditionLight::addCondition(Creature* creature, const Condition* condition)
{
	if (updateCondition(condition)) {
		setTicks(condition->getTicks());

		const ConditionLight& conditionLight = static_cast<const ConditionLight&>(*condition);
		lightInfo.level = conditionLight.lightInfo.level;
		lightInfo.color = conditionLight.lightInfo.color;
		lightChangeInterval = ticks / lightInfo.level;
		internalLightTicks = 0;
		creature->setCreatureLight(lightInfo);
		g_game.changeLight(creature);
	}
}

bool ConditionLight::setParam(ConditionParam_t param, int32_t value)
{
	bool ret = Condition::setParam(param, value);
	if (ret) {
		return false;
	}

	switch (param) {
		case CONDITION_PARAM_LIGHT_LEVEL:
			lightInfo.level = value;
			return true;

		case CONDITION_PARAM_LIGHT_COLOR:
			lightInfo.color = value;
			return true;

		default:
			return false;
	}
}

bool ConditionLight::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_LIGHTCOLOR) {
		uint32_t value;
		if (!propStream.read<uint32_t>(value)) {
			return false;
		}

		lightInfo.color = value;
		return true;
	} else if (attr == CONDITIONATTR_LIGHTLEVEL) {
		uint32_t value;
		if (!propStream.read<uint32_t>(value)) {
			return false;
		}

		lightInfo.level = value;
		return true;
	} else if (attr == CONDITIONATTR_LIGHTTICKS) {
		return propStream.read<uint32_t>(internalLightTicks);
	} else if (attr == CONDITIONATTR_LIGHTINTERVAL) {
		return propStream.read<uint32_t>(lightChangeInterval);
	}
	return Condition::unserializeProp(attr, propStream);
}

void ConditionLight::serialize(PropWriteStream& propWriteStream)
{
	Condition::serialize(propWriteStream);

	// TODO: color and level could be serialized as 8-bit if we can retain backwards
	// compatibility, but perhaps we should keep it like this in case they increase
	// in the future...
	propWriteStream.write<uint8_t>(CONDITIONATTR_LIGHTCOLOR);
	propWriteStream.write<uint32_t>(lightInfo.color);

	propWriteStream.write<uint8_t>(CONDITIONATTR_LIGHTLEVEL);
	propWriteStream.write<uint32_t>(lightInfo.level);

	propWriteStream.write<uint8_t>(CONDITIONATTR_LIGHTTICKS);
	propWriteStream.write<uint32_t>(internalLightTicks);

	propWriteStream.write<uint8_t>(CONDITIONATTR_LIGHTINTERVAL);
	propWriteStream.write<uint32_t>(lightChangeInterval);
}

void ConditionSpellCooldown::addCondition(Creature* creature, const Condition* condition)
{
	if (updateCondition(condition)) {
		setTicks(condition->getTicks());

		if (subId != 0 && ticks > 0) {
			Player* player = creature->getPlayer();
			if (player) {
				player->sendSpellCooldown(subId, ticks);
			}
		}
	}
}

bool ConditionSpellCooldown::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

	if (subId != 0 && ticks > 0) {
		Player* player = creature->getPlayer();
		if (player) {
			player->sendSpellCooldown(subId, ticks);
		}
	}
	return true;
}

void ConditionSpellGroupCooldown::addCondition(Creature* creature, const Condition* condition)
{
	if (updateCondition(condition)) {
		setTicks(condition->getTicks());

		if (subId != 0 && ticks > 0) {
			Player* player = creature->getPlayer();
			if (player) {
				player->sendSpellGroupCooldown(static_cast<SpellGroup_t>(subId), ticks);
			}
		}
	}
}

bool ConditionSpellGroupCooldown::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

	if (subId != 0 && ticks > 0) {
		Player* player = creature->getPlayer();
		if (player) {
			player->sendSpellGroupCooldown(static_cast<SpellGroup_t>(subId), ticks);
		}
	}
	return true;
}
