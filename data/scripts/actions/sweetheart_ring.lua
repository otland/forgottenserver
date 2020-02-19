local sweetheartRing = Action()

function sweetheartRing.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local ringItem = player:getSlotItem(CONST_SLOT_RING)
    if item == ringItem then
        player:getPosition():sendMagicEffect(CONST_ME_HEARTS)
        return true
    else
        return false
    end
end

sweetheartRing:id(24324)
sweetheartRing:register()
