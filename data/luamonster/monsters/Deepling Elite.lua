local mType = Game.createMonsterType("Deepling Elite")
local monster = {}
monster.description = "a deepling elite"
monster.experience = 3000
monster.outfit = {
	lookType = 441
}

monster.health = 3200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15176
monster.speed = 210
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

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 86, chance = 100000},
	{id = "deepling warts", chance = 25000},
	{id = "great health potion", chance = 25000},
	{id = "eye of a deepling", chance = 25000},
	{id = "great mana potion", chance = 24000},
	{id = "deepling filet", chance = 25000},
	{id = "deepling ridge", chance = 19000},
	{id = "deeptags", chance = 21700},
	{id = "vortex bolt", maxCount = 5, chance = 24000},
	{id = "life ring", chance = 5360},
	{id = "small emerald", maxCount = 2, chance = 6290},
	{id = "heavy trident", chance = 3380},
	{id = "fish fin", chance = 2000},
	{id = "warrior's shield", chance = 1234},
	{id = "warrior's axe", chance = 640}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 90, minDamage = 0, maxDamage = -400, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -225, range = 7, shootEffect = 12}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 150, maxDamage = 200, shootEffect = 12, effect = 13}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)