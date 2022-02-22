// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CONDITION_H_F92FF8BDDD5B4EA59E2B1BB5C9C0A086
#define FS_CONDITION_H_F92FF8BDDD5B4EA59E2B1BB5C9C0A086

#include "fileloader.h"
#include "enums.h"

class Creature;
class Player;
class PropStream;

enum ConditionAttr_t {
	CONDITIONATTR_TYPE = 1,
	CONDITIONATTR_ID,
	CONDITIONATTR_TICKS,
	CONDITIONATTR_HEALTHTICKS,
	CONDITIONATTR_HEALTHGAIN,
	CONDITIONATTR_MANATICKS,
	CONDITIONATTR_MANAGAIN,
	CONDITIONATTR_DELAYED,
	CONDITIONATTR_OWNER,
	CONDITIONATTR_INTERVALDATA,
	CONDITIONATTR_SPEEDDELTA,
	CONDITIONATTR_FORMULA_MINA,
	CONDITIONATTR_FORMULA_MINB,
	CONDITIONATTR_FORMULA_MAXA,
	CONDITIONATTR_FORMULA_MAXB,
	CONDITIONATTR_LIGHTCOLOR,
	CONDITIONATTR_LIGHTLEVEL,
	CONDITIONATTR_LIGHTTICKS,
	CONDITIONATTR_LIGHTINTERVAL,
	CONDITIONATTR_SOULTICKS,
	CONDITIONATTR_SOULGAIN,
	CONDITIONATTR_SKILLS,
	CONDITIONATTR_STATS,
	CONDITIONATTR_OUTFIT,
	CONDITIONATTR_PERIODDAMAGE,
	CONDITIONATTR_ISBUFF,
	CONDITIONATTR_SUBID,
	CONDITIONATTR_ISAGGRESSIVE,
	CONDITIONATTR_DISABLEDEFENSE,
	CONDITIONATTR_SPECIALSKILLS,

	//reserved for serialization
	CONDITIONATTR_END = 254,
};

struct IntervalInfo {
	int32_t timeLeft;
	int32_t value;
	int32_t interval;
};

class Condition
{
	public:
		Condition() = default;
		Condition(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff = false, uint32_t subId = 0, bool aggressive = false) :
			endTime(ticks == -1 ? std::numeric_limits<int64_t>::max() : 0),
			subId(subId), ticks(ticks), conditionType(type), isBuff(buff), aggressive(aggressive), id(id) {}
		virtual ~Condition() = default;

		virtual bool startCondition(Creature* creature);
		virtual bool executeCondition(Creature* creature, int32_t interval);
		virtual void endCondition(Creature* creature) = 0;
		virtual void addCondition(Creature* creature, const Condition* condition) = 0;
		virtual uint32_t getIcons() const;
		ConditionId_t getId() const {
			return id;
		}
		uint32_t getSubId() const {
			return subId;
		}

		virtual Condition* clone() const = 0;

		ConditionType_t getType() const {
			return conditionType;
		}
		int64_t getEndTime() const {
			return endTime;
		}
		int32_t getTicks() const {
			return ticks;
		}
		void setTicks(int32_t newTicks);
		bool isAggressive() const {
			return aggressive;
		}

		static Condition* createCondition(ConditionId_t id, ConditionType_t type, int32_t ticks, int32_t param = 0, bool buff = false, uint32_t subId = 0, bool aggressive = false);
		static Condition* createCondition(PropStream& propStream);

		virtual bool setParam(ConditionParam_t param, int32_t value);
		virtual int32_t getParam(ConditionParam_t param);

		//serialization
		bool unserialize(PropStream& propStream);
		virtual void serialize(PropWriteStream& propWriteStream);
		virtual bool unserializeProp(ConditionAttr_t attr, PropStream& propStream);

		bool isPersistent() const;

	protected:
		virtual bool updateCondition(const Condition* addCondition);

		int64_t endTime;
		uint32_t subId;
		int32_t ticks;
		ConditionType_t conditionType;
		bool isBuff;
		bool aggressive;

	private:
		ConditionId_t id;
};

class ConditionGeneric : public Condition
{
	public:
		ConditionGeneric(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff = false, uint32_t subId = 0, bool aggressive = false):
			Condition(id, type, ticks, buff, subId, aggressive) {}

		bool startCondition(Creature* creature) override;
		bool executeCondition(Creature* creature, int32_t interval) override;
		void endCondition(Creature* creature) override;
		void addCondition(Creature* creature, const Condition* condition) override;
		uint32_t getIcons() const override;

		ConditionGeneric* clone() const override {
			return new ConditionGeneric(*this);
		}
};

class ConditionAttributes final : public ConditionGeneric
{
	public:
		ConditionAttributes(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff = false, uint32_t subId = 0, bool aggressive = false) :
			ConditionGeneric(id, type, ticks, buff, subId, aggressive) {}

		bool startCondition(Creature* creature) override;
		bool executeCondition(Creature* creature, int32_t interval) override;
		void endCondition(Creature* creature) override;
		void addCondition(Creature* creature, const Condition* condition) override;

