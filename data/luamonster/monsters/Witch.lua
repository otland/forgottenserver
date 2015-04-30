local mType = Game.createMonsterType("Witch")
local monster = {}
monster.description = "a witch"
monster.experience = 120
monster.outfit = {
	lookType = 54
}

monster.health = 300
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20535
monster.speed = 195
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
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Horax pokti!", yell = false},
	{text = "Herba budinia ex!", yell = false},
	{text = "Hihihihi!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 64000},
	{id = "cookie", maxCount = 8, chance = 29750},
	{id = "leather boots", chance = 4950},
	{id = "bag of apple slices", chance = 920},
	{id = "sickle", chance = 3910},
	{id = "cape", chance = 4870},
	{id = "coat", chance = 2010},
	{id = "wolf tooth chain", chance = 10120},
	{id = "star herb", chance = 8950},
	{id = "garlic necklace", subType = 150, chance = 1000},
	{id = "silver dagger", chance = 500},
	{id = "necrotic rod", chance = 1140},
	{id = "witch hat", chance = 80},
	{id = "stuffed toad", chance = 10},
	{id = "witch broom", chance = 10000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -30, maxDamage = -75, range = 7, shootEffect = 4, effect = 16},
	{name = "firefield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 4, effect = 16},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, radius = 1, shootEffect = 4, effect = 16}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
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