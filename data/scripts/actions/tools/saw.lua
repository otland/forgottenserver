local saw = Action()

function saw.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid ~= 5901 then -- wood
		return false
	end

	target:transform(10033) -- wooden ties
	return true
end

saw:id(2558)
saw:register()
