local mType = Game.createMonsterType("Quara Constrictor Scout")
local monster = {}
monster.description = "a quara constrictor scout"
monster.experience = 200
monster.outfit = {
	lookType = 46
}

monster.health = 450
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6065
monster.speed = 200
monsters.runHealth = 20
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
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Gaaahhh!", yell = false},
	{text = "Gluh! Gluh!", yell = false},
	{text = "Tssss!", yell = false},
	{text = "Boohaa!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 49, chance = 98800},
	{id = "quara tentacle", chance = 15600},
	{id = "longsword", chance = 8310},
	{id = "brass armor", chance = 4660},
	{id = "small amethyst", chance = 4350},
	{id = "fish fin", chance = 290},
	{id = "shrimp", maxCount = 3, chance = 9680}
}

monster.attacks = {
	{name = "melee",  attack = 45, skill = 50, minDamage = 0, maxDamage = -135, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -80, radius = 3, effect = 10}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)