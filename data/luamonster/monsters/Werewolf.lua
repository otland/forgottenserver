local mType = Game.createMonsterType("Werewolf")
local monster = {}
monster.description = "a werewolf"
monster.experience = 1900
monster.outfit = {
	lookType = 308
}

monster.health = 1955
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20380
monster.speed = 200
monster.maxSummons = 2

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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.summons = {
    {name = "war wolf", chance = 40, interval = 2000},
	{name = "war wolf", chance = 40, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "BLOOD!", yell = true},
	{text = "HRAAAAAAAAAARRRRRR!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 35000},
	{id = "gold coin", maxCount = 90, chance = 35000},
	{id = "gold coin", maxCount = 45, chance = 35000},
	{id = "halberd", chance = 3000},
	{id = "plate legs", chance = 6500},
	{id = "plate shield", chance = 10340},
	{id = "platinum amulet", chance = 870},
	{id = "berserk potion", chance = 1200},
	{id = "bonebreaker", chance = 400},
	{id = "epee", chance = 560},
	{id = "relic sword", chance = 480},
	{id = "rusty armor", chance = 210},
	{id = "stone skin amulet", subType = 5, chance = 1000},
	{id = "strong health potion", chance = 5000},
	{id = "time ring", chance = 800},
	{id = "troll green", chance = 1900},
	{id = "wolf paw", chance = 5200},
	{id = "ultimate health potion", chance = 2400},
	{id = "dreaded cleaver", chance = 160},
	{id = "werewolf fur", chance = 10650},
	{id = "brown mushroom", chance = 6940}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 90, interval = 2000, effect = 0},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, target = true, radius = 1, effect = 24},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -80, maxDamage = -200, length = 4, spread = 2, radius = 1, effect = 20},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 25},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, effect = 19},
	{name = "werewolf skill reducer",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 1, radius = 1, effect = 19}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 120, maxDamage = 225, radius = 1, effect = 15},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 400, range = 7, radius = 1, effect = 23}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 65},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)