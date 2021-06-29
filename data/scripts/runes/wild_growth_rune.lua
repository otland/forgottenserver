local combat = Combat()
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_WILDGROWTH)

local wildGrowthRune = Spell("rune")

function wildGrowthRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

wildGrowthRune:group("attack")
wildGrowthRune:id(94)
wildGrowthRune:name("Wild Growth Rune")
wildGrowthRune:runeId(2269)
wildGrowthRune:allowFarUse(true)
wildGrowthRune:charges(2)
wildGrowthRune:level(27)
wildGrowthRune:magicLevel(8)
wildGrowthRune:cooldown(2000)
wildGrowthRune:groupCooldown(2000)
wildGrowthRune:isBlocking(true, true)
wildGrowthRune:vocation("Druid;true", "Elder Druid")
wildGrowthRune:register()
