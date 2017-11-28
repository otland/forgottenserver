local drunk = Condition(CONDITION_DRUNK)
drunk:setParameter(CONDITION_PARAM_TICKS, 60000)

local poison = Condition(CONDITION_POISON)
poison:setParameter(CONDITION_PARAM_DELAYED, true)
poison:setParameter(CONDITION_PARAM_MINVALUE, -50)
poison:setParameter(CONDITION_PARAM_MAXVALUE, -120)
poison:setParameter(CONDITION_PARAM_STARTVALUE, -5)
poison:setParameter(CONDITION_PARAM_TICKINTERVAL, 4000)
poison:setParameter(CONDITION_PARAM_FORCEUPDATE, true)

local fluidMessage = {
	[3] = 'Aah...',
	[4] = 'Urgh!',
	[5] = 'Mmmh.',
	[7] = 'Aaaah...',
	[10] = 'Aaaah...',
	[11] = 'Urgh!',
	[13] = 'Urgh!',
	[15] = 'Aah...',
	[19] = 'Urgh!',
	[43] = 'Aaaah...'
}

local function graveStoneTeleport(cid, fromPosition, toPosition)
	local player = Player(cid)
	if not player then
		return true
	end

	player:teleportTo(toPosition)
	player:say('Muahahahaha..', TALKTYPE_MONSTER_SAY, false, player)
	fromPosition:sendMagicEffect(CONST_ME_DRAWBLOOD)
	toPosition:sendMagicEffect(CONST_ME_MORTAREA)
end

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local targetType = ItemType(target.itemid)
	if targetType:isFluidContainer() then
		if target.type == 0 and item.type ~= 0 then
			target:transform(target:getId(), item.type)
			item:transform(item:getId(), 0)
			return true
		elseif target.type ~= 0 and item.type == 0 then
			target:transform(target:getId(), 0)
			item:transform(item:getId(), target.type)
			return true
		end
	end

	if target.itemid == 1 then
		if item.type == 0 then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, 'It is empty.')

		elseif target.uid == player.uid then
			if table.contains({3, 15, 43}, item.type) then
				player:addCondition(drunk)

			elseif item.type == 4 then
				player:addCondition(poison)

			elseif item.type == 7 then
				player:addMana(math.random(50, 150))
				fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)

			elseif item.type == 10 then
				player:addHealth(60)
				fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
			end

			player:say(fluidMessage[item.type] or 'Gulp.', TALKTYPE_MONSTER_SAY)
			item:transform(item:getId(), 0)
		else
			Game.createItem(2016, item.type, toPosition):decay()
			item:transform(item:getId(), 0)
		end
	else

		local fluidSource = targetType:getFluidSource()
		if fluidSource ~= 0 then
			item:transform(item:getId(), fluidSource)

		elseif item.type == 0 then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, 'It is empty.')

		else
			if item.type == 2 and target.actionid == 2023 then
				toPosition.y = toPosition.y + 1
				local creatures, destination = Tile(toPosition):getCreatures(), Position(32791, 32332, 10)
				if #creatures == 0 then
					graveStoneTeleport(player.uid, fromPosition, destination)
				else
					local creature
					for i = 1, #creatures do
						creature = creatures[i]
						if creature and creature:isPlayer() then
							graveStoneTeleport(creature.uid, toPosition, destination)
						end
					end
				end

			else
				if toPosition.x == CONTAINER_POSITION then
					toPosition = player:getPosition()
				end

			Game.createItem(2016, item.type, toPosition):decay()
			item:transform(item:getId(), 0)
			end
		end
	end

	return true
end
