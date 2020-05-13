-- What slots should be checked for elemental damage, crit, multi shot, mana leech, spelldamage etc
local checkweaponslots = {
	CONST_SLOT_LEFT,
	CONST_SLOT_RIGHT,
	CONST_SLOT_NECKLACE,
	CONST_SLOT_HEAD,
	CONST_SLOT_RING
}

-- What slots should be checked for resistances (basically all of them, as shields can be in either hand)
local checkallslots = {
	CONST_SLOT_LEFT,
	CONST_SLOT_RIGHT,
	CONST_SLOT_HEAD,
	CONST_SLOT_NECKLACE,
	CONST_SLOT_ARMOR,
	CONST_SLOT_LEGS,
	CONST_SLOT_FEET,
	CONST_SLOT_RING
}

-- Match the animation to the ammunation type
local animation = {
	[2543] = CONST_ANI_BOLT,
	[2547] = CONST_ANI_POWERBOLT,
	[6529] = CONST_ANI_INFERNALBOLT,
	[7363] = CONST_ANI_PIERCINGBOLT,
	[15649] = CONST_ANI_VORTEXBOLT,
	[18435] = CONST_ANI_PRISMATICBOLT,
	[18436] = CONST_ANI_DRILLBOLT,

	[2544] = CONST_ANI_ARROW,
	[2545] = CONST_ANI_POISONARROW,
	[2546] = CONST_ANI_BURSTARROW,
	[7365] = CONST_ANI_ONYXARROW,
	[7364] = CONST_ANI_SNIPERARROW,
	[7838] = CONST_ANI_FLASHARROW,
	[7840] = CONST_ANI_FLAMMINGARROW,
	[7839] = CONST_ANI_SHIVERARROW,
	[7850] = CONST_ANI_EARTHARROW,
	[15648] = CONST_ANI_TARSALARROW,
	[18437] = CONST_ANI_ENVENOMEDARROW,
	[18304] = CONST_ANI_CRYSTALLINEARROW,

	[2111] = CONST_ANI_SNOWBALL,
	[2389] = CONST_ANI_SPEAR,
	[3965] = CONST_ANI_HUNTINGSPEAR,
	[7367] = CONST_ANI_ENCHANTEDSPEAR,
	[7378] = CONST_ANI_ROYALSPEAR,
	[2399] = CONST_ANI_THROWINGSTAR,
	[7368] = CONST_ANI_REDSTAR,
	[7366] = CONST_ANI_GREENSTAR,
	[2410] = CONST_ANI_THROWINGKNIFE
}

-- Used for shuffling targets, so random ones are chosen
function shuffle(t)
	local rand = math.random 
	assert(t, "table.shuffle() expected a table, got nil")
	local iterations = #t
	local j
	for i = iterations, 2, -1 do
		j = rand(i)
		t[i], t[j] = t[j], t[i]
	end
end

-- Check player slots and get resistence information	
function getResistences(resistanceitemdesc, resistances, attackerisplayer)
	for k,v in pairs(resistances) do
		-- Get custom roll resistances
		if string.match(resistanceitemdesc, "%[" .. v.String .. " Resistance") then
			v.Custom = v.Custom + (string.match(resistanceitemdesc, '%[' .. v.String .. ' Resistance: %+(%d+)%%%]') or 0)
		end
		-- If attacker is player roll native rolls so custom elemental rolls are reduced by that as-well
		if attackerisplayer then
			-- Get natural item resistances, can't do it through native TFS functions properly (so it's pulled from description)
			local nativeResist = v.String:lower()
			if string.match(resistanceitemdesc, nativeResist .. " ([+-]%d+)%%") then
				local nativePercent = string.match(resistanceitemdesc, nativeResist .. " ([+-]%d+)%%")
				v.Native = v.Native + nativePercent
			end
		end
	end
end

