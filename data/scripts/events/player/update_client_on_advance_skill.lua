local event = Event()

event.onPlayerAdvance = function(self, skill, oldLevel, newLevel)
	if skill == SKILL_LEVEL then
		return
	end

	if newLevel > oldLevel then
		self:takeScreenshot(SCREENSHOT_TYPE_SKILLUP)
	end
end

event:register()
