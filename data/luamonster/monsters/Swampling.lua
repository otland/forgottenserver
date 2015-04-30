local mType = Game.createMonsterType("Swampling")
local monster = {}
monster.description = "a swampling"
monster.experience = 45
monster.outfit = {
	lookType = 535
}

monster.health = 80
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 19902
monster.speed = 190
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Gnark!", yell = false},
	{text = "Crrrck!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 60000},
	{id = "white mushroom", maxCount = 5, chance = 8980},
	{id = "swampling moss", chance = 12610},
	{id = "piece of swampling wood", chance = 8270},
	{id = "rat cheese", chance = 18670},
	{id = "rope", chance = 4540}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -2, maxDamage = -15, range = 7, shootEffect = 15},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = -300, length = 3, spread = 2, shootEffect = 15, effect = 54}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
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