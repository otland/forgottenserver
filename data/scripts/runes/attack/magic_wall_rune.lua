local combat = Combat()
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_MAGICWALL)

local spell = Spell(SPELL_RUNE)

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:group("attack")
spell:id(86)
spell:name("Magic Wall Rune")
spell:runeId(2293)
spell:allowFarUse(true)
spell:charges(3)
spell:runeLevel(32)
spell:runeMagicLevel(9)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:isBlocking(true, true)
spell:register()
