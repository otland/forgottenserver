local mType = Game.createMonsterType("Behemoth")
local monster = {}
monster.description = "a behemoth"
monster.experience = 2500
monster.outfit = {
	lookType = 55
}

monster.health = 4000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5999
monster.speed = 260
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You're so little!", yell = false},
	{text = "Human flesh - delicious!", yell = false},
	{text = "Crush the intruders!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 99, chance = 40000},
	{id = "platinum coin", maxCount = 5, chance = 59800},
	{id = "amphora", chance = 100},
	{id = "assassin star", maxCount = 5, chance = 9750},
	{id = "big bone", chance = 670},
	{id = "crowbar", chance = 100},
	{id = "dark armor", chance = 4370},
	{id = "double axe", chance = 10510},
	{id = "meat", maxCount = 6, chance = 30000},
	{id = "perfect behemoth fang", chance = 1090},
	{id = "pick", chance = 650},
	{id = "plate armor", chance = 3930},
	{id = "small amethyst", maxCount = 5, chance = 6380},
	{id = "steel boots", chance = 380},
	{id = "two handed sword", chance = 5980},
	{id = "giant sword", chance = 1006},
	{id = "great health potion", chance = 5120},
	{id = "strange symbol", chance = 750},
	{id = "behemoth claw", chance = 430},
	{id = "war axe", chance = 50},
	{id = "behemoth trophy", chance = 170},
	{id = "crystal necklace", chance = 2530},
	{id = "titan axe", chance = 90},
	{id = "battle stone", chance = 14000}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 80, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -200, range = 7, shootEffect = 12}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 300, shootEffect = 12, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)