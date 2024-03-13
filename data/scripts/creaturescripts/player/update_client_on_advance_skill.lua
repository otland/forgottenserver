local updateClientOnAdvanceSkill = CreatureEvent("Update Client On Advance Skill")

function updateClientOnAdvanceSkill.onAdvance(player, skill, oldLevel, newLevel)
	if skill == SKILL_LEVEL then
		return true
	end

	if newLevel > oldLevel then
		player:takeScreenshot(SCREENSHOT_TYPE_SKILLUP)
	end
	return true
end

updateClientOnAdvanceSkill:register()
