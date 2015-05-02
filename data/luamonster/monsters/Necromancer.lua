local mType = Game.createMonsterType("Necromancer")
local monster = {}
monster.description = "a necromancer"
monster.experience = 580
monster.outfit = {
	lookType = 9
}

monster.health = 580
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20455
monster.speed = 200
monster.maxSummons = 2

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
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "Ghoul", chance = 15, interval = 2000},
	{name = "Ghost", chance = 5, interval = 2000},
	{name = "Mummy", chance = 5, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Your corpse will be mine!", yell = false},
	{text = "Taste the sweetness of death!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 30050},
	{id = "poison arrow", maxCount = 5, chance = 15000},
	{id = "green mushroom", chance = 1470},
	{id = "clerical mace", chance = 390},
	{id = "strong mana potion", chance = 300},
	{id = "mystic turban", chance = 500},
	{id = "skull staff", chance = 100},
	{id = "noble axe", chance = 10},
	{id = "spellbook of warding", chance = 130},
	{id = "boots of haste", chance = 210},
	{id = "book of necromantic rituals", chance = 10130},
	{id = "necromantic robe", chance = 1001}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, minDamage = 0, maxDamage = -80, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = -60, maxDamage = -120, range = 1, shootEffect = 11, effect = 39},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -65, maxDamage = -120, range = 7, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 2000, minDamage = 50, maxDamage = 80, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = -5}
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