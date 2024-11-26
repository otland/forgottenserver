
do
	-- Request add vip
	local handler = PacketHandler(0xDC)
	
	function handler.onReceive(player, msg)
		local name = msg:getString()
		player:addVip(name)
	end
	
	handler:register()
end

do
	-- Request remove vip
	local handler = PacketHandler(0xDD)
	
	function handler.onReceive(player, msg)
		local vipGuid = msg:getU32()
		player:removeVip(vipGuid)
	end
	
	handler:register()
end

do
	-- Request edit vip
	local handler = PacketHandler(0xDE)
	
	function handler.onReceive(player, msg)
		local vipGuid = msg:getU32()
		local description = msg:getString()
		local icon = math.min(10, msg:getU32()) -- 10 is max icon in 9.63
		local notify = msg:getByte() ~= 0
		player:editVip(vipGuid, description, icon, notify)
	end
	
	handler:register()
end
