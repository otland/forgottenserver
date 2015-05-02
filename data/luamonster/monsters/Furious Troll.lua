local mType = Game.createMonsterType("Furious Troll")
local monster = {}
monster.description = "a furious troll"
monster.experience = 185
monster.outfit = {
	lookType = 15
}

monster.health = 245
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5960
monster.speed = 190
monster.maxSummons = 2

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
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "Mechanical Fighter", chance = 90, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Slice! Slice!", yell = false},
	{text = "DIE!!!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 146, chance = 93000},
	{id = "platinum coin", chance = 6000},
	{id = "bunch of troll hair", chance = 4400},
	{id = "war hammer", chance = 750}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0}
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