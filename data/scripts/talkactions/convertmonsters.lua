local coniditionImmunities = {"earth","fire","energy","bleed","paralyze","outfit","invisible","drunk","drown","ice","holy","death"}
local combatImmunities = {"physical","energy","earth","fire","undefined","lifedrain","manadrain","heal","drown","ice","holy","death"}
local races = {"venom","blood","undead","fire","energy"}
local convertElements = {
	[1] = "COMBAT_PHYSICALDAMAGE", [2] = "COMBAT_ENERGYDAMAGE", [4] = "COMBAT_EARTHDAMAGE", [8] = "COMBAT_FIREDAMAGE", [16] = "COMBAT_UNDEFINEDDAMAGE", [32] = "COMBAT_LIFEDRAIN", [64] = "COMBAT_MANADRAIN", [128] = "COMBAT_HEALING",
	[256] = "COMBAT_DROWNDAMAGE", [512] = "COMBAT_ICEDAMAGE", [1024] = "COMBAT_HOLYDAMAGE", [2048] = "COMBAT_DEATHDAMAGE"
}
local combats = {
	["physical"] = "COMBAT_PHYSICALDAMAGE", ["bleed"] = "COMBAT_PHYSICALDAMAGE", ["poison"] = "COMBAT_EARTHDAMAGE", ["earth"] = "COMBAT_EARTHDAMAGE", ["fire"] = "COMBAT_FIREDAMAGE", ["energy"] = "COMBAT_ENERGYDAMAGE",
	["drown"] = "COMBAT_DROWNDAMAGE", ["ice"] = "COMBAT_ICEDAMAGE", ["holy"] = "COMBAT_HOLYDAMAGE", ["death"] = "COMBAT_DEATHDAMAGE", ["lifedrain"] = "COMBAT_LIFEDRAIN", ["manadrain"] = "COMBAT_MANADRAIN", ["healing"] = "COMBAT_HEALING"
}
local combatBits = {
	[1] = "physical", [2] = "energy", [4] = "poison", [8] = "fire", [16] = "undefined", [32] = "lifedrain", [64] = "manadrain", [128] = "healing",
	[256] = "drown", [512] = "ice", [1024] = "holy", [2048] = "death"
}
local conditionBits = {
	[1] = "poison", [2] = "fire", [4] = "energy", [8] = "physical", [16] = "haste", [32] = "paralyze", [64] = "outfit", [128] = "invisible",
	[256] = "light", [512] = "manashield", [1024] = "infight", [2048] = "drunk", [32768] = "drown", [1048576] = "freezing", 
	[bit.lshift(1, 21)] = "holy", [bit.lshift(1, 22)] = "cursed"
}
local conditions = {
	["firecondition"] = "CONDITION_FIRE", ["earthcondition"] = "CONDITION_POISON", ["poisoncondition"] = "CONDITION_POISON", ["energycondition"] = "CONDITION_ENERGY", ["drowncondition"] = "CONDITION_DROWN",
	["icecondition"] = "CONDITION_FREEZING", ["freezecondition"] = "CONDITION_FREEZING", ["holycondition"] = "CONDITION_DAZZLED", ["deathcondition"] = "CONDITION_CURSED", ["cursecondition"] = "CONDITION_CURSED", ["physicalcondition"] = "CONDITION_BLEEDING", ["bleedcondition"] = "CONDITION_BLEEDING"
}

local function serializeOutfit(mType)
	local outfit = mType:getOutfit()
	local s = ""
	local first = true
	for k,v in pairs(outfit) do
		if v ~= 0 then
			if not first then
				s = s ..',\n	'
			end
			s = s ..''.. k ..' = '.. v
			first = false
		end
	end
	return s
end

local function serializeEvents(mType)
	local events = mType:getCreatureEvents()
	local s = ""
	local first = true
	for k,v in pairs(events) do
		if not first then
			s = s ..',\n	'
		end
		s = s ..'"'.. v ..'"'
		first = false
	end
	return s
