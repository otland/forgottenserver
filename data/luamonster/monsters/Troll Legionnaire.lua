local mType = Game.createMonsterType("Troll Legionnaire")
local monster = {}
monster.description = "a troll legionnaire"
monster.experience = 140
monster.outfit = {
	lookType = 53
}

monster.health = 210
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5998
monster.speed = 190
monster.runHealth = 30
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 5
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 9,
	{text = "Attack!", yell = false},
	{text = "Graaaaar!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 155, chance = 92000},
	{id = "throwing star", maxCount = 10, chance = 28000},
	{id = "frosty ear of a troll", chance = 5120},
	{id = "stealth ring", chance = 560}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -35, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 18, interval = 2000, minDamage = 0, maxDamage = -130, range = 6, shootEffect = 8}
}

monster.defenses = {
	defense = 9,
    armor = 7,
    {name = "invisible",  chance = 18, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 8, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 28, interval = 2000, minDamage = 17, maxDamage = 25, shootEffect = 8, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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