-- Paralyze condition for ice weapons
local paralyze = Condition(CONDITION_PARALYZE)
paralyze:setParameter(CONDITION_PARAM_TICKS, 3000)
paralyze:setParameter(CONDITION_PARAM_SPEED, -150)
								
-- Stun animation loop
function stunanimation(stunnedcreature, stunnedpos, counter)
	if counter ~= 0 and Creature(stunnedcreature) then
		stunnedpos:sendMagicEffect(CONST_ME_STUN)
		counter = counter - 1
		stunnedcreature = Creature(stunnedcreature).uid
		addEvent(stunanimation, 500, stunnedcreature, stunnedpos, counter)
	end
end

-- Stun condition
function stunTarget(itemdesc, stunduration)
	if not Creature(stuncreature) then
		return false
	end
	
	-- Convert back to userdata
	local stuncreature = Creature(stuncreature)
	if stuncreature:isPlayer() then
		stunduration = stunduration / 2
	end
	-- Stun
	--local stun = Condition(CONDITION_STUN)
	--stun:setParameter(CONDITION_PARAM_TICKS, stunduration)
	
	-- Mute
	local mute = Condition(CONDITION_MUTED)
	mute:setParameter(CONDITION_PARAM_TICKS, stunduration)
	
	-- Apply conditions
	--stuncreature:addCondition(stun)
	stuncreature:addCondition(mute)
	
	-- Run animation
	addEvent(stunAnimation, 0, stuncreature.uid, stuncreature:getPosition(), (stunduration / 1000) * 2)
end

-- Input damage, it's type and creature immunites/resistances - output reduced damage
function filterResistance(damage, damageType, immunity, resistance)
	-- adjust by monster resistance
	if resistance[damageType] then
		local resistancePercent = (100 - resistance[damageType])
		damage = (resistancePercent / 100) * damage
	end
	-- set it to 0 if monster is immune
	if bit.band(immunity, damageType) == damageType then
		damage = 0
	end
	return math.floor(damage)
end

-- Roll elemental damage
function elementalDmg(weaponDescription, elementType, extraAnimation, creature, resistances, primaryDamage, primaryType, secondaryDamage, secondaryType, elementalroll)
	
	-- Get damage from item
	local dmgmin, dmgmax = string.match(weaponDescription, '%[Enhanced ' .. resistances[elementType].String .. ' Damage: (%d+)%-(%d+)%]')
	local eleDmg = (math.random(dmgmin, dmgmax))

	-- Damage reduction
	if creature:isMonster() then -- If monster is resistant to elementType, adjust the added elemental damage
		local resistance = creature:getType():getElementList()
		local immunity = creature:getType():getCombatImmunities()
		eleDmg = filterResistance(eleDmg, elementType, immunity, resistance)
	elseif creature:isPlayer() then -- PVP
		-- If player, halve the damage
		eleDmg = eleDmg / 2
		-- If about to add elemental damage, check custom and vanilla resistence and adjust
		if (resistances[elementType].Native + resistances[elementType].Custom ~= 0) then
			local resistancePercent = (100 - (resistances[elementType].Custom + resistances[elementType].Native))
			eleDmg = (resistancePercent / 100) * eleDmg
		end
	end
	
	-- Apply damage
	if eleDmg ~= 0 then -- only do this if damage is actually applied
		-- Animation
		if extraAnimation == true then
			local pos = creature:getPosition()
			local EFFECT_TYPE = 0
			if elementType == COMBAT_FIREDAMAGE then
				EFFECT_TYPE = CONST_ME_FIREATTACK
			elseif elementType == COMBAT_ENERGYDAMAGE then
				EFFECT_TYPE = CONST_ME_ENERGYAREA
			end
			pos:sendMagicEffect(EFFECT_TYPE)
		end
		
		-- Previous elemental damage
		if elementalroll then
			if primaryType == elementType then -- 'secondary' damage is the same, just add to it
				primaryDamage = primaryDamage + eleDmg
			else -- add to 'main' damage and overwrite its type
				if primaryType == 0 then -- is this  wand?
					if secondaryType == elementType then -- is the wand damage the same type?
						secondaryDamage = secondaryDamage + eleDmg
					else -- no it's not, make use of empty primaryValues
						primaryDamage = eleDmg
						primaryType = elementType
					end
				else -- not a wand, add eleDmg and change from physical to elementType
					secondaryDamage = secondaryDamage + eleDmg
					secondaryType = elementType
				end
			end
		else -- This is the first elemental roll on this weapon
			local originalDamage = primaryDamage
			local originalType = primaryType
			
			-- Swap primary & secondary types
			primaryDamage = secondaryDamage + eleDmg
			primaryType = elementType
			secondaryDamage = originalDamage
			secondaryType = originalType
			
			-- damageTypes have been swapped, flag it
			elementalroll = true
		end
	end
	return primaryDamage, primaryType, secondaryDamage, secondaryType, elementalroll
