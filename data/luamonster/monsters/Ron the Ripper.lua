local mType = Game.createMonsterType("Ron the Ripper")
local monster = {}
monster.description = "ron the ripper"
monster.experience = 500
monster.outfit = {
	lookFeet = 97,
	lookLegs = 117,
	lookHead = 95,
	lookType = 151,
	lookAddons = 1,
	lookBody = 94
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20502
monster.speed = 240
monsters.runHealth = 250
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 50
}

monster.loot = {
    {id = "Ron the Ripper's sabre", chance = 100000},
	{id = "gold coin", maxCount = 50, chance = 100000},
	{id = "knight armor", chance = 1200},
	{id = "double axe", chance = 1500},
	{id = "plate armor", chance = 4000},
	{id = "dagger", chance = 8500},
	{id = "skull of Ratha", maxCount = 2, chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -200, maxDamage = -250, interval = 2000, effect = 0}
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