end

local function serializeSummons(mType)
	local summons = mType:getSummonList()
	local s = ""
	local first = true
	for i = 1, #summons do
		if not first then
			s = s ..',\n	'
		end
		s = s ..'{name = "'.. summons[i].name ..'", chance = '.. summons[i].chance ..', interval = '.. summons[i].speed ..'}'
		first = false
	end
	return s
end

local function serializeVoices(mType)
	local voices = mType:getVoices()
	local s = ""
	local first = true
	for i = 1, #voices do
		if not first then
			s = s ..',\n	'
		end
		s = s ..'{text = "'.. voices[i].text ..'", yell = '.. tostring(voices[i].yellText == 1 and true or false) ..'}'
		first = false
	end
	return s
end

local function serializeElements(mType)
	local elements = mType:getElementList()
	local s = ""
	local first = true
	for k,v in pairs(elements) do
		if not first then
			s = s ..',\n	'
		end
		s = s ..'{type = '.. convertElements[k] ..', percent = '.. v ..'}'
		first = false
	end
	return s
end

local function serializeImmunities(mType)
	local condition = mType:getConditionImmunities()
	local combat = mType:getCombatImmunities()
	local s = ""
	local first = true
	local t = {}
	for k,v in pairs(conditionBits) do
		hasCondition = hasBitSet(k, condition)
		if hasCondition then
			t[v] = {condition = hasCondition}
		end
	end
	for k,v in pairs(combatBits) do
		hasCombat = hasBitSet(k, combat)
		if hasCombat then
			if t[v] then
				t[v].combat = hasCombat
			else
				t[v] = {combat = hasCombat}
			end
		end
	end
	for k,v in pairs(t) do
		if not first then
			s = s ..',\n	'
		end
		s = s ..'{type = "'.. k ..'", condition = '.. tostring(v.condition == 1 and true or false) ..', combat = '.. tostring(v.combat == 1 and true or false) ..'}'
		first = false
	end
	return s
end

local function serializeLoot(mType)
	local loot = mType:getLoot()
	local s = ""
	local first = true
	for i = 1, #loot do
		if not first then
			s = s ..',\n	' 
		end
		s = s ..'{id = '.. loot[i].itemId ..' --[['.. ItemType(loot[i].itemId):getName() ..']], '
		if loot[i].maxCount > 1 then
			s = s ..'maxCount = '.. loot[i].maxCount ..', '
		end
		if loot[i].subType ~= -1 then
			s = s ..'subType = '.. loot[i].subType ..', '
		end
		if loot[i].actionId ~= -1 then
			s = s ..'actionId = '.. loot[i].actionId ..', '
		end
		s = s ..'chance = '.. loot[i].chance ..'}'
		first = false
	end
	return s
end

