local mType = Game.createMonsterType("Mahrdis")
local monster = {}
monster.description = "a mahrdis"
monster.experience = 3050
monster.outfit = {
	lookType = 90
}

monster.health = 3900
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6025
monster.speed = 340
monster.maxSummons = 4

monster.changeTarget = {
    interval = 5000,
	chance = 8
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

monster.summons = {
    {name = "Fire Elemental", chance = 3000, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ashes to ashes!", yell = false},
	{text = "Fire, Fire!", yell = false},
	{text = "The eternal flame demands its due!", yell = false},
	{text = "This is why I'm hot.", yell = false},
	{text = "May my flames engulf you!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 50000},
	{id = "gold coin", maxCount = 70, chance = 50000},
	{id = "gold coin", maxCount = 64, chance = 50000},
	{id = "small ruby", maxCount = 3, chance = 7000},
	{id = "life ring", chance = 1500},
	{id = "great health potion", chance = 1500},
	{id = "red gem", chance = 1500},
	{id = "fire axe", chance = 750},
	{id = "holy falcon", chance = 500},
	{id = "phoenix shield", chance = 300},
	{id = "burning heart", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 70, minDamage = 0, maxDamage = -400, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 7, interval = 1600, minDamage = -60, maxDamage = -600, range = 1, effect = 14},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 7, interval = 1000, minDamage = -60, maxDamage = -600, range = 7, shootEffect = 4, effect = 7},
	{name = "speed",  chance = 13, interval = 2000, minDamage = 0, maxDamage = 0, duration = 50000, speedChange = -850, range = 7, shootEffect = 4, effect = 14},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 34, interval = 2000, minDamage = -80, maxDamage = -800, radius = 3, shootEffect = 4, effect = 5},
	{name = "firefield",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 4, effect = 4},
	{name = "condition", type = CONDITION_FIRE,  chance = 13, interval = 2000, minDamage = -50, maxDamage = -500, length = 8, spread = 3, shootEffect = 4, effect = 6}
}

monster.defenses = {
	defense = 30,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 1000, minDamage = 20, maxDamage = 800, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 45},
	{type = COMBAT_HOLYDAMAGE, percent = -22},
	{type = COMBAT_ICEDAMAGE, percent = -25}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)