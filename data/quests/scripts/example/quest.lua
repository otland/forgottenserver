function Quest:setName(player)
	return self:getName():format(player:getName())
end

function Quest:isStarted(player)
	return true
end

function Quest:isCompleted(player)
	return true
end
