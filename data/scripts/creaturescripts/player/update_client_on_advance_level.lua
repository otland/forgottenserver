local updateClientOnAdvanceLevel = CreatureEvent("Update Client On Advance Level")

function updateClientOnAdvanceLevel.onAdvance(player, skill, oldLevel, newLevel)
	if skill ~= SKILL_LEVEL then
		return true
	end

	player:updateClientExpDisplay()

	if newLevel > oldLevel then
		player:takeScreenshot(SCREENSHOT_TYPE_LEVELUP)
	end
	return true
end

updateClientOnAdvanceLevel:register()
