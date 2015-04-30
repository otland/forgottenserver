local mType = Game.createMonsterType("Shadow Pupil")
local monster = {}
monster.description = "a shadow pupil"
monster.experience = 410
monster.outfit = {
	lookType = 551
}

monster.health = 450
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21254
monster.speed = 210
monster.maxSummons = 4

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

monster.summons = {
    {name = "Skeleton", chance = 10, interval = 2000},
	{name = "Ghost", chance = 5, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "When I'm finished with you, you'll be a shadow of your former self.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 64, chance = 100000},
	{id = "incantation notes", chance = 9130},
	{id = "horoscope", chance = 15000},
	{id = "book of necromantic rituals", chance = 6920},
	{id = "brown mushroom", chance = 5170},
	{id = "pieces of magic chalk", chance = 4300},
	{id = "strong mana potion", chance = 470},
	{id = "mystic turban", chance = 580},
	{id = "boots of haste", chance = 60},
	{id = "clerical mace", chance = 290},
	{id = "spellbook of enlightenment", chance = 290}
}

monster.attacks = {
	{name = "melee",  attack = 35, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = -60, maxDamage = -80, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -30, maxDamage = -60, radius = 3, shootEffect = 32, effect = 18}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = 5}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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