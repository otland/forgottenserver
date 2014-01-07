/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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
#include "creature.h"
#include "tools.h"
#include "combat.h"

extern Game g_game;

Condition::Condition(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId) :
	id(_id),
	subId(_subId),
	ticks(_ticks),
	conditionType(_type),
	isBuff(_buff)
{
	if (_ticks == -1) {
		endTime = std::numeric_limits<int64_t>::max();
	} else {
		endTime = 0;
	}
}

bool Condition::setParam(ConditionParam_t param, int32_t value)
{
	switch (param) {
		case CONDITIONPARAM_TICKS: {
			ticks = value;
			return true;
		}

		case CONDITIONPARAM_BUFF_SPELL: {
			isBuff = (value != 0);
			return true;
		}

		case CONDITIONPARAM_SUBID: {
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
	while (propStream.GET_UCHAR(attr_type) && attr_type != CONDITIONATTR_END) {
		if (!unserializeProp((ConditionAttr_t)attr_type, propStream)) {
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
			if (!propStream.GET_VALUE(value)) {
				return false;
			}

			conditionType = (ConditionType_t)value;
			return true;
		}

		case CONDITIONATTR_ID: {
			int32_t value;
			if (!propStream.GET_VALUE(value)) {
				return false;
			}

			id = (ConditionId_t)value;
			return true;
		}

		case CONDITIONATTR_TICKS: {
			int32_t value;
			if (!propStream.GET_VALUE(value)) {
				return false;
			}

			ticks = value;
			return true;
		}

		case CONDITIONATTR_ISBUFF: {
			int8_t value;
			if (!propStream.GET_VALUE(value)) {
				return false;
			}

			isBuff = value != 0;
			return true;
		}

		case CONDITIONATTR_SUBID: {
			int32_t value;
			if (!propStream.GET_VALUE(value)) {
				return false;
			}

			subId = value;
			return true;
		}

		case CONDITIONATTR_END:
			return true;

		default:
			return false;
	}
}

bool Condition::serialize(PropWriteStream& propWriteStream)
{
	propWriteStream.ADD_UCHAR(CONDITIONATTR_TYPE);
	propWriteStream.ADD_VALUE((int32_t)conditionType);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_ID);
	propWriteStream.ADD_VALUE((int32_t)id);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_TICKS);
	propWriteStream.ADD_VALUE(ticks);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_ISBUFF);
	propWriteStream.ADD_VALUE((int8_t)isBuff);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_SUBID);
	propWriteStream.ADD_VALUE(subId);
	return true;
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

Condition* Condition::createCondition(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, int32_t param/* = 0*/, bool _buff/* = false*/, uint32_t _subId/* = 0*/)
{
	switch (_type) {
		case CONDITION_POISON:
		case CONDITION_FIRE:
		case CONDITION_ENERGY:
		case CONDITION_DROWN:
		case CONDITION_FREEZING:
		case CONDITION_DAZZLED:
		case CONDITION_CURSED:
		case CONDITION_BLEEDING:
			return new ConditionDamage(_id, _type, _buff, _subId);

		case CONDITION_HASTE:
		case CONDITION_PARALYZE:
			return new ConditionSpeed(_id, _type, _ticks, _buff, _subId, param);

		case CONDITION_INVISIBLE:
			return new ConditionInvisible(_id, _type, _ticks, _buff, _subId);

		case CONDITION_OUTFIT:
			return new ConditionOutfit(_id, _type, _ticks, _buff, _subId);

		case CONDITION_LIGHT:
			return new ConditionLight(_id, _type, _ticks, _buff, _subId, param & 0xFF, (param & 0xFF00) >> 8);

		case CONDITION_REGENERATION:
			return new ConditionRegeneration(_id, _type, _ticks, _buff, _subId);

		case CONDITION_SOUL:
			return new ConditionSoul(_id, _type, _ticks, _buff, _subId);

		case CONDITION_MANASHIELD:
			return new ConditionManaShield(_id, _type, _ticks, _buff, _subId);

		case CONDITION_ATTRIBUTES:
			return new ConditionAttributes(_id, _type, _ticks, _buff, _subId);

		case CONDITION_SPELLCOOLDOWN:
			return new ConditionSpellCooldown(_id, _type, _ticks, _buff, _subId);

		case CONDITION_SPELLGROUPCOOLDOWN:
			return new ConditionSpellGroupCooldown(_id, _type, _ticks, _buff, _subId);

		case CONDITION_INFIGHT:
		case CONDITION_DRUNK:
		case CONDITION_EXHAUST_WEAPON:
		case CONDITION_EXHAUST_COMBAT:
		case CONDITION_EXHAUST_HEAL:
		case CONDITION_MUTED:
		case CONDITION_CHANNELMUTEDTICKS:
		case CONDITION_YELLTICKS:
		case CONDITION_PACIFIED:
			return new ConditionGeneric(_id, _type, _ticks, _buff, _subId);

		default:
			return nullptr;
	}
}


