local mType = Game.createMonsterType("hero")
local monster = {}

monster.name = "Hero"
monster.description = "a hero"
monster.experience = 1200
monster.outfit = {
	lookType = 73,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1400
monster.maxHealth = 1400
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20415
monster.speed = 280
monster.summonCost = 0
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
	pushable = false,
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 90,
	targetDistance = 1,
	healthHidden = false,
	canWalkOnEnergy = false,
	canWalkOnFire = false,
	canWalkOnPoison = false
}

monster.light = {
	level = 0,
	color = 0
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Let's have a fight!", yell = false},
	{text = "Welcome to my battleground.", yell = false},
	{text = "Have you seen princess Lumelia?", yell = false},
	{text = "I will sing a tune at your grave.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 40},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = -20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -240, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -120, range = 7, ShootEffect = CONST_ANI_ARROW}
}

monster.defenses = {
	defense = 35,
	armor = 35,
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 200, maxDamage = 250, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 60000, maxCount = 100},
	{id = 1949, chance = 45000},
	{id = "arrow", chance = 26000, maxCount = 13},
	{id = "red rose", chance = 20450},
	{id = "grapes", chance = 19850},
	{id = "bow", chance = 13300},
	{id = "sniper arrow", chance = 11400, maxCount = 4},
	{id = "meat", chance = 8200, maxCount = 3},
	{id = "green tunic", chance = 8000},
	{id = "wedding ring", chance = 5200},
	{id = "scroll of heroic deeds", chance = 5000},
	{id = 2120, chance = 2190},
	{id = "red piece of cloth", chance = 2006},
	{id = 2071, chance = 1640},
	{id = "two handed sword", chance = 1500},
	{id = "scarf", chance = 1110},
	{id = "small notebook", chance = 930},
	{id = "war hammer", chance = 910},
	{id = "great health potion", chance = 720},
	{id = "crown armor", chance = 650},
	{id = "crown legs", chance = 590},
	{id = "might ring", chance = 500},
	{id = "fire sword", chance = 490},
	{id = "crown helmet", chance = 460},
	{id = "crown shield", chance = 210},
	{id = "piggy bank", chance = 80}
}

mType:register(monster)
