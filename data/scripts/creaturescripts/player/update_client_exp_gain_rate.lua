local updateClientExpDisplay = CreatureEvent("Update Client Xp Gain Rate")

function updateClientExpDisplay.onAdvance(player, skill, oldLevel, newLevel)
	if skill ~= SKILL_LEVEL then
		return true
	end

	player:updateClientExpDisplay()
	return true
end

updateClientExpDisplay:register()
