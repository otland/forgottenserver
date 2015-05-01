local mType = Game.createMonsterType("Draken Elite")
local monster = {}
monster.description = "a draken elite"
monster.experience = 4200
monster.outfit = {
	lookType = 362
}

monster.health = 5550
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 12609
monster.speed = 250
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "For ze emperor!", yell = false},
	{text = "You will die zhouzandz deazhz!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 47000},
	{id = "platinum coin", maxCount = 8, chance = 50360},
	{id = "broken slicer", chance = 24670},
	{id = "meat", chance = 30175},
	{id = "great mana potion", maxCount = 3, chance = 9340},
	{id = "small diamond", maxCount = 4, chance = 2440},
	{id = "draken wristbands", chance = 14030},
	{id = "broken draken mail", chance = 16930},
	{id = "ultimate health potion", maxCount = 3, chance = 9250},
	{id = "draken sulphur", chance = 7600},
	{id = "twiceslicer", chance = 910},
	{id = "magic sulphur", chance = 2100},
	{id = "assassin dagger", chance = 980},
	{id = "Zaoan sword", chance = 490},
	{id = "draken boots", chance = 600},
	{id = "blade of corruption", chance = 20},
	{id = "cobra crown", chance = 10},
	{id = "Zaoan helmet", chance = 150},
	{id = "Zaoan legs", chance = 770},
	{id = "Zaoan armor", chance = 490},
	{id = "snake god's wristguard", chance = 80},
	{id = "elite draken mail", chance = 110}
}

monster.attacks = {
	{name = "melee",  attack = 59, skill = 110, minDamage = 0, maxDamage = -354, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -240, maxDamage = -550, length = 4, spread = 3, effect = 6},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -200, maxDamage = -300, range = 7, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -280, maxDamage = -410, target = true, radius = 4, shootEffect = 4, effect = 3},
	{name = "soulfire",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 3},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -250, maxDamage = -320, range = 7, shootEffect = 15, effect = 3}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 510, maxDamage = 600, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 40},
	{type = COMBAT_DEATHDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE, percent = 30}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)