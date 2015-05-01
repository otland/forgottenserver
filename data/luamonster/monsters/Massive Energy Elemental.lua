local mType = Game.createMonsterType("Massive Energy Elemental")
local monster = {}
monster.description = "a massive energy elemental"
monster.experience = 950
monster.outfit = {
	lookType = 290
}

monster.health = 1100
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 8966
monster.speed = 210
monsters.runHealth = 1
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
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
	staticAttack = 85
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 91, chance = 50000},
	{id = "strong mana potion", chance = 17450},
	{id = "dead cat", maxCount = 15, chance = 6550},
	{id = "great mana potion", chance = 5450},
	{id = "rusty armor", chance = 730},
	{id = "small amethyst", maxCount = 3, chance = 3270},
	{id = "wand of starstorm", chance = 730},
	{id = "lightning pendant", subType = 200, chance = 730},
	{id = "spellbook of warding", chance = 360},
	{id = "lightning legs", chance = 150},
	{id = "energy spike sword", subType = 1000, chance = 500},
	{id = "shockwave amulet", subType = 5, chance = 500}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 40, minDamage = 0, maxDamage = -175, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -270, maxDamage = -615, target = true, range = 7, radius = 2, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -175, maxDamage = -205, range = 7, shootEffect = 36, effect = 12},
	{name = "massive energy elemental electrify",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 36, effect = 12}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 190, maxDamage = 250, shootEffect = 36, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 70},
	{type = COMBAT_EARTHDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 1},
	{type = COMBAT_HOLYDAMAGE, percent = 25}
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
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)