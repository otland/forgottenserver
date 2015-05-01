local mType = Game.createMonsterType("Honour Guard")
local monster = {}
monster.description = "a honour guard"
monster.experience = 55
monster.outfit = {
	lookType = 298
}

monster.health = 85
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 2843
monster.speed = 180
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Horestis curse upon you!", yell = false},
	{text = "Vengeance!", yell = false},
	{text = "Thy punishment is at hand!", yell = false},
	{text = "For the pharao!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 15, chance = 50000},
	{id = "mace", chance = 3760},
	{id = "scimitar", chance = 1640},
	{id = "brown mushroom", maxCount = 2, chance = 6120},
	{id = "scarab coin", chance = 2600},
	{id = "pelvis bone", chance = 8700}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -10, maxDamage = -40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -5, maxDamage = -13, range = 1, effect = 14}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)