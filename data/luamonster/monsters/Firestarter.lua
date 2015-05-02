local mType = Game.createMonsterType("Firestarter")
local monster = {}
monster.description = "a firestarter"
monster.experience = 80
monster.outfit = {
	lookFeet = 79,
	lookLegs = 78,
	lookHead = 94,
	lookType = 159,
	lookBody = 77
}

monster.health = 180
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20599
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
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "FIRE!", yell = true},
	{text = "BURN!", yell = true},
	{text = "DEATH to the FALSE GOD!!", yell = true},
	{text = "You shall burn in the thornfires!!", yell = false},
	{text = "DOWN with the followers of the bog!!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 35, chance = 79000},
	{id = "longsword", chance = 6000},
	{id = "grapes", chance = 20000},
	{id = "bow", chance = 4000},
	{id = "heaven blossom", chance = 930},
	{id = "elvish bow", chance = 100},
	{id = "flaming arrow", maxCount = 12, chance = 30000},
	{id = "elvish talisman", chance = 5000},
	{id = "coal", chance = 15280},
	{id = "flintstone", chance = 340}
}

monster.attacks = {
	{name = "melee",  attack = 10, skill = 20, minDamage = 0, maxDamage = -15, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -21, target = true, radius = 1, shootEffect = 7, effect = 6},
	{name = "firefield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, radius = 1, shootEffect = 4, effect = 6}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)