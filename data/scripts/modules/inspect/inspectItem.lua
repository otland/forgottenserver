function Player:sendItemInspection(item, descriptions, openCompendium)
	local response = NetworkMessage()
	response:addByte(0x76)
	response:addByte(0x00) -- responseType 0x00 = ok
	response:addByte(openCompendium and 0x01 or 0x00)
	response:addByte(0x01) --?
	
	if tonumber(item) then
		local itemType = ItemType(item)
		if not itemType then
			player:sendCancelMessage("Sorry, not possible.")
			return
		end
		
		response:addString(
			string.format("%s", table.concat({itemType:getArticle(), itemType:getName()}, " "))
		)
		response:addItemType(itemType)
	else
		response:addString(
			string.format("%s", item:getNameDescription(item:getSubType(), true))
		)
		response:addItem(item)	
	end

	-- imbuements count (5 max, 6th gets cut in view window)
	-- structure: u16 imbuement icon id
	response:addByte(0)

	-- fields count
	-- structure:
	-- string key
	-- string value
	if descriptions and #descriptions > 0 then
		response:addByte(#descriptions)
		
		-- structure:
		-- descriptions = {{"Name", "Test item"}, {"Weight", "3.10 oz"}}
		for i = 1, #descriptions do
			response:addString(descriptions[i][1])
			response:addString(descriptions[i][2])
		end
	else
		response:addByte(0)
	end
	response:sendToPlayer(self)
end

local showAtkWeaponTypes = {WEAPON_CLUB, WEAPON_SWORD, WEAPON_AXE, WEAPON_DISTANCE}
local showDefWeaponTypes = {WEAPON_CLUB, WEAPON_SWORD, WEAPON_AXE, WEAPON_DISTANCE, WEAPON_SHIELD}

function getItemDetails(item)
	local isVirtual = false
	local itemType
	
	if tonumber(item) then
		-- number in function argument
		-- pull data from itemType instead
		isVirtual = true
		itemType = ItemType(item)
		if not itemType then
			return {}
		end
		
		-- polymorphism for item attributes (atk, def, etc)
		item = itemType
	else
		itemType = item:getType()
	end
	
	local descriptions = {}
	
	-- container capacity
	local isContainer = itemType:isContainer()
	if isContainer then
		descriptions[#descriptions + 1] = {"Capacity", itemType:getCapacity()}
	end
	
	-- key
	if not isVirtual and itemType:isKey() then
		descriptions[#descriptions + 1] = {"Key", string.format("%0.4d", item:getActionId())}
	end

	-- description
	local desc = itemType:getDescription()
	if not isVirtual and item:hasAttribute(ITEM_ATTRIBUTE_DESCRIPTION) then
		desc = item:getAttribute(ITEM_ATTRIBUTE_DESCRIPTION)
	end
	
	if desc and #desc > 0 then
		descriptions[#descriptions + 1] = {"Description", desc}
	end
	
	-- read weapon type
	local weaponType = itemType:getWeaponType()
	local ammoType = itemType:getAmmoType()
	
	-- atk
	local showAtk = table.contains(showAtkWeaponTypes, weaponType)
	if showAtk then
		local atk = item:getAttack()
		if weaponType == WEAPON_DISTANCE and (ammoType == AMMO_ARROW or ammoType == AMMO_BOLT) then
			if atk ~= 0 then
				descriptions[#descriptions + 1] = {"Attack", atk}
			end
		else
			descriptions[#descriptions + 1] = {"Attack", atk}
		end
	end
	
	-- bonus element
	local elementDmg = itemType:getElementDamage()
	if elementDmg > 0 then
		descriptions[#descriptions][2] = string.format("%d +%d %s", descriptions[#descriptions][2], elementDmg, elementToStringMap[itemType:getElementType()])
	end
	
	-- def
	-- note: "defence" is actual spelling, it is a correct form in British English
	local showDef = table.contains(showDefWeaponTypes, weaponType)
	if showDef then
		local def = item:getDefense()
		if weaponType == WEAPON_DISTANCE then
			-- throwables
			if ammoType ~= AMMO_ARROW and ammoType ~= AMMO_BOLT then
				descriptions[#descriptions + 1] = {"Defence", def}
			end
		else
			descriptions[#descriptions + 1] = {"Defence", def}
		end
	end
	
	-- extra def
	local xD = item:getExtraDefense()
	if xD ~= 0 then
		descriptions[#descriptions][2] = string.format("%d +%d", descriptions[#descriptions][2], xD)
	end
	
	-- armor
	local arm = item:getArmor()
	if itemType:isHelmet() or itemType:isArmor() or itemType:isLegs() or itemType:isBoots() then
		descriptions[#descriptions + 1] = {"Armor", arm}
	end
	
	-- attack speed
	local attackSpeed = item:getAttackSpeed()
	if attackSpeed ~= 0 then
		descriptions[#descriptions + 1] = {"Attack Speed", attackSpeed}
	end

	local abilities = itemType:getAbilities()
	
	-- protection
	local protections = {}
	for element, value in pairs(abilities.absorbPercent) do
		if value ~= 0 then
			protections[#protections + 1] = string.format("%s %s%d%%", elementToStringMap[2^(element-1)], (value >= 0 and "+" or ""), value)
		end
	end
	
	if #protections > 0 then
		descriptions[#descriptions + 1] = {"Protection", table.concat(protections, ", ")}
	end
	protections = nil
	
	-- skill boost
	local skillBoosts = {}
	if abilities.manaGain > 0 or abilities.healthGain > 0 or abilities.regeneration then
		skillBoosts[#skillBoosts + 1] = "faster regeneration"
	end
	
	if abilities.invisible then
		skillBoosts[#skillBoosts + 1] = "invisibility"
	end
	
	if abilities.manaShield then
		skillBoosts[#skillBoosts + 1] = "mana shield"
	end
	
	-- stats (hp/mp/soul/ml)
	for stat, value in pairs(abilities.stats) do
		if value ~= 0 then
			skillBoosts[#skillBoosts + 1] = string.format("%s %s%d", statToStringMap[stat-1], (value >= 0 and "+" or ""), value)
		end
	end
	
	-- stats but in %
	for stat, value in pairs(abilities.statsPercent) do
		if value ~= 0 then
			skillBoosts[#skillBoosts + 1] = string.format("%s %s%d%%", statToStringMap[stat-1], (value >= 0 and "+" or ""), value)
		end
	end
		
	if abilities.speed ~= 0 then
		skillBoosts[#skillBoosts + 1] = string.format("speed %s%d", (abilities.speed >= 0 and "+" or ""), abilities.speed)
	end
	
	for skill, value in pairs(abilities.skills) do
		if value ~= 0 then
			skillBoosts[#skillBoosts + 1] = string.format("%s %s%d", skillToStringMap[skill-1], (value >= 0 and "+" or ""), value)
		end
	end
	
	for skill, value in pairs(abilities.specialSkills) do
		if value ~= 0 then
			skillBoosts[#skillBoosts + 1] = string.format("%s %s%d", specialSkillToStringMap[skill-1], (value >= 0 and "+" or ""), value)
		end
	end
	
	if #skillBoosts > 0 then
		descriptions[#descriptions + 1] = {"Skill Boost", table.concat(skillBoosts, ", ")}
	end
	skillBoosts = nil
	
	-- imbuement slot n: empty (new line for each n)
	-- to do: implement together with imbuements system
	
	-- spell
	local spellName = itemType:getRuneSpellName()
	if spellName then
		descriptions[#descriptions + 1] = {"Rune", spellName}
	end
	
	-- charges
	if itemType:hasShowCharges() then
		if isVirtual then
			descriptions[#descriptions + 1] = {"Total Charges", itemType:getCharges()}
		else
			descriptions[#descriptions + 1] = {"Charges", string.format("%d/%d", item:getCharges(), itemType:getCharges())}
		end
	end
	
	-- expires
	if itemType:hasShowDuration() then
		if isVirtual then
			descriptions[#descriptions + 1] = {"Total Expire Time", (math.floor(itemType:getDuration()/1000) or "unknown")}
		else
			descriptions[#descriptions + 1] = {"Expires", Game.getCountdownString(math.floor(item:getDuration()/1000))}
		end
	end
	
	-- weight
	if itemType:isMovable() and itemType:isPickupable() then
		local itemWeight = item:getWeight()
		local typeWeight = itemType:getWeight()
		
		descriptions[#descriptions + 1] = {
			-- key
			(itemWeight == typeWeight and "Weight" or "Total Weight"),
			-- value
			string.format("%0.2f oz", itemWeight/100)
		}
	end
	
	-- level
	local minLevel = itemType:getMinReqLevel()
	if minLevel and minLevel > 0 then
		descriptions[#descriptions + 1] = {"Required Level", minLevel}
	end
	
	-- magic level
	local minMagicLevel = itemType:getMinReqMagicLevel()
	if minMagicLevel and minMagicLevel > 0 then
		descriptions[#descriptions + 1] = {"Required Magic Level", minMagicLevel}
	end
	
	-- vocation
	local vocations = itemType:getVocationString()
	if vocations and vocations:len() > 0 then
		descriptions[#descriptions + 1] = {"Professions", vocations}
	end
	
	-- weapon type
	local isTwoHanded = itemType:isTwoHanded()
	local weaponType = itemType:getWeaponType()
	local isWeapon = itemType:isWeapon()
	if isWeapon then
		local weaponString = "unknown"
		if weaponType == WEAPON_CLUB then
			weaponString = "blunt instrument"
		elseif weaponType == WEAPON_SWORD then
			weaponString = "stabbing weapon"
		elseif weaponType == WEAPON_AXE then
			weaponString = "cutting weapon"
		elseif weaponType == WEAPON_DISTANCE then
			weaponString = itemType:isBow() and "firearm" or "missile"
		elseif weaponType == WEAPON_WAND then
			weaponString = "wand/rod"
		end
		
		if isTwoHanded then
			weaponString = string.format("%s, two-handed", weaponString)
		end
		
		descriptions[#descriptions + 1] = {"Weapon Type", weaponString}
	end
	
	-- tradeable
	local tradeable = item:isStoreItem() and "no" or "yes"
	if isVirtual then
		descriptions[#descriptions + 1] = {"Tradeable In Market", tradeable}
	else
		descriptions[#descriptions + 1] = {"Tradeable", tradeable}
	end
	
	-- slot
	local bodyPosition
	if isTwoHanded then
		bodyPosition = "two-handed"
	elseif isContainer then
		bodyPosition = "container"
	elseif weaponType == WEAPON_SHIELD then -- or quiver
		bodyPosition = "shield hand"
	elseif isWeapon then
		bodyPosition = "weapon hand"
	elseif itemType:isHelmet() then
		bodyPosition = "head"
	elseif itemType:isNecklace() then
		bodyPosition = "neck"
	elseif itemType:isArmor() then
		bodyPosition = "body"
	elseif itemType:isLegs() then
		bodyPosition = "legs"
	elseif itemType:isBoots() then
		bodyPosition = "feet"
	elseif itemType:isRing() then
		bodyPosition = "finger"
	elseif itemType:isTrinket() then
		bodyPosition = "extra slot"
	end
	
	if bodyPosition then
		descriptions[#descriptions + 1] = {"Body Position", bodyPosition}		
	end

-- hit chance / range?
	return descriptions
end

local onInspectItem = function(self, item)
	local descriptions = getItemDetails(item)
	self:sendItemInspection(item, descriptions, false)
end

local onInspectTradeItem = function(self, tradePartner, item)
	local descriptions = getItemDetails(item)
	self:sendItemInspection(item, descriptions, false)
end

local onInspectNpcTradeItem = function(self, npc, itemId)
	local descriptions = getItemDetails(itemId)
	self:sendItemInspection(itemId, descriptions, false)
end

local onInspectCompendiumItem = function(self, itemId)
	local descriptions = getItemDetails(itemId)
	self:sendItemInspection(itemId, descriptions, true)
end

local callbacks = {
	["onInspectItem"] = onInspectItem,
	["onInspectTradeItem"] = onInspectTradeItem,
	["onInspectNpcTradeItem"] = onInspectNpcTradeItem,
	["onInspectCompendiumItem"] = onInspectCompendiumItem
}

for callName, callback in pairs(callbacks) do
	local ec = EventCallback
	ec[callName] = callback
	ec:register()
end