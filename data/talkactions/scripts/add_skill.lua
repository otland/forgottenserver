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
	elseif skillName:sub(1, 4) == "fist" then
		return SKILL_FIST
	elseif skillName:sub(1, 1) == "m" then
		return SKILL_MAGLEVEL
	elseif skillName == "level" or skillName:sub(1, 1) == "l" or skillName:sub(1, 1) == "e" then
		return SKILL_LEVEL
	end

	return nil
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

	local skillId = getSkillId(split[2])
	if not skillId then
		player:sendCancelMessage("Unknown skill.")
		return false
	end

	target:addSkill(skillId, count)
	return false
end
