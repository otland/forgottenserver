local mType = Game.createMonsterType("Orc Rider")
local monster = {}
monster.description = "a orc rider"
monster.experience = 110
monster.outfit = {
	lookType = 4
}

monster.health = 180
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6010
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
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Grrrrrrr", yell = false},
	{text = "Orc arga Huummmak!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 81, chance = 46000},
	{id = "meat", maxCount = 3, chance = 24000},
	{id = "orcish axe", chance = 6880},
	{id = "wolf tooth chain", chance = 10210},
	{id = "torch", chance = 980},
	{id = "battle shield", chance = 9900},
	{id = "scale armor", chance = 610},
	{id = "obsidian lance", chance = 1100},
	{id = "orc leather", chance = 9760},
	{id = "orc tooth", chance = 2000},
	{id = "warwolf fur", chance = 9410}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 55, minDamage = 0, maxDamage = -130, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 200, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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