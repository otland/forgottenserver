local mType = Game.createMonsterType("Yaga The Crone")
local monster = {}
monster.description = "an yaga the crone"
monster.experience = 375
monster.outfit = {
	lookType = 54
}

monster.health = 620
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6081
monster.speed = 240
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
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Where did I park my hut?", yell = false},
	{text = "You will taste so sweet!", yell = false},
	{text = "Hexipooh, bewitched are you!", yell = false}
}

monster.loot = {
    {id = "cookie", maxCount = 8, chance = 30000},
	{id = "necrotic rod", chance = 2500},
	{id = "coat", chance = 25000},
	{id = "broom", chance = 15000},
	{id = "cape", chance = 15000},
	{id = "wolf tooth chain", chance = 10000},
	{id = "spellbook of mind control", chance = 2000},
	{id = "star herb", chance = 20000},
	{id = "garlic necklace", subType = 150, chance = 8000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 50, interval = 2500, minDamage = -30, maxDamage = -50, range = 7, shootEffect = 4, effect = 16},
	{name = "condition", type = CONDITION_POISON,  chance = 13, interval = 3000, minDamage = -200, maxDamage = -300, range = 7, shootEffect = 15, effect = 16},
	{name = "firefield",  chance = 13, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 4, effect = 16}
}

monster.defenses = {
	defense = 20,
    armor = 15,
    {name = "invisible",  chance = 9, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 14},
	{name = "outfit",  chance = 9, interval = 4000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -1},
	{type = COMBAT_EARTHDAMAGE, percent = 1},
	{type = COMBAT_DEATHDAMAGE, percent = -5}
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