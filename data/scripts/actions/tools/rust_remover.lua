local config = {
	[9808] = { -- Rusty Armor (Common)
		[1] = {id = 2464, chance = 6994}, -- Chain Armor
		[2] = {id = 2483, chance = 3952}, -- Scale Armor
		[3] = {id = 2465, chance = 1502}, -- Brass Armor
		[4] = {id = 2463, chance = 197} -- Plate Armor
	},
	[9809] = { -- Rusty Armor (Semi-rare)
		[1] = {id = 2483, chance = 6437}, -- Scale Armor
		[2] = {id = 2464, chance = 4606}, -- Chain Armor
		[3] = {id = 2465, chance = 3029}, -- Brass Armor
		[4] = {id = 2463, chance = 1559}, -- Plate Armor
		[5] = {id = 2476, chance = 595}, -- Knight Armor
		[6] = {id = 8891, chance = 283}, -- Paladin Armor
		[7] = {id = 2487, chance = 49} -- Crown Armor
	},
	[9810] = { -- Rusty Armor (Rare)
		[1] = {id = 2465, chance = 6681}, -- Brass Armor
		[2] = {id = 2463, chance = 3767}, -- Plate Armor
		[3] = {id = 2476, chance = 1832}, -- Knight Armor
		[4] = {id = 2487, chance = 177}, -- Crown Armor
		[5] = {id = 8891, chance = 31}, -- Paladin Armor
		[6] = {id = 2466, chance = 10} -- Golden Armor
	},
	[9811] = { -- Rusty Legs (Common)
		[1] = {id = 2648, chance = 6949}, -- Chain Legs
		[2] = {id = 2468, chance = 3692}, -- Studded Legs
		[3] = {id = 2478, chance = 1307}, -- Brass Legs
		[4] = {id = 2647, chance = 133} -- Plate Legs
	},
	[9812] = { -- Rusty Legs (Semi-Rare)
		[1] = {id = 2468, chance = 5962}, -- Studded Legs
		[2] = {id = 2648, chance = 4037}, -- Chain Legs
		[3] = {id = 2478, chance = 2174}, -- Brass Legs
		[4] = {id = 2647, chance = 1242}, -- Plate Legs
		[5] = {id = 2477, chance = 186}, -- Knight Legs
	},
	[9813] = { -- Rusty Legs (Rare)
		[1] = {id = 2478, chance = 6500}, -- Brass Legs
		[2] = {id = 2647, chance = 3800}, -- Plate Legs
		[3] = {id = 2477, chance = 200}, -- Knight Legs
		[4] = {id = 2488, chance = 52}, -- Crown Legs
		[5] = {id = 2470, chance = 30} -- Golden Legs
	},
	[9814] = { -- Heavily Rusted Shield
	},
	[9815] = { -- Rusted Shield
	},
	[9816] = { -- Slightly Rusted Shield
		[1] = {id = 2510, chance = 3137}, -- Plate Shield
		[2] = {id = 2532, chance = 2887}, -- Ancient Shield
		[3] = {id = 7460, chance = 929}, -- Norse Shield
		[4] = {id = 2519, chance = 23}, -- Crown Shield
		[5] = {id = 2534, chance = 10} -- Vampire Shield
	},
	[9820] = { -- Heavily Rusted Helmet
	},
	[9821] = { -- Rusted Helmet
		[1] = {id = 2460, chance = 2200}, -- Brass Helmet
		[2] = {id = 2482, chance = 1870}, -- Studded Helmet
		[3] = {id = 2459, chance = 1490}, -- Iron Helmet
		[4] = {id = 2457, chance = 1010}, -- Steel Helmet
		[5] = {id = 2491, chance = 190}, -- Crown Helmet
		[6] = {id = 2497, chance = 10} -- Crusader Helmet
	},
	[9822] = { -- Slightly Rusted Helmet
		[1] = {id = 2459, chance = 3156}, -- Iron Helmet
		[2] = {id = 2457, chance = 2976}, -- Steel Helmet
		[3] = {id = 2491, chance = 963}, -- Crown Helmet
		[4] = {id = 2497, chance = 210}, -- Crusader Helmet
		[5] = {id = 2498, chance = 7} -- Royal Helmet
	},
	[9817] = { -- Heavily Rusted Boots
	},
	[9818] = { -- Rusted Boots
	},
	[9819] = { -- Slightly Rusted Boots
	},
}

local rustRemover = Action()

function rustRemover.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local targetItem = config[target.itemid]
	if not targetItem then
		return true
	end

	local randomChance = math.random(10000)
	local index = false

	if targetItem[1].chance >= randomChance then -- implying first item in the table index always has the highest chance.
		while not index do
			randomIndex = math.random(#targetItem)
			if targetItem[randomIndex].chance >= randomChance then
				index = randomIndex
			end
		end
	end

	if not index then
		if table.contains({9808, 9809, 9810}, target.itemid) then
			msg = "The armor was already damaged so badly that it broke when you tried to clean it."
		end
		if table.contains({9811, 9812, 9813}, target.itemid) then
			msg = "The legs were already damaged so badly that they broke when you tried to clean them."
		end
		if table.contains({9814, 9815, 9816}, target.itemid) then
			msg = "The shield was already damaged so badly that it broke when you tried to clean it."
		end
		if table.contains({9817, 9818, 9819}, target.itemid) then
			msg = "The boots were already damaged so badly that they broke when you tried to clean them."
		end
		if table.contains({9820, 9821, 9822}, target.itemid) then
			msg = "The helmet was already damaged so badly that it broke when you tried to clean it."
		end
		player:say(msg, TALKTYPE_MONSTER_SAY)
		target:getPosition():sendMagicEffect(CONST_ME_BLOCKHIT)
		target:remove()
	else
		target:transform(targetItem[index].id)
		target:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
		player:addAchievementProgress("Polisher", 1000)
	end
	return item:remove(1)
end

rustRemover:id(9930)
rustRemover:register()
