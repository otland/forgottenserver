local config = {
	[8894] = { -- Rusty Armor (Common)
		[1] = {id = 3358, chance = 6994}, -- Chain Armor
		[2] = {id = 3377, chance = 3952}, -- Scale Armor
		[3] = {id = 3359, chance = 1502}, -- Brass Armor
		[4] = {id = 3357, chance = 197} -- Plate Armor
	},
	[8895] = { -- Rusty Armor (Semi-rare)
		[1] = {id = 3358, chance = 6437}, -- Scale Armor
		[2] = {id = 3358, chance = 4606}, -- Chain Armor
		[3] = {id = 3359, chance = 3029}, -- Brass Armor
		[4] = {id = 3357, chance = 1559}, -- Plate Armor
		[5] = {id = 3370, chance = 595}, -- Knight Armor
		[6] = {id = 8063, chance = 283}, -- Paladin Armor
		[7] = {id = 3381, chance = 49} -- Crown Armor
	},
	[8896] = { -- Rusty Armor (Rare)
		[1] = {id = 3359, chance = 6681}, -- Brass Armor
		[2] = {id = 3357, chance = 3767}, -- Plate Armor
		[3] = {id = 3370, chance = 1832}, -- Knight Armor
		[4] = {id = 3381, chance = 177}, -- Crown Armor
		[5] = {id = 8063, chance = 31}, -- Paladin Armor
		[6] = {id = 3360, chance = 10} -- Golden Armor
	},
	[8897] = { -- Rusty Legs (Common)
		[1] = {id = 3558, chance = 6949}, -- Chain Legs
		[2] = {id = 3362, chance = 3692}, -- Studded Legs
		[3] = {id = 3372, chance = 1307}, -- Brass Legs
		[4] = {id = 3557, chance = 133} -- Plate Legs
	},
	[8898] = { -- Rusty Legs (Semi-Rare)
		[1] = {id = 3362, chance = 5962}, -- Studded Legs
		[2] = {id = 3558, chance = 4037}, -- Chain Legs
		[3] = {id = 3372, chance = 2174}, -- Brass Legs
		[4] = {id = 3557, chance = 1242}, -- Plate Legs
		[5] = {id = 3371, chance = 186}, -- Knight Legs
	},
	[8899] = { -- Rusty Legs (Rare)
		[1] = {id = 3372, chance = 6500}, -- Brass Legs
		[2] = {id = 3557, chance = 3800}, -- Plate Legs
		[3] = {id = 3371, chance = 200}, -- Knight Legs
		[4] = {id = 3382, chance = 52}, -- Crown Legs
		[5] = {id = 3364, chance = 30} -- Golden Legs
	},
	[8900] = { -- Heavily Rusted Shield
	},
	[8901] = { -- Rusted Shield
	},
	[8902] = { -- Slightly Rusted Shield
		[1] = {id = 3410, chance = 3137}, -- Plate Shield
		[2] = {id = 3432, chance = 2887}, -- Ancient Shield
		[3] = {id = 7460, chance = 929}, -- Norse Shield
		[4] = {id = 3419, chance = 23}, -- Crown Shield
		[5] = {id = 3434, chance = 10} -- Vampire Shield
	},
	[8903] = { -- Heavily Rusted Boots
	},
	[8904] = { -- Rusted Boots
	},
	[8905] = { -- Slightly Rusted Boots
	},
	[8906] = { -- Heavily Rusted Helmet
	},
	[8907] = { -- Rusted Helmet
		[1] = {id = 3354, chance = 2200}, -- Brass Helmet
		[2] = {id = 3376, chance = 1870}, -- Studded Helmet
		[3] = {id = 3353, chance = 1490}, -- Iron Helmet
		[4] = {id = 3351, chance = 1010}, -- Steel Helmet
		[5] = {id = 3385, chance = 190}, -- Crown Helmet
		[6] = {id = 3391, chance = 10} -- Crusader Helmet
	},
	[8908] = { -- Slightly Rusted Helmet
		[1] = {id = 3353, chance = 3156}, -- Iron Helmet
		[2] = {id = 3351, chance = 2976}, -- Steel Helmet
		[3] = {id = 3385, chance = 963}, -- Crown Helmet
		[4] = {id = 3391, chance = 210}, -- Crusader Helmet
		[5] = {id = 3392, chance = 7} -- Royal Helmet
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
		if table.contains({8894, 8895, 8896}, target.itemid) then
			msg = "The armor was already damaged so badly that it broke when you tried to clean it."
		end
		if table.contains({8897, 8898, 8899}, target.itemid) then
			msg = "The legs were already damaged so badly that they broke when you tried to clean them."
		end
		if table.contains({8900, 8901, 8902}, target.itemid) then
			msg = "The shield was already damaged so badly that it broke when you tried to clean it."
		end
		if table.contains({8903, 8904, 8905}, target.itemid) then
			msg = "The boots were already damaged so badly that they broke when you tried to clean them."
		end
		if table.contains({8906, 8907, 8908}, target.itemid) then
			msg = "The helmet was already damaged so badly that it broke when you tried to clean it."
		end
		player:say(msg, TALKTYPE_MONSTER_SAY)
		target:getPosition():sendMagicEffect(CONST_ME_BLOCKHIT)
		target:remove()
	else
		target:transform(targetItem[index].id)
		target:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	end
	return item:remove(1)
end

rustRemover:id(9016)
rustRemover:register()
