function Creature:onMove(oldPos, newPos, isTeleport)
	if isTeleport then
		return true -- we still want the creature to be able to be teleported.
	end
	return self:isAbleToMove() -- please don't change this, else canMove(bool) will take no effect
end
