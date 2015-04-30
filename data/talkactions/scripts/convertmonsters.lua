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
	for k,v in pairs(condition) do
		t[coniditionImmunities[k]] = {condition = v}
	end
	for k,v in pairs(combat) do
		if t[combatImmunities[k]] then
			t[combatImmunities[k]].combat = v
		else
			t[combatImmunities[k]] = {combat = v}
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
		s = s ..'{id = "'.. ItemType(loot[i].itemId):getName() ..'", '
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
		if isInArray({"physical","bleed","poison","earth","fire","energy","drown","ice","holy","death","lifedrain","manadrain","healing"}, attacks[i].name) then
			s = s ..'{name = "combat", type = '.. combats[attacks[i].name] ..', '
		elseif isInArray({"firecondition","earthcondition","poisoncondition","energycondition","freezecondition","icecondition","cursecondition","deathcondition","dazzlecondition","holycondition","physicalcondition","bleedcondition","drowncondition"}, attacks[i].name) then
			s = s ..'{name = "condition", type = '.. conditions[attacks[i].name] ..', '
		else
			s = s ..'{name = "'.. attacks[i].name ..'", '
		end
		if attacks[i].name == "melee" then
			s = s ..' attack = '.. attacks[i].attack ..', skill = '.. attacks[i].skill ..', interval = '.. attacks[i].speed ..', effect = '.. attacks[i].effect ..'}'
		else
			s = s ..' chance = '.. attacks[i].chance ..', interval = '.. attacks[i].speed ..', minDamage = '.. attacks[i].minCombatValue ..', maxDamage = '.. attacks[i].maxCombatValue ..''
			if attacks[i].needTarget == 1 then
				s = s ..', target = '.. tostring(attacks[i].needTarget == 1 and true or false) ..''
			end
			if attacks[i].duration ~= 0 then
				s = s ..', duration = '.. attacks[i].duration ..''
			end
			if attacks[i].speedChange ~= 0 then
				s = s ..', speedChange = '.. attacks[i].speedChange ..''
			end
			if attacks[i].range ~= 0 then
				s = s ..', range = '.. attacks[i].range ..''
			end
			if attacks[i].length ~= 0 then
				s = s ..', length = '.. attacks[i].length ..''
			end
			if attacks[i].spread ~= 0 then
				s = s ..', spread = '.. attacks[i].spread ..''
			end
			if attacks[i].radius ~= 0 then
				s = s ..', radius = '.. attacks[i].radius ..''
			end
			if attacks[i].shootType ~= 0 then
				s = s ..', shootEffect = '.. attacks[i].shootType ..''
			end
			if attacks[i].effect ~= 0 then
				s = s ..', effect = '.. attacks[i].effect ..''
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
	local s = 'defense = '.. mType:getDefense() ..',\n    armor = '.. mType:getArmor() ..',\n    '
	for i = 1, #defenses do
		if not first then
			s = s ..',\n	'
		end
		if isInArray({"physical","bleed","poison","earth","fire","energy","drown","ice","holy","death","lifedrain","manadrain","healing"}, defenses[i].name) then
			s = s ..'{name = "combat", type = '.. combats[defenses[i].name] ..', '
		elseif isInArray({"firecondition","earthcondition","poisoncondition","energycondition","freezecondition","icecondition","cursecondition","deathcondition","dazzlecondition","holycondition","physicalcondition","bleedcondition","drowncondition"}, defenses[i].name) then
			s = s ..'{name = "condition", type = '.. conditions[defenses[i].name] ..', '
		else
			s = s ..'{name = "'.. defenses[i].name ..'", '
		end
		if defenses[i].name == "melee" then
			s = s ..' attack = '.. defenses[i].attack ..', skill = '.. defenses[i].skill ..', interval = '.. defenses[i].speed ..', effect = '.. defenses[i].effect ..'}'
		else
			s = s ..' chance = '.. defenses[i].chance ..', interval = '.. defenses[i].speed ..', minDamage = '.. defenses[i].minCombatValue ..', maxDamage = '.. defenses[i].maxCombatValue ..''
			if defenses[i].needTarget == 1 then
				s = s ..', target = '.. tostring(defenses[i].needTarget == 1 and true or false) ..''
			end
			if defenses[i].duration ~= 0 then
				s = s ..', duration = '.. defenses[i].duration ..''
			end
			if defenses[i].speedChange ~= 0 then
				s = s ..', speedChange = '.. defenses[i].speedChange ..''
			end
			if defenses[i].range ~= 0 then
				s = s ..', range = '.. defenses[i].range ..''
			end
			if defenses[i].length ~= 0 then
				s = s ..', length = '.. defenses[i].length ..''
			end
			if defenses[i].spread ~= 0 then
				s = s ..', spread = '.. defenses[i].spread ..''
			end
			if defenses[i].radius ~= 0 then
				s = s ..', radius = '.. defenses[i].radius ..''
			end
			if defenses[i].shootType ~= 0 then
				s = s ..', shootEffect = '.. defenses[i].shootType ..''
			end
			if defenses[i].effect ~= 0 then
				s = s ..', effect = '.. defenses[i].effect ..''
			end
			s = s ..'}'
		end
		first = false
	end
	return s
