local combat = createCombatObject()
local area = createCombatArea(AREA_CROSS5X5)
setCombatArea(combat, area)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)
setCombatParam(combat, COMBAT_PARAM_AGGRESSIVE, 0)

local condition = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(condition, CONDITION_PARAM_SUBID, 1)
setConditionParam(condition, CONDITION_PARAM_BUFF_SPELL, 1)
setConditionParam(condition, CONDITION_PARAM_TICKS, 2 * 60 * 1000)
setConditionParam(condition, CONDITION_PARAM_SKILL_MELEE, 3)
setConditionParam(condition, CONDITION_PARAM_SKILL_DISTANCE, 3)

local baseMana = 60
function onCastSpell(cid, var)
	local pos = getCreaturePosition(cid)

	local membersList = getPartyMembers(cid)
	if(membersList == nil or type(membersList) ~= 'table' or #membersList <= 1) then
		doPlayerSendCancel(cid, "No party members in range.")
		doSendMagicEffect(pos, CONST_ME_POFF)
		return LUA_ERROR
	end

	local affectedList = {}
	for _, pid in ipairs(membersList) do
		if(getDistanceBetween(getCreaturePosition(pid), pos) <= 36) then
			table.insert(affectedList, pid)
		end
	end

	local tmp = #affectedList
	if(tmp <= 1) then
		doPlayerSendCancel(cid, "No party members in range.")
		doSendMagicEffect(pos, CONST_ME_POFF)
		return LUA_ERROR
	end

	local mana = math.ceil((0.9 ^ (tmp - 1) * baseMana) * tmp)
	if(getPlayerMana(cid) < mana) then
		doPlayerSendDefaultCancel(cid, RETURNVALUE_NOTENOUGHMANA)
		doSendMagicEffect(pos, CONST_ME_POFF)
		return LUA_ERROR
	end

	if(doCombat(cid, combat, var) ~= LUA_NO_ERROR) then
		doPlayerSendDefaultCancel(cid, RETURNVALUE_NOTPOSSIBLE)
		doSendMagicEffect(pos, CONST_ME_POFF)
		return LUA_ERROR
	end

	doPlayerAddMana(cid, -(mana - baseMana), FALSE)
	doPlayerAddManaSpent(cid, (mana - baseMana))
	for _, pid in ipairs(affectedList) do
		doAddCondition(pid, condition)
	end

	return LUA_NO_ERROR
end
