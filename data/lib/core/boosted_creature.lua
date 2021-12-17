function Game.updateBoostedMonster()
	local oldname = Game.getBoostedMonster()
	local boost = oldname
	local raceid = 2
	local monsters = Game.getMonstersByRace()
	repeat
		local possible = math.random(#monsters)
		local mType = MonsterType(monsters[possible].name)
		boost = monsters[possible].name
		raceid = monsters[possible].raceid
	until boost ~= oldname and mType

	Game.setBoostedMonster(boost, raceid)
	return true
end

function Monster.isBoosted(self)
	return self:getName():lower() == Game.getBoostedMonster():lower()
end
