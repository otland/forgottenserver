do
	local map = {
		[COMBAT_PHYSICALDAMAGE] = ITEM_ABILITY_ABSORBPHYSICAL,
		[COMBAT_ENERGYDAMAGE] = ITEM_ABILITY_ABSORBENERGY,
		[COMBAT_EARTHDAMAGE] = ITEM_ABILITY_ABSORBEARTH,
		[COMBAT_FIREDAMAGE] = ITEM_ABILITY_ABSORBFIRE,
		[COMBAT_DROWNDAMAGE] = ITEM_ABILITY_ABSORBWATER,
		[COMBAT_ICEDAMAGE] = ITEM_ABILITY_ABSORBICE,
		[COMBAT_HOLYDAMAGE] = ITEM_ABILITY_ABSORBHOLY,
		[COMBAT_DEATHDAMAGE] = ITEM_ABILITY_ABSORBDEATH,
	}

	function combatToAbilityType(combat)
		return map[combat]
	end
end

local function damageFunc(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	local player = creature:getPlayer()
	local abilities = player:getAbilities()
	local primaryKey = combatToAbilityType(primaryType)
	local secondaryKey = combatToAbilityType(secondaryType)
	local dodge = player:getAbilityValue(ITEM_ABILITY_DODGE)
	if math.random(100) <= dodge then
		player:getPosition():sendMagicEffect(CONST_ME_ASSASSIN)
		return 0, primaryType, 0, secondaryType
	end
	for id, ability in ipairs(abilities) do
		local changed = false
		if primaryKey and ability.key == primaryKey then
			primaryDamage = primaryDamage - (primaryDamage * (ability.value / 100))
			changed = true
		end
		if secondaryKey and ability.key == secondaryKey then
			secondaryDamage = secondaryDamage - (secondaryDamage * (ability.value / 100))
			changed = true
		end
		if changed then
			break
		end
	end
	return primaryDamage, primaryType, secondaryDamage, secondaryType
end

local healthChange = CreatureEvent('IA_healthChange')
healthChange:type('healthchange')

function healthChange.onHealthChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	return damageFunc(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
end

healthChange:register()

local manaChange = CreatureEvent('IA_manaChange')
manaChange:type('manachange')

function manaChange.onManaChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	return damageFunc(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
end

manaChange:register()

----------------------------------------------------------------------------------------------------------------------------
--                                                   REGISTRATION
----------------------------------------------------------------------------------------------------------------------------

local login = CreatureEvent('IA_login')
login:type('login')

function login.onLogin(player)
	for i = 1, 10 do
		local item = player:getSlotItem(i)
		if item then
			ItemAbilities.internalInventoryUpdate(player, item, i, true)
		end
	end
	player:registerEvent('IA_logout')
	player:registerEvent('IA_death')
	player:registerEvent('IA_healthChange')
	player:registerEvent('IA_manaChange')
	return true
end

login:register()

local logout = CreatureEvent('IA_logout')
logout:type('logout')

function logout.onLogout(player)
	for i = 1, 10 do
		local item = player:getSlotItem(i)
		if item then
			ItemAbilities.internalInventoryUpdate(player, item, i, false)
		end
	end
	return true
end

logout:register()

local death = CreatureEvent('IA_death')
death:type('death')

function death.onDeath(player)
	for i = 1, 10 do
		local item = player:getSlotItem(i)
		if item then
			ItemAbilities.internalInventoryUpdate(player, item, i, false)
		end
	end
	return true
end

death:register()