Condition* Condition::createCondition(PropStream& propStream)
{
	uint8_t attr;
	if (!propStream.GET_UCHAR(attr) || attr != CONDITIONATTR_TYPE) {
		return nullptr;
	}

	uint32_t _type;
	if (!propStream.GET_ULONG(_type)) {
		return nullptr;
	}

	if (!propStream.GET_UCHAR(attr) || attr != CONDITIONATTR_ID) {
		return nullptr;
	}

	uint32_t _id;
	if (!propStream.GET_ULONG(_id)) {
		return nullptr;
	}

	if (!propStream.GET_UCHAR(attr) || attr != CONDITIONATTR_TICKS) {
		return nullptr;
	}

	uint32_t _ticks;
	if (!propStream.GET_ULONG(_ticks)) {
		return nullptr;
	}

	if (!propStream.GET_UCHAR(attr) || attr != CONDITIONATTR_ISBUFF) {
		return nullptr;
	}

	uint8_t _buff;
	if (!propStream.GET_UCHAR(_buff)) {
		return nullptr;
	}

	if (!propStream.GET_UCHAR(attr) || attr != CONDITIONATTR_SUBID) {
		return nullptr;
	}

	uint32_t _subId;
	if (!propStream.GET_ULONG(_subId)) {
		return nullptr;
	}

	return createCondition((ConditionId_t)_id, (ConditionType_t)_type, _ticks, 0, _buff != 0, _subId);
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

	if (!(id == CONDITIONID_DEFAULT || id == CONDITIONID_COMBAT)) {
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

ConditionGeneric::ConditionGeneric(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId) :
	Condition(_id, _type, _ticks, _buff, _subId)
{
	//
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

void ConditionGeneric::addCondition(Creature*, const Condition* addCondition)
{
	if (updateCondition(addCondition)) {
		setTicks(addCondition->getTicks());
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

ConditionAttributes::ConditionAttributes(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId) :
ConditionGeneric(_id, _type, _ticks, _buff, _subId), skills(), skillsPercent(), stats(), statsPercent()
{
	currentSkill = 0;
	currentStat = 0;
}

void ConditionAttributes::addCondition(Creature* creature, const Condition* addCondition)
{
	if (updateCondition(addCondition)) {
		setTicks(addCondition->getTicks());

		const ConditionAttributes& conditionAttrs = static_cast<const ConditionAttributes&>(*addCondition);
		//Remove the old condition
		endCondition(creature);

		//Apply the new one
		memcpy(skills, conditionAttrs.skills, sizeof(skills));
		memcpy(skillsPercent, conditionAttrs.skillsPercent, sizeof(skillsPercent));
		memcpy(stats, conditionAttrs.stats, sizeof(stats));
		memcpy(statsPercent, conditionAttrs.statsPercent, sizeof(statsPercent));

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
		int32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		skills[currentSkill++] = value;
		return true;
	} else if (attr == CONDITIONATTR_STATS) {
		int32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		stats[currentStat++] = value;
		return true;
	}

	return Condition::unserializeProp(attr, propStream);
}

bool ConditionAttributes::serialize(PropWriteStream& propWriteStream)
{
	if (!Condition::serialize(propWriteStream)) {
		return false;
	}

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		propWriteStream.ADD_UCHAR(CONDITIONATTR_SKILLS);
		propWriteStream.ADD_VALUE(skills[i]);
	}

	for (int32_t i = STAT_FIRST; i <= STAT_LAST; ++i) {
		propWriteStream.ADD_UCHAR(CONDITIONATTR_STATS);
		propWriteStream.ADD_VALUE(stats[i]);
	}

	return true;
}

bool ConditionAttributes::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

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
				stats[i] = (int32_t)(player->getMaxHealth() * ((statsPercent[i] - 100) / 100.f));
				break;

			case STAT_MAXMANAPOINTS:
				stats[i] = (int32_t)(player->getMaxMana() * ((statsPercent[i] - 100) / 100.f));
				break;

			case STAT_SOULPOINTS:
				stats[i] = (int32_t)(player->getPlayerInfo(PLAYERINFO_SOUL) * ((statsPercent[i] - 100) / 100.f));
				break;

			case STAT_MAGICPOINTS:
				stats[i] = (int32_t)(player->getMagicLevel() * ((statsPercent[i] - 100) / 100.f));
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
			player->setVarStats((stats_t)i, stats[i]);
		}
	}

	if (needUpdateStats) {
		player->sendStats();
	}
}

void ConditionAttributes::updatePercentSkills(Player* player)
{
	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (skillsPercent[i] == 0) {
			continue;
		}

		int32_t currSkill = player->getSkill((skills_t)i, SKILL_LEVEL);
		skills[i] = (int32_t)(currSkill * ((skillsPercent[i] - 100) / 100.f));
	}
}

