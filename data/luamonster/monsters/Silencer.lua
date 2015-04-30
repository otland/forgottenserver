local mType = Game.createMonsterType("Silencer")
local monster = {}
monster.description = "silencer"
monster.experience = 3800
monster.outfit = {
	lookType = 585
}

monster.health = 4900
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 22489
monster.speed = 230
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 5
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
	{text = "Prrrroooaaaah!!! PRROAAAH!!", yell = false},
	{text = "PRRRROOOOOAAAAAHHHH!!!", yell = false},
	{text = "HUUUSSSSSSSSH!!", yell = false},
	{text = "Hussssssh!!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 8, chance = 100000},
	{id = "stealth ring", chance = 1200},
	{id = "boots of haste", chance = 360},
	{id = "dark shield", chance = 2000},
	{id = "assassin star", maxCount = 10, chance = 7600},
	{id = "diamond sceptre", chance = 960},
	{id = "haunted blade", chance = 2000},
	{id = "titan axe", chance = 2200},
	{id = "shadow sceptre", chance = 640},
	{id = "glorious axe", chance = 2400},
	{id = "terra legs", chance = 960},
	{id = "terra boots", chance = 480},
	{id = "cluster of solace", chance = 560},
	{id = "silencer claws", chance = 17000},
	{id = "silencer resonating chamber", chance = 8410}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 80, interval = 2000, effect = 0},
	{name = "silencer skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 3},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = -40, maxDamage = -150, target = true, radius = 4, shootEffect = 23, effect = 14}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 450, radius = 4, shootEffect = 23, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 220, maxDamage = 425, radius = 4, shootEffect = 23, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE, percent = -25},
	{type = COMBAT_DEATHDAMAGE, percent = 70},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)