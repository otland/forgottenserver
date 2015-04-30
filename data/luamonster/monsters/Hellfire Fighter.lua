local mType = Game.createMonsterType("Hellfire Fighter")
local monster = {}
monster.description = "a hellfire fighter"
monster.experience = 3900
monster.outfit = {
	lookType = 243
}

monster.health = 3800
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6324
monster.speed = 220
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
	{id = "gold coin", maxCount = 100, chance = 40000},
	{id = "gold coin", maxCount = 46, chance = 10000},
	{id = "blank rune", maxCount = 2, chance = 30000},
	{id = "bronze amulet", subType = 200, chance = 1000},
	{id = "burnt scroll", chance = 50000},
	{id = "demonic essence", chance = 14500},
	{id = "emerald bangle", chance = 2200},
	{id = "fire sword", chance = 4140},
	{id = "magma legs", chance = 730},
	{id = "magma coat", chance = 470},
	{id = "small diamond", chance = 1400},
	{id = "soul orb", chance = 12150},
	{id = "wand of inferno", chance = 9450},
	{id = "fire axe", chance = 440},
	{id = "demonbone amulet", chance = 190},
	{id = "coal", chance = 670},
	{id = "fiery heart", chance = 9570},
	{id = "piece of hellfire armor", chance = 5060}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 120, interval = 2000, effect = 0},
	{name = "firefield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 3, shootEffect = 4},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -392, maxDamage = -1500, length = 8, radius = 3, shootEffect = 4, effect = 37},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -330, range = 7, radius = 3, shootEffect = 4, effect = 16},
	{name = "hellfire fighter soulfire",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, shootEffect = 4, effect = 16}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -25}
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