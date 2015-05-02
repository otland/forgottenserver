local mType = Game.createMonsterType("The Obliverator")
local monster = {}
monster.description = "the obliverator"
monster.experience = 6000
monster.outfit = {
	lookType = 35
}

monster.health = 9500
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 7349
monster.speed = 280
monster.maxSummons = 3

monster.changeTarget = {
    interval = 0,
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
	targetDistance = 1,
	staticAttack = 95
}

monster.summons = {
    {name = "fire elemental", chance = 50, interval = 2000}
}

monster.voices = {
	interval = 1000,
	chance = 10,
	{text = "NO ONE WILL BEAT ME!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 140, skill = 90, minDamage = 0, maxDamage = -700, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 1000, minDamage = -100, maxDamage = -250, target = true, range = 5, radius = 7, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 30, interval = 3000, minDamage = -200, maxDamage = -500, length = 8, shootEffect = 4, effect = 12}
}

monster.defenses = {
	defense = 45,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 4000, minDamage = 50, maxDamage = 200, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = 35},
	{type = COMBAT_HOLYDAMAGE, percent = -15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)