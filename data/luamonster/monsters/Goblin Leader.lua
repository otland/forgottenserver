local mType = Game.createMonsterType("Goblin Leader")
local monster = {}
monster.description = "a goblin leader"
monster.experience = 50
monster.outfit = {
	lookType = 61
}

monster.health = 75
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6002
monster.speed = 160
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
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Go go, Gobo attack !!", yell = false},
	{text = "Me the greenest and the meanest!", yell = false},
	{text = "Me have power to crush you!", yell = false},
	{text = "Goblinkiller! Catch him !!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 40000},
	{id = "bone", chance = 11500},
	{id = "leather helmet", chance = 16670},
	{id = "leather armor", chance = 5000},
	{id = "dagger", chance = 10300},
	{id = "fish", chance = 15000},
	{id = "mouldy cheese", chance = 9000},
	{id = "small axe", chance = 12800},
	{id = "short sword", chance = 15400},
	{id = "bone club", chance = 1300}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -45, range = 7, shootEffect = 10}
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