end
											
function statChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)

	-- Defaults
	local spellmodifier = 0 -- Spell Damage
	local manaleech = 0
	local lifeleech = 0
	local stunDuration = 2000
	local sourceDamage = primaryDamage -- Used for multi-shot when secondary targets are immune
	local elementalroll = false -- Flag for if weapon has an element on it
	local doubleroll = false -- Flag for if the weapon has two elements on it
	--local dawnbreaker = false
	--local rainbowshield = false
	
	-- What elements should be checked when calculating resistances
	local resistances = {
		[COMBAT_PHYSICALDAMAGE] = {Native = 0, Custom = 0, String = "Physical"},
		[COMBAT_FIREDAMAGE] = {Native = 0, Custom = 0, String = "Fire"},
		[COMBAT_ICEDAMAGE] = {Native = 0, Custom = 0, String = "Ice"},
		[COMBAT_ENERGYDAMAGE] = {Native = 0, Custom = 0, String = "Energy"},
		[COMBAT_EARTHDAMAGE] = {Native = 0, Custom = 0, String = "Earth"},
		[COMBAT_DEATHDAMAGE] = {Native = 0, Custom = 0, String = "Death"}
	}
	
	-- Check resistences if victim is a player before applying extra elemental damage below
	if creature:isPlayer() then
		for i = 1,#checkallslots do
			if creature:getSlotItem(checkallslots[i]) ~= nil then
				local resistanceitem = creature:getSlotItem(checkallslots[i])
				local resistanceitemdesc = resistanceitem:getDescription()
				getResistences(resistanceitemdesc, resistances, true)
				--[[
				-- if rainbow shield
				if resistanceitem.itemid == 8905 then
					if checkallslots[i] == CONST_SLOT_LEFT or checkallslots[i] == CONST_SLOT_RIGHT then -- Only if equipped
						rainbowshield = resistanceitem
					end
				end
				--]]
			end
		end
		
		 -- Reduce main damage on custom resistance rolls before applying extra elemental damage below
		if primaryType ~= 0 then
			if resistances[primaryType] then
				if resistances[primaryType].Custom ~= 0 then
					local resistancePercent = (100 - resistances[primaryType].Custom)
					primaryDamage = (resistancePercent / 100) * primaryDamage
				end
			end
		end
		if secondaryType ~= 0 then
			if resistances[secondaryType] then
				if resistances[secondaryType].Custom ~= 0 then
					local resistancePercent = (100 - resistances[secondaryType].Custom)
					secondaryDamage = (resistancePercent / 100) * secondaryDamage
				end
			end
		end
	end

	if attacker then -- Ignore HP changes from healing fountains, terrain elemental fields (neutral sources basically)
		if attacker:isPlayer() then
			if origin == ORIGIN_RANGED or origin == ORIGIN_MELEE then -- Skip if player is using spells
				-- Natural elemental damage (wand, fire sword, flaming arrow etc) swap Types around for better visuals and set flags
				if secondaryType ~= 0 or primaryType ~= COMBAT_PHYSICALDAMAGE then
					-- Cache original physical damage
					local originalDamage = primaryDamage
					local originalType = primaryType
					
					-- Swap primary & secondary
					primaryDamage = secondaryDamage
					primaryType = secondaryType
					secondaryDamage = originalDamage
					secondaryType = originalType
					
					-- damageTypes have been swapped, flag it
					elementalroll = true
				end
				
				-- Check weapons for crit/elemental damage
				for i = 1,#checkweaponslots do -- Roll for each slot
					if attacker:getSlotItem(checkweaponslots[i]) ~= nil then
						local slotitem = attacker:getSlotItem(checkweaponslots[i])
						local slotitemdesc = slotitem:getDescription()
				
						--[[
						-- If dawnbreaker
						if slotitem.itemid == 7744 or slotitem.itemid == 7751 or slotitem.itemid == 7756 then
							dawnbreaker = slotitem
						end
						--]]
						
						--[[ if wand or rod reduce damage on custom elemental resistences
						if primaryType ~= 0 and ItemType(slotitem.itemid):getWeaponType() == WEAPON_WAND then
							if resistances[primaryType] and resistances[primaryType].Custom ~= 0 then
								local resistancePercent = (100 - resistances[primaryType].Custom)
								primaryDamage = (resistancePercent / 100) * primaryDamage
							end
						end
						--]]
						
						-- Check elemental damage
						if slotitemdesc:find "%[Enhanced Fire Damage" then
							primaryDamage, primaryType, secondaryDamage, secondaryType, elementalroll = elementalDmg(slotitemdesc, COMBAT_FIREDAMAGE, true, creature, resistances, primaryDamage, primaryType, secondaryDamage, secondaryType, elementalroll)
							if math.random(1,3) == 3 then -- 33% chance to apply burn
								local burnDamage = 20
								if creature:isPlayer() then
									if resistances[COMBAT_FIREDAMAGE].Custom ~= 0 or resistances[COMBAT_FIREDAMAGE].Native ~= 0 then
										local resistancePercent = (100 - (resistances[COMBAT_FIREDAMAGE].Custom + resistances[COMBAT_FIREDAMAGE].Native))
										burnDamage = (resistancePercent / 100) * burnDamage
										if burnDamage ~= 0 then
											attacker:addDamageCondition(creature, CONDITION_FIRE, DAMAGELIST_CONSTANT_PERIOD, burnDamage, 2, 10)
										end
									end
								elseif creature:isMonster() then
									if bit.band(creature:getType():getCombatImmunities(), COMBAT_FIREDAMAGE) ~= COMBAT_FIREDAMAGE then -- Monster isn't immune to fire
										attacker:addDamageCondition(creature, CONDITION_FIRE, DAMAGELIST_CONSTANT_PERIOD, 20, 2, 10)
									end
								end
							end
						end
						if slotitemdesc:find "%[Enhanced Ice Damage" then
							primaryDamage, primaryType, secondaryDamage, secondaryType, elementalroll = elementalDmg(slotitemdesc, COMBAT_ICEDAMAGE, false, creature, resistances, primaryDamage, primaryType, secondaryDamage, secondaryType, elementalroll)
							if math.random(1,5) == 5 then -- 20% to paralyze/slow
								creature:addCondition(paralyze)
							end
						end
						if slotitemdesc:find "%[Enhanced Energy Damage" then
							primaryDamage, primaryType, secondaryDamage, secondaryType, elementalroll = elementalDmg(slotitemdesc, COMBAT_ENERGYDAMAGE, true, creature, resistances, primaryDamage, primaryType, secondaryDamage, secondaryType, elementalroll)
						end
						
						-- Stun Chance
						if slotitemdesc:find "%[Stun Chance" then
							local stunchance = tonumber(string.match(slotitemdesc, '%[Stun Chance: %+(%d+)%%%]')) or 0
							local rollchance = math.random(1,100)
							if stunchance >= rollchance and attacker ~= creature then
								stunTarget(creature.uid, stunDuration)
							end
						end
							
						-- Multi-shot
						if slotitemdesc:find "%[Multi Shot" then
							if attacker:getSlotItem(CONST_SLOT_AMMO) ~= nil then -- Does player have ammo
								local multishot = tonumber(string.match(slotitemdesc, "%[Multi Shot: %+(%d+)%]"))
								local ammoSlot = attacker:getSlotItem(CONST_SLOT_AMMO).itemid
								local validammo = false
								for k,v in pairs(animation) do
									if k == ammoSlot then
										validammo = true -- Ammo they're using exists in animation table
										break
									end
								end
								if validammo then -- (!) You may need to add more CONST_ANI_XXXX animations to the animation table
									local targetpos = creature:getPosition()
									local targets = getSpectators(targetpos, 2, 2) -- get possible multi-shot targets
									local victims = {}
									if targets ~= nil then
										-- do the shuffle
										shuffle(targets)
										for i = 1,#targets do
											local target = Creature(targets[i])
											if target:isMonster() then -- only target monsters
												if isSightClear(attacker:getPosition(), target:getPosition()) then -- that are in sight
													if target:getPosition() ~= targetpos then -- ignore the original target
														local victimcount = #victims or 0
														if victimcount < multishot then
															table.insert(victims, target) -- collate valid targets into victims table
														else
															break -- exit the for loop, have enough targets
														end
													end
												end
											end
										end
									end
									-- this a rats nest due to swapping primaryType/secondaryType above for visuals
									if victims ~= nil then
										for i = 1,#victims do
											-- defaults
											local damage = 0
											local elementalDamage = 0
											local backupDamage = 0
											local mainType = primaryType
											local altType = secondaryType
											local resistance = victims[i]:getType():getElementList()
											local immunity = victims[i]:getType():getCombatImmunities()
											local distanceEffect = animation[ammoSlot]
											-- animation
											attacker:getPosition():sendDistanceEffect(victims[i]:getPosition(), animation[ammoSlot])
											-- damage
											if elementalroll then -- check if types are swapped
												mainType = secondaryType
												altType = primaryType
												damage = filterResistance(secondaryDamage, secondaryType, immunity, resistance)
												elementalDamage = filterResistance(primaryDamage, primaryType, immunity, resistance)
											else -- types haven't been swapped
												damage = filterResistance(primaryDamage, primaryType, immunity, resistance)
												elementalDamage = filterResistance(secondaryDamage, secondaryType, immunity, resistance)
											end
											-- apply secondary if damage
											if elementalDamage ~= 0 then
												doTargetCombatHealth(attacker, victims[i], altType, math.ceil((80 / 100) * elementalDamage), elementalDamage)
											end
											-- apply main if damage
											if damage ~= 0 then
												doTargetCombatHealth(attacker, victims[i], mainType, math.ceil((80 / 100) * damage), damage)
											else -- no damage, monster is immune fallback to original physical damage
												backupDamage = filterResistance(sourceDamage, COMBAT_PHYSICALDAMAGE, immunity, resistance)
												if backupDamage ~= 0 then -- try rolling back to base physical damage
													doTargetCombatHealth(attacker, victims[i], COMBAT_PHYSICALDAMAGE, math.ceil((80 / 100) * sourceDamage), sourceDamage)
												else -- monster immune to physical damage too or w/e
													victims[i]:getPosition():sendMagicEffect(CONST_ME_BLOCKHIT)
												end
											end
										end
									end
								end
							end
						end
					end
				end
			elseif origin == ORIGIN_SPELL then	
				-- Reduce spell damage based on custom resistances
				if resistances[primaryType] then
					if resistances[primaryType].Custom ~= 0 then
						local resistancePercent = (100 - resistances[primaryType].Custom)
						primaryDamage = (resistancePercent / 100) * primaryDamage
					end
				end
				
				-- Check slots that can roll the following attributes
				for i = 1,#checkweaponslots do
					if attacker:getSlotItem(checkweaponslots[i]) ~= nil then
						local slotitem = attacker:getSlotItem(checkweaponslots[i])
						local slotitemdesc = slotitem:getDescription()
						
						-- Check crit chance of slots and add together
						if slotitemdesc:find "%[Spell Damage" then
							local spellDamage = string.match(slotitemdesc, "Spell Damage: [+-](%d+)%%")
							spellmodifier = spellmodifier + spellDamage
						end
						
						-- Stun Chance
						if slotitemdesc:find "%[Stun Chance" then
							local stunchance = tonumber(string.match(slotitemdesc, '%[Stun Chance: %+(%d+)%%%]')) or 0
							local rollchance = math.random(1,100)
							if stunchance >= rollchance and attacker ~= creature then
								stunTarget(creature.uid, stunDuration)
							end
						end
					end
				end
				
				-- if using a +%spelldamage wand, adjust damage
				if spellmodifier > 0 then
					local extraDamage = (spellmodifier / 100) * primaryDamage
					primaryDamage = primaryDamage + extraDamage
				end
			end
		end
	end
	
	--[[
	-- If rainbow shield
	if rainbowshield then
		local rainbowTransformation = 0
		local rainbowTransformations = {
			[COMBAT_FIREDAMAGE] = 8906,
			[COMBAT_ICEDAMAGE] = 8907,
			[COMBAT_ENERGYDAMAGE] = 8908,
			[COMBAT_EARTHDAMAGE] = 8909
		}
		
		-- Check primary first
		if primaryType ~= 0 then
			if rainbowTransformations[primaryType] then
				rainbowTransformation = rainbowTransformations[primaryType]
			end
		end
		
		-- Check secondary second (This prioritizes secondaryType which is what we want)
		if secondaryType ~= 0 then
			if rainbowTransformations[secondaryType] then
				rainbowTransformation = rainbowTransformations[secondaryType]
			end
		end
		-- Transform rainbow shield
		if rainbowTransformation ~= 0 then
			rainbowshield:transform(rainbowTransformation)
			rainbowshield:decay()
		end
	end
	--]]
	
	return primaryDamage, primaryType, secondaryDamage, secondaryType, origin
