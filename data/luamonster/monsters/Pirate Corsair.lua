local mType = Game.createMonsterType("Pirate Corsair")
local monster = {}
monster.description = "a pirate corsair"
monster.experience = 350
monster.outfit = {
	lookType = 98
}

monster.health = 675
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20475
monster.speed = 250
monsters.runHealth = 40
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Give up!", yell = false},
	{text = "Hiyaa!", yell = false},
	{text = "Plundeeeeer!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 88, chance = 50000},
	{id = "dark armor", chance = 1650},
	{id = "sabre", chance = 10000},
	{id = "sword", chance = 3000},
	{id = "dark shield", chance = 1000},
	{id = "throwing star", maxCount = 12, chance = 8400},
	{id = "pirate backpack", chance = 930},
	{id = "hook", chance = 600},
	{id = "peg leg", chance = 600},
	{id = "eye patch", chance = 500},
	{id = "strong health potion", chance = 820},
	{id = "skull candle", chance = 130},
	{id = "pirate hat", chance = 1150},
	{id = "pirate boots", chance = 220},
	{id = "piggy bank", chance = 150},
	{id = "compass", chance = 11050},
	{id = "rum flask", chance = 130},
	{id = "compass", chance = 11020}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 75, minDamage = 0, maxDamage = -170, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -150, range = 7, shootEffect = 8},
	{name = "pirate corsair skill reducer",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 8}
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