local function serializeAttacks(mType)
	local attacks = mType:getAttackList()
	local s = ""
	local first = true
	for i = 1, #attacks do
		if not first then
			s = s ..',\n	'
		end
		if table.contains({"physical","bleed","poison","earth","fire","energy","drown","ice","holy","death","lifedrain","manadrain","healing"}, attacks[i].name) then
			s = s ..'{name = "combat", type = '.. combats[attacks[i].name] ..', '
		elseif table.contains({"firecondition","earthcondition","poisoncondition","energycondition","freezecondition","icecondition","cursecondition","deathcondition","dazzlecondition","holycondition","physicalcondition","bleedcondition","drowncondition"}, attacks[i].name) then
			s = s ..'{name = "condition", type = '.. conditions[attacks[i].name] ..', '
		else
			s = s ..'{name = "'.. attacks[i].name ..'", '
		end
		if attacks[i].name == "melee" then
			s = s ..' attack = '.. attacks[i].attack ..', skill = '.. attacks[i].skill ..', minDamage = '.. attacks[i].minCombatValue ..', maxDamage = '.. attacks[i].maxCombatValue ..', interval = '.. attacks[i].speed ..'}'
		else
			s = s ..' chance = '.. attacks[i].chance ..', interval = '.. attacks[i].speed ..', minDamage = '.. attacks[i].minCombatValue ..', maxDamage = '.. attacks[i].maxCombatValue ..''
			if attacks[i].needTarget == 1 then
				s = s ..', target = '.. tostring(attacks[i].needTarget == 1 and true or false) ..''
			end
			if attacks[i].duration ~= nil then
				s = s ..', duration = '.. attacks[i].duration ..''
			end
			if attacks[i].speedChange ~= nil then
				s = s ..', speedChange = '.. attacks[i].speedChange ..''
			end
			if attacks[i].range ~= nil then
				s = s ..', range = '.. attacks[i].range ..''
			end
			if attacks[i].length ~= nil then
				s = s ..', length = '.. attacks[i].length ..''
			end
			if attacks[i].spread ~= nil then
				s = s ..', spread = '.. attacks[i].spread ..''
			end
			if attacks[i].radius ~= nil then
				s = s ..', radius = '.. attacks[i].radius ..''
			end
			if attacks[i].shootType ~= nil then
				s = s ..', shootEffect = '.. attacks[i].shootType ..''
			end
			s = s ..'}'
		end
		first = false
	end
	return s
end

local function serializeDefenses(mType)
	local defenses = mType:getDefenseList()
	local first = true
	local s = 'defense = '.. mType:defense() ..',\n    armor = '.. mType:armor() ..',\n    '
	for i = 1, #defenses do
		if not first then
			s = s ..',\n	'
		end
		if table.contains({"physical","bleed","poison","earth","fire","energy","drown","ice","holy","death","lifedrain","manadrain","healing"}, defenses[i].name) then
			s = s ..'{name = "combat", type = '.. combats[defenses[i].name] ..', '
		elseif table.contains({"firecondition","earthcondition","poisoncondition","energycondition","freezecondition","icecondition","cursecondition","deathcondition","dazzlecondition","holycondition","physicalcondition","bleedcondition","drowncondition"}, defenses[i].name) then
			s = s ..'{name = "condition", type = '.. conditions[defenses[i].name] ..', '
		else
			s = s ..'{name = "'.. defenses[i].name ..'", '
		end
		if defenses[i].name == "melee" then
			s = s ..' attack = '.. defenses[i].attack ..', skill = '.. defenses[i].skill ..', minDamage = '.. attacks[i].minCombatValue ..', maxDamage = '.. attacks[i].maxCombatValue ..', interval = '.. defenses[i].speed ..'}'
		else
			s = s ..' chance = '.. defenses[i].chance ..', interval = '.. defenses[i].speed ..', minDamage = '.. defenses[i].minCombatValue ..', maxDamage = '.. defenses[i].maxCombatValue ..''
			if defenses[i].needTarget == 1 then
				s = s ..', target = '.. tostring(defenses[i].needTarget == 1 and true or false) ..''
			end
			if defenses[i].duration ~= nil then
				s = s ..', duration = '.. defenses[i].duration ..''
			end
			if defenses[i].speedChange ~= nil then
				s = s ..', speedChange = '.. defenses[i].speedChange ..''
			end
			if defenses[i].range ~= nil then
				s = s ..', range = '.. defenses[i].range ..''
			end
			if defenses[i].length ~= nil then
				s = s ..', length = '.. defenses[i].length ..''
			end
			if defenses[i].spread ~= nil then
				s = s ..', spread = '.. defenses[i].spread ..''
			end
			if defenses[i].radius ~= nil then
				s = s ..', radius = '.. defenses[i].radius ..''
			end
			if defenses[i].shootType ~= nil then
				s = s ..', shootEffect = '.. defenses[i].shootType ..''
			end
			s = s ..'}'
		end
		first = false
	end
	return s
