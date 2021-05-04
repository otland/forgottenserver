local constructionKits = {
	[3901] = 1666, -- red cushioned chair
	[3902] = 1670, -- green cushioned chair
	[3903] = 1652, -- wooden chair
	[3904] = 1674, -- rocking chair
	[3905] = 1658, -- sofa chair
	[3906] = 3813, -- tusk chair
	[3907] = 3817, -- ivory chair
	[3908] = 1619, -- small table
	[3909] = 12799, -- cabinet
	[3910] = 2105, -- christmas tree
	[3911] = 1614, -- big table
	[3912] = 3806, -- stone table
	[3913] = 3807, -- tusk table
	[3914] = 3809, -- bamboo table
	[3915] = 1716, -- drawers
	[3916] = 1724, -- dresser
	[3917] = 1732, -- locker
	[3918] = 1775, -- trough
	[3919] = 1774, -- barrel
	[3920] = 1750, -- large trunk
	[3921] = 3832, -- bamboo drawer
	[3922] = 2095, -- birdcage
	[3923] = 2098, -- globe
	[3924] = 2064, -- table lamp
	[3925] = 2582, -- telescope
	[3926] = 2117, -- rocking horse
	[3927] = 1728, -- pendulum clock
	[3928] = 1442, -- statue
	[3929] = 1446, -- minotaur statue
	[3930] = 1447, -- goblin statue
	[3931] = 2034, -- large amphora
	[3932] = 2604, -- coal basin
	[3933] = 2080, -- piano
	[3934] = 2084, -- harp
	[3935] = 3821, -- small trunk
	[3936] = 3811, -- thick trunk
	[3937] = 2101, -- indoor plant
	[3938] = 3812, -- ornamented stone table
	[5086] = 5046, -- monkey statue (See no evil)
	[5087] = 5055, -- monkey statue (Hear no evil)
	[5088] = 5056, -- monkey statue (Speak no evil)
	[6114] = 6111, -- armor rack
	[6115] = 6109, -- weapon rack
	[6372] = 6356, -- oven
	[6373] = 6371, -- bookcase
	[8692] = 8688, -- chimney
	[9974] = 9975, -- crystal table
	[11126] = 11127, -- lizard weapon rack
	[11133] = 11129, -- dragon statue
	[11124] = 11125, -- snowman
	[11205] = 11203, -- dragon throne
	[14328] = 1616, -- small round table
	[14329] = 1615, -- square table
	[16075] = 16020, -- artist's easel
	[16099] = 16098, -- cake cabinet
	[20254] = 20295, -- cabinet
	[20255] = 20297, -- wardrobe
	[20257] = 20299 -- drawer
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local kit = constructionKits[item:getId()]
	if not kit then
		return false
	end

	local tile = Tile(item:getPosition())
	if not tile or tile and not tile:getHouse() then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You may construct this only inside a house.")
		return true
	end

	if fromPosition.x == CONTAINER_POSITION and item:getParent():getId() ~= ITEM_BROWSEFIELD then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "Put the construction kit on the floor first.")
		return true
	end

	item:transform(kit)
	fromPosition:sendMagicEffect(CONST_ME_POFF)
	return true
end
