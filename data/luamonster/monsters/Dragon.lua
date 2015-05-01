local mType = Game.createMonsterType("Dragon")
local monster = {}
monster.description = "a dragon"
monster.experience = 700
monster.outfit = {
	lookType = 34
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5973
monster.speed = 185
monsters.runHealth = 300
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
	{text = "GROOAAARRR", yell = true},
	{text = "FCHHHHH", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 70, chance = 47500},
	{id = "gold coin", maxCount = 45, chance = 37500},
	{id = "dragon's tail", chance = 9740},
	{id = "dragon ham", maxCount = 3, chance = 65500},
	{id = "steel shield", chance = 15000},
	{id = "crossbow", chance = 10000},
	{id = "longsword", chance = 4000},
	{id = "steel helmet", chance = 3000},
	{id = "plate legs", chance = 2000},
	{id = "broadsword", chance = 1950},
	{id = "strong health potion", chance = 1000},
	{id = "double axe", chance = 960},
	{id = "wand of inferno", chance = 1005},
	{id = "green dragon scale", chance = 1000},
	{id = "dragon hammer", chance = 560},
	{id = "green dragon leather", chance = 1005},
	{id = "small diamond", chance = 380},
	{id = "dragon shield", chance = 320},
	{id = "serpent sword", chance = 420},
	{id = "dragonbone staff", chance = 110},
	{id = "life crystal", chance = 120},
	{id = "burst arrow", maxCount = 10, chance = 8060}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 50, minDamage = 0, maxDamage = -120, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -140, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -170, length = 8, spread = 3, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 40, maxDamage = 70, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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