local mType = Game.createMonsterType("Mr. Punish")
local monster = {}
monster.description = "a mr. punish"
monster.experience = 5500
monster.outfit = {
	lookType = 234
}

monster.health = 12000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6331
monster.speed = 470
monsters.runHealth = 2000
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
	targetDistance = 1,
	staticAttack = 50
}

monster.loot = {
    {id = "Mr. Punish's handcuffs", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -660, maxDamage = -1280, interval = 2000, effect = 0}
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
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)