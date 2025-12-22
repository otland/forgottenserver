function NetworkMessage:getBool()
	local value = self:getByte()
	if value > 1 then
		print("[Warning - NetworkMessage::getBool] Invalid boolean value received: " .. value)
	end
	return value ~= 0
end

function NetworkMessage:addBool(value)
	self:addByte(value and 1 or 0)
end
