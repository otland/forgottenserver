local mType = Game.createMonsterType("Gnarlhound")
local monster = {}
monster.description = "a gnarlhound"
monster.experience = 60
monster.outfit = {
	lookType = 341
}

monster.health = 198
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11250
monster.speed = 280
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Gnarllll!", yell = false},
	{text = "Grrrrrr!", yell = false}
}

monster.loot = {
    {id = "meat", maxCount = 3, chance = 39075},
	{id = "gold coin", maxCount = 30, chance = 48000},
	{id = "worm", maxCount = 3, chance = 33300},
	{id = "shaggy tail", chance = 25550}
}

monster.attacks = {
	{name = "melee",  attack = 35, skill = 30, interval = 2000, effect = 0}
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