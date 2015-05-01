local mType = Game.createMonsterType("Vashresamun")
local monster = {}
monster.description = "a vashresamun"
monster.experience = 2950
monster.outfit = {
	lookType = 90
}

monster.health = 4000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6025
monster.speed = 340
monster.maxSummons = 2

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
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "Banshee", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Come my maidens, we have visitors!", yell = false},
	{text = "Are you enjoying my music?", yell = false},
	{text = "If music is the food of death, drop dead.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 50000},
	{id = "gold coin", maxCount = 80, chance = 50000},
	{id = "gold coin", maxCount = 65, chance = 50000},
	{id = "white pearl", chance = 7000},
	{id = "lute", chance = 7000},
	{id = "flower pot", chance = 7000},
	{id = "blue robe", chance = 2500},
	{id = "crystal ring", chance = 1500},
	{id = "panpipes", chance = 1500},
	{id = "crystal mace", chance = 500},
	{id = "ancient tiara", chance = 300},
	{id = "blue note", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 40, minDamage = 0, maxDamage = -200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 30, interval = 2000, minDamage = -200, maxDamage = -750, radius = 5, effect = 23}
}

monster.defenses = {
	defense = 35,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 1000, minDamage = 60, maxDamage = 450, effect = 13},
	{name = "speed",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = 350, range = 7, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 20}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)