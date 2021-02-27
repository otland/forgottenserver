local ge = GlobalEvent("cacheTalkActions")
local path = 'data/scripts/cache/commands/'

function ge.onStartup(interval)
	print('Caching talkactions ... ')
	local talkactions = Game.getTalkactions()

	for a = ACCOUNT_TYPE_GOD, ACCOUNT_TYPE_NORMAL, -1 do
		local ta = talkactions[a]
		if ta then
			local fileName = a .. '.txt'
			local commands = ''
			for i = 1, #ta do
				commands = commands .. ta[i] .. '\n'
			end
			writeToCache(path .. fileName, commands)
		end
	end

	print('Caching done.')
	return true
end

ge:register()

function writeToCache(path, data)
	local file = io.open(path, "w")
	file:write(("%s\n"):format(data))
	file:close()
end
