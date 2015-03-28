function Position.getNextPosition(self, direction, steps)
	steps = steps or 1
	if direction == WEST then
		self.x = self.x - steps
	elseif direction == EAST then
		self.x = self.x + steps
	elseif direction == NORTH then
		self.y = self.y - steps
	elseif direction == SOUTH then
		self.y = self.y + steps
	elseif direction == NORTHWEST then
		self.x = self.x - steps
		self.y = self.y - steps
	elseif direction == NORTHEAST then
		self.x = self.x + steps
		self.y = self.y - steps
	elseif direction == SOUTHWEST then
		self.x = self.x - steps
		self.y = self.y + steps
	elseif direction == SOUTHEAST then
		self.x = self.x + steps
		self.y = self.y + steps
	end
end
