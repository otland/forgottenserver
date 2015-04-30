local mType = Game.createMonsterType("Nightmare Scion")
local monster = {}
monster.description = "a nightmare scion"
monster.experience = 1350
monster.outfit = {
	lookType = 321
}

monster.health = 1400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9919
monster.speed = 230
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Weeeheeheee!", yell = false},
	{text = "Pffffrrrrrrrrrrrr.", yell = false},
	{text = "Peak a boo, I killed you!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 53, chance = 50000},
	{id = "meat", maxCount = 4, chance = 50000},
	{id = "essence of a bad dream", chance = 7692},
	{id = "scythe leg", chance = 4761},
	{id = "platinum coin", maxCount = 3, chance = 961},
	{id = "crown helmet", chance = 666},
	{id = "diamond sceptre", chance = 340},
	{id = "focus cape", chance = 340},
	{id = "bar of chocolate", chance = 280},
	{id = "death ring", chance = 250},
	{id = "shadow sceptre", chance = 270},
	{id = "crystal of focus", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -115, maxDamage = -180, target = true, range = 7, radius = 4, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -70, maxDamage = -130, target = true, range = 7, radius = 1, shootEffect = 32, effect = 39}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 60, maxDamage = 95, radius = 1, shootEffect = 32, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = -25},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)