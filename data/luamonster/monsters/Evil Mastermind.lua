local mType = Game.createMonsterType("Evil Mastermind")
local monster = {}
monster.description = "a evil mastermind"
monster.experience = 675
monster.outfit = {
	lookType = 256
}

monster.health = 1095
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 10321
monster.speed = 350
monster.maxSummons = 1

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
	targetDistance = 3,
	staticAttack = 90
}

monster.summons = {
    {name = "vampire", chance = 40, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You won't stop my masterplan to flood the world market with fake Bonelord language dictionaries!", yell = false},
	{text = "My calculations tell me you'll die!", yell = false},
	{text = "You can't stop me!", yell = false},
	{text = "Beware! My evil monolog is coming!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -77, interval = 1200, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 30, interval = 2000, minDamage = -50, maxDamage = -78, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 30, interval = 2000, minDamage = -66, maxDamage = -72, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 30, interval = 2000, minDamage = -36, maxDamage = -57, range = 7, shootEffect = 5, effect = 38},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 30, interval = 2000, minDamage = -70, maxDamage = -73, range = 7, shootEffect = 5, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 30, interval = 2000, minDamage = -59, maxDamage = -75, range = 7, shootEffect = 5, effect = 14},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -600, range = 7, shootEffect = 5, effect = 14}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 2000, minDamage = 50, maxDamage = 110, shootEffect = 5, effect = 12}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 90},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)