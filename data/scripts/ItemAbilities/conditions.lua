abilityRegenCondition = Condition(CONDITION_REGENERATION, CONDITIONID_DEFAULT)
abilityRegenCondition:setParameter(CONDITION_PARAM_SUBID, 5879)

ABILITY_CONDITIONS = {
	IA_SPEED = {
		conditionType = CONDITION_HASTE,
		condition = Condition(CONDITION_HASTE, CONDITIONID_DEFAULT),
		formula = true
	},
	IA_HEALTHGAIN = {
		key = CONDITION_PARAM_HEALTHGAIN,
		conditionType = CONDITION_REGENERATION,
		condition = abilityRegenCondition,
		subId = 5879
	},
	IA_HEALTHTICKS = {
		key = CONDITION_PARAM_HEALTHTICKS,
		conditionType = CONDITION_REGENERATION,
		condition = abilityRegenCondition,
		subId = 5879
	},
	IA_MANAGAIN = {
		key = CONDITION_PARAM_MANAGAIN,
		conditionType = CONDITION_REGENERATION,
		condition = abilityRegenCondition,
		subId = 5879
	},
	IA_MANATICKS = {
		key = CONDITION_PARAM_MANATICKS,
		conditionType = CONDITION_REGENERATION,
		condition = abilityRegenCondition,
		subId = 5879
	},
	IA_MANASHIELD = {
		conditionType = CONDITION_MANASHIELD
	},
	IA_INVISIBLE = {
		conditionType = CONDITION_INVISIBLE
	}
}

local enums = ItemAbilities.getEnums()
for _, enum in ipairs(enums) do
	local _type = enum:match('ITEM_ABILITY_([%a_]*)')
	local cTypeA = _G['CONDITION_PARAM_' .. _type]
	local cTypeB = _G['CONDITION_PARAM_SPECIALSKILL_' .. _type]
	if _type and (cTypeA or cTypeB) and not ABILITY_CONDITIONS[_G[enum]] then
		ABILITY_CONDITIONS[_G[enum]] = {
			key = cTypeA or cTypeB
		}
	end
end

local subId = 5880

for attrKey, def in pairs(ABILITY_CONDITIONS) do
	if not def.conditionType then
		def.conditionType = CONDITION_ATTRIBUTES
	end
	if not def.condition then
		def.condition = Condition(def.conditionType, CONDITIONID_DEFAULT)
	end
	if not def.subId then
		def.subId = subId
		subId = subId + 1
		def.condition:setParameter(CONDITION_PARAM_SUBID, def.subId)
	end
	def.condition:setParameter(CONDITION_PARAM_TICKS, -1)
end