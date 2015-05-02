local mType = Game.createMonsterType("Brutus Bloodbeard")
local monster = {}
monster.description = "brutus bloodbeard"
monster.experience = 500
monster.outfit = {
	lookType = 98
}

monster.health = 1200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20478
monster.speed = 240
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
    {id = "Brutus Bloodbeard's hat", chance = 100000},
	{id = "gold coin", maxCount = 40, chance = 100000},
	{id = "knight armor", chance = 1200},
	{id = "plate armor", chance = 4000},
	{id = "skull of Ratha", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -450, maxDamage = -500, interval = 2000, effect = 0}
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