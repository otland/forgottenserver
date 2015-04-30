local mType = Game.createMonsterType("Mutated Tiger")
local monster = {}
monster.description = "a mutated tiger"
monster.experience = 750
monster.outfit = {
	lookType = 318
}

monster.health = 1100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9913
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "GRAAARRRRRR", yell = false},
	{text = "CHHHHHHHHHHH", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 5, chance = 50000},
	{id = "striped fur", chance = 20130},
	{id = "meat", maxCount = 2, chance = 29500},
	{id = "sabretooth", chance = 10600},
	{id = "life ring", chance = 5580},
	{id = "strong health potion", chance = 6000},
	{id = "guardian shield", chance = 380},
	{id = "glorious axe", chance = 870},
	{id = "angelic axe", chance = 440},
	{id = "silky tapestry", chance = 730}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -200, length = 5, spread = 3, effect = 4}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 150, maxDamage = 300, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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