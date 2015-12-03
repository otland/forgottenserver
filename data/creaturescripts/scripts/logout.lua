function onLogout(player)
	local playerId = player:getId()
	if nextUseStaminaTime[playerId] ~= nil then
		nextUseStaminaTime[playerId] = nil
	end

    local stats = player:inBossFight()
    if(stats) then
        local boss = Monster(stats.bossId)
        -- Player logged out (or died) in the middle of a boss fight, store his damageOut
        if(boss) then
            local dmgOut = boss:getDamageMap()[playerId]
            if(dmgOut) then
                print(player:getName() .. " logged out while killing " .. boss:getName() .. ". Storing his damageOut: " .. dmgOut.total)
                stats.damageOut = (stats.damageOut or 0) + dmgOut.total
            end
        end
    end
	return true
end
