local stairs = {
	410,411,427,428,429,369,370,383,385,392,469,470,482,484,485,409,423,432,461,
	462,475,476,479,480,489,924,3135,3136,3137,3138,3219,3220,4834,4835,4836,4837,
	5081,5731,6127,6128,6129,6130,6173,6917,6918,6919,6920,6921,6922,6923,6924,
	8276,8277,8279,8280,8281,8282,8283,8284,8285,8286,8559,8560,8561,8562,8563,
	8564,8565,8566,9574,9606,9846
}

local stairscheck = MoveEvent()

function stairscheck.onStepIn(creature, item, position, fromPosition)
	if not creature or not creature:isPlayer() then
		return false
	end

	local p = item:getPosition()
	local destinationTile = Tile(p.x, p.y, p.z + 1)
	if not destinationTile then
		item:transform(findReplacementItem(p))
		p:sendMagicEffect(CONST_ME_POFF)
		return false
	end

	if ItemType(destinationTile:getGround():getId()):isBlocking() then
		creature:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "[Warning] Invalid stairs found. Notify staff " .. positionToReadable(p))
		creature:teleportTo(fromPosition)

		item:transform(findReplacementItem(p))
		p:sendMagicEffect(CONST_ME_POFF)
		print("[Warning] Invalid stairs at position " .. positionToReadable(p))
	end
	return true
end

stairscheck:id(unpack(stairs))
stairscheck:register()

function findReplacementItem(position)
	local defaultReplacementId = 100 -- void
	for y = position.y-1, position.y+1 do
		for x = position.x-1, position.x+1 do
			if y ~= position.y and x ~= position.x then
				local t = Tile(x, y, position.z)
				if t and t:getGround() then
					local groundId = t:getGround():getId()
					local it = ItemType(groundId)
					if not it:isBlocking() and not table.contains(stairs, groundId) then
						return groundId
					end
				end
			end
		end
	end
	return defaultReplacementId
end

function positionToReadable(p)
	return "( ".. p.x .." / ".. p.y .." / ".. p.z .." )"
end
