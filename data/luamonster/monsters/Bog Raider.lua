local mType = Game.createMonsterType("Bog Raider")
local monster = {}
monster.description = "a bog raider"
monster.experience = 800
monster.outfit = {
	lookType = 299
}

monster.health = 1300
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 8951
monster.speed = 240
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 60
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Tchhh!", yell = false},
	{text = "Slurp!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50750},
	{id = "gold coin", maxCount = 5, chance = 50750},
	{id = "boggy dreads", chance = 9870},
	{id = "great health potion", chance = 2000},
	{id = "springsprout rod", chance = 1030},
	{id = "great spirit potion", chance = 2008},
	{id = "plate legs", chance = 2040},
	{id = "ultimate health potion", chance = 740},
	{id = "belted cape", chance = 590},
	{id = "paladin armor", chance = 110}
}

monster.attacks = {
	{name = "melee",  attack = 53, skill = 59, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -90, maxDamage = -140, range = 7, effect = 14},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -175, radius = 3, effect = 26},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -96, maxDamage = -110, range = 7, radius = 3, shootEffect = 39, effect = 26},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -600, range = 7, radius = 3, shootEffect = 39, effect = 46}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 65, maxDamage = 95, radius = 3, shootEffect = 39, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 30},
	{type = COMBAT_FIREDAMAGE, percent = 85},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)