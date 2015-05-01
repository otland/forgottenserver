local mType = Game.createMonsterType("Shadow Hound")
local monster = {}
monster.description = "a shadow hound"
monster.experience = 600
monster.outfit = {
	lookType = 322
}

monster.health = 555
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9923
monster.speed = 360
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
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
	{text = "Grrrr!", yell = true}
}

monster.loot = {
    {id = "midnight shard", chance = 8333}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 60, minDamage = 0, maxDamage = -350, interval = 2000, effect = 0},
	{name = "fireball",  chance = 24, interval = 2000, minDamage = -60, maxDamage = -160},
	{name = "soulfire",  chance = 14, interval = 2000, minDamage = 0, maxDamage = 0}
}

monster.defenses = {
	defense = 55,
    armor = 50,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 1000, minDamage = 60, maxDamage = 230, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 30},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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