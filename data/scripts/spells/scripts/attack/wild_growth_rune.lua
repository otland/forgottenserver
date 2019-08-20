local combat = Combat()
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EARTH)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_WILDGROWTH)

local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:name("Wild Growth Rune")
spell:group("attack")
spell:vocation("druid;true", "elder druid")
spell:id(94)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(27)
spell:magicLevel(8)
spell:runeId(2269)
spell:charges(2)
spell:isBlocking(true, true)
spell:allowFarUse(true)
spell:register()
