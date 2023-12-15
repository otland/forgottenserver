local updateClientOnAdvance = CreatureEvent("Update Client On Advance")

function updateClientOnAdvance.onAdvance(player, skill, oldLevel, newLevel)
	if skill == SKILL_LEVEL then
		player:updateClientExpDisplay()

		if newLevel > oldLevel then
			player:takeScreenshot(SCREENSHOT_TYPE_LEVELUP)
		end
	else
		player:takeScreenshot(SCREENSHOT_TYPE_SKILLUP)
	end

	return true
end

updateClientOnAdvance:register()
