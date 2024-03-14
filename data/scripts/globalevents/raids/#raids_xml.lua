-- loads legacy raids from data/raids/raids.xml
local messageTypes = {
	["warning"] = MESSAGE_STATUS_WARNING,
	["event"] = MESSAGE_EVENT_ADVANCE,
	["default"] = MESSAGE_EVENT_DEFAULT,
	["description"] = MESSAGE_INFO_DESCR,
	["smallstatus"] = MESSAGE_STATUS_SMALL,
	["blueconsole"] = MESSAGE_STATUS_CONSOLE_BLUE,
	["redconsole"] = MESSAGE_STATUS_CONSOLE_RED,
}
local defaultMessageType = "event"

local function parseAnnounce(node, filename)
	local message = node:attribute("message")
	if not message then
		io.write("[Error] Missing message attribute, check data/raids/" .. filename .. "\n")
	end

	local type = node:attribute("type")
	if not type then
		io.write("[Notice] Missing type for announce event in " .. filename .. ". Using default: " .. defaultMessageType .. ".\n")
		type = defaultMessageType
	end

	local messageType = messageTypes[type:lower()]
	if not messageType then
		io.write("[Notice] Unknown type " .. type .. " for announce event in " .. filename .. ". Using default: " .. messageTypes[defaultMessageType] .. ".\n")
		messageType = messageTypes[defaultMessageType]
	end

	return function()
		Game.broadcastMessage(message, messageType)
	end
end

