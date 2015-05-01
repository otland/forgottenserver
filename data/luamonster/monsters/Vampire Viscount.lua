local mType = Game.createMonsterType("Vampire Viscount")
local monster = {}
monster.description = "a vampire viscount"
monster.experience = 800
monster.outfit = {
	lookType = 555
}

monster.health = 1200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21275
monster.speed = 250
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
	{text = "Prepare to BLEED!", yell = false},
	{text = "Don't struggle. We don't want to waste a drop of blood now, do we?", yell = false},
	{text = "Ah, refreshments have arrived!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 50, chance = 83000},
	{id = "strong mana potion", chance = 8180},
	{id = "vampire teeth", chance = 7200},
	{id = "strong health potion", chance = 7100},
	{id = "tooth file", chance = 6560},
	{id = "vampire's cape chain", chance = 4460},
	{id = "small ruby", maxCount = 2, chance = 3040},
	{id = "blood preservation", chance = 2910},
	{id = "black pearl", chance = 2500},
	{id = "ice rapier", subType = 1, chance = 810},
	{id = "red gem", chance = 540},
	{id = "vampire shield", chance = 200},
	{id = "red piece of cloth", chance = 70}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -150, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -50, maxDamage = -100, target = true, range = 6, radius = 3, shootEffect = 11, effect = 18},
	{name = "condition", type = CONDITION_BLEEDING,  chance = 10, interval = 2000, minDamage = -320, maxDamage = -560, radius = 6, shootEffect = 11, effect = 67}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "outfit",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 11, effect = 67},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 4000, speedChange = 300, shootEffect = 11, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = -5}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)