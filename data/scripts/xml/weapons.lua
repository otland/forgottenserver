-- If you don't intend to use weapons.xml, you can delete this file.

local function configureWeaponEvent(node)
	local id = tonumber(node:attribute("id"))
	if not id then
		io.write("[Error] Missing attribute id for weapon.\n")
		return nil
	end

	local itemType = ItemType(id)
	if not itemType then
		io.write("[Error] Invalid weapon id " .. id .. ".\n")
		return nil
	end

	local weaponType = itemType:getWeaponType()
	if not table.contains({WEAPON_SWORD, WEAPON_CLUB, WEAPON_AXE, WEAPON_DISTANCE, WEAPON_WAND, WEAPON_AMMO}, weaponType) then
		io.write("[Error] Item id " .. id .. " is not a weapon, skipping\n")
		return nil
	end

	local weapon = Weapon(weaponType)
	weapon:id(id)

	local level = tonumber(node:attribute("level"))
	if level ~= nil then
		weapon:level(level)
	end

	local magicLevel = tonumber(node:attribute("magLv")) or tonumber(node:attribute("maglevel"))
	if magicLevel ~= nil then
		weapon:magicLevel(magicLevel)
	end

	local mana = tonumber(node:attribute("mana"))
	if mana ~= nil then
		weapon:mana(mana)
	end

	local manaPercent = tonumber(node:attribute("manapercent"))
	if manaPercent ~= nil then
		weapon:manaPercent(manaPercent)
	end

	local health = tonumber(node:attribute("health"))
	if health ~= nil then
		weapon:health(health)
	end

	local healthPercent = tonumber(node:attribute("healthpercent"))
	if healthPercent ~= nil then
		weapon:healthPercent(healthPercent)
	end

	local soul = tonumber(node:attribute("soul"))
	if soul ~= nil then
		weapon:soul(soul)
	end

	local min = tonumber(node:attribute("min"))
	local max = tonumber(node:attribute("max"))
	if min ~= nil and max ~= nil then
		if weaponType == WEAPON_WAND then
			weapon:damage(min, max)
		else
			io.write("[Warning] Only wands can have min and max damage attributes, ignoring...\n")
		end
	end

	local prem = tobool(node:attribute("prem"))
	if prem ~= nil then
		weapon:premium(true)
	end

	local breakChance = tonumber(node:attribute("breakchance"))
	if breakChance ~= nil then
		weapon:breakChance(breakChance)
	end

	local action = node:attribute("action")
	if action ~= nil then
		weapon:action(action)
	end

	local enabled = tobool(node:attribute("enabled"))
	if enabled ~= nil then
		weapon:enabled(enabled)
	end

	local unproperly = tobool(node:attribute("unproperly"))
	if unproperly ~= nil then
		weapon:wieldUnproperly(unproperly)
	end

	local vocations = {}
	for vocationNode in node:children() do
		local name = vocationNode:attribute("name")
		if not name then
			io.write("[Error] Missing attribute name for vocation, check data/weapons/weapons.xml.\n")
			return nil
		end

		local showInDescription = tobool(vocationNode:attribute("showindescription") or "1")
		vocations[#vocations + 1] = {name = name, showInDescription = showInDescription}
	end

	for i, vocation in ipairs(vocations) do
		weapon:vocation(vocation.name, vocation.showInDescription, i == #vocations)
	end

	if script ~= nil then
		local scriptFile = "data/weapons/scripts/" .. script

		local env = {}
		for k, v in pairs(_G) do
			env[k] = v
		end

		local f = assert(loadfile(scriptFile, "bt", env))
		f()

		if not env.onUseWeapon then
			io.write("[Error] Can not load weapon script, check " .. scriptFile .. " for a missing onUseWeapon callback\n")
			return nil
		end
		weapon.onUseWeapon = env.onUseWeapon
	end

	return weapon
end

local function loadXMLWeapons()
	local doc = XMLDocument("data/weapons/weapons.xml")
	if not doc then
		io.write("[Warning - Scripts::XML::loadXMLWeapons] Could not load weapons.xml.\n")
		return
	end

	local weapons = doc:child("weapons")

    io.write(">> Loading legacy XML weapons from data/weapons/weapons.xml...\n")

	local loaded, start = 0, os.mtime()
	for node in weapons:children() do
		local weapon = configureWeaponEvent(node)
		if weapon then
			weapon:register()
			loaded = loaded + 1
		end
	end

    io.write(">> Loaded " .. loaded .. " weapons in " .. os.mtime() - start .. "ms.\n")
end

loadXMLWeapons()
