local mType = Game.createMonsterType("Primitive")
local monster = {}
monster.description = "a primitive"
monster.experience = 45
monster.outfit = {
	lookFeet = 1,
	lookLegs = 1,
	lookHead = 1,
	lookType = 143,
	lookBody = 1
}

monster.health = 200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6080
monster.speed = 300
monster.runHealth = 5
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
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
	{text = "We don't need a future!", yell = false},
	{text = "I'll rook you all!", yell = false},
	{text = "They thought they'd beaten us!", yell = false},
	{text = "You are history!", yell = false},
	{text = "There can only be one world!", yell = false},
	{text = "Valor who?", yell = false},
	{text = "Die noob!", yell = false},
	{text = "There are no dragons!", yell = false},
	{text = "I'll quit forever! Again ...", yell = false},
	{text = "You all are noobs!", yell = false},
	{text = "Beware of the cyclops!", yell = false},
	{text = "Just had a disconnect.", yell = false},
	{text = "Magic is only good for girls!", yell = false},
	{text = "We'll be back!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 12500},
	{id = "studded shield", chance = 1200},
	{id = "studded helmet", chance = 9500},
	{id = "studded armor", chance = 7000},
	{id = "axe", chance = 12250},
	{id = "sabre", chance = 10250},
	{id = "surprise bag", chance = 500},
	{id = "surprise bag", chance = 500}
}

monster.attacks = {
	{name = "melee",  attack = 16, skill = 30, minDamage = 0, maxDamage = -32, interval = 2000, effect = 0},
	{name = "drunk",  chance = 20, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 5},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 34, interval = 1000, minDamage = -20, maxDamage = -20, target = true, range = 7, radius = 3, shootEffect = 4, effect = 7},
	{name = "outfit",  chance = 2, interval = 1000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 4, effect = 2}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
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