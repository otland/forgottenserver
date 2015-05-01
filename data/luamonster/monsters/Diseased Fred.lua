local mType = Game.createMonsterType("Diseased Fred")
local monster = {}
monster.description = "a diseased fred"
monster.experience = 800
monster.outfit = {
	lookType = 299
}

monster.health = 1300
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 8951
monster.speed = 150
monsters.runHealth = 1
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
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
	staticAttack = 95
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "Tchhh!", yell = false},
	{text = "Slurp!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 65, chance = 28000},
	{id = "gold coin", maxCount = 64, chance = 28000},
	{id = "great health potion", chance = 5500},
	{id = "plate legs", chance = 2000},
	{id = "ultimate health potion", chance = 3500},
	{id = "great spirit potion", chance = 3500},
	{id = "belted cape", chance = 1000},
	{id = "springsprout rod", chance = 800},
	{id = "hibiscus dress", chance = 300},
	{id = "paladin armor", chance = 200}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 59, minDamage = 0, maxDamage = -207, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 100, interval = 2000, minDamage = -90, maxDamage = -140, effect = 14},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 1000, minDamage = -100, maxDamage = -175, radius = 2, shootEffect = 39, effect = 14},
	{name = "paralyze",  chance = 40, interval = 3000, minDamage = 0, maxDamage = 0, shootEffect = 39, effect = 14}
}

monster.defenses = {
	defense = 15,
    armor = 10,
    {name = "speed",  chance = 40, interval = 10000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = 310, shootEffect = 39, effect = 15},
	{name = "combat", type = COMBAT_HEALING,  chance = 60, interval = 5000, minDamage = 50, maxDamage = 80, shootEffect = 39, effect = 13}
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