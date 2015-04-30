local mType = Game.createMonsterType("Rottie the Rotworm")
local monster = {}
monster.description = "a rottie the rotworm"
monster.experience = 40
monster.outfit = {
	lookFeet = 50,
	lookLegs = 40,
	lookHead = 20,
	lookType = 26,
	lookBody = 30
}

monster.health = 65
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5967
monster.speed = 180
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 6230},
	{id = "copper shield", chance = 2850},
	{id = "meat", maxCount = 2, chance = 3260},
	{id = "worm", maxCount = 5, chance = 32500},
	{id = "mace", chance = 3335},
	{id = "ham", maxCount = 2, chance = 3160},
	{id = "sword", chance = 3335},
	{id = "katana", chance = 900},
	{id = "legion helmet", chance = 1250}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 30, interval = 2000, effect = 0}
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