local event = GlobalEvent("raids")

local CHECK_RAIDS_INTERVAL = 60
local MAX_RAND_RANGE = 10000000

event:interval(CHECK_RAIDS_INTERVAL)

local running = nil
local lastRaidEnd = 0

function event.onTime(interval)
	io.write(">> Executing raids event...\n")
	if running then
		return true
	end

	local now = os.mtime()

	local raids = Raid.getRaids()
	for key, raid in pairs(raids) do
		local chance = (MAX_RAND_RANGE * CHECK_RAIDS_INTERVAL) / raid.interval
		if now >= lastRaidEnd + raid.margin and chance >= math.random(0, MAX_RAND_RANGE) then
			running = key

			io.write("Executing raid: " .. raid.name .. "\n")
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
