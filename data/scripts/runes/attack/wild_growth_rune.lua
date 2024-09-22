local combat = Combat()
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_WILDGROWTH)

local spell = Spell(SPELL_RUNE)

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:group("attack")
spell:id(94)
spell:name("Wild Growth Rune")
spell:runeId(2269)
spell:allowFarUse(true)
spell:charges(2)
spell:runeLevel(27)
spell:runeMagicLevel(8)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:isBlocking(true, true)
spell:vocation("druid;true", "elder druid")
spell:register()
