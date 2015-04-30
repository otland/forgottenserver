local mType = Game.createMonsterType("Drillworm")
local monster = {}
monster.description = "drillworm"
monster.experience = 1100
monster.outfit = {
	lookType = 527
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 19705
monster.speed = 240
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 20
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
	{text = "Knarrrk!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 50, chance = 50000},
	{id = "worm", maxCount = 5, chance = 4580},
	{id = "drill bolt", maxCount = 2, chance = 4870},
	{id = "coal", chance = 12130},
	{id = "lump of earth", chance = 15000},
	{id = "pick", chance = 5280},
	{id = "iron ore", chance = 1750},
	{id = "terra amulet", subType = 200, chance = 2360},
	{id = "guardian shield", chance = 2240},
	{id = "spiked squelcher", chance = 540},
	{id = "clay lump", chance = 700},
	{id = "vein of ore", chance = 7280},
	{id = "pulverized ore", chance = 5400},
	{id = "green crystal splinter", chance = 9780},
	{id = "brown crystal splinter", chance = 9260},
	{id = "blue crystal splinter", chance = 9610}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -600, length = 8, effect = 14},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -300, length = 8, effect = 17},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -150, target = true, radius = 3, effect = 21}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_DEATHDAMAGE, percent = 15},
	{type = COMBAT_ICEDAMAGE, percent = 16},
	{type = COMBAT_HOLYDAMAGE, percent = 25},
	{type = COMBAT_FIREDAMAGE, percent = -5}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)