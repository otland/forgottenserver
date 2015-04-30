local mType = Game.createMonsterType("Chakoya Toolshaper")
local monster = {}
monster.description = "a chakoya toolshaper"
monster.experience = 40
monster.outfit = {
	lookType = 259
}

monster.health = 80
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7320
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Chikuva!!", yell = false},
	{text = "Jinuma jamjam!", yell = false},
	{text = "Suvituka siq chuqua!", yell = false},
	{text = "Kiyosa sipaju!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 20, chance = 60000},
	{id = "fish", maxCount = 2, chance = 25000},
	{id = "mace", chance = 4000},
	{id = "pick", chance = 940},
	{id = "bone shield", chance = 870},
	{id = "mammoth whopper", chance = 150},
	{id = "ice cube", chance = 470},
	{id = "green perch", chance = 100},
	{id = "northern pike", chance = 120},
	{id = "rainbow trout", chance = 170}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 25, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -45, target = true, range = 7, radius = 3, shootEffect = 10}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = 40}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)