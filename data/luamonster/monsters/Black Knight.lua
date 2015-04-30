local mType = Game.createMonsterType("Black Knight")
local monster = {}
monster.description = "a black knight"
monster.experience = 1600
monster.outfit = {
	lookFeet = 95,
	lookLegs = 95,
	lookHead = 95,
	lookType = 131,
	lookAddons = 3,
	lookBody = 95
}

monster.health = 1800
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20355
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "MINE!", yell = true},
	{text = "No prisoners!", yell = true},
	{text = "NO MERCY!", yell = true},
	{text = "By Bolg's Blood!", yell = false},
	{text = "You're no match for me!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 23000},
	{id = "gold coin", maxCount = 56, chance = 23000},
	{id = "spear", maxCount = 3, chance = 30800},
	{id = "brown bread", maxCount = 2, chance = 21600},
	{id = "brass legs", chance = 12200},
	{id = "rope", chance = 16020},
	{id = "halberd", chance = 11850},
	{id = "plate armor", chance = 10370},
	{id = "steel helmet", chance = 11220},
	{id = "two handed sword", chance = 8470},
	{id = "warrior helmet", chance = 5610},
	{id = "battle hammer", chance = 6980},
	{id = "knight axe", chance = 3280},
	{id = "knight armor", chance = 320},
	{id = "dark armor", chance = 1900},
	{id = "dark helmet", chance = 2330},
	{id = "knight legs", chance = 1050},
	{id = "ruby necklace", chance = 740},
	{id = "dragon lance", chance = 110},
	{id = "lightning legs", chance = 950},
	{id = "piggy bank", chance = 210},
	{id = "boots of haste", chance = 320}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -200, range = 7, shootEffect = 1}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 95},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -10}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)