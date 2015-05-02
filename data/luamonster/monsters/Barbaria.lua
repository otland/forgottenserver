local mType = Game.createMonsterType("Barbaria")
local monster = {}
monster.description = "barbaria"
monster.experience = 355
monster.outfit = {
	lookFeet = 121,
	lookLegs = 95,
	lookHead = 78,
	lookType = 264,
	lookBody = 116
}

monster.health = 555
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20339
monster.speed = 280
monster.maxSummons = 1

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
    {name = "War Wolf", chance = 40, interval = 2000}
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "To me, creatures of the wild!", yell = false},
	{text = "My instincts tell me about your cowardice.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 35, chance = 48000},
	{id = "chain armor", chance = 11000},
	{id = "hunting spear", chance = 12500},
	{id = "fur bag", chance = 1000},
	{id = "torch", chance = 25000},
	{id = "book", chance = 15000}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 60, minDamage = 0, maxDamage = -70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 34, interval = 2000, minDamage = -30, maxDamage = -80, target = true, range = 7, radius = 1, shootEffect = 13},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 3000, minDamage = -35, maxDamage = -70, range = 7, shootEffect = 4, effect = 12}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 50, maxDamage = 80, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
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