--[[

	Burst Arrow example

]]
local area = createCombatArea({
	{1, 1, 1},
	{1, 3, 1},
	{1, 1, 1}
})

local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_EXPLOSIONAREA)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, true)
combat:setFormula(COMBAT_FORMULA_SKILL, 0, 0, 1, 0)
combat:setArea(area)

local burstarrow = Weapon(WEAPON_AMMO)

burstarrow.onUseWeapon = function(player, variant)
	if player:getSkull() == SKULL_BLACK then
		return false
	end

	return combat:execute(player, variant)
end

burstarrow:id(2546)
burstarrow:attack(27)
burstarrow:shootType(CONST_ANI_BURSTARROW)
burstarrow:ammoType("arrow")
burstarrow:maxHitChance(100)
burstarrow:register()

--[[

	Wand of Vortex example

]]
local wov = Weapon(WEAPON_WAND)
wov:id(2190)
wov:damage(8, 18)
wov:element("energy")
wov:level(7)
wov:mana(2)
wov:vocation("sorcerer", true, true)
wov:vocation("master sorcerer")
wov:register()

--[[

	Arbalest example

]]
local arbalest = Weapon(WEAPON_DISTANCE)
arbalest:id(5803)
arbalest:slotType("two-handed") -- it's now a 2h weapon
arbalest:ammoType("bolt")
arbalest:range(6)
arbalest:attack(2)
arbalest:hitChance(2)
arbalest:level(75)
arbalest:wieldedUnproperly(true)
arbalest:register()

--[[

	Earth Barbarian Axe example

]]
local eba = Weapon(WEAPON_AXE)
eba:id(7859)
eba:attack(23)
eba:defense(18, 1)
eba:extraElement(5, COMBAT_EARTHDAMAGE)
eba:charges(1000, true) -- showCharges = true
eba:action("removecharge")
eba:decayTo(2429)
eba:register()
