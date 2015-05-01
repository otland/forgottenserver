local mType = Game.createMonsterType("Yielothax")
local monster = {}
monster.description = "yielothax"
monster.experience = 1250
monster.outfit = {
	lookType = 408
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 13752
monster.speed = 240
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 75
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "IIEEH!! Iiih iih ih iiih!!!", yell = true},
	{text = "Bsssssssm Bssssssm Bsssssssssssm!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 27, chance = 50000},
	{id = "strong health potion", chance = 20000},
	{id = "strong mana potion", chance = 20000},
	{id = "brown mushroom", maxCount = 3, chance = 9090},
	{id = "small diamond", maxCount = 5, chance = 4761},
	{id = "might ring", subType = 20, chance = 3846},
	{id = "talon", chance = 917},
	{id = "lightning pendant", subType = 200, chance = 862},
	{id = "shockwave amulet", subType = 5, chance = 571},
	{id = "epee", chance = 564},
	{id = "mastermind potion", chance = 490},
	{id = "wand of cosmic energy", chance = 523},
	{id = "lightning legs", chance = 480},
	{id = "yielocks", chance = 320},
	{id = "yielowax", chance = 300},
	{id = "broken ring of ending", chance = 261}
}

monster.attacks = {
	{name = "melee",  attack = 54, skill = 65, minDamage = 0, maxDamage = -203, interval = 1000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -130, length = 4, effect = 38},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -150, maxDamage = -250, radius = 3, effect = 17},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -70, maxDamage = -120, target = true, radius = 3, effect = 17},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -50, maxDamage = -150, length = 4, spread = 3, effect = 14}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 100, maxDamage = 150, effect = 17}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = 25},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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