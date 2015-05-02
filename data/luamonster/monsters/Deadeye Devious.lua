local mType = Game.createMonsterType("Deadeye Devious")
local monster = {}
monster.description = "deadeye devious"
monster.experience = 500
monster.outfit = {
	lookFeet = 117,
	lookLegs = 35,
	lookHead = 115,
	lookType = 151,
	lookAddons = 2,
	lookBody = 76
}

monster.health = 2000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20378
monster.speed = 300
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 50
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Let's kill 'em", yell = false},
	{text = "Arrrgh!", yell = false},
	{text = "You'll never take me alive!", yell = false}
}

monster.loot = {
    {id = "Deadeye Devious' eye patch", chance = 100000},
	{id = "gold coin", maxCount = 65, chance = 100000},
	{id = "knight armor", chance = 1200},
	{id = "plate armor", chance = 4000},
	{id = "skull of Ratha", maxCount = 2, chance = 100000},
	{id = "pirate backpack", chance = 4000},
	{id = "meat", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -250, maxDamage = -550, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 60, interval = 4000, minDamage = -200, maxDamage = -250, shootEffect = 9}
}

monster.defenses = {
	defense = 50,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 65, interval = 6000, minDamage = 200, maxDamage = 250, shootEffect = 9, effect = 13}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)