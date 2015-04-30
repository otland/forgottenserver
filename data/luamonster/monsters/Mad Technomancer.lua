local mType = Game.createMonsterType("Mad Technomancer")
local monster = {}
monster.description = "mad technomancer"
monster.experience = 55
monster.outfit = {
	lookType = 66
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6015
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 500,
	chance = 25
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I'm going to make them an offer they can't refuse.", yell = false},
	{text = "My masterplan cannot fail!", yell = false},
	{text = "Gentlemen, you can't fight here! This is the War Room!", yell = false}
}

monster.loot = {
    {id = "technomancer beard", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 50, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 1000, minDamage = -50, maxDamage = -120, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 34, interval = 1000, minDamage = -55, maxDamage = -105, range = 7, radius = 4, shootEffect = 12, effect = 7},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 25, interval = 1000, minDamage = -50, maxDamage = -80, range = 7, radius = 4, shootEffect = 12, effect = 7}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 1000, minDamage = 75, maxDamage = 325, radius = 4, shootEffect = 12, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 60},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)