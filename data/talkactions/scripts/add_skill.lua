local function getSkillId(skillName)
	if skillName == "club" then
		return SKILL_CLUB
	elseif skillName == "sword" then
		return SKILL_SWORD
	elseif skillName == "axe" then
		return SKILL_AXE
	elseif skillName:sub(1, 4) == "dist" then
		return SKILL_DISTANCE
	elseif skillName:sub(1, 6) == "shield" then
		return SKILL_SHIELD
	elseif skillName:sub(1, 4) == "fish" then
		return SKILL_FISHING
	else
		return SKILL_FIST
	end
end

function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	local split = param:splitTrimmed(",")
	if not split[2] then
		player:sendCancelMessage("Insufficient parameters.")
		return false
	end

	local target = Player(split[1])
	if not target then
		player:sendCancelMessage("A player with that name is not online.")
		return false
	end

	local count = 1
	if split[3] then
		count = tonumber(split[3])
	end

	local ch = split[2]:sub(1, 1)
	for i = 1, count do
		if ch == "l" or ch == "e" then
			target:addExperience(getExperienceForLevel(target:getLevel() + 1) - target:getExperience(), false)
		elseif ch == "m" then
			target:addManaSpent(target:getVocation():getRequiredManaSpent(target:getBaseMagicLevel() + 1) - target:getManaSpent())
		else
			local skillId = getSkillId(split[2])
			target:addSkillTries(skillId, target:getVocation():getRequiredSkillTries(skillId, target:getSkillLevel(skillId) + 1) - target:getSkillTries(skillId))
		end
	end
	return false
end
