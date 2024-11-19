-- If you don't intend to use actions.xml, you can delete this file.

local function parseIds(itemid)
	local ids = {}
	for _, id in ipairs(itemid:split(";")) do
		ids[#ids + 1] = tonumber(id)
	end
	return ids
end

local function getIds(singleIdKey, fromIdKey, toIdKey)
	return function(node)
		local itemid = node:attribute(singleIdKey)
		if itemid then
			return parseIds(itemid)
		end

		local fromid = tonumber(node:attribute(fromIdKey))
		if not fromid then
			return {}
		end

		local toid = tonumber(node:attribute(toIdKey))
		if not toid then
			io.write("[Error] Missing attribute " .. toIdKey .. ", check data/actions/actions.xml.\n")
			return {}
		end

		local ids = {}
		for id = fromid, toid do
			ids[#ids + 1] = id
		end
		return ids
	end
end

local function configureAttribute(attributeName, actionMethod)
	local value = tobool(node:attribute(attributeName))
	if value ~= nil then
		actionMethod(value)
	end
end

local function configureActionEvent(node)
	local action = Action()
	local itemIds = getItemIds(node)

	if #itemIds == 0 then
		local uniqueIds = getUniqueIds(node)
		if #uniqueIds == 0 then
			local actionIds = getActionIds(node)
			if #actionIds == 0 then
				io.write("[Error] Missing attribute itemid or uniqueid or actionid, check 'data/actions/actions.xml'.\n")
				return nil
			end
			action:aid(unpack(actionIds))
		else
			action:uid(unpack(uniqueIds))
		end
	else
		action:id(unpack(itemIds))
	end

	configureAttribute("allowfaruse", action.allowFarUse)
	configureAttribute("blockwalls", action.blockWalls)
	configureAttribute("checkfloor", action.checkFloor)

	local function_ = node:attribute("function")
	local script = node:attribute("script")
	if not function_ and not script then
		io.write("[Warning] Missing function or script attribute for action in '" .. node:attribute("name") .. "' (check 'data/actions/actions.xml').\n")
		return nil
	end

	if script then
		local scriptFile = "data/actions/scripts/" .. script
		dofile(scriptFile)
		if not onUse then
			io.write("[Error] Can not load action script, check '" .. scriptFile .. "' for a missing onUse callback\n")
			return nil
		end

		action:onUse(onUse)
		onUse = nil
	end
	return action
end

local event = GlobalEvent("load actions.xml")

function event.onStartup()
	local doc = XMLDocument("data/actions/actions.xml")
	if not doc then
		io.write("[Warning - GlobalEvent::onStartup] Could not load data/actions/actions.xml.\n")
		return true
	end

	local actions = doc:child("actions")

	io.write(">> Loading legacy XML actions from data/actions/actions.xml...\n")

	local loaded, start = 0, os.mtime()
	for node in actions:children() do
		local action = configureActionEvent(node)
		if action then
			action:register()
			loaded = loaded + 1
		end
	end

	local elapsedTime = os.mtime() - start
	io.write(">> Loaded " .. loaded .. " actions in " .. elapsedTime .. "ms.\n")
end

event:register()
