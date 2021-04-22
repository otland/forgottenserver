local combat = Combat()
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_MAGICWALL)

local magicWallRune = Spell("rune")

function magicWallRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

magicWallRune:group("attack")
magicWallRune:id(86)
magicWallRune:name("Magic Wall Rune")
magicWallRune:runeId(2293)
magicWallRune:allowFarUse(true)
magicWallRune:charges(3)
magicWallRune:level(32)
magicWallRune:magicLevel(9)
magicWallRune:cooldown(2000)
magicWallRune:groupCooldown(2000)
magicWallRune:isBlocking(true, true)
magicWallRune:register()