local function parseAreaSpawn(node, filename)
	local fromx, fromy, fromz, tox, toy, toz

	local radius = tonumber(node:attribute("radius"))
	if radius then
		local centerx, centery, centerz = tonumber(node:attribute("centerx")), tonumber(node:attribute("centery")), tonumber(node:attribute("centerz"))
		if not centerx or not centery or not centerz then
			io.write("[Error] Missing one of: centerx, centery, centerz, check data/raids/" .. filename .. "\n")
		end

		fromx, fromy, fromz = centerx - radius, centery - radius, z
		tox, toy, toz = centerx + radius, centery + radius, z
	else
		fromx, fromy, fromz = tonumber(node:attribute("fromx")), tonumber(node:attribute("fromy")), tonumber(node:attribute("fromz"))
		if not fromx or not fromy or not fromz then
			io.write("[Error] Missing one of: fromx, fromy, fromz, check data/raids/" .. filename .. "\n")
		end

		tox, toy, toz = tonumber(node:attribute("tox")), tonumber(node:attribute("toy")), tonumber(node:attribute("toz"))
		if not tox or not toy or not toz then
			io.write("[Error] Missing one of: tox, toy, toz, check data/raids/" .. filename .. "\n")
		end
	end

	local spawns = {}
	for spawnNode in node:children() do
		local name = spawnNode:attribute("name")
		if not name then
			io.write("[Error] Missing attribute name, check data/raids/" .. filename .. "\n")
			return nil
		end

		local minAmount, maxAmount = tonumber(spawnNode:attribute("minamount")), tonumber(spawnNode:attribute("maxamount"))
		if not minAmount and not maxAmount then
			local amount = tonumber(spawnNode:attribute("amount"))
			if not amount then
				io.write("[Error] Missing attributes minamount/maxamount or amount, check data/raids/" .. filename .. "\n")
			end

			minAmount, maxAmount = amount, amount
		elseif not minAmount then
			io.write("[Warning] Missing attribute minamount in " .. filename .. ", using maxamount as default.\n")
			minAmount = maxAmount
		elseif not maxAmount then
			io.write("[Warning] Missing attribute maxamount in " .. filename .. ", using minamount as default.\n")
			maxAmount = minAmount
		end

		spawns[#spawns] = { monsterName = name, minAmount = minAmount, maxAmount = maxAmount }
	end

	return function()
		for _, spawn in ipairs(spawns) do
			for _ = 1, math.random(spawn.minAmount, spawn.maxAmount) do
				local x, y = math.random(fromx, tox), math.random(fromy, toy)
				Game.createMonster(spawns.name, Position(x, y, z))
			end
		end
	end
end

local function parseScript(node)
	local script = node:attribute("script")
	if not script then
		io.write("[Error] Missing attribute script, check data/raids/" .. filename .. "\n")
		return nil
	end

	local scriptFile = "data/raids/scripts/" .. script
	dofile(script)
	if not onRaid then
		io.write("[Error] Can not load raid script, check " .. scriptFile .. " for a missing onRaid callback\n")
		return nil
	end

	local callback = onRaid

	-- let it be garbage collected
	onRaid = nil

	return callback
end

local function parseSingleSpawn(node, filename)
	local name = node:attribute("name")
	if not name then
		io.write("[Error] Missing attribute name, check data/raids/" .. filename .. "\n")
		return nil
	end

	local x, y, z = tonumber(node:attribute("x")), tonumber(node:attribute("y")), tonumber(node:attribute("z"))
	if not x or not y or not z then
		io.write("[Error] Missing one of: x, y, z, check data/raids/" .. filename .. "\n")
	end

	return function()
		Game.createMonster(spawns.name, Position(x, y, z))
	end
end

local eventParsers = {
	["announce"] = parseAnnounce,
	["areaspawn"] = parseAreaSpawn,
	["script"] = parseScript,
	["singlespawn"] = parseSingleSpawn,
}

local function parseRaid(filename)
	local doc = XMLDocument("data/raids/" .. filename)
	local eventNodes = doc:child("raid")

	local events = {}
	for eventNode in eventNodes:children() do
		local parse = eventParsers[eventNode:name()]
		if not parse then
			io.write("[Error] Unknown event type: " .. eventNode:name() .. ".\n")
			return nil
		end

		local delay = tonumber(eventNode:attribute("delay"))
		if not delay then
			io.write("[Error] Missing attribute delay, check data/raids/" .. filename .. "\n")
			return nil
		end

		local callback = parse(eventNode, filename)
		if not callback then
			return nil
		end

		events[#events] = { delay = delay, callback = callback }
	end

	return events
end

local function configureRaidEvent(node)
	local name = node:attribute("name")
	if not name then
		io.write("[Error] Missing attribute name for raid.\n")
		return nil
	end

	local filename = node:attribute("file")
	if not filename then
		io.write('[Warning] file attribute missing for raid "' .. name .. '". Using default: "' .. name .. '.xml"\n')
		filename = name .. ".xml"
	end

	-- using filename instead of name because name can be duplicate, but filename cannot
	local raid = Raid("data/raids/" .. filename)

	local interval = tonumber(node:attribute("interval2"))
	if not interval or interval == 0 then
		io.write("[Error] interval2 attribute missing or zero (would divide by 0), check raid " .. name .. " in data/raids/raids.xml\n")
		return nil
	end
	raid.interval = interval

	local margin = tonumber(node:attribute("margin"))
	if margin and margin > 0 then
		raid.margin = margin * 60 * 1000
	else
		io.write("[Warning] margin attribute missing for raid " .. name .. ". Using default: 0\n")
	end

	local repeats = tobool(node:attribute("repeat"))
	if repeats then
		raid.repeats = repeats
	end

	local events = parseRaid(filename)
	if not events then
		return nil
	end

	for _, event in ipairs(events) do
		raid:addEvent(event.delay, event.callback)
	end

	return raid
end

local event = GlobalEvent("load raids.xml")

function event.onStartup()
	local doc = XMLDocument("data/raids/raids.xml")
	if not doc then
		return true
	end

	local raids = doc:child("raids")

	io.write(">> Loading legacy XML raids from data/raids/raids.xml...\n")
	local loaded, start = 0, os.mtime()
	for node in raids:children() do
		local enabled = node:attribute("enabled")
		if enabled == nil or tobool(enabled) then
			local raid = configureRaidEvent(node)
			if raid then
				raid:register()
				loaded = loaded + 1
			end
		end
	end
	io.write(">> Loaded " .. loaded .. " raids in " .. os.mtime() - start .. "ms.\n")
end

event:register()
