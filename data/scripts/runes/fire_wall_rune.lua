local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_FIREFIELD_PVP_FULL)
combat:setArea(createCombatArea(AREA_WALLFIELD, AREADIAGONAL_WALLFIELD))

local fireWallRune = Spell("rune")

function fireWallRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

fireWallRune:group("attack")
fireWallRune:id(28)
fireWallRune:name("Fire Wall Rune")
fireWallRune:runeId(2303)
fireWallRune:allowFarUse(true)
fireWallRune:charges(4)
fireWallRune:level(33)
fireWallRune:magicLevel(6)
fireWallRune:cooldown(2000)
fireWallRune:groupCooldown(2000)
fireWallRune:isBlocking(true)
fireWallRune:register()
