local mType = Game.createMonsterType("Barbarian Brutetamer")
local monster = {}
monster.description = "a barbarian brutetamer"
monster.experience = 90
monster.outfit = {
	lookFeet = 121,
	lookLegs = 95,
	lookHead = 78,
	lookType = 264,
	lookBody = 116
}

monster.health = 145
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20339
monster.speed = 190
monsters.runHealth = 10
monster.maxSummons = 2

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
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "War Wolf", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "To me, creatures of the wild!", yell = false},
	{text = "My instincts tell me about your cowardice.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 15, chance = 58000},
	{id = "hunting spear", chance = 5250},
	{id = "corncob", maxCount = 2, chance = 11000},
	{id = "chain armor", chance = 8900},
	{id = "staff", chance = 6500},
	{id = "fur bag", chance = 7540},
	{id = "book", chance = 5000},
	{id = "mana potion", chance = 630},
	{id = "brutetamer's staff", chance = 380},
	{id = "fur boots", chance = 160},
	{id = "mammoth fur shorts", chance = 90},
	{id = "mammoth fur cape", chance = 160}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 10, minDamage = 0, maxDamage = -20, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -34, target = true, range = 7, radius = 1, shootEffect = 13},
	{name = "barbarian brutetamer skill reducer",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, shootEffect = 13}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "combat", type = COMBAT_HEALING,  chance = 40, interval = 2000, minDamage = 50, maxDamage = 80, shootEffect = 13, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -1},
	{type = COMBAT_ICEDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
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