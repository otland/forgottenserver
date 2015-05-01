local mType = Game.createMonsterType("Deepling Brawler")
local monster = {}
monster.description = "a deepling brawler"
monster.experience = 260
monster.outfit = {
	lookType = 470
}

monster.health = 380
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13840
monster.speed = 190
monsters.runHealth = 40
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
	staticAttack = 95
}

monster.loot = {
    {id = "gold coin", maxCount = 44, chance = 61000},
	{id = "deepling scales", chance = 14500},
	{id = "eye of a deepling", chance = 6600},
	{id = "heavy trident", chance = 2940},
	{id = "eye of a deepling", chance = 6620},
	{id = "fish fin", chance = 740},
	{id = "fish", maxCount = 3, chance = 19120}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 40, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -120, range = 7, shootEffect = 1, effect = 2}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = -1}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)