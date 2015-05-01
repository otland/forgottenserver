local mType = Game.createMonsterType("Lizard Magistratus")
local monster = {}
monster.description = "a lizard magistratus"
monster.experience = 2000
monster.outfit = {
	lookType = 115
}

monster.health = 8000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6041
monster.speed = 200
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
	{text = "Shhhhhhhh", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 50, chance = 77230},
	{id = "platinum coin", maxCount = 19, chance = 13400},
	{id = "great mana potion", chance = 4480},
	{id = "small ruby", maxCount = 5, chance = 8970},
	{id = "strong mana potion", chance = 6280},
	{id = "lizard scale", chance = 450},
	{id = "lizard leather", chance = 220}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 30, minDamage = 0, maxDamage = -60, interval = 2000, effect = 0},
	{name = "lizard magistratus curse",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 5}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 2000, minDamage = 200, maxDamage = 250, effect = 13}
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