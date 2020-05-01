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