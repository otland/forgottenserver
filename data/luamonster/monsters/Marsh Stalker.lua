local mType = Game.createMonsterType("Marsh Stalker")
local monster = {}
monster.description = "a marsh stalker"
monster.experience = 50
monster.outfit = {
	lookType = 530
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19708
monster.speed = 210
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
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
	{text = "Rrrawk! Rrrrawk!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 23, chance = 65000},
	{id = "worm", maxCount = 2, chance = 15000},
	{id = "fish", maxCount = 2, chance = 20290},
	{id = "meat", maxCount = 2, chance = 20360},
	{id = "", chance = 7180},
	{id = "marsh stalker beak", chance = 9300},
	{id = "longsword", chance = 7180},
	{id = "seeds", chance = 120},
	{id = "rope", chance = 4530}
}

monster.attacks = {
	{name = "melee",  attack = 10, skill = 10, minDamage = 0, maxDamage = -10, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -8, target = true, range = 7, radius = 1, shootEffect = 41, effect = 5}
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