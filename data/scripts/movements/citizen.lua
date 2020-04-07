local citizen = MoveEvent()

function citizen.onStepIn(creature, item, position, fromPosition)
	if item.actionid > 30020 and item.actionid < 30050 then
		if not creature:isPlayer() then
			return false
		end

		creature:setTown(Town(item.actionid - 30020))
	end
	return true
end

citizen:id(1949)
citizen:register()
