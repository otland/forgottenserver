-- flash arrow
local flashArrow = Weapon(WEAPON_AMMO)
flashArrow:id(761)
flashArrow:level(20)
flashArrow:attack(14)
flashArrow:extraElement(14, "energy")
flashArrow:maxHitChance(91)
flashArrow:wieldUnproperly(true)
flashArrow:action("removecount")
flashArrow:shootType("flasharrow")
flashArrow:ammoType("arrow")
flashArrow:register()

-- shiver arrow
local shiverArrow = Weapon(WEAPON_AMMO)
shiverArrow:id(762)
shiverArrow:level(20)
shiverArrow:attack(14)
shiverArrow:extraElement(14, "ice")
shiverArrow:maxHitChance(91)
shiverArrow:wieldUnproperly(true)
shiverArrow:action("removecount")
shiverArrow:shootType("shiverarrow")
shiverArrow:ammoType("arrow")
shiverArrow:register()

-- flaming arrow
local flamingArrow = Weapon(WEAPON_AMMO)
flamingArrow:id(763)
flamingArrow:level(20)
flamingArrow:attack(14)
flamingArrow:extraElement(14, "fire")
flamingArrow:maxHitChance(91)
flamingArrow:wieldUnproperly(true)
flamingArrow:action("removecount")
flamingArrow:shootType("flammingarrow")
flamingArrow:ammoType("arrow")
flamingArrow:register()

-- earth arrow
local earthArrow = Weapon(WEAPON_AMMO)
earthArrow:id(774)
earthArrow:level(20)
earthArrow:attack(14)
earthArrow:extraElement(14, "earth")
earthArrow:maxHitChance(91)
earthArrow:wieldUnproperly(true)
earthArrow:action("removecount")
earthArrow:shootType("eartharrow")
earthArrow:slotType("ammo")
earthArrow:ammoType("arrow")
earthArrow:register()

-- bolt
local bolt = Weapon(WEAPON_AMMO)
bolt:id(3446)
bolt:attack(30)
bolt:maxHitChance(87)
bolt:action("removecount")
bolt:shootType("bolt")
bolt:ammoType("bolt")
bolt:register()

-- arrow
local arrow = Weapon(WEAPON_AMMO)
arrow:id(3447)
arrow:attack(25)
arrow:maxHitChance(91)
arrow:action("removecount")
arrow:shootType("arrow")
arrow:ammoType("arrow")
arrow:register()

-- poison arrow
local poisonArrow = Weapon(WEAPON_AMMO)

local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_POISONARROW)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, true)
combat:setFormula(COMBAT_FORMULA_SKILL, 0, 0, 1, 0)

local condition = Condition(CONDITION_POISON)
condition:setParameter(CONDITION_PARAM_DELAYED, true)
condition:addDamage(4, 4000, -3)
condition:addDamage(9, 4000, -2)
condition:addDamage(20, 4000, -1)
combat:addCondition(condition)

poisonArrow.onUseWeapon = function(player, variant)
	return combat:execute(player, variant)
end

poisonArrow:id(3448)
poisonArrow:attack(23)
poisonArrow:maxHitChance(91)
poisonArrow:action("removecount")
poisonArrow:shootType("poisonarrow")
poisonArrow:ammoType("arrow")
poisonArrow:register()

-- burst arrow
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

local burstArrow = Weapon(WEAPON_AMMO)

burstArrow.onUseWeapon = function(player, variant)
	if player:getSkull() == SKULL_BLACK then
		return false
	end

	return combat:execute(player, variant)
end

burstArrow:id(3449)
burstArrow:attack(27)
burstArrow:shootType("burstarrow")
burstArrow:ammoType("arrow")
burstArrow:maxHitChance(100)
burstArrow:register()

-- power bolt
local powerBolt = Weapon(WEAPON_AMMO)
powerBolt:id(3450)
powerBolt:level(55)
powerBolt:attack(40)
powerBolt:maxHitChance(91)
powerBolt:wieldUnproperly(true)
powerBolt:action("removecount")
powerBolt:shootType("powerbolt")
powerBolt:ammoType("bolt")
powerBolt:register()

-- infernal bolt
local infernalBolt = Weapon(WEAPON_AMMO)
infernalBolt:id(6528)
infernalBolt:level(110)
infernalBolt:attack(72)
infernalBolt:maxHitChance(91)
infernalBolt:wieldUnproperly(true)
infernalBolt:action("removecount")
infernalBolt:shootType("infernalbolt")
infernalBolt:ammoType("bolt")
infernalBolt:register()

