local mType = Game.createMonsterType("Deepling Worker")
local monster = {}
monster.description = "a deepling worker"
monster.experience = 130
monster.outfit = {
	lookType = 470
}

monster.health = 190
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15497
monster.speed = 210
monster.runHealth = 20
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Qjell afar gou jey!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 25, chance = 70000},
	{id = "deepling scales", chance = 6950},
	{id = "heavy trident", chance = 510},
	{id = "fish fin", chance = 350},
	{id = "eye of a deepling", chance = 283},
	{id = "fish", maxCount = 3, chance = 12020},
	{id = "small emerald", maxCount = 3, chance = 110}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, minDamage = 0, maxDamage = -80, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = -20}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)