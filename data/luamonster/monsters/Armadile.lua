local mType = Game.createMonsterType("Armadile")
local monster = {}
monster.description = "a armadile"
monster.experience = 2900
monster.outfit = {
	lookType = 487
}

monster.health = 3800
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 18378
monster.speed = 210
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
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Creak!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 98, chance = 100000},
	{id = "platinum coin", maxCount = 7, chance = 95000},
	{id = "mana potion", maxCount = 3, chance = 14285},
	{id = "great health potion", maxCount = 2, chance = 15000},
	{id = "strong health potion", maxCount = 2, chance = 14285},
	{id = "crystalline spikes", chance = 14540},
	{id = "great mana potion", maxCount = 2, chance = 15920},
	{id = "strong mana potion", maxCount = 2, chance = 15000},
	{id = "battle stone", chance = 12460},
	{id = "envenomed arrow", maxCount = 10, chance = 10230},
	{id = "drill bolt", maxCount = 5, chance = 7692},
	{id = "coal", chance = 6000},
	{id = "green crystal fragment", maxCount = 2, chance = 5690},
	{id = "green crystal splinter", maxCount = 2, chance = 3920},
	{id = "terra boots", chance = 2850},
	{id = "titan axe", chance = 1540},
	{id = "bonebreaker", chance = 1150},
	{id = "time ring", chance = 1000},
	{id = "tower shield", chance = 620},
	{id = "crystalline armor", chance = 230}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 50, interval = 2000, effect = 0},
	{name = "drunk",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, radius = 4, effect = 7},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = -430, maxDamage = -550, range = 7, radius = 4, effect = 13},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -200, maxDamage = -400, radius = 4, effect = 21}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = 15},
	{type = COMBAT_DEATHDAMAGE, percent = 1},
	{type = COMBAT_ICEDAMAGE, percent = 15}
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