		bool setParam(ConditionParam_t param, int32_t value) override;
		int32_t getParam(ConditionParam_t param) override;

		ConditionAttributes* clone() const override {
			return new ConditionAttributes(*this);
		}

		//serialization
		void serialize(PropWriteStream& propWriteStream) override;
		bool unserializeProp(ConditionAttr_t attr, PropStream& propStream) override;

	private:
		int32_t skills[SKILL_LAST + 1] = {};
		int32_t skillsPercent[SKILL_LAST + 1] = {};
		int32_t specialSkills[SPECIALSKILL_LAST + 1] = {};
		int32_t stats[STAT_LAST + 1] = {};
		int32_t statsPercent[STAT_LAST + 1] = {};
		int32_t currentSkill = 0;
		int32_t currentSpecialSkill = 0;
		int32_t currentStat = 0;

		bool disableDefense = false;

		void updatePercentStats(Player* player);
		void updateStats(Player* player);
		void updatePercentSkills(Player* player);
		void updateSkills(Player* player);
};

class ConditionRegeneration final : public ConditionGeneric
{
	public:
		ConditionRegeneration(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff = false, uint32_t subId = 0, bool aggressive = false):
			ConditionGeneric(id, type, ticks, buff, subId, aggressive) {}

		void addCondition(Creature* creature, const Condition* condition) override;
		bool executeCondition(Creature* creature, int32_t interval) override;

		bool setParam(ConditionParam_t param, int32_t value) override;
		int32_t getParam(ConditionParam_t param) override;

		ConditionRegeneration* clone() const override {
			return new ConditionRegeneration(*this);
		}

		//serialization
		void serialize(PropWriteStream& propWriteStream) override;
 		bool unserializeProp(ConditionAttr_t attr, PropStream& propStream) override;

	private:
		uint32_t internalHealthTicks = 0;
		uint32_t internalManaTicks = 0;

		uint32_t healthTicks = 1000;
		uint32_t manaTicks = 1000;
		uint32_t healthGain = 0;
		uint32_t manaGain = 0;
};

class ConditionSoul final : public ConditionGeneric
{
	public:
		ConditionSoul(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff = false, uint32_t subId = 0, bool aggressive = false) :
			ConditionGeneric(id, type, ticks, buff, subId, aggressive) {}

		void addCondition(Creature* creature, const Condition* condition) override;
		bool executeCondition(Creature* creature, int32_t interval) override;

		bool setParam(ConditionParam_t param, int32_t value) override;
		int32_t getParam(ConditionParam_t param) override;

		ConditionSoul* clone() const override {
			return new ConditionSoul(*this);
		}

		//serialization
		void serialize(PropWriteStream& propWriteStream) override;
		bool unserializeProp(ConditionAttr_t attr, PropStream& propStream) override;

	private:
		uint32_t internalSoulTicks = 0;
		uint32_t soulTicks = 0;
		uint32_t soulGain = 0;
};

class ConditionInvisible final : public ConditionGeneric
{
	public:
		ConditionInvisible(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff = false, uint32_t subId = 0, bool aggressive = false) :
			ConditionGeneric(id, type, ticks, buff, subId, aggressive) {}

		bool startCondition(Creature* creature) override;
		void endCondition(Creature* creature) override;

		ConditionInvisible* clone() const override {
			return new ConditionInvisible(*this);
		}
};

class ConditionDamage final : public Condition
{
	public:
		ConditionDamage() = default;
		ConditionDamage(ConditionId_t id, ConditionType_t type, bool buff = false, uint32_t subId = 0, bool aggressive = true) :
			Condition(id, type, 0, buff, subId, aggressive) {}

		static void generateDamageList(int32_t amount, int32_t start, std::list<int32_t>& list);

		bool startCondition(Creature* creature) override;
		bool executeCondition(Creature* creature, int32_t interval) override;
		void endCondition(Creature* creature) override;
		void addCondition(Creature* creature, const Condition* condition) override;
		uint32_t getIcons() const override;

		ConditionDamage* clone() const override {
			return new ConditionDamage(*this);
		}

		bool setParam(ConditionParam_t param, int32_t value) override;
		int32_t getParam(ConditionParam_t param) override;

		bool addDamage(int32_t rounds, int32_t time, int32_t value);
		bool doForceUpdate() const {
			return forceUpdate;
		}
		int32_t getTotalDamage() const;

		void setInitDamage(int32_t initDamage) {
			this->initDamage = initDamage;
		}

		//serialization
		void serialize(PropWriteStream& propWriteStream) override;
		bool unserializeProp(ConditionAttr_t attr, PropStream& propStream) override;

	private:
		int32_t maxDamage = 0;
		int32_t minDamage = 0;
		int32_t startDamage = 0;
		int32_t periodDamage = 0;
		int32_t periodDamageTick = 0;
		int32_t tickInterval = 2000;
		int32_t initDamage = 0;

		bool forceUpdate = false;
		bool delayed = false;
		bool field = false;
		uint32_t owner = 0;

