local mType = Game.createMonsterType("Evil Sheep Lord")
local monster = {}
monster.description = "an evil sheep lord"
monster.experience = 340
monster.outfit = {
	lookType = 13
}

monster.health = 400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5994
monster.speed = 300
monster.maxSummons = 3

monster.changeTarget = {
    interval = 2000,
	chance = 20
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
    {name = "Evil Sheep", chance = 30, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You can COUNT on us!", yell = false},
	{text = "Maeh!", yell = false},
	{text = "I feel you're getting sleepy! Maeh!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 60, chance = 75000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -118, interval = 2000, effect = 0},
	{name = "outfit",  chance = 20, interval = 3000, minDamage = 0, maxDamage = 0, range = 7, effect = 13}
}

monster.defenses = {
	defense = 35,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 50, maxDamage = 100, effect = 13},
	{name = "outfit",  chance = 50, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_FIREDAMAGE, percent = -5}
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