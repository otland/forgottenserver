local mType = Game.createMonsterType("War Wolf")
local monster = {}
monster.description = "a war wolf"
monster.experience = 55
monster.outfit = {
	lookType = 3
}

monster.health = 140
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6009
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Grrrrrrr", yell = false},
	{text = "Yoooohhuuuu!", yell = true}
}

monster.loot = {
    {id = "ham", maxCount = 2, chance = 35000},
	{id = "wolf paw", chance = 710},
	{id = "worm", chance = 40},
	{id = "warwolf fur", chance = 5230}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 40, minDamage = 0, maxDamage = -50, interval = 2000, effect = 0}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = true, combat = false},
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