-- piercing bolt
local piercingBolt = Weapon(WEAPON_AMMO)
piercingBolt:id(7363)
piercingBolt:level(30)
piercingBolt:attack(33)
piercingBolt:maxHitChance(87)
piercingBolt:wieldUnproperly(true)
piercingBolt:action("removecount")
piercingBolt:shootType("piercingbolt")
piercingBolt:ammoType("bolt")
piercingBolt:register()

-- sniper arrow
local sniperArrow = Weapon(WEAPON_AMMO)
sniperArrow:id(7364)
sniperArrow:level(20)
sniperArrow:attack(28)
sniperArrow:maxHitChance(100)
sniperArrow:wieldUnproperly(true)
sniperArrow:action("removecount")
sniperArrow:shootType("sniperarrow")
sniperArrow:ammoType("arrow")
sniperArrow:register()

-- onyx arrow
local onyxArrow = Weapon(WEAPON_AMMO)
onyxArrow:id(7365)
onyxArrow:level(40)
onyxArrow:attack(38)
onyxArrow:maxHitChance(94)
onyxArrow:wieldUnproperly(true)
onyxArrow:action("removecount")
onyxArrow:shootType("onyxarrow")
onyxArrow:ammoType("arrow")
onyxArrow:register()

-- tarsal arrow
local tarsalArrow = Weapon(WEAPON_AMMO)
tarsalArrow:id(14251)
tarsalArrow:level(30)
tarsalArrow:attack(33)
tarsalArrow:maxHitChance(94)
tarsalArrow:wieldUnproperly(true)
tarsalArrow:action("removecount")
tarsalArrow:shootType("tarsalarrow")
tarsalArrow:ammoType("bolt")
tarsalArrow:register()

-- vortex bolt
local vortexBolt = Weapon(WEAPON_AMMO)
vortexBolt:id(14252)
vortexBolt:level(40)
vortexBolt:attack(36)
vortexBolt:maxHitChance(89)
vortexBolt:wieldUnproperly(true)
vortexBolt:action("removecount")
vortexBolt:shootType("vortexbolt")
vortexBolt:ammoType("bolt")
vortexBolt:register()

-- crystal bolt
local crystalBolt = Weapon(WEAPON_AMMO)
crystalBolt:id(15792)
crystalBolt:attack(36)
crystalBolt:shootType("vortexbolt")
crystalBolt:ammoType("bolt")
crystalBolt:maxHitChance(90)
crystalBolt:register()

-- crystalline arrow
local crystallineArrow = Weapon(WEAPON_AMMO)
crystallineArrow:id(15793)
crystallineArrow:level(90)
crystallineArrow:attack(65)
crystallineArrow:maxHitChance(95)
crystallineArrow:wieldUnproperly(true)
crystallineArrow:action("removecount")
crystallineArrow:shootType("crystallinearrow")
crystallineArrow:ammoType("arrow")
crystallineArrow:register()

-- prismatic bolt
local prismaticBolt = Weapon(WEAPON_AMMO)
prismaticBolt:id(16141)
prismaticBolt:level(90)
prismaticBolt:attack(66)
prismaticBolt:maxHitChance(90)
prismaticBolt:wieldUnproperly(true)
prismaticBolt:action("removecount")
prismaticBolt:shootType("prismaticbolt")
prismaticBolt:ammoType("bolt")
prismaticBolt:register()

-- drill bolt
local drillBolt = Weapon(WEAPON_AMMO)
drillBolt:id(16142)
drillBolt:level(70)
drillBolt:attack(56)
drillBolt:maxHitChance(90)
drillBolt:wieldUnproperly(true)
drillBolt:action("removecount")
drillBolt:shootType("drillbolt")
drillBolt:ammoType("bolt")
drillBolt:register()

-- envenomed arrow
local envenomedeArrow = Weapon(WEAPON_AMMO)
envenomedeArrow:id(16143)
envenomedeArrow:level(70)
envenomedeArrow:attack(27)
envenomedeArrow:extraElement(27, "earth")
envenomedeArrow:maxHitChance(93)
envenomedeArrow:wieldUnproperly(true)
envenomedeArrow:action("removecount")
envenomedeArrow:shootType("envenomedarrow")
envenomedeArrow:ammoType("arrow")
envenomedeArrow:register()

-- simple arrow
local simpleArrow = Weapon(WEAPON_AMMO)
simpleArrow:id(21470)
simpleArrow:level(1)
simpleArrow:attack(20)
simpleArrow:maxHitChance(40)
simpleArrow:wieldUnproperly(false)
simpleArrow:action("removecount")
simpleArrow:shootType("arrow")
simpleArrow:ammoType("arrow")
simpleArrow:register()
