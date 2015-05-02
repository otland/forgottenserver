local mType = Game.createMonsterType("Barbarian Bloodwalker")
local monster = {}
monster.description = "a barbarian bloodwalker"
monster.experience = 195
monster.outfit = {
	lookFeet = 132,
	lookLegs = 132,
	lookHead = 114,
	lookType = 255,
	lookBody = 132
}

monster.health = 305
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20335
monster.speed = 230
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "YAAAHEEE!", yell = false},
	{text = "SLAUGHTER!", yell = false},
	{text = "CARNAGE!", yell = false},
	{text = "You can run but you can't hide", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 50000},
	{id = "ham", chance = 4970},
	{id = "lamp", chance = 8280},
	{id = "halberd", chance = 6730},
	{id = "chain helmet", chance = 10440},
	{id = "battle axe", chance = 6000},
	{id = "chain armor", chance = 10290},
	{id = "red piece of cloth", chance = 530},
	{id = "health potion", chance = 950},
	{id = "beastslayer axe", chance = 390},
	{id = "fur boots", chance = 100},
	{id = "shard", chance = 290}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 50, minDamage = 0, maxDamage = -240, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 240, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 50}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)