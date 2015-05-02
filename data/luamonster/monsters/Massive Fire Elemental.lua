local mType = Game.createMonsterType("Massive Fire Elemental")
local monster = {}
monster.description = "a massive fire elemental"
monster.experience = 1400
monster.outfit = {
	lookType = 242
}

monster.health = 1200
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6324
monster.speed = 210
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 25000},
	{id = "gold coin", maxCount = 12, chance = 25000},
	{id = "bronze amulet", subType = 200, chance = 15000},
	{id = "small ruby", maxCount = 2, chance = 6100},
	{id = "rusty armor", chance = 1330},
	{id = "wand of inferno", chance = 2240},
	{id = "magma amulet", subType = 200, chance = 1300},
	{id = "magma boots", chance = 560},
	{id = "fire sword", chance = 530},
	{id = "magma legs", chance = 210}
}

monster.attacks = {
	{name = "melee",  attack = 75, skill = 70, minDamage = 0, maxDamage = -300, interval = 2000, effect = 0},
	{name = "firefield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 3, shootEffect = 4},
	{name = "firefield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 3, shootEffect = 4},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -700, length = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -250, radius = 3, shootEffect = 4, effect = 6},
	{name = "massive fire elemental soulfire",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 6}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 40},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)