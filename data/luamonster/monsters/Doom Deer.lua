local mType = Game.createMonsterType("Doom Deer")
local monster = {}
monster.description = "a doom deer"
monster.experience = 200
monster.outfit = {
	lookType = 31
}

monster.health = 405
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5970
monster.speed = 300
monster.runHealth = 25
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
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
	{text = "I bet it was you who killed my mom!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 30, interval = 4000, minDamage = -35, maxDamage = -55, length = 5, spread = 3, effect = 41}
}

monster.defenses = {
	defense = 35,
    armor = 30,
    {name = "speed",  chance = 30, interval = 3000, minDamage = 0, maxDamage = 0, duration = 8000, speedChange = 400, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
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