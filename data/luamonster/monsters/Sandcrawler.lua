local mType = Game.createMonsterType("Sandcrawler")
local monster = {}
monster.description = "a sandcrawler"
monster.experience = 20
monster.outfit = {
	lookType = 350
}

monster.health = 30
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 11357
monster.speed = 170
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Chrk chrk!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 6, chance = 33333},
	{id = "sandcrawler shell", chance = 2173}
}

monster.attacks = {
	{name = "melee",  attack = 3, skill = 10, minDamage = 0, maxDamage = -3, interval = 2000, effect = 0}
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