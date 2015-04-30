local mType = Game.createMonsterType("Hydra")
local monster = {}
monster.description = "a hydra"
monster.experience = 2100
monster.outfit = {
	lookType = 121
}

monster.health = 2350
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6048
monster.speed = 250
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "FCHHHHH", yell = false},
	{text = "HISSSS", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 34000},
	{id = "gold coin", maxCount = 100, chance = 34000},
	{id = "gold coin", maxCount = 46, chance = 20000},
	{id = "platinum coin", maxCount = 3, chance = 48000},
	{id = "ham", maxCount = 4, chance = 60000},
	{id = "strong mana potion", chance = 380},
	{id = "life crystal", chance = 570},
	{id = "stone skin amulet", subType = 5, chance = 900},
	{id = "small sapphire", chance = 5000},
	{id = "hydra egg", chance = 930},
	{id = "ring of healing", chance = 1190},
	{id = "warrior helmet", chance = 890},
	{id = "knight armor", chance = 1000},
	{id = "medusa shield", chance = 270},
	{id = "royal helmet", chance = 210},
	{id = "boots of haste", chance = 130},
	{id = "hydra head", chance = 10120},
	{id = "cucumber", chance = 4780}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 80, interval = 2000, effect = 0},
	{name = "speed",  chance = 25, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -700, range = 7, radius = 4, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -250, length = 8, spread = 3, radius = 4, shootEffect = 15, effect = 2},
	{name = "icicle",  chance = 10, interval = 2000, minDamage = -80, maxDamage = -155, radius = 4, shootEffect = 15, effect = 2},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -66, maxDamage = -320, length = 8, spread = 3, radius = 4, shootEffect = 15, effect = 47}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 2000, minDamage = 260, maxDamage = 407, radius = 4, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 30},
	{type = COMBAT_ICEDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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