local mType = Game.createMonsterType("Killer Rabbit")
local monster = {}
monster.description = "a killer rabbit"
monster.experience = 160
monster.outfit = {
	lookType = 74
}

monster.health = 205
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6017
monster.speed = 300
monster.maxSummons = 2

monster.changeTarget = {
    interval = 5000,
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

monster.summons = {
    {name = "killer rabbit", chance = 30, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Who is lunch NOW?", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 50000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -100, interval = 1200, effect = 0},
	{name = "Blood Rage",  chance = 30, interval = 2000, minDamage = 0, maxDamage = 0}
}

monster.defenses = {
	defense = 35,
    armor = 30,
    {name = "speed",  chance = 40, interval = 1000, minDamage = 0, maxDamage = 0, duration = 8000, speedChange = 380, effect = 12},
	{name = "invisible",  chance = 30, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13}
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