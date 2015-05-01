local mType = Game.createMonsterType("Infernal Frog")
local monster = {}
monster.description = "a infernal frog"
monster.experience = 190
monster.outfit = {
	lookFeet = 66,
	lookLegs = 69,
	lookHead = 69,
	lookType = 224,
	lookBody = 66
}

monster.health = 655
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6079
monster.speed = 220
monster.runHealth = 40
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
	{text = "Ribbit!", yell = false},
	{text = "Ribbit! Ribbit!", yell = false},
	{text = "No Kisses for you!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 65, chance = 77000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -20, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 30, interval = 2000, minDamage = -16, maxDamage = -32, shootEffect = 15}
}

monster.defenses = {
	defense = 5,
    armor = 5,
    {name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 8000, speedChange = 400, shootEffect = 15, effect = 14}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)