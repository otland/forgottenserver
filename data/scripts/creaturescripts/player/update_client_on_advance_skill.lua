local updateClientOnAdvanceSkill = CreatureEvent("Update Client On Advance Skill")

function updateClientOnAdvanceSkill.onAdvance(player, skill, oldLevel, newLevel)
	if skill == SKILL_LEVEL then
		return true
	end

	player:takeScreenshot(SCREENSHOT_TYPE_SKILLUP)
	return true
end

updateClientOnAdvanceSkill:register()
