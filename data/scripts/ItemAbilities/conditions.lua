ABILITY_CONDITIONS = {

}

local enums = ItemAbilities.getEnums()
for _, enum in ipairs(enums) do
	local _type = enum:match('ITEM_ABILITY_([%a_]*)')
	if _type and _G['CONDITION_PARAM_' .. _type] then
		ABILITY_CONDITIONS[_G[enum]] = {
			key = _G['CONDITION_PARAM_' .. _type]
		}
	end
end

local subId = 5880

for attrKey, def in pairs(ABILITY_CONDITIONS) do
	if not def.conditionType and not def.condition then
		def.conditionType = CONDITION_ATTRIBUTES
		def.condition = Condition(CONDITION_ATTRIBUTES, CONDITIONID_DEFAULT)
	end
	def.subId = subId
	subId = subId + 1

	def.condition:setParameter(CONDITION_PARAM_SUBID, def.subId)
	def.condition:setParameter(CONDITION_PARAM_TICKS, -1)
end