local goldfishBowl = Action()

function goldfishBowl.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid ~= 5554 then
		return false
	end

	target:remove(1)
	item:transform(5929)
	player:addAchievement("Silent Pet")
	return true
end

goldfishBowl:id(5928)
goldfishBowl:register()
