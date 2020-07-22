local constructionKits = {
	--[itemid] = {transformto, client_version}
	[3901] = {1666, 750}, -- red cushioned chair kit
	[3902] = {1670, 750}, -- green cushioned chair kit
	[3903] = {1652, 750}, -- wooden chair kit
	[3904] = {1674, 750}, -- rocking chair kit
	[3905] = {1658, 750}, -- sofa chair kit
	[3906] = {3813, 750}, -- tusk chair kit
	[3907] = {3817, 750}, -- ivory chair kit
	[3908] = {1619, 750}, -- small table kit
	[3909] = {12799, 870}, -- cabinet kit
	[3910] = {2105, 750}, -- christmas tree package
	[3911] = {1614, 750}, -- big table kit
	[3912] = {3806, 750}, -- stone table kit
	[3913] = {3807, 750}, -- tusk table kit
	[3914] = {3809, 750}, -- bamboo table kit
	[3915] = {1716, 750}, -- drawer kit
	[3916] = {1724, 750}, -- dresser kit
	[3917] = {1732, 750}, -- locker kit
	[3918] = {1775, 750}, -- trough kit
	[3919] = {1774, 750}, -- barrel kit
	[3920] = {1750, 750}, -- trunk kit
	[3921] = {3832, 750}, -- bamboo drawer kit
	[3922] = {2095, 750}, -- birdcage kit
	[3923] = {2098, 750}, -- globe kit
	[3924] = {2064, 750}, -- table lamp kit
	[3925] = {2582, 750}, -- telescope kit
	[3926] = {2117, 750}, -- rocking horse kit
	[3927] = {1728, 750}, -- pendulum clock kit
	[3928] = {1442, 750}, -- knight statue kit
	[3929] = {1446, 750}, -- minotaur statue kit
	[3930] = {1447, 750}, -- goblin statue kit
	[3931] = {2034, 750}, -- large amphora kit
	[3932] = {2604, 750}, -- coal basin kit
	[3933] = {2080, 750}, -- piano kit
	[3934] = {2084, 750}, -- harp kit
	[3935] = {3821, 750}, -- trunk chair kit
	[3936] = {3811, 750}, -- trunk table kit
	[3937] = {2101, 750}, -- indoor plant kit
	[3938] = {3812, 750}, -- ornamented stone table kit
	[5086] = {5046, 760}, -- monkey statue 'see' kit
	[5087] = {5055, 760}, -- monkey statue 'hear' kit
	[5088] = {5056, 760}, -- monkey statue 'speak' kit
	[6114] = {6111, 780}, -- armor rack kit
	[6115] = {6109, 780}, -- weapon rack kit
	[6372] = {6356, 790}, -- oven kit
	[6373] = {6371, 790}, -- bookcase kit
	[8692] = {8688, 820}, -- chimney kit
	[9974] = {9975, 840}, -- crystal table kit
	[11126] = {11127, 854}, -- lizard weapon rack kit
	[11133] = {11129, 854}, -- dragon statue kit
	[11124] = {11125, 854}, -- snowman package
	[11205] = {11203, 854}, -- dragon throne kit
	[14328] = {1616, 920}, -- round table kit
	[14329] = {1615, 920}, -- square table kit
	[16075] = {16020, 944}, -- easel kit
	[16099] = {16098, 950}, -- cake cabinet kit
	[20254] = {20295, 980}, -- venorean cabinet kit
	[20255] = {20297, 980}, -- venorean wardrobe kit
	[20257] = {20299, 980} -- venorean drawer kit
}
local constructionKit = Action()

function constructionKit.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local kit = constructionKits[item.itemid]
	if not kit then
		return false
	end

	if fromPosition.x == CONTAINER_POSITION then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "Put the construction kit on the floor first.")
	elseif not Tile(fromPosition):getHouse() then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You may construct this only inside a house.")
	else
		item:transform(kit[1])
		fromPosition:sendMagicEffect(CONST_ME_POFF)
	end
	return true
end

for i, c in pairs(constructionKits) do
	if Game.getClientVersion().min >= c[2] then
		constructionKit:id(i)
	end
end
constructionKit:register()
