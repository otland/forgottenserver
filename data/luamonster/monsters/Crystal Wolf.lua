local mType = Game.createMonsterType("Crystal Wolf")
local monster = {}
monster.description = "a crystal wolf"
monster.experience = 275
monster.outfit = {
	lookType = 391
}

monster.health = 750
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 13584
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Raaarrr!", yell = false},
	{text = "Aaaauuuuuooooooo!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 52, chance = 56000},
	{id = "meat", maxCount = 4, chance = 52000},
	{id = "shiver arrow", maxCount = 10, chance = 11000},
	{id = "crystalline armor", chance = 3700},
	{id = "hailstorm rod", chance = 7400},
	{id = "wolf paw", chance = 3700}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, minDamage = 0, maxDamage = -80, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -130, length = 3, spread = 2, effect = 45},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 2000, minDamage = -80, maxDamage = -150, target = true, range = 6, radius = 3, shootEffect = 37, effect = 53},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -25, maxDamage = -80, range = 7, shootEffect = 37, effect = 53}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 15, maxDamage = 55, shootEffect = 37, effect = 13}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)