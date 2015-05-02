local mType = Game.createMonsterType("Cyclops Smith")
local monster = {}
monster.description = "a cyclops smith"
monster.experience = 255
monster.outfit = {
	lookType = 277
}

monster.health = 435
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7740
monster.speed = 210
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Outis emoi g' onoma.", yell = false},
	{text = "Whack da humy!", yell = false},
	{text = "Ai humy phary ty kaynon", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 70, chance = 82920},
	{id = "meat", chance = 49950},
	{id = "battle shield", chance = 6190},
	{id = "battle hammer", chance = 5200},
	{id = "plate shield", chance = 2000},
	{id = "battle axe", chance = 5450},
	{id = "heavy machete", chance = 2000},
	{id = "double axe", chance = 880},
	{id = "dark helmet", chance = 200},
	{id = "cyclops trophy", chance = 140},
	{id = "strong health potion", chance = 390},
	{id = "club ring", chance = 90},
	{id = "spiked squelcher", chance = 150},
	{id = "cyclops toe", chance = 10280}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 50, minDamage = 0, maxDamage = -150, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -70, range = 7, shootEffect = 27},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 27, effect = 32}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 1}
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