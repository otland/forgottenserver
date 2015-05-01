local mType = Game.createMonsterType("Hairman The Huge")
local monster = {}
monster.description = "hairman the huge"
monster.experience = 335
monster.outfit = {
	lookFeet = 50,
	lookLegs = 40,
	lookHead = 20,
	lookType = 116,
	lookBody = 30
}

monster.health = 600
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6043
monster.speed = 230
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 14
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
	{text = "Hugah!", yell = false},
	{text = "Ungh! Ungh!", yell = false},
	{text = "Huaauaauaauaa!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 60, chance = 50000},
	{id = "ape fur", chance = 2500},
	{id = "banana", maxCount = 2, chance = 7500},
	{id = "protection amulet", subType = 250, chance = 3000},
	{id = "power ring", chance = 7500},
	{id = "plate armor", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, minDamage = 0, maxDamage = -110, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 25,
    armor = 20,
    {name = "speed",  chance = 7, interval = 1000, minDamage = 0, maxDamage = 0, duration = 3000, speedChange = 260, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
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