local mType = Game.createMonsterType("Rorc")
local monster = {}
monster.description = "a rorc"
monster.experience = 105
monster.outfit = {
	lookType = 550
}

monster.health = 260
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21223
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
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
	{text = "Kroaaah!!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 25, chance = 73640},
	{id = "hatched rorc egg", chance = 12670},
	{id = "rorc feather", chance = 12520},
	{id = "plate shield", chance = 6700},
	{id = "wolf tooth chain", chance = 7410},
	{id = "orcish axe", chance = 2040},
	{id = "obsidian lance", chance = 930},
	{id = "rorc egg", chance = 790}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 40, interval = 2000, effect = 14}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 40, maxDamage = 55, shootEffect = 23, effect = 14},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 3000, speedChange = 300, shootEffect = 23, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)