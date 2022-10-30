local logFormat = "[%s] %s %s\n"

function logCommand(player, words, param)
	local file = io.open("data/logs/" .. player:getName() .. " commands.log", "a")
	if not file then
		return
	end

	file:write(logFormat:format(os.date("%d/%m/%Y %H:%M"), words, param:trim()))
	file:close()
end
