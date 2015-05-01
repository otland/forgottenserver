local mType = Game.createMonsterType("Boogey")
local monster = {}
monster.description = "a boogey"
monster.experience = 475
monster.outfit = {
	lookType = 300
}

monster.health = 930
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 10324
monster.speed = 400
monster.maxSummons = 2

monster.changeTarget = {
    interval = 5000,
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

monster.summons = {
    {name = "demon skeleton", chance = 40, interval = 4000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Since you didn't eat your spinach Bogey comes to get you!", yell = true},
	{text = "Too bad you did not eat your lunch, now I have to punish you!", yell = true},
	{text = "Even if you beat me, I'll hide in your closet until you one day drop your guard!", yell = true},
	{text = "You better had believe in me!", yell = true},
	{text = "I'll take you into the darkness ... forever!", yell = true}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -120, interval = 1200, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 30, interval = 1500, minDamage = 0, maxDamage = -30, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 30, interval = 1500, minDamage = -12, maxDamage = -20, target = true, range = 7, radius = 4, shootEffect = 4, effect = 1},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 1500, minDamage = -20, maxDamage = -30, shootEffect = 32, effect = 18}
}

monster.defenses = {
	defense = 50,
    armor = 50,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 80, maxDamage = 120, shootEffect = 32, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 25}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)