void ConditionAttributes::updateSkills(Player* player)
{
	bool needUpdateSkills = false;

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (skills[i]) {
			needUpdateSkills = true;
			player->setVarSkill((skills_t)i, skills[i]);
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
				const int new_skill = skills[i];
				player->setVarSkill((skills_t)i, -new_skill);
			}
		}

		if (needUpdateSkills) {
			player->sendSkills();
		}

		bool needUpdateStats = false;

		for (int32_t i = STAT_FIRST; i <= STAT_LAST; ++i) {
			if (stats[i]) {
				needUpdateStats = true;
				player->setVarStats((stats_t)i, -stats[i]);
			}
		}

		if (needUpdateStats) {
			player->sendStats();
		}
	}
}

bool ConditionAttributes::setParam(ConditionParam_t param, int32_t value)
{
	bool ret = ConditionGeneric::setParam(param, value);

	switch (param) {
		case CONDITIONPARAM_SKILL_MELEE: {
			skills[SKILL_CLUB] = value;
			skills[SKILL_AXE] = value;
			skills[SKILL_SWORD] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_MELEEPERCENT: {
			skillsPercent[SKILL_CLUB] = value;
			skillsPercent[SKILL_AXE] = value;
			skillsPercent[SKILL_SWORD] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_FIST: {
			skills[SKILL_FIST] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_FISTPERCENT: {
			skillsPercent[SKILL_FIST] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_CLUB: {
			skills[SKILL_CLUB] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_CLUBPERCENT: {
			skillsPercent[SKILL_CLUB] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_SWORD: {
			skills[SKILL_SWORD] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_SWORDPERCENT: {
			skillsPercent[SKILL_SWORD] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_AXE: {
			skills[SKILL_AXE] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_AXEPERCENT: {
			skillsPercent[SKILL_AXE] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_DISTANCE: {
			skills[SKILL_DIST] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_DISTANCEPERCENT: {
			skillsPercent[SKILL_DIST] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_SHIELD: {
			skills[SKILL_SHIELD] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_SHIELDPERCENT: {
			skillsPercent[SKILL_SHIELD] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_FISHING: {
			skills[SKILL_FISH] = value;
			return true;
		}

		case CONDITIONPARAM_SKILL_FISHINGPERCENT: {
			skillsPercent[SKILL_FISH] = value;
			return true;
		}

		case CONDITIONPARAM_STAT_MAXHITPOINTS: {
			stats[STAT_MAXHITPOINTS] = value;
			return true;
		}

		case CONDITIONPARAM_STAT_MAXMANAPOINTS: {
			stats[STAT_MAXMANAPOINTS] = value;
			return true;
		}

		case CONDITIONPARAM_STAT_SOULPOINTS: {
			stats[STAT_SOULPOINTS] = value;
			return true;
		}

		case CONDITIONPARAM_STAT_MAGICPOINTS: {
			stats[STAT_MAGICPOINTS] = value;
			return true;
		}

		case CONDITIONPARAM_STAT_MAXHITPOINTSPERCENT: {
			statsPercent[STAT_MAXHITPOINTS] = std::max<int32_t>(0, value);
			return true;
		}

		case CONDITIONPARAM_STAT_MAXMANAPOINTSPERCENT: {
			statsPercent[STAT_MAXMANAPOINTS] = std::max<int32_t>(0, value);
			return true;
		}

		case CONDITIONPARAM_STAT_SOULPOINTSPERCENT: {
			statsPercent[STAT_SOULPOINTS] = std::max<int32_t>(0, value);
			return true;
		}

		case CONDITIONPARAM_STAT_MAGICPOINTSPERCENT: {
			statsPercent[STAT_MAGICPOINTS] = std::max<int32_t>(0, value);
			return true;
		}

		default:
			return ret;
	}
}

ConditionRegeneration::ConditionRegeneration(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId) :
	ConditionGeneric(_id, _type, _ticks, _buff, _subId)
{
	internalHealthTicks = 0;
	internalManaTicks = 0;

	healthTicks = 1000;
	manaTicks = 1000;

	healthGain = 0;
	manaGain = 0;
}

void ConditionRegeneration::addCondition(Creature*, const Condition* addCondition)
{
	if (updateCondition(addCondition)) {
		setTicks(addCondition->getTicks());

		const ConditionRegeneration& conditionRegen = static_cast<const ConditionRegeneration&>(*addCondition);

		healthTicks = conditionRegen.healthTicks;
		manaTicks = conditionRegen.manaTicks;

		healthGain = conditionRegen.healthGain;
		manaGain = conditionRegen.manaGain;
	}
}

bool ConditionRegeneration::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_HEALTHTICKS) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		healthTicks = value;
		return true;
	} else if (attr == CONDITIONATTR_HEALTHGAIN) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		healthGain = value;
		return true;
	} else if (attr == CONDITIONATTR_MANATICKS) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		manaTicks = value;
		return true;
	} else if (attr == CONDITIONATTR_MANAGAIN) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		manaGain = value;
		return true;
	}

	return Condition::unserializeProp(attr, propStream);
}

bool ConditionRegeneration::serialize(PropWriteStream& propWriteStream)
{
	if (!Condition::serialize(propWriteStream)) {
		return false;
	}

	propWriteStream.ADD_UCHAR(CONDITIONATTR_HEALTHTICKS);
	propWriteStream.ADD_VALUE(healthTicks);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_HEALTHGAIN);
	propWriteStream.ADD_VALUE(healthGain);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_MANATICKS);
	propWriteStream.ADD_VALUE(manaTicks);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_MANAGAIN);
	propWriteStream.ADD_VALUE(manaGain);
	return true;
}

bool ConditionRegeneration::executeCondition(Creature* creature, int32_t interval)
{
	internalHealthTicks += interval;
	internalManaTicks += interval;

	if (creature->getZone() != ZONE_PROTECTION) {
		if (internalHealthTicks >= healthTicks) {
			internalHealthTicks = 0;

			int32_t realHealthGain = creature->getHealth();
			creature->changeHealth(healthGain);
			realHealthGain = creature->getHealth() - realHealthGain;

			if (isBuff && realHealthGain > 0) {
				Player* player = creature->getPlayer();
				if (player) {
					std::ostringstream ss;
					ss << ucfirst(player->getNameDescription()) << " was healed for " << realHealthGain << " hitpoint" << (realHealthGain != 1 ? "s." : ".");
					std::string message = ss.str();

					std::ostringstream tmpSs;
					tmpSs << "You were healed for " << realHealthGain << " hitpoint" << (realHealthGain != 1 ? "s." : ".");
					player->sendHealMessage(MSG_HEALED, tmpSs.str(), player->getPosition(), realHealthGain, TEXTCOLOR_MAYABLUE);

					SpectatorVec list;
					g_game.getSpectators(list, player->getPosition(), false, true);
					for (Creature* spectator : list) {
						if (spectator != player) {
							spectator->getPlayer()->sendHealMessage(MSG_HEALED_OTHERS, message, player->getPosition(), realHealthGain, TEXTCOLOR_MAYABLUE);
						}
					}
				}
			}
		}

		if (internalManaTicks >= manaTicks) {
			internalManaTicks = 0;
			creature->changeMana(manaGain);
		}
	}

	return ConditionGeneric::executeCondition(creature, interval);
}

bool ConditionRegeneration::setParam(ConditionParam_t param, int32_t value)
{
	bool ret = ConditionGeneric::setParam(param, value);

	switch (param) {
		case CONDITIONPARAM_HEALTHGAIN:
			healthGain = value;
			return true;

		case CONDITIONPARAM_HEALTHTICKS:
			healthTicks = value;
			return true;

		case CONDITIONPARAM_MANAGAIN:
			manaGain = value;
			return true;

		case CONDITIONPARAM_MANATICKS:
			manaTicks = value;
			return true;

		default:
			return ret;
	}
}

ConditionSoul::ConditionSoul(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId) :
	ConditionGeneric(_id, _type, _ticks, _buff, _subId)
{
	internalSoulTicks = 0;
	soulTicks = 0;
	soulGain = 0;
}

void ConditionSoul::addCondition(Creature*, const Condition* addCondition)
{
	if (updateCondition(addCondition)) {
		setTicks(addCondition->getTicks());

		const ConditionSoul& conditionSoul = static_cast<const ConditionSoul&>(*addCondition);

		soulTicks = conditionSoul.soulTicks;
		soulGain = conditionSoul.soulGain;
	}
}

bool ConditionSoul::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_SOULGAIN) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		soulGain = value;
		return true;
	} else if (attr == CONDITIONATTR_SOULTICKS) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		soulTicks = value;
		return true;
	}

	return Condition::unserializeProp(attr, propStream);
}

bool ConditionSoul::serialize(PropWriteStream& propWriteStream)
{
	if (!Condition::serialize(propWriteStream)) {
		return false;
	}

	propWriteStream.ADD_UCHAR(CONDITIONATTR_SOULGAIN);
	propWriteStream.ADD_VALUE(soulGain);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_SOULTICKS);
	propWriteStream.ADD_VALUE(soulTicks);

	return true;
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
		case CONDITIONPARAM_SOULGAIN:
			soulGain = value;
			return true;

		case CONDITIONPARAM_SOULTICKS:
			soulTicks = value;
			return true;

		default:
			return ret;
	}
}