end

local talk = TalkAction("/xmltolua")

function talk.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end
	for name, mType in pairs(Game.getMonsterTypes()) do
		if mType then
			local exists = io.open("data/xmltolua/monsters/".. name ..".lua", "r")
			if exists then
				exists:close()
				goto continue
			end
			local f = io.open("data/xmltolua/monsters/".. name ..".lua", "w")
			f:write('local mtype = Game.createMonsterType("'.. name ..'")\n')
			f:write('local monster = {}\n')
			f:write('monster.description = "'.. mType:nameDescription():lower() ..'"\n')
			f:write('monster.experience = '.. mType:experience() ..'\n')
			f:write('monster.outfit = {\n	'.. serializeOutfit(mType) ..'\n}\n\n')
			f:write('monster.health = '.. mType:health() ..'\n')
			f:write('monster.maxHealth = monster.health\n')
			f:write('monster.race = "'.. races[mType:race()] ..'"\n')
			f:write('monster.corpse = '.. mType:corpseId() ..'\n')
			f:write('monster.speed = '.. mType:baseSpeed() ..'\n')
			if mType:runHealth() > 0 then
				f:write('monster.runHealth = '.. mType:runHealth() ..'\n')
			end
			f:write('monster.maxSummons = '.. mType:maxSummons() ..'\n\n')
			f:write('monster.changeTarget = {\n    interval = '.. mType:changeTargetSpeed() ..',\n    chance = '.. mType:changeTargetChance() ..'\n}\n\n')
			f:write('monster.flags = {\n    healthHidden = '.. tostring(mType:isHealthHidden()) ..',\n    summonable = '.. tostring(mType:isSummonable()) ..',\n    attackable = '.. tostring(mType:isAttackable()) ..',\n    hostile = '.. tostring(mType:isHostile()) ..',\n    convinceable = '.. tostring(mType:isConvinceable()) ..',\n    illusionable = '.. tostring(mType:isIllusionable()) ..',\n    canPushItems = '.. tostring(mType:canPushItems()) ..',\n    canPushCreatures = '.. tostring(mType:canPushCreatures()) ..',\n    targetDistance = '.. mType:getTargetDistance() ..',\n    staticAttack = '.. mType:getStaticAttackChance() ..'\n}\n\n')
			if #mType:getSummonList() ~= 0 then
				f:write('monster.summons = {\n    '.. serializeSummons(mType) ..'\n}\n\n')
			end
			if #mType:getVoices() ~= 0 then
				f:write('monster.voices = {\n	interval = '.. mType:yellSpeedTicks() ..',\n	chance = '.. mType:yellChance() ..',\n')
				f:write('	'.. serializeVoices(mType) ..'\n}\n\n')
			end
			if #mType:getLoot() ~= 0 then
				f:write('monster.loot = {\n    '.. serializeLoot(mType) ..'\n}\n\n')
			end
			if #mType:getAttackList() ~= 0 then
				f:write('monster.attacks = {\n	'.. serializeAttacks(mType) ..'\n}\n\n')
			end
			if #mType:getDefenseList() ~= 0 then
				f:write('monster.defenses = {\n	'.. serializeDefenses(mType) ..'\n}\n\n')
			end
			if #mType:getElementList() ~= 0 then
				f:write('monster.elements = {\n    '.. serializeElements(mType) ..'\n}\n\n')
			end
			if mType:getConditionImmunities() or mType:getCombatImmunities() then
				f:write('monster.immunities = {\n	'.. serializeImmunities(mType) ..'\n}\n\n')
			end
			if #mType:getCreatureEvents() ~= 0 then
				f:write('monster.events = {\n    '.. serializeEvents(mType) ..'\n}\n\n')
			end
			f:write('mtype:register(monster)')
			f:close()
			::continue::
		end
	end
	return false
end

talk:register()