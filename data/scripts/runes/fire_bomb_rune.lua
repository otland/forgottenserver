local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_FIREFIELD_PVP_FULL)
combat:setArea(createCombatArea(AREA_SQUARE1X1))

local fireBombRune = Spell("rune")

function fireBombRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

fireBombRune:group("attack")
fireBombRune:id(17)
fireBombRune:name("Fire Bomb Rune")
fireBombRune:runeId(2305)
fireBombRune:allowFarUse(true)
fireBombRune:charges(2)
fireBombRune:level(27)
fireBombRune:magicLevel(5)
fireBombRune:cooldown(2000)
fireBombRune:groupCooldown(2000)
fireBombRune:isBlocking(true)
fireBombRune:register()
