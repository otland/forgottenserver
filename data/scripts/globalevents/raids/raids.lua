local event = GlobalEvent("raids")

local CHECK_RAIDS_INTERVAL = 60 * 1000
local MAX_RAND_RANGE = 10000000

event:interval(CHECK_RAIDS_INTERVAL)

local running = nil
local lastRaidEnd = 0

function event.onTime(interval)
	logInfo(">> Executing raids event...")
	if running then
		return true
	end

	local now = os.mtime()

	local raids = Raid.getRaids()
	for key, raid in pairs(raids) do
		local chance = (MAX_RAND_RANGE * CHECK_RAIDS_INTERVAL) / raid.interval
		if now >= lastRaidEnd + raid.margin and chance >= math.random(0, MAX_RAND_RANGE) then
			running = key

			logInfo("Executing raid: " .. raid.name)
			raid:execute()

			if not raid.repeats then
				raids[key] = nil
			end
			break
		end
	end
	return true
end

event:register()