end

function onHealthChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)

	if primaryType ~= 128 then -- Ignore health potions
		primaryDamage, primaryType, secondaryDamage, secondaryType, origin = statChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin) -- This is for feeding both onHealthChange and onManaChange through the same damage/buff formula
	end
	
	return primaryDamage, primaryType, secondaryDamage, secondaryType, origin
end

function onManaChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)

	if primaryType ~= 64 then -- Ignore mana potions
		primaryDamage, primaryType, secondaryDamage, secondaryType, origin = statChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin) -- This is for feeding both onHealthChange and onManaChange through the same damage/buff formula
		
		-- Apply magic shield bonus, even if neutral source
		if creature:isPlayer() then
			local manashield = 0
			for i = 1,#checkweaponslots do
				if creature:getSlotItem(checkweaponslots[i]) ~= nil then
					local slotitem = creature:getSlotItem(checkweaponslots[i])
					local slotitemdesc = slotitem:getDescription()
					if slotitemdesc:find "%[Mana Shield" then
						manashield = manashield + tonumber(string.match(slotitemdesc, '%[Mana Shield: %+(%d+)%%%]'))
					end
				end
			end
			if manashield ~= 0 then
				local shieldPercent = (100 - manashield)
				primaryDamage = (shieldPercent / 100) * primaryDamage
				secondaryDamage = (shieldPercent / 100) * secondaryDamage
			end
		end
		
	end
	
	return primaryDamage, primaryType, secondaryDamage, secondaryType, origin
end