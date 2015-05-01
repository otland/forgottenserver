local mType = Game.createMonsterType("Destroyer")
local monster = {}
monster.description = "a destroyer"
monster.experience = 2500
monster.outfit = {
	lookType = 236
}

monster.health = 3700
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6320
monster.speed = 270
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "COME HERE AND DIE!", yell = false},
	{text = "Destructiooooon!", yell = false},
	{text = "It's a good day to destroy!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 60000},
	{id = "gold coin", maxCount = 100, chance = 40000},
	{id = "gold coin", maxCount = 100, chance = 40000},
	{id = "gold coin", maxCount = 41, chance = 60000},
	{id = "meat", maxCount = 6, chance = 50000},
	{id = "demonic essence", chance = 20000},
	{id = "crowbar", chance = 14285},
	{id = "burst arrow", maxCount = 12, chance = 12500},
	{id = "dark armor", chance = 10000},
	{id = "small amethyst", maxCount = 2, chance = 7692},
	{id = "metal spike", chance = 7142},
	{id = "soul orb", chance = 6666},
	{id = "pick", chance = 6250},
	{id = "plate armor", chance = 4347},
	{id = "platinum coin", maxCount = 3, chance = 4166},
	{id = "giant sword", chance = 1694},
	{id = "great health potion", chance = 1136},
	{id = "steel boots", chance = 992},
	{id = "chaos mace", chance = 869},
	{id = "dreaded cleaver", chance = 833},
	{id = "crystal necklace", chance = 578},
	{id = "mind stone", chance = 564},
	{id = "death ring", chance = 144},
	{id = "skull helmet", chance = 108}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 90, minDamage = 0, maxDamage = -500, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -200, range = 7, shootEffect = 12}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 420, shootEffect = 12, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE, percent = -3},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -15}
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
	{type = "energy", condition = true, combat = true},
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