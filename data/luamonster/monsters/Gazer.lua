local mType = Game.createMonsterType("Gazer")
local monster = {}
monster.description = "a gazer"
monster.experience = 90
monster.outfit = {
	lookType = 109
}

monster.health = 120
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6036
monster.speed = 180
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Mommy!?", yell = false},
	{text = "Buuuuhaaaahhaaaaa!", yell = false},
	{text = "Me need mana!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 16, chance = 65000},
	{id = "small flask of eyedrops", chance = 3330}
}

monster.attacks = {
	{name = "melee",  attack = 15, skill = 10, minDamage = 0, maxDamage = -15, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -25, maxDamage = -35, range = 7, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -10, maxDamage = -35, range = 7, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)