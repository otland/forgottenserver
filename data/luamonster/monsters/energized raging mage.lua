local mType = Game.createMonsterType("energized raging mage")
local monster = {}
monster.description = "a energized raging mage"
monster.experience = 0
monster.outfit = {
	lookType = 423
}

monster.health = 3500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 0
monster.speed = 230
monsters.runHealth = 300
monster.maxSummons = 1

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
	staticAttack = 70
}

monster.summons = {
    {name = "golden servant", chance = 50, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Behold the all permeating powers I draw from this gate!!", yell = false},
	{text = "ENERGY!!", yell = false},
	{text = "I FEEL, I FEEEEL... OMNIPOTENCE!!", yell = false},
	{text = "MWAAAHAHAAA!! NO ONE!! NO ONE CAN DEFEAT MEEE!!!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 37, skill = 32, minDamage = 0, maxDamage = -78, interval = 2000, effect = 0},
	{name = "thunderstorm",  chance = 40, interval = 2000, minDamage = -150, maxDamage = -230, range = 7},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 20, interval = 2000, minDamage = -100, maxDamage = -200, range = 7},
	{name = "energyfield",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 2, shootEffect = 5},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 25, interval = 2000, minDamage = -200, maxDamage = -300, length = 8, shootEffect = 5, effect = 41}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 155, maxDamage = 200, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -50},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
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