area = {
	{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0}
}

local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_POFF)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)
combat:setArea(createCombatArea(area))

function onTargetCreature(creature, target)
	local min = 200
	local max = 700
	local master = target:getMaster()
	if target:isPlayer() and not master or master and master:isPlayer() then
		doTargetCombat(0, target, COMBAT_ICEDAMAGE, min, max, CONST_ME_NONE)
		return true
	end

	doTargetCombat(0, target, COMBAT_HEALING, min, max, CONST_ME_NONE)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("frozen minion wave")
spell:words("###56")
spell:blockWalls(true)
spell:needLearn(true)
spell:needDirection(true)
spell:register()
