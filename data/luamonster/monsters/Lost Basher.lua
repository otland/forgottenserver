local mType = Game.createMonsterType("Lost Basher")
local monster = {}
monster.description = "a lost basher"
monster.experience = 1800
monster.outfit = {
	lookType = 538
}

monster.health = 2600
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19963
monster.speed = 250
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Yhouuuu!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 60000},
	{id = "platinum coin", maxCount = 2, chance = 70000},
	{id = "brown mushroom", maxCount = 2, chance = 15170},
	{id = "basalt fetish", chance = 8800},
	{id = "ultimate health potion", chance = 10250},
	{id = "great mana potion", chance = 11240},
	{id = "small topaz", chance = 10200},
	{id = "basalt figurine", chance = 9470},
	{id = "buckle", chance = 10930},
	{id = "bonecarving knife", chance = 7320},
	{id = "coal", chance = 21130},
	{id = "lost basher's spike", chance = 17260},
	{id = "mad froth", chance = 11870},
	{id = "leather harness", chance = 12500},
	{id = "dwarven ring (hard drinking)", chance = 2560},
	{id = "black shield", chance = 3710},
	{id = "fiery knight axe", subType = 1000, chance = 1200},
	{id = "iron ore", chance = 5280},
	{id = "fire axe", chance = 310},
	{id = "pair of iron fists", chance = 1410},
	{id = "piggy bank", chance = 4450},
	{id = "lost basher's spike", chance = 14380},
	{id = "bloody dwarven beard", chance = 1730},
	{id = "spiked squelcher", chance = 420},
	{id = "terra boots", chance = 780},
	{id = "chaos mace", chance = 160},
	{id = "knight legs", chance = 310},
	{id = "war axe", chance = 120},
	{id = "blue crystal shard", chance = 840}
}

monster.attacks = {
	{name = "melee",  attack = 78, skill = 80, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -220, target = true, range = 7, radius = 3, shootEffect = 26, effect = 5},
	{name = "drunk",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, radius = 4, shootEffect = 27, effect = 20},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -650, radius = 2, shootEffect = 27, effect = 12}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 250, maxDamage = 500, radius = 2, shootEffect = 27, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_ICEDAMAGE, percent = 1},
	{type = COMBAT_DEATHDAMAGE, percent = 1},
	{type = COMBAT_FIREDAMAGE, percent = 40}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)