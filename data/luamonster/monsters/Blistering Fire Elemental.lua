local mType = Game.createMonsterType("Blistering Fire Elemental")
local monster = {}
monster.description = "a blistering fire elemental"
monster.experience = 1300
monster.outfit = {
	lookType = 242
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 8964
monster.speed = 230
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
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
    {id = "gold coin", maxCount = 65, chance = 12500},
	{id = "gold coin", maxCount = 60, chance = 12500},
	{id = "small ruby", maxCount = 3, chance = 3200},
	{id = "glimmering soil", chance = 2500},
	{id = "wand of draconia", chance = 1250}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 11, interval = 1000, minDamage = -65, maxDamage = -510, length = 7, spread = 3},
	{name = "condition", type = CONDITION_FIRE,  chance = 12, interval = 1000, minDamage = -50, maxDamage = -200, radius = 6, effect = 7},
	{name = "firefield",  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 3, shootEffect = 4, effect = 7}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)