local mType = Game.createMonsterType("Nightmare")
local monster = {}
monster.description = "a nightmare"
monster.experience = 2150
monster.outfit = {
	lookType = 245
}

monster.health = 2700
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6340
monster.speed = 240
monsters.runHealth = 300
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Close your eyes... I want to show you something.", yell = false},
	{text = "I will haunt you forever!", yell = false},
	{text = "Pffffrrrrrrrrrrrr.", yell = false},
	{text = "I will make you scream.", yell = false},
	{text = "Take a ride with me.", yell = false},
	{text = "Weeeheeheeeheee!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 55, chance = 50000},
	{id = "ham", maxCount = 2, chance = 29000},
	{id = "soul orb", chance = 20000},
	{id = "concentrated demonic blood", maxCount = 2, chance = 19666},
	{id = "essence of a bad dream", chance = 15240},
	{id = "demonic essence", chance = 10000},
	{id = "power bolt", maxCount = 4, chance = 9090},
	{id = "scythe leg", chance = 9090},
	{id = "platinum coin", maxCount = 3, chance = 2564},
	{id = "death ring", chance = 1298},
	{id = "ancient shield", chance = 990},
	{id = "knight legs", chance = 961},
	{id = "boots of haste", chance = 337},
	{id = "skeleton decoration", chance = 337},
	{id = "mysterious voodoo skull", chance = 123},
	{id = "war axe", chance = 95}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 50, minDamage = 0, maxDamage = -150, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -120, maxDamage = -170, target = true, range = 7, radius = 1, shootEffect = 11, effect = 39},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -150, maxDamage = -350, target = true, range = 7, radius = 4, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 60, maxDamage = 100, shootEffect = 15, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 420, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = -25},
	{type = COMBAT_FIREDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)