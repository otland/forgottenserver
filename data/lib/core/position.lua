local mt = rawgetmetatable("Position")

function mt.__add(lhs, rhs)
	local stackpos = lhs.stackpos or rhs.stackpos
	return Position(lhs.x + (rhs.x or 0), lhs.y + (rhs.y or 0), lhs.z + (rhs.z or 0), stackpos)
end

function mt.__sub(lhs, rhs)
	local stackpos = lhs.stackpos or rhs.stackpos
	return Position(lhs.x - (rhs.x or 0), lhs.y - (rhs.y or 0), lhs.z - (rhs.z or 0), stackpos)
end

function mt.__concat(lhs, rhs) return tostring(lhs) .. tostring(rhs) end
function mt.__eq(lhs, rhs) return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z end
function mt.__tostring(self) return string.format("Position(%d, %d, %d)", self.x, self.y, self.z) end

---@alias directionOffsets table<Directions, {x: number, y: number}>

---@type directionOffsets
Position.directionOffset = {
	[DIRECTION_NORTH] = {x = 0, y = -1},
	[DIRECTION_EAST] = {x = 1, y = 0},
	[DIRECTION_SOUTH] = {x = 0, y = 1},
	[DIRECTION_WEST] = {x = -1, y = 0},
	[DIRECTION_SOUTHWEST] = {x = -1, y = 1},
	[DIRECTION_SOUTHEAST] = {x = 1, y = 1},
	[DIRECTION_NORTHWEST] = {x = -1, y = -1},
	[DIRECTION_NORTHEAST] = {x = 1, y = -1}
}

local abs, max = math.abs, math.max
---Returns the distance between two positions
---@param self Position
---@param positionEx Position
---@return number maxDistance -- The maximum distance between two positions
function Position:getDistance(positionEx)
	local dx = abs(self.x - positionEx.x)
	local dy = abs(self.y - positionEx.y)
	local dz = abs(self.z - positionEx.z)
	return max(dx, dy, dz)
end

---Returns the next position in a specific direction and how many steps in that direction
---@param direction directionOffsets
---@param steps? number
function Position:getNextPosition(direction, steps)
	local offset = Position.directionOffset[direction]
	if offset then
		steps = steps or 1
		self.x = self.x + offset.x * steps
		self.y = self.y + offset.y * steps
	end
end

---Returns the next position up one floor
---@return Position
function Position:moveUpstairs()
	local swap = function(lhs, rhs)
		lhs.x, rhs.x = rhs.x, lhs.x
		lhs.y, rhs.y = rhs.y, lhs.y
		lhs.z, rhs.z = rhs.z, lhs.z
	end

	self.z = self.z - 1

	local defaultPosition = self + Position.directionOffset[DIRECTION_SOUTH]
	local toTile = Tile(defaultPosition)
	if not toTile or not toTile:isWalkable() then
		for direction = DIRECTION_NORTH, DIRECTION_NORTHEAST do
			if direction == DIRECTION_SOUTH then
				direction = DIRECTION_WEST
			end

			local position = Position(self)
			position:getNextPosition(direction)
			toTile = Tile(position)
			if toTile and toTile:isWalkable() then
				swap(self, position)
				return self
			end
		end
	end
	swap(self, defaultPosition)
	return self
end

---Checks if a position is in range of two other positions
---@param from Position
---@param to Position
---@return boolean
function Position:isInRange(from, to)
	-- No matter what corner from and to is, we want to make
	-- life easier by calculating north-west and south-east
	local zone = {
		nW = {
			x = (from.x < to.x and from.x or to.x),
			y = (from.y < to.y and from.y or to.y),
			z = (from.z < to.z and from.z or to.z)
		},
		sE = {
			x = (to.x > from.x and to.x or from.x),
			y = (to.y > from.y and to.y or from.y),
			z = (to.z > from.z and to.z or from.z)
		}
	}

	if self.x >= zone.nW.x and self.x <= zone.sE.x
	and self.y >= zone.nW.y and self.y <= zone.sE.y
	and self.z >= zone.nW.z and self.z <= zone.sE.z then
		return true
	end
	return false
end

---Adds the summon to the target list of all spectators in the area
---@param summon Monster
function Position:notifySummonAppear(summon)
	local spectators = Game.getSpectators(self)
	for _, spectator in ipairs(spectators) do
		if spectator:isMonster() and spectator ~= summon then
			spectator:addTarget(summon)
		end
	end
end