end

function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end
	
	for _, name in pairs(Game.getMonsterTypes()) do
		if name ~= "" and MonsterType(name) then
			if not MonsterType(name):fromLuaFile() then
				local mType = MonsterType(name)
				if mType then
					local f = io.open("data/luamonster/monsters/".. name ..".lua", "w")
					f:write('local mType = Game.createMonsterType("'.. name ..'")\n')
					f:write('local monster = {}\n')
					f:write('monster.description = "'.. mType:getNameDescription():lower() ..'"\n')
					f:write('monster.experience = '.. mType:getExperience() ..'\n')
					f:write('monster.outfit = {\n	'.. serializeOutfit(mType) ..'\n}\n\n')
					f:write('monster.health = '.. mType:getHealth() ..'\n')
					f:write('monster.maxHealth = monster.health\n')
					f:write('monster.race = "'.. races[mType:getRace()] ..'"\n')
					f:write('monster.corpse = '.. mType:getCorpseId() ..'\n')
					f:write('monster.speed = '.. mType:getBaseSpeed() ..'\n')
					f:write('monster.maxSummons = '.. mType:getMaxSummons() ..'\n\n')
					f:write('monster.changeTarget = {\n    interval = '.. mType:getChangeTargetSpeed() ..',\n	chance = '.. mType:getChangeTargetChance() ..'\n}\n\n')
					f:write('monster.flags = {\n	summonable = '.. tostring(mType:isSummonable()) ..',\n    attackable = '.. tostring(mType:isAttackable()) ..',\n	hostile = '.. tostring(mType:isHostile()) ..',\n	convinceable = '.. tostring(mType:isConvinceable()) ..',\n    illusionable = '.. tostring(mType:isIllusionable()) ..',\n	canPushItems = '.. tostring(mType:canPushItems()) ..',\n	canPushCreatures = '.. tostring(mType:canPushCreatures()) ..',\n	targetDistance = '.. mType:getTargetDistance() ..',\n	staticAttack = '.. mType:getStaticAttackChance() ..'\n}\n\n')
					if #mType:getSummonList() ~= 0 then
						f:write('monster.summons = {\n    '.. serializeSummons(mType) ..'\n}\n\n')
					end
					if #mType:getVoices() ~= 0 then
						f:write('monster.voices = {\n	interval = '.. mType:getYellSpeedTicks() ..',\n	chance = '.. mType:getYellChance() ..',\n')
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
					f:write('monster.immunities = {\n	'.. serializeImmunities(mType) ..'\n}\n\n')
					if #mType:getCreatureEvents() ~= 0 then
						f:write('monster.events = {\n    '.. serializeEvents(mType) ..'\n}\n\n')
					end
					f:write('mType:register(monster)')
					f:close()
				end
			end
		end
	end
	return false
end
