function onSay(player, words, param)
	local defaultParam = param
	
	param = param:lower()
	if param == '' or param == "on" then
		if not player:isLiveCasting() then
			player:startLiveCasting()
		end
	elseif param == "off" then
		if player:isLiveCasting() then
			player:stopLiveCasting()
		end
	else -- a password
		if not player:isLiveCasting() then
			player:startLiveCasting(defaultParam)
		end
	end
end
