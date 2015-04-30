local mType = Game.createMonsterType("Stampor")
local monster = {}
monster.description = "a stampor"
monster.experience = 780
monster.outfit = {
	lookType = 381
}

monster.health = 1200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13312
monster.speed = 240
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "KRRRRRNG", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 242, chance = 30000},
	{id = "stampor talons", maxCount = 2, chance = 9950},
	{id = "platinum coin", maxCount = 2, chance = 9920},
	{id = "small topaz", maxCount = 2, chance = 7940},
	{id = "strong mana potion", maxCount = 2, chance = 5000},
	{id = "strong health potion", maxCount = 2, chance = 5000},
	{id = "stampor horn", chance = 4920},
	{id = "hollow stampor hoof", chance = 3020},
	{id = "war hammer", chance = 1010},
	{id = "knight armor", chance = 870},
	{id = "spiked squelcher", chance = 160}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = -150, maxDamage = -280, radius = 3, effect = 35},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -75, maxDamage = -100, radius = 3, shootEffect = 10, effect = 35},
	{name = "stampor skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, radius = 3, shootEffect = 10, effect = 35}
}

monster.defenses = {
	defense = 0,
    armor = 0,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 90, maxDamage = 120, radius = 3, shootEffect = 10, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 20}
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