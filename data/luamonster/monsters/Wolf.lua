local mType = Game.createMonsterType("Wolf")
local monster = {}
monster.description = "a wolf"
monster.experience = 18
monster.outfit = {
	lookType = 27
}

monster.health = 25
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5968
monster.speed = 160
monsters.runHealth = 8
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Yoooohhuuu!", yell = false},
	{text = "Grrrrrr", yell = false}
}

monster.loot = {
    {id = "meat", maxCount = 2, chance = 55000},
	{id = "wolf paw", chance = 980}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 10, minDamage = 0, maxDamage = -20, interval = 2000, effect = 0}
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