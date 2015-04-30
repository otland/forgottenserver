local mType = Game.createMonsterType("Hellspawn")
local monster = {}
monster.description = "a hellspawn"
monster.experience = 2550
monster.outfit = {
	lookType = 322
}

monster.health = 3500
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 9923
monster.speed = 260
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
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
	{text = "Your fragile bones are like toothpicks to me.", yell = false},
	{text = "You little weasel will not live to see another day.", yell = false},
	{text = "I'm just a messenger of what's yet to come.", yell = false},
	{text = "HRAAAAAAAAAAAAAAAARRRR", yell = true},
	{text = "I'm taking you down with me!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 60000},
	{id = "gold coin", maxCount = 100, chance = 60000},
	{id = "gold coin", maxCount = 36, chance = 60000},
	{id = "great health potion", chance = 40333},
	{id = "hellspawn tail", chance = 20000},
	{id = "broadsword", chance = 10000},
	{id = "morning star", chance = 10000},
	{id = "assassin star", maxCount = 2, chance = 9090},
	{id = "ultimate health potion", chance = 9090},
	{id = "carrot cake", chance = 9090},
	{id = "red mushroom", maxCount = 2, chance = 7692},
	{id = "small topaz", maxCount = 3, chance = 5882},
	{id = "rusty armor", chance = 3125},
	{id = "rusty armor", chance = 3125},
	{id = "knight legs", chance = 3030},
	{id = "warrior helmet", chance = 1886},
	{id = "spiked squelcher", chance = 970},
	{id = "berserk potion", chance = 934},
	{id = "black skull", chance = 151},
	{id = "dracoyle statue", chance = 140},
	{id = "onyx flail", chance = 103}
}

monster.attacks = {
	{name = "melee",  attack = 88, skill = 70, interval = 2000, effect = 0},
	{name = "fireball",  chance = 20, interval = 2000, minDamage = -150, maxDamage = -175},
	{name = "hellspawn soulfire",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 5}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 120, maxDamage = 230, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 270, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 40},
	{type = COMBAT_HOLYDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)