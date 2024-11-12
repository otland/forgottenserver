do
	-- Invite packet
	local handler = PacketHandler(0xA3)

	function handler.onReceive(player, msg)
		local targetId = msg:getU32()
		local target = Player(targetId)
		if not target then
			return
		end

		local party = player:getParty()
		if party then
			party:invite(player, target)
		end
	end

	handler:register()
end

do
	-- Join packet
	local handler = PacketHandler(0xA4)

	function handler.onReceive(player, msg)
		local targetId = msg:getU32()
		local target = Player(targetId)
		if not target then
			return
		end

		local party = player:getParty()
		if party then
			party:join(player, target)
		end
	end

	handler:register()
end

do
	-- Revoke invite packet
	local handler = PacketHandler(0xA5)

	function handler.onReceive(player, msg)
		local targetId = msg:getU32()
		local target = Player(targetId)
		if not target then
			return
		end

		local party = player:getParty()
		if party then
			party:revokeInvitation(player, target)
		end
	end

	handler:register()
end

do
	-- Pass leadership packet
	local handler = PacketHandler(0xA6)

	function handler.onReceive(player, msg)
		local targetId = msg:getU32()
		local target = Player(targetId)
		if not target then
			return
		end

		local party = player:getParty()
		if party then
			party:passLeadership(player, target)
		end
	end

	handler:register()
end

do
	-- Leave packet
	local handler = PacketHandler(0xA7)

	function handler.onReceive(player, msg)
		local party = player:getParty()
		if party then
			party:leave(player)
		end
	end

	handler:register()
end

do
	-- Shared experience packet
	local handler = PacketHandler(0xA8)

	function handler.onReceive(player, msg)
		local active = msg:getByte() == 1
		local party = player:getParty()
		if party then
			party:setSharedExperience(player, active)
		end
	end

	handler:register()
end
