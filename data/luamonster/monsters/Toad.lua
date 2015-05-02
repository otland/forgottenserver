local mType = Game.createMonsterType("Toad")
local monster = {}
monster.description = "a toad"
monster.experience = 60
monster.outfit = {
	lookType = 222
}

monster.health = 135
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6077
monster.speed = 210
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ribbit, ribbit!", yell = false},
	{text = "Ribbit!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 20, chance = 80000},
	{id = "fish", chance = 20000},
	{id = "poisonous slime", chance = 4761},
	{id = "mace", chance = 2854},
	{id = "war hammer", chance = 148}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 10, minDamage = 0, maxDamage = -30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = -8, maxDamage = -17, range = 7, shootEffect = 15, effect = 9}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 200, shootEffect = 15, effect = 14}
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