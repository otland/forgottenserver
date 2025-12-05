local talkaction = TalkAction("/attr")

function talkaction.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local position = player:getPosition()
	position:getNextPosition(player:getDirection())

	local tile = Tile(position)
	if not tile then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "There is no tile in front of you.")
		return false
	end

	local thing = tile:getTopVisibleThing(player)
	if not thing then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "There is an empty tile in front of you.")
		return false
	end

	local separatorPos = param:find(',')
	if not separatorPos then
		player:sendTextMessage(MESSAGE_INFO_DESCR, string.format("Usage: %s attribute, value.", words))
		return false
	end

	local attribute = string.trim(param:sub(0, separatorPos - 1))
	local value = string.trim(param:sub(separatorPos + 1))

	if thing:isItem() then
		local attributeId = Game.getItemAttributeByName(attribute)
		if attributeId == ITEM_ATTRIBUTE_NONE then
			player:sendTextMessage(MESSAGE_INFO_DESCR, "Invalid attribute name.")
			return false
		end

		if not thing:setAttribute(attribute, value) then
			player:sendTextMessage(MESSAGE_INFO_DESCR, "Could not set attribute.")
			return false
		end

		player:sendTextMessage(MESSAGE_INFO_DESCR, string.format("Attribute %s set to: %s", attribute, thing:getAttribute(attributeId)))
		position:sendMagicEffect(CONST_ME_MAGIC_GREEN)
	else
		player:sendTextMessage(MESSAGE_INFO_DESCR, "Thing in front of you is not supported.")
		return false
	end
end

talkaction:separator(" ")
talkaction:register()
