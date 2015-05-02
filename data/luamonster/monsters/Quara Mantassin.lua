local mType = Game.createMonsterType("Quara Mantassin")
local monster = {}
monster.description = "a quara mantassin"
monster.experience = 400
monster.outfit = {
	lookType = 72
}

monster.health = 800
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6064
monster.speed = 260
monster.runHealth = 40
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Zuerk Pachak!", yell = false},
	{text = "Shrrrr", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 29, chance = 50000},
	{id = "mantassin tail", chance = 11600},
	{id = "shrimp", maxCount = 5, chance = 5000},
	{id = "halberd", chance = 5000},
	{id = "two handed sword", chance = 1010},
	{id = "cape", chance = 1050},
	{id = "stealth ring", chance = 960},
	{id = "small sapphire", chance = 1000},
	{id = "fish fin", chance = 630},
	{id = "strange helmet", chance = 100},
	{id = "blue robe", chance = 50}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 45, minDamage = 0, maxDamage = -138, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "invisible",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 400, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_EARTHDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)