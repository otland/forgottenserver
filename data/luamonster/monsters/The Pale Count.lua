local mType = Game.createMonsterType("The Pale Count")
local monster = {}
monster.description = "the pale count"
monster.experience = 18000
monster.outfit = {
	lookType = 558
}

monster.health = 20000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21279
monster.speed = 300
monster.maxSummons = 4

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

monster.summons = {
    {name = "Nightfiend", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Feel the hungry kiss of death!", yell = false},
	{text = "The monsters in the mirror will come eat your dreams.", yell = false},
	{text = "Your pitiful life has come to an end!", yell = false},
	{text = "I will squish you like a maggot and suck you dry!", yell = false},
	{text = "Yield to the inevitable!", yell = false},
	{text = "Some day I shall see my beautiful face in a mirror again.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 120, skill = 80, minDamage = 0, maxDamage = -540, interval = 2000, effect = 0},
	{name = "speed",  chance = 17, interval = 1000, minDamage = 0, maxDamage = 0, target = true, duration = 1500, speedChange = -600, range = 7, radius = 4},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 21, interval = 2000, minDamage = -130, maxDamage = -350, target = true, range = 6, radius = 2, shootEffect = 37, effect = 53},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -120, range = 7, shootEffect = 30, effect = 47}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -40}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
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