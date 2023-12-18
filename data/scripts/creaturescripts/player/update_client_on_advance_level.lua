local updateClientOnAdvanceLevel = CreatureEvent("Update Client On Advance Level")

function updateClientOnAdvanceLevel.onAdvance(player, skill, oldLevel, newLevel)
	if skill ~= SKILL_LEVEL then
		return true
	end

	player:updateClientExpDisplay()
	player:takeScreenshot(SCREENSHOT_TYPE_LEVELUP)
	return true
end

updateClientOnAdvanceLevel:register()
