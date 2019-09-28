local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_POISONFIELD_PVP)
combat:setArea(createCombatArea(AREA_WALLFIELD, AREADIAGONAL_WALLFIELD))

local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:name("Poison Wall Rune")
spell:group("attack")
spell:id(32)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(29)
spell:magicLevel(5)
spell:runeId(2289)
spell:charges(4)
spell:isBlocking(true)
spell:allowFarUse(true)
spell:register()
