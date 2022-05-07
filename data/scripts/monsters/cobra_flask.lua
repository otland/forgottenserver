local cobras = {"cobra scout", "cobra vizier", "cobra assassin"}

local ev = EventCallback

function ev.onSpawn(monster, position, startup, artificial)
	if table.contains(cobras, monster:getName():lower()) then
		local storage = Game.getStorageValue(GlobalStorageKeys.cobraBastionFlask)
		if storage then
			if storage >= os.time() then
				monster:setHealth(monster:getMaxHealth() * 0.75)
				monster:getPosition():sendMagicEffect(CONST_ME_GREEN_RINGS)
			else
				Game.setStorageValue(GlobalStorageKeys.cobraBastionFlask, -1)
			end
		end
	end
	return true
end

ev:register(-1)

local cobraFlask = Action()

function cobraFlask.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if table.contains({33940, 33941, 33942, 33943}, target:getId()) then
		target:getPosition():sendMagicEffect(CONST_ME_GREENSMOKE)
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You carefully pour just a tiny, little, finely dosed... and there goes the whole content of the bottle. Stand back!")
		item:transform(33953)
		Game.setStorageValue(GlobalStorageKeys.cobraBastionFlask, os.time() + 30 * 60)
	end
	return true
end

cobraFlask:id(33952)
cobraFlask:register()
