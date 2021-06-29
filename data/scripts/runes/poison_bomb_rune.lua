local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_POISONFIELD_PVP)
combat:setArea(createCombatArea(AREA_SQUARE1X1))

local poisonBombRune = Spell("rune")

function poisonBombRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

poisonBombRune:group("attack")
poisonBombRune:id(91)
poisonBombRune:name("Poison Bomb Rune")
poisonBombRune:runeId(2286)
poisonBombRune:allowFarUse(true)
poisonBombRune:charges(2)
poisonBombRune:level(25)
poisonBombRune:magicLevel(4)
poisonBombRune:cooldown(2000)
poisonBombRune:groupCooldown(2000)
poisonBombRune:isBlocking(true)
poisonBombRune:register()
