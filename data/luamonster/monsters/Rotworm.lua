local mType = Game.createMonsterType("Rotworm")
local monster = {}
monster.description = "a rotworm"
monster.experience = 40
monster.outfit = {
	lookType = 26
}

monster.health = 65
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5967
monster.speed = 150
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
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 70
}

monster.loot = {
    {id = "gold coin", maxCount = 17, chance = 71760},
	{id = "worm", maxCount = 3, chance = 3000},
	{id = "meat", chance = 20000},
	{id = "ham", chance = 20120},
	{id = "sword", chance = 3000},
	{id = "mace", chance = 4500},
	{id = "lump of dirt", chance = 10000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -40, interval = 2000, effect = 0}
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