local mType = Game.createMonsterType("Mephiles")
local monster = {}
monster.description = "a mephiles"
monster.experience = 415
monster.outfit = {
	lookType = 237
}

monster.health = 415
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 10323
monster.speed = 300
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 3,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I have a contract here which you should sign!", yell = false},
	{text = "I sence so much potential in you. It's almost a shame I have to kill you.", yell = false},
	{text = "Yes, slay me for the loot I might have. Give in to your greed.", yell = false},
	{text = "Wealth, Power, it is all at your fingertips. All you have to do is a bit blackmailing and bullying.", yell = false},
	{text = "Come on. being a bit evil won't hurt you.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -35, interval = 1200, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 70, interval = 1500, minDamage = -15, maxDamage = -45, target = true, range = 7, radius = 2, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 35,
    armor = 30,
    {name = "speed",  chance = 40, interval = 1000, minDamage = 0, maxDamage = 0, duration = 40000, speedChange = 400, shootEffect = 4, effect = 14},
	{name = "invisible",  chance = 50, interval = 4000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)