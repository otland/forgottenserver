local updateClientXpGainRate = CreatureEvent("Update Client Xp Gain Rate")

function updateClientXpGainRate.onAdvance(player, skill, oldLevel, newLevel)
	if skill ~= SKILL_LEVEL or newLevel <= oldLevel then
		return true
	end

	player:updateClientXpGainRate()
	return true
end

updateClientXpGainRate:register()
