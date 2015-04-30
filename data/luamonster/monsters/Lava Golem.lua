local mType = Game.createMonsterType("Lava Golem")
local monster = {}
monster.description = "lava golem"
monster.experience = 6200
monster.outfit = {
	lookType = 491
}

monster.health = 9000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 17333
monster.speed = 290
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Grrrrunt", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 199, chance = 100000},
	{id = "platinum coin", maxCount = 11, chance = 100000},
	{id = "strong health potion", maxCount = 2, chance = 18830},
	{id = "strong mana potion", maxCount = 2, chance = 18150},
	{id = "great mana potion", maxCount = 2, chance = 16030},
	{id = "mana potion", maxCount = 2, chance = 20720},
	{id = "ultimate health potion", chance = 15390},
	{id = "prismatic bolt", maxCount = 5, chance = 12580},
	{id = "fiery heart", chance = 13510},
	{id = "iron ore", chance = 11570},
	{id = "yellow piece of cloth", chance = 7230},
	{id = "yellow gem", chance = 6480},
	{id = "white piece of cloth", chance = 4810},
	{id = "wand of inferno", chance = 2950},
	{id = "fire sword", chance = 1710},
	{id = "magma amulet", subType = 200, chance = 2900},
	{id = "magma boots", chance = 2650},
	{id = "red piece of cloth", chance = 3530},
	{id = "wand of everblazing", chance = 1390},
	{id = "green crystal splinter", maxCount = 2, chance = 13990},
	{id = "red crystal fragment", chance = 10990},
	{id = "violet crystal shard", chance = 6200},
	{id = "magma clump", maxCount = 2, chance = 15370},
	{id = "blazing bone", chance = 15200},
	{id = "red gem", chance = 1180},
	{id = "crown shield", chance = 1180},
	{id = "fire axe", chance = 1560},
	{id = "magma coat", chance = 430},
	{id = "spellbook of mind control", chance = 450}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -350, maxDamage = -700, length = 8, effect = 37},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -600, maxDamage = -1300, length = 8, effect = 18},
	{name = "lava golem soulfire",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, effect = 18},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -220, maxDamage = -350, target = true, radius = 4, effect = 7},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -800, length = 5, spread = 3, radius = 4, effect = 4},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 30, interval = 2000, minDamage = -280, maxDamage = -350, radius = 3, effect = 16}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)