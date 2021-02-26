local ge = GlobalEvent("cacheTalkActions")

function ge.onStartup(interval)
	print("loading talkactions ... ")
	local ta = Game.getTalkactions()
	for k, v in pairs(ta) do
		print(k .. ' ' .. v:getWords())
	end
	return true
end

ge:register()
