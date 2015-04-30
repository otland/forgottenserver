local mType = Game.createMonsterType("Blood Hand")
local monster = {}
monster.description = "a blood hand"
monster.experience = 750
monster.outfit = {
	lookType = 552
}

monster.health = 700
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21257
monster.speed = 220
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
	{text = "Blood for the dark god!", yell = false},
	{text = "Die, filth!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 130, chance = 100000},
	{id = "blood tincture in a vial", chance = 15460},
	{id = "lancet", chance = 10680},
	{id = "incantation notes", chance = 8820},
	{id = "book of necromantic rituals", chance = 9340},
	{id = "horoscope", chance = 7950},
	{id = "pieces of magic chalk", chance = 6120},
	{id = "strong mana potion", chance = 5590},
	{id = "necrotic rod", chance = 3000},
	{id = "spellbook of enlightenment", chance = 790},
	{id = "rough red gem", chance = 710},
	{id = "white piece of cloth", chance = 840},
	{id = "mystic turban", chance = 790},
	{id = "boots of haste", chance = 210},
	{id = "red piece of cloth", chance = 500},
	{id = "skull staff", chance = 130},
	{id = "noble axe", chance = 10}
}

monster.attacks = {
	{name = "melee",  attack = 45, skill = 60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -50, maxDamage = -100, radius = 4, effect = 14},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -600, radius = 4, effect = 4},
	{name = "condition", type = CONDITION_BLEEDING,  chance = 15, interval = 2000, minDamage = -120, maxDamage = -160, radius = 6, effect = 10}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, effect = 69}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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