ConditionDamage::ConditionDamage(ConditionId_t _id, ConditionType_t _type, bool _buff, uint32_t _subId) :
	Condition(_id, _type, 0, _buff, _subId)
{
	delayed = false;
	forceUpdate = false;
	owner = 0;
	minDamage = 0;
	maxDamage = 0;
	startDamage = 0;
	periodDamage = 0;
	periodDamageTick = 0;
	tickInterval = 2000;
}

bool ConditionDamage::setParam(ConditionParam_t param, int32_t value)
{
	bool ret = Condition::setParam(param, value);

	switch (param) {
		case CONDITIONPARAM_OWNER:
			owner = value;
			return true;

		case CONDITIONPARAM_FORCEUPDATE:
			forceUpdate = (value != 0);
			return true;

		case CONDITIONPARAM_DELAYED:
			delayed = (value != 0);
			return true;

		case CONDITIONPARAM_MAXVALUE:
			maxDamage = std::abs(value);
			break;

		case CONDITIONPARAM_MINVALUE:
			minDamage = std::abs(value);
			break;

		case CONDITIONPARAM_STARTVALUE:
			startDamage = std::abs(value);
			break;

		case CONDITIONPARAM_TICKINTERVAL:
			tickInterval = std::abs(value);
			break;

		case CONDITIONPARAM_PERIODICDAMAGE:
			periodDamage = value;
			break;

		default:
			return false;
	}

	return ret;
}

