local mType = Game.createMonsterType("Elder Mummy")
local monster = {}
monster.description = "an elder mummy"
monster.experience = 560
monster.outfit = {
	lookFeet = 50,
	lookLegs = 40,
	lookHead = 20,
	lookType = 65,
	lookBody = 30
}

monster.health = 850
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6004
monster.speed = 180
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 160, chance = 87000},
	{id = "worm", maxCount = 3, chance = 20000},
	{id = "flask of embalming fluid", chance = 12600},
	{id = "gauze bandage", chance = 10000},
	{id = "scarab coin", maxCount = 3, chance = 10000},
	{id = "magic light wand", chance = 6000},
	{id = "strange talisman", subType = 200, chance = 4500},
	{id = "silver brooch", chance = 4000},
	{id = "pharaoh banner", chance = 2400},
	{id = "crystal ring", chance = 1650},
	{id = "black pearl", chance = 1340},
	{id = "poison dagger", chance = 380}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -120, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -130, range = 1, effect = 18},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -300, range = 7, effect = 14}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)