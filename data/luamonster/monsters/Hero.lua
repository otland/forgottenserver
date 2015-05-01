local mType = Game.createMonsterType("Hero")
local monster = {}
monster.description = "a hero"
monster.experience = 1200
monster.outfit = {
	lookType = 73
}

monster.health = 1400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20415
monster.speed = 240
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
	{text = "Let's have a fight!", yell = false},
	{text = "Welcome to my battleground.", yell = false},
	{text = "Have you seen princess Lumelia?", yell = false},
	{text = "I will sing a tune at your grave.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 59500},
	{id = "arrow", maxCount = 13, chance = 26000},
	{id = "scroll of heroic deeds", chance = 5000},
	{id = "small notebook", chance = 930},
	{id = "rope", chance = 2190},
	{id = "scroll", chance = 45000},
	{id = "lyre", chance = 1640},
	{id = "green tunic", chance = 8000},
	{id = "bow", chance = 13300},
	{id = "scarf", chance = 1110},
	{id = "meat", maxCount = 3, chance = 8200},
	{id = "grapes", chance = 19850},
	{id = "red rose", chance = 20450},
	{id = "wedding ring", chance = 4910},
	{id = "two handed sword", chance = 1500},
	{id = "sniper arrow", maxCount = 4, chance = 11400},
	{id = "war hammer", chance = 870},
	{id = "red piece of cloth", chance = 2006},
	{id = "crown armor", chance = 490},
	{id = "crown shield", chance = 280},
	{id = "crown helmet", chance = 450},
	{id = "fire sword", chance = 550},
	{id = "great health potion", chance = 720},
	{id = "piggy bank", chance = 80},
	{id = "might ring", subType = 20, chance = 470},
	{id = "crown legs", chance = 660}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 70, minDamage = 0, maxDamage = -240, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -120, range = 7, shootEffect = 3}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 200, maxDamage = 250, shootEffect = 3, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 30},
	{type = COMBAT_ENERGYDAMAGE, percent = 40},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = -20},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)