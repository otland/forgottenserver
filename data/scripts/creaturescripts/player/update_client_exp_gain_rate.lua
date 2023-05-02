local updateClientExpGainRate = CreatureEvent("Update Client Xp Gain Rate")

function updateClientExpGainRate.onAdvance(player, skill, oldLevel, newLevel)
	if skill ~= SKILL_LEVEL then
		return true
	end

	player:updateClientExpGainRate()
	return true
end

updateClientExpGainRate:register()
