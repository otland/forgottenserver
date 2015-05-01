local mType = Game.createMonsterType("Troll Guard")
local monster = {}
monster.description = "a troll guard"
monster.experience = 25
monster.outfit = {
	lookType = 281
}

monster.health = 60
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7926
monster.speed = 180
monsters.runHealth = 17
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 20
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 58000},
	{id = "meat", maxCount = 2, chance = 14000},
	{id = "rope", chance = 10000},
	{id = "studded club", chance = 3000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -15, interval = 2000, effect = 0}
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