bool ConditionDamage::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_DELAYED) {
		bool value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		delayed = value;
		return true;
	} else if (attr == CONDITIONATTR_PERIODDAMAGE) {
		int32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		periodDamage = value;
		return true;
	} else if (attr == CONDITIONATTR_OWNER) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		// owner = value;
		return true;
	} else if (attr == CONDITIONATTR_INTERVALDATA) {
		IntervalInfo damageInfo;
		if (!propStream.GET_VALUE(damageInfo)) {
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

bool ConditionDamage::serialize(PropWriteStream& propWriteStream)
{
	if (!Condition::serialize(propWriteStream)) {
		return false;
	}

	propWriteStream.ADD_UCHAR(CONDITIONATTR_DELAYED);
	propWriteStream.ADD_VALUE(delayed);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_PERIODDAMAGE);
	propWriteStream.ADD_VALUE(periodDamage);

	// propWriteStream.ADD_UCHAR(CONDITIONATTR_OWNER);
	// propWriteStream.ADD_VALUE(owner);

	for (const IntervalInfo& intervalInfo : damageList) {
		propWriteStream.ADD_UCHAR(CONDITIONATTR_INTERVALDATA);
		propWriteStream.ADD_VALUE(intervalInfo);
	}
	return true;
}

bool ConditionDamage::updateCondition(const Condition* addCondition)
{
	const ConditionDamage& conditionDamage = static_cast<const ConditionDamage&>(*addCondition);
	if (conditionDamage.doForceUpdate()) {
		return true;
	}

	if (ticks == -1 && conditionDamage.getTicks() > 0) {
		return false;
	}

	if (conditionDamage.getTicks() <= ticks) {
		return false;
	}

	if (conditionDamage.getTotalDamage() < getTotalDamage()) {
		return false;
	}

	if (conditionDamage.periodDamage < periodDamage) {
		return false;
	}

	return true;
}

bool ConditionDamage::addDamage(int32_t rounds, int32_t time, int32_t value)
{
	if (rounds == -1) {
		//periodic damage
		periodDamage = value;
		setParam(CONDITIONPARAM_TICKINTERVAL, time);
		setParam(CONDITIONPARAM_TICKS, -1);
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

	if (damageList.empty()) {
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
	}
	return (!damageList.empty());
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

bool ConditionDamage::doDamage(Creature* creature, int32_t damage)
{
	if (creature->isSuppress(getType())) {
		return true;
	}

	CombatType_t combatType = Combat::ConditionToDamageType(conditionType);
	Creature* attacker = g_game.getCreatureByID(owner);
	if (g_game.combatBlockHit(combatType, attacker, creature, damage, false, false)) {
		return false;
	}

	return g_game.combatChangeHealth(combatType, attacker, creature, damage);
}

void ConditionDamage::endCondition(Creature*)
{
	//
}

void ConditionDamage::addCondition(Creature* creature, const Condition* addCondition)
{
	if (addCondition->getType() != conditionType) {
		return;
	}

	if (!updateCondition(addCondition)) {
		return;
	}

	const ConditionDamage& conditionDamage = static_cast<const ConditionDamage&>(*addCondition);

	setTicks(addCondition->getTicks());
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
	int32_t result = 0;
	if (!damageList.empty()) {
		for (const IntervalInfo& intervalInfo : damageList) {
			result += intervalInfo.value;
		}
	} else {
		result = maxDamage + (maxDamage - minDamage) / 2;
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
	int32_t med = 0;
	double x1, x2;

	for (int32_t i = start; i > 0; --i) {
		int32_t n = start + 1 - i;
		med = (n * amount) / start;

		do {
			sum += i;
			list.push_back(i);

			x1 = std::fabs(1.0 - (((float)sum) + i) / med);
			x2 = std::fabs(1.0 - (((float)sum) / med));
		} while (x1 < x2);
	}
}

ConditionSpeed::ConditionSpeed(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId, int32_t changeSpeed) :
	Condition(_id, _type, _ticks, _buff, _subId)
{
	speedDelta = changeSpeed;
	mina = 0.0f;
	minb = 0.0f;
	maxa = 0.0f;
	maxb = 0.0f;
}

void ConditionSpeed::setFormulaVars(float _mina, float _minb, float _maxa, float _maxb)
{
	mina = _mina;
	minb = _minb;
	maxa = _maxa;
	maxb = _maxb;
}

void ConditionSpeed::getFormulaValues(int32_t var, int32_t& min, int32_t& max) const
{
	min = (int32_t)std::ceil(var * 1.f * mina + minb);
	max = (int32_t)std::ceil(var * 1.f * maxa + maxb);
}

bool ConditionSpeed::setParam(ConditionParam_t param, int32_t value)
{
	Condition::setParam(param, value);
	if (param != CONDITIONPARAM_SPEED) {
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
		int32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		speedDelta = value;
		return true;
	} else if (attr == CONDITIONATTR_FORMULA_MINA) {
		float value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		mina = value;
		return true;
	} else if (attr == CONDITIONATTR_FORMULA_MINB) {
		float value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		minb = value;
		return true;
	} else if (attr == CONDITIONATTR_FORMULA_MAXA) {
		float value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		maxa = value;
		return true;
	} else if (attr == CONDITIONATTR_FORMULA_MAXB) {
		float value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		maxb = value;
		return true;
	}

	return Condition::unserializeProp(attr, propStream);
}

bool ConditionSpeed::serialize(PropWriteStream& propWriteStream)
{
	if (!Condition::serialize(propWriteStream)) {
		return false;
	}

	propWriteStream.ADD_UCHAR(CONDITIONATTR_SPEEDDELTA);
	propWriteStream.ADD_VALUE(speedDelta);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_FORMULA_MINA);
	propWriteStream.ADD_VALUE(mina);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_FORMULA_MINB);
	propWriteStream.ADD_VALUE(minb);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_FORMULA_MAXA);
	propWriteStream.ADD_VALUE(maxa);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_FORMULA_MAXB);
	propWriteStream.ADD_VALUE(maxb);
	return true;
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

void ConditionSpeed::addCondition(Creature* creature, const Condition* addCondition)
{
	if (conditionType != addCondition->getType()) {
		return;
	}

	if (ticks == -1 && addCondition->getTicks() > 0) {
		return;
	}

	setTicks(addCondition->getTicks());

	const ConditionSpeed& conditionSpeed = static_cast<const ConditionSpeed&>(*addCondition);
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

ConditionInvisible::ConditionInvisible(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId) :
	ConditionGeneric(_id, _type, _ticks, _buff, _subId)
{
	//
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

ConditionOutfit::ConditionOutfit(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId) :
	Condition(_id, _type, _ticks, _buff, _subId)
{
	//
}

void ConditionOutfit::addOutfit(const Outfit_t& outfit)
{
	outfits.push_back(outfit);
}

bool ConditionOutfit::unserializeProp(ConditionAttr_t attr, PropStream& propStream)
{
	if (attr == CONDITIONATTR_OUTFIT) {
		Outfit_t outfit;

		if (!propStream.GET_VALUE(outfit)) {
			return false;
		}

		outfits.push_back(outfit);
		return true;
	}

	return Condition::unserializeProp(attr, propStream);
}

bool ConditionOutfit::serialize(PropWriteStream& propWriteStream)
{
	if (!Condition::serialize(propWriteStream)) {
		return false;
	}

	for (const Outfit_t& outfit : outfits) {
		propWriteStream.ADD_UCHAR(CONDITIONATTR_OUTFIT);
		propWriteStream.ADD_VALUE(outfit);
	}
	return true;
}

bool ConditionOutfit::startCondition(Creature* creature)
{
	if (!Condition::startCondition(creature)) {
		return false;
	}

	changeOutfit(creature);
	return true;
}

bool ConditionOutfit::executeCondition(Creature* creature, int32_t interval)
{
	return Condition::executeCondition(creature, interval);
}

void ConditionOutfit::changeOutfit(Creature* creature, int32_t index /*= -1*/)
{
	if (!outfits.empty()) {
		if (index == -1) {
			index = uniform_random(0, outfits.size() - 1);
		}

		Outfit_t outfit = outfits[index];
		g_game.internalCreatureChangeOutfit(creature, outfit);
	}
}

void ConditionOutfit::endCondition(Creature* creature)
{
	g_game.internalCreatureChangeOutfit(creature, creature->getDefaultOutfit());
}

void ConditionOutfit::addCondition(Creature* creature, const Condition* addCondition)
{
	if (updateCondition(addCondition)) {
		setTicks(addCondition->getTicks());

		const ConditionOutfit& conditionOutfit = static_cast<const ConditionOutfit&>(*addCondition);
		outfits = conditionOutfit.outfits;

		changeOutfit(creature);
	}
}

ConditionLight::ConditionLight(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId, int32_t _lightlevel, int32_t _lightcolor) :
	Condition(_id, _type, _ticks, _buff, _subId)
{
	lightInfo.level = _lightlevel;
	lightInfo.color = _lightcolor;
	internalLightTicks = 0;
	lightChangeInterval = 0;
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
		LightInfo creatureLight;
		creature->getCreatureLight(creatureLight);

		if (creatureLight.level > 0) {
			--creatureLight.level;
			creature->setCreatureLight(creatureLight);
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

void ConditionLight::addCondition(Creature* creature, const Condition* addCondition)
{
	if (updateCondition(addCondition)) {
		setTicks(addCondition->getTicks());

		const ConditionLight& conditionLight = static_cast<const ConditionLight&>(*addCondition);
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
		case CONDITIONPARAM_LIGHT_LEVEL:
			lightInfo.level = value;
			return true;

		case CONDITIONPARAM_LIGHT_COLOR:
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
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		lightInfo.color = value;
		return true;
	} else if (attr == CONDITIONATTR_LIGHTLEVEL) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		lightInfo.level = value;
		return true;
	} else if (attr == CONDITIONATTR_LIGHTTICKS) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		internalLightTicks = value;
		return true;
	} else if (attr == CONDITIONATTR_LIGHTINTERVAL) {
		uint32_t value;
		if (!propStream.GET_VALUE(value)) {
			return false;
		}

		lightChangeInterval = value;
		return true;
	}

	return Condition::unserializeProp(attr, propStream);
}

bool ConditionLight::serialize(PropWriteStream& propWriteStream)
{
	if (!Condition::serialize(propWriteStream)) {
		return false;
	}

	propWriteStream.ADD_UCHAR(CONDITIONATTR_LIGHTCOLOR);
	propWriteStream.ADD_VALUE(lightInfo.color);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_LIGHTLEVEL);
	propWriteStream.ADD_VALUE(lightInfo.level);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_LIGHTTICKS);
	propWriteStream.ADD_VALUE(internalLightTicks);

	propWriteStream.ADD_UCHAR(CONDITIONATTR_LIGHTINTERVAL);
	propWriteStream.ADD_VALUE(lightChangeInterval);
	return true;
}

ConditionSpellCooldown::ConditionSpellCooldown(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId) :
	ConditionGeneric(_id, _type, _ticks, _buff, _subId)
{
	//
}

void ConditionSpellCooldown::addCondition(Creature* creature, const Condition* addCondition)
{
	if (updateCondition(addCondition)) {
		setTicks(addCondition->getTicks());

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

ConditionSpellGroupCooldown::ConditionSpellGroupCooldown(ConditionId_t _id, ConditionType_t _type, int32_t _ticks, bool _buff, uint32_t _subId) :
	ConditionGeneric(_id, _type, _ticks, _buff, _subId)
{
	//
}

void ConditionSpellGroupCooldown::addCondition(Creature* creature, const Condition* addCondition)
{
	if (updateCondition(addCondition)) {
		setTicks(addCondition->getTicks());

		if (subId != 0 && ticks > 0) {
			Player* player = creature->getPlayer();
			if (player) {
				player->sendSpellGroupCooldown((SpellGroup_t)subId, ticks);
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
			player->sendSpellGroupCooldown((SpellGroup_t)subId, ticks);
		}
	}
	return true;
}
