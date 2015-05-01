local mType = Game.createMonsterType("Undead Minion")
local monster = {}
monster.description = "an undead minion"
monster.experience = 550
monster.outfit = {
	lookType = 37
}

monster.health = 850
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 5963
monster.speed = 230
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 1
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 100000},
	{id = "blank rune", chance = 10000},
	{id = "battle hammer", chance = 5000},
	{id = "battle shield", chance = 1000},
	{id = "surprise bag", chance = 5538},
	{id = "surprise bag", chance = 1538},
	{id = "guardian shield", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 55, skill = 80, minDamage = 0, maxDamage = -248, interval = 1000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 13, interval = 1000, minDamage = -100, maxDamage = -160, radius = 4, effect = 18}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
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