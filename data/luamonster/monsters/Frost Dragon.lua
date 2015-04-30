local mType = Game.createMonsterType("Frost Dragon")
local monster = {}
monster.description = "a frost dragon"
monster.experience = 2100
monster.outfit = {
	lookType = 248
}

monster.health = 1800
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7091
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "YOU WILL FREEZE!", yell = true},
	{text = "ZCHHHHH!", yell = true},
	{text = "I am so cool", yell = false},
	{text = "Chill out!.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 33000},
	{id = "gold coin", maxCount = 100, chance = 33000},
	{id = "gold coin", maxCount = 42, chance = 33000},
	{id = "dragon ham", maxCount = 5, chance = 80370},
	{id = "power bolt", maxCount = 6, chance = 6000},
	{id = "green mushroom", chance = 12000},
	{id = "gemmed book", chance = 8500},
	{id = "small sapphire", chance = 5200},
	{id = "ice cube", chance = 4000},
	{id = "energy ring", chance = 5000},
	{id = "golden mug", chance = 3000},
	{id = "shard", chance = 550},
	{id = "life crystal", chance = 520},
	{id = "dragon scale mail", chance = 80},
	{id = "strange helmet", chance = 450},
	{id = "royal helmet", chance = 210},
	{id = "tower shield", chance = 340},
	{id = "dragon slayer", chance = 120},
	{id = "ice rapier", subType = 1, chance = 350}
}

monster.attacks = {
	{name = "melee",  attack = 67, skill = 57, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -175, maxDamage = -380, length = 8, spread = 3, effect = 3},
	{name = "speed",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, duration = 12000, speedChange = -700, radius = 3, effect = 3},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 18000, speedChange = -850, length = 7, spread = 3, radius = 3, effect = 44},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 5, interval = 2000, minDamage = -60, maxDamage = -120, radius = 3, effect = 43},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -240, target = true, radius = 4, effect = 42},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 5, interval = 2000, minDamage = 0, maxDamage = -220, length = 1, radius = 4, effect = 3},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 12000, speedChange = -600, radius = 4, effect = 42}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 150, maxDamage = 200, radius = 4, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 290, radius = 4, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = 10}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)