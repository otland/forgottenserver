local mType = Game.createMonsterType("Swarmer")
local monster = {}
monster.description = "a swarmer"
monster.experience = 350
monster.outfit = {
	lookType = 460
}

monster.health = 460
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 15388
monster.speed = 190
monsters.runHealth = 50
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
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Flzlzlzlzlzlzlz!", yell = false},
	{text = "Rzlrzlrzlrzlrzlrzl!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 75, chance = 100000},
	{id = "swarmer antenna", chance = 15300},
	{id = "compound eye", chance = 12500},
	{id = "small emerald", chance = 920},
	{id = "epee", chance = 450}
}

monster.attacks = {
	{name = "melee",  attack = 37, skill = 45, minDamage = 0, maxDamage = -102, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -50, maxDamage = -110, range = 7, effect = 14}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 220, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 75},
	{type = COMBAT_ICEDAMAGE, percent = -1},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)