		bool init();

		std::list<IntervalInfo> damageList;

		bool getNextDamage(int32_t& damage);
		bool doDamage(Creature* creature, int32_t healthChange);

		bool updateCondition(const Condition* addCondition) override;
};

class ConditionSpeed final : public Condition
{
	public:
		ConditionSpeed(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff, uint32_t subId, int32_t changeSpeed, bool aggressive = false) :
			Condition(id, type, ticks, buff, subId, aggressive), speedDelta(changeSpeed) {}

		bool startCondition(Creature* creature) override;
		bool executeCondition(Creature* creature, int32_t interval) override;
		void endCondition(Creature* creature) override;
		void addCondition(Creature* creature, const Condition* condition) override;
		uint32_t getIcons() const override;

		ConditionSpeed* clone() const override {
			return new ConditionSpeed(*this);
		}

		bool setParam(ConditionParam_t param, int32_t value) override;
		int32_t getParam(ConditionParam_t param) override;

		void setFormulaVars(float mina, float minb, float maxa, float maxb);

		//serialization
		void serialize(PropWriteStream& propWriteStream) override;
		bool unserializeProp(ConditionAttr_t attr, PropStream& propStream) override;

	private:
		int32_t speedDelta;

		//formula variables
		float mina = 0.0f;
		float minb = 0.0f;
		float maxa = 0.0f;
		float maxb = 0.0f;
};

class ConditionOutfit final : public Condition
{
	public:
		ConditionOutfit(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff = false, uint32_t subId = 0, bool aggressive = false) :
			Condition(id, type, ticks, buff, subId, aggressive) {}

		bool startCondition(Creature* creature) override;
		bool executeCondition(Creature* creature, int32_t interval) override;
		void endCondition(Creature* creature) override;
		void addCondition(Creature* creature, const Condition* condition) override;

		ConditionOutfit* clone() const override {
			return new ConditionOutfit(*this);
		}

		void setOutfit(const Outfit_t& outfit);

		//serialization
		void serialize(PropWriteStream& propWriteStream) override;
		bool unserializeProp(ConditionAttr_t attr, PropStream& propStream) override;

	private:
		Outfit_t outfit;
};

class ConditionLight final : public Condition
{
	public:
		ConditionLight(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff, uint32_t subId, uint8_t lightlevel, uint8_t lightcolor, bool aggressive = false) :
			Condition(id, type, ticks, buff, subId, aggressive), lightInfo(lightlevel, lightcolor) {}

		bool startCondition(Creature* creature) override;
		bool executeCondition(Creature* creature, int32_t interval) override;
		void endCondition(Creature* creature) override;
		void addCondition(Creature* creature, const Condition* condition) override;

		ConditionLight* clone() const override {
			return new ConditionLight(*this);
		}

		bool setParam(ConditionParam_t param, int32_t value) override;
		int32_t getParam(ConditionParam_t param) override;

		//serialization
		void serialize(PropWriteStream& propWriteStream) override;
		bool unserializeProp(ConditionAttr_t attr, PropStream& propStream) override;

	private:
		LightInfo lightInfo;
		uint32_t internalLightTicks = 0;
		uint32_t lightChangeInterval = 0;
};

class ConditionSpellCooldown final : public ConditionGeneric
{
	public:
		ConditionSpellCooldown(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff = false, uint32_t subId = 0, bool aggressive = false) :
			ConditionGeneric(id, type, ticks, buff, subId, aggressive) {}

		bool startCondition(Creature* creature) override;
		void addCondition(Creature* creature, const Condition* condition) override;

		ConditionSpellCooldown* clone() const override {
			return new ConditionSpellCooldown(*this);
		}
};

class ConditionSpellGroupCooldown final : public ConditionGeneric
{
	public:
		ConditionSpellGroupCooldown(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff = false, uint32_t subId = 0, bool aggressive = false) :
			ConditionGeneric(id, type, ticks, buff, subId, aggressive) {}

		bool startCondition(Creature* creature) override;
		void addCondition(Creature* creature, const Condition* condition) override;

		ConditionSpellGroupCooldown* clone() const override {
			return new ConditionSpellGroupCooldown(*this);
		}
};

class ConditionDrunk final : public Condition
{
	public:
		ConditionDrunk(ConditionId_t id, ConditionType_t type, int32_t ticks, bool buff, uint32_t subId, uint8_t drunkenness, bool aggressive = false)
		    	: Condition(id, type, ticks, buff, subId, aggressive) {
			if (drunkenness != 0) {
				this->drunkenness = drunkenness;
			}
		}

		uint32_t getIcons() const override;
		void endCondition(Creature* creature) override;
		bool startCondition(Creature* creature) override;
		bool setParam(ConditionParam_t param, int32_t value) override;
		void addCondition(Creature* creature, const Condition* condition) override;

		ConditionDrunk* clone() const override {
			return new ConditionDrunk(*this);
		}

	private:
		uint8_t drunkenness = 25;

		bool updateCondition(const Condition* addCondition) override;
};

#endif
