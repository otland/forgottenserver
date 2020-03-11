local stats = { -- Define the attribute and their rolls
	[1] = { -- Attack
		attribute = {
			name = 'Attack',
			rare = {1, 3}, -- Customize roll numbers here
			epic = {4, 6},
			legendary = {7, 10},
		},
		value = "Static",
		base = ITEM_ATTRIBUTE_ATTACK -- If attribute is a vanilla stat, it should have a default or 'base' amount, what is it?
	},
	[2] = { -- Defense
		attribute = {
			name = 'Defense',
			rare = {1, 2},
			epic = {3, 4},
			legendary = {5, 6},
		},
		value = "Static",
		base = ITEM_ATTRIBUTE_DEFENSE
	},
	[3] = { -- Extra Defense
		attribute = {
			name = 'Extra Defense',
			rare = {1, 1},
			epic = {2, 3},
			legendary = {4, 5},
		},
		value = "Static",
		base = ITEM_ATTRIBUTE_EXTRADEFENSE	
	},
	[4] = { -- Armor
		attribute = {
			name = 'Armor',
			rare = {1, 1},
			epic = {2, 3},
			legendary = {4, 5},
		},
		value = "Static",
		base = ITEM_ATTRIBUTE_ARMOR
	},
	[5] = { -- Accuracy
		attribute = {
			name = 'Accuracy',
			rare = {1, 5},
			epic = {6, 10},
			legendary = {11, 15},
		},
		value = "Percent",
		base = ITEM_ATTRIBUTE_HITCHANCE
	},
	[6] = { -- Range
		attribute = {
			name = 'Range',
			rare = {1, 1},
			epic = {2, 2},
			legendary = {3, 3},
		},
		value = "Static",
		base = ITEM_ATTRIBUTE_SHOOTRANGE
	},
	[7] = { -- Equipment with < 50 charges
		attribute = {
			name = 'Charges',
			rare = {5, 10},
			epic = {15, 20},
			legendary = {31, 35},
		},
		value = "Static",
		base = ITEM_ATTRIBUTE_CHARGES
	},
	[8] = { -- Equipment with >= 50 charges
		attribute = {
			name = 'Charges',
			rare = {100, 250},
			epic = {350, 500},
			legendary = {1000, 2000},
		},
		value = "Static",
		base = ITEM_ATTRIBUTE_CHARGES
	},
	[9] = { -- Time
		attribute = {
			name = 'Time',
			rare = {300000, 300000},
			epic = {900000, 900000},
			legendary = {2700000, 2700000},
		},
		value = "Duration",
		base = ITEM_ATTRIBUTE_DURATION
	},
	[10] = { -- Crit Chance
		attribute = {
			name = 'Crit Chance',
			rare = {5, 10},
			epic = {10, 15},
			legendary = {16, 20},
		},
		value = "Percent",
		items = { -- These are specifically targeted items, that can roll this attribute.
			2136, -- Demonbone Amulet
			21693,-- Horn
			14327,-- Bronze Ring
			11374 -- Beetle Necklace
		}
	},
	[11] = { -- Crit Amount (Currently Unused)
		attribute = {
			name = 'Crit Amount',
			rare = {10, 30},
			epic = {35, 60},
			legendary = {65, 100},
		},
		value = "Percent",
	},
	[12] = { -- Fire Damage
		attribute = {
			name = 'Enhanced Fire Damage',
			rare = {15, 30},
			epic = {30, 45},
			legendary = {45, 60},
		},
		value = "Damage",
		items = {
			7386, -- Merc Sword
			2392, -- Fire Sword
			7430, -- Dragonbone Staff
			2539, -- Phoenix Shield
			2432, -- Fire Axe
			8858, -- Elemental Bow
			7402, -- Dragonslayer Sword
			7453, -- Executioner
			2414, -- Dragon Lance
			2520, -- Demon Shield
			8927, -- Dark Trinity Mace
			7411, -- Ornamented Axe
			11305,-- Drakinata
			7382  -- Demonrage Sword
		}
	},
	[13] = { -- Ice Damage
		attribute = {
			name = 'Enhanced Ice Damage',
			rare = {10, 15},
			epic = {15, 25},
			legendary = {25, 35},
		},
		value = "Damage",
		items = {
			7386, -- Merc Sword
			7453, -- Executioner
			2445, -- Crystal Mace
			7428, -- Bonebreaker
			7407, -- Haunted Blade
			7437, -- Sapphire Hammer
			7387, -- Diamond Hammer
			7455, -- Mythril Axe
			21696,-- Icicle Bow
			7390, -- Justice Seeker
			21697,-- Runic Ice Shield
			8858, -- Elemental Bow
			7410, -- Queen's Sceptre
			8927, -- Dark Trinity Mace
			2534  -- Vampire Shield
		}
	},
	[14] = { -- Energy Damage
		attribute = {
			name = 'Enhanced Energy Damage',
			rare = {50, 75},
			epic = {100, 125},
			legendary = {200, 250},
		},
		value = "Damage",
		items = {
			7386, -- Merc Sword
			7421, -- Onyx Flail
			8858, -- Elemental Bow
			7388, -- Vile Axe
			7410, -- Queen's Sceptre
			7404, -- Assassin Dagger
			2414, -- Dragon Lance
			2444, -- Hammer of Wrath
			7402, -- Dragonslayer Sword
			2514, -- Mastermind Shield
			2542, -- Tempest Shield
			8924, -- Hellforged Axe
			7433, -- Ravenwing
			2424, -- Silver Mace
			8927, -- Dark Trinity Mace
			7384, -- Mystic Blade
			7382  -- Demonrage Sword
		}
	},
	[15] = { -- Fire Resistance
		attribute = {
			name = 'Fire Resistance',
			rare = {2, 3},
			epic = {4, 6},
			legendary = {7, 10},
		},
		value = "Percent",
		items = {
			2516, -- Dragon Shield
			2520, -- Demon Shield
			2539, -- Phoenix Shield
			2519, -- Crown Shield
			2491, -- Crown Helmet
			2493, -- Demon Helmet
			2494, -- Demon Armor
			2495, -- Demon Legs
			13756,-- Mage's Cap
			2487, -- Crown Armor
			7899, -- Magma Coat
			8867, -- Dragon Robe
			11356,-- Zaoan Robe
			2488, -- Crown Legs
			7894, -- Magma Legs
			7891, -- Magma Boots
			2492, -- Dragon Scale Mail
			2498, -- Royal Helmet
			2655, -- Red Robe
			7900, -- Magma Monocle
			2133, -- Ruby Necklace
			8877, -- Lavos Armor
			2508, -- Native Armor
			2123 -- Ring of the Sky
		}
	},
	[16] = { -- Ice Resistance
		attributes = {
			name = 'Ice Resistance',
			rare = {2, 3},
			epic = {4, 6},
			legendary = {7, 10},
		},
		value = "Percent",
		items = {
			3973, -- Tusk Shield
			7460, -- Norse Shield
			21697,-- Runic Ice Shield
			7461, -- Krimhorn Helmet
			7462, -- Ragnir Helmet
			7902, -- Glacier Mask
			7463, -- Mammoth Fur Cape
			7897, -- Glacier Robe
			8878, -- Crystalline Armor
			8887, -- Frozen Armor
			7896, -- Glacier Kilt
			21700,-- Icy Culotte
			7457, -- Fur Boots
			11117,-- Crystal Boots
			7892, -- Glacier Shoes
			7464, -- Mammoth Fur Shorts
			2125, -- Crystal Necklace
			2124  -- Crystal Ring
		}
	},
	[17] = { -- Energy Resistance
		attribute = {
			name = 'Energy Resistance',
			rare = {2, 3},
			epic = {4, 6},
			legendary = {7, 10},
		},
		value = "Percent",
		items = {
			2514, -- Mastermind Shield
			2515, -- Guardian Shield
			2528, -- Tower Shield
			12644,-- Shield of Corruption
			2535, -- Castle Shield
			2475, -- Warrior Helmet
			2497, -- Crusader Helmet
			7901, -- Lightning Headband
			2472, -- Magic Plate Armor
			2476, -- Knight Helmet
			2492, -- Dragon Scale Mail
			7898, -- Lightning Robe
			8871, -- Focus Cape
			8879, -- Voltage Armor
			11355,-- Spellweaver's Robe
			12607,-- Elite Draken Mail
			2477, -- Knight Legs
			7895, -- Lightning Legs
			23539,-- Mino Shield
			2123, -- Ring of the Sky
			2508, -- Native Armor
			7893  -- Lightning Boots
		}
	},
	[18] = { -- Earth Resistance
		attribute = {
			name = 'Earth Resistance',
			rare = {2, 3},
			epic = {4, 6},
			legendary = {7, 10},
		},
		value = "Percent",
		items = {
			2518, -- Beholder Shield
			2535, -- Castle Shield
			6131, -- Tortoise Shield
			15491,-- Carapace Shield
			20090,-- Spike Shield
			2540, -- Scarab Shield
			2479, -- Strange Helmet
			3971, -- Charmer's Tiara
			3972, -- Beholder Helmet
			7885, -- Terra Legs
			7903, -- Terra Hood
			2664, -- Wood Cape
			7463, -- Mammoth Fur Cape
			7884, -- Terra Mantle
			8869, -- Greenwood Coat
			8880, -- Swamplair Armor
			3982, -- Crocodile Boots
			2123, -- Ring of the Sky
			2135, -- Scarab Amulet
			7886, -- Terra Boots
			2508, -- Native Armor
			21706 -- Goo Shell
		}
	},
	[19] = { -- Physical Resistance
		attribute = {
			name = 'Physical Resistance',
			rare = {2, 3},
			epic = {4, 6},
			legendary = {7, 10},
		},
		value = "Percent",
		items = {
			2514, -- Mastermind Shield
			2528, -- Tower Shield
			2535, -- Castle Shield
			2536, -- Medusa Shield
			12644,-- Shield of Corruption
			2532, -- Ancient Shield
			2497, -- Crusader Helmet
			3969, -- Horse Helmet
			5741, -- Skull Helmet
			2472, -- Magic Plate Armor
			2466, -- Golden Armor
			3968, -- Leopard Armor
			8889, -- Skullcracker Armor
			8891, -- Paladin Armor
			21692,-- Albino Plate
			2470, -- Golden Legs
			11304,-- Zaoan Legs
			2645, -- Steel Boots
			20109,-- Buckle
			2179, -- Gold Ring
			21693,-- Horn
			11302,-- Zaoan Helmet
			2503, -- Dwarven Armor
			2508, -- Native Armor
			8821, -- Witchunter's Coat
			15406 -- Ornate Chest
		}
	},
	[20] = { -- Death Resistance
		attribute = {
			name = 'Death Resistance',
			rare = {2, 3},
			epic = {4, 6},
			legendary = {7, 10},
		},
		value = "Percent",
		items = {
			2521, -- Dark Shield
			2529, -- Black Shield
			2532, -- Ancient Shield
			2536, -- Medusa Shield
			2462, -- Devil Helmet
			2490, -- Dark Helmet
			10016,-- Batwing Hat
			2489, -- Dark Helmet
			8889, -- Skullcracker Armor
			12607,-- Elite Draken Mail
			20109,-- Buckle
			5462, -- Pirate Boots
			2129, -- Wolf Tooth Chain
			2508, -- Native Armor
			8752, -- Ring of the Count
			8885, -- Divine Plate
			21253,-- Vampire Lord Crown
			8821  -- Witchunter's Coat
		}
	},
	[21] = { -- Spell Damage
		attribute = {
			name = 'Spell Damage',
			rare = {3, 5},
			epic = {8, 10},
			legendary = {15, 20},
		},
		value = "Percent",
		items = {
			2123, -- Ring of the Sky
			2131, -- Star Amulet
			8752, -- Ring of the Count
			2662, -- Magician Hat
			21253 -- Vampire Lord Crown
		}
	},
	[22] = { -- Multi Shot
		attribute = {
			name = 'Multi Shot',
			rare = {1, 1},
			epic = {2, 2},
			legendary = {3, 3},
		},
		value = "Static"
		-- Items targeted by this attribute are defined in a different way below.
	},
	[23] = { -- Stun Chance
		attribute = {
			name = 'Stun Chance',
			rare = {3, 5},
			epic = {6, 10},
			legendary = {11, 15},
		},
		value = "Percent"
	},
	[24] = { -- Mana Shield
		atrribute = {
			name = 'Mana Shield',
			rare = {5, 10},
			epic = {11, 20},
			legendary = {21, 30},
		},
		value = "Percent"
	},
	[25] = { -- Sword Skill
		attribute = {
			name = 'Sword Skill',
			rare = {1, 2},
			epic = {3, 5},
			legendary = {6, 10},
		},
		value = "Static",
		items = {
			8881, -- Fireborn
			12644 -- Shield of Corruption
		}
	},
	[26] = { -- Skill Axe
		attribute = {
			name = 'Axe Skill',
			rare = {1, 2},
			epic = {3, 5},
			legendary = {6, 10},
		},
		value = "Static",
		items = {
			8882 -- Earthborn
		}
	},
	[27] = { -- Skill Club
		attribute = {
			name = 'Club Skill',
			rare = {1, 2},
			epic = {3, 5},
			legendary = {6, 10},
		},
		value = "Static",
		items = {
			8883 -- Windborn
		}
	},
	[28] = { -- Skill Melee
		attribute = {
			name = 'Melee Skills',
			rare = {1, 2},
			epic = {3, 5},
			legendary = {6, 10},
		},
		value = "Static",
		items = {
			2342, -- HOTA
			2471, -- Golden Helmet
			15406, -- Ornate Armor
			15412, -- Ornate Legs
			2494, -- Demon Armor
			9776, -- Yalahari Armor
			21692, -- Albino Plate
			2537, -- Amazon Shield
			2522, -- Great Shield
			6391 -- Nightmare Shield
		}
	},
	[29] = { -- Skill Distance
		attribute = {
			name = 'Distance Skill',
			rare = {1, 2},
			epic = {3, 5},
			legendary = {6, 10},
		},
		value = "Static",
		items = {
			8888, -- MAA
			2499, -- Amazon Helmet
			2474, -- Winged Helmet
			12645,-- Elite Draken Helm
			5741, -- Skull Helmet
			2500, -- Amazon Armor
			2505, -- Elven Mail
			8891, -- Paladin Armor
			9777, -- Yalahari Legs
			14327,-- Bronze Ring
			2218, -- Paw Amulet
			15407 -- Depth Lorica
		}
	},
	[30] = { -- Skill Shielding
		attribute = {
			name = 'Shield Skill',
			rare = {1, 2},
			epic = {3, 5},
			legendary = {6, 10},
		},
		value = "Static",
		items = {
			11240, -- Guardian Boots
			2195, -- Boots of Haste
			2503, -- Dwarven Armor
			2505, -- Elven Armor
			15406, -- Depth Lorica
			15410, -- Depth Calcei
			15409, -- Depth Ocrea
			2502 -- Dwarven Helmet
		}
	},
	[31] = { -- Magic Level
		attribute = {
			name = 'Magic Level',
			rare = {1, 2},
			epic = {3, 5},
			legendary = {6, 10},
		},
		value = "Static",
		items = {
			2323, -- Hat of the Mad
			2662, -- Magician Hat
			2506, -- Dragon Scale Helmet
			9778, -- Yalahari Mask
			10016,-- Batwing Hat
			13756,-- Mage's Cape
			18398,-- Gill Gudgel
			8865, -- Dark Lords Cape
			8866, -- Robe of the Ice Queen
			8867, -- Dragon Robe
			8869, -- Greenwood Coat
			8890, -- Robe of the Underworld
			8821, -- Witchhunters Coat
			11355,-- Spell Weavers
			12643,-- Royal Scale Robe
			15489,-- Calop Cape
			11117,-- Crystal Boots
			21700,-- Icy Culotte
			2542, -- Tempest Shield
			6433, -- Necromancer Shield
			2501, -- Ceremonial Mask
			8900, -- Spellbooks
			8901,
			8902,
			8903,
			8904,
			8918,
			12647,
			18401,
			16112,
			22423,
			22424,
			21725, -- Furious Frock
			2139 -- Ancient Tiara
		}
	},
	[32] = { -- Max Health (+100)
		attribute = {
			name = 'Max Health',
			rare = {30, 60},
			epic = {80, 120},
			legendary = {150, 250},
		},
		value = "Static",
		items = {
			2493, -- Demon Helmet
			2496, -- Horned Helmet
			2471, -- Golden Helmet
			2474, -- Winged Helmet
			2498, -- Royal Helmet
			9778, -- Yalahari Mask
			11302,-- Zaoan Helmet
			12645,-- Elite Draken Helmet
			2472, -- MPA
			2492, -- DSM
			2655, -- Red Robe
			2494, -- Demon Armor
			2508, -- Native Armor
			2505, -- Elven Armor
			3968, -- Leopard Armor
			7463, -- Mammoth Fur Cape
			8881, -- Fireborn
			8882, -- Earthborn
			8883, -- Windborn
			8885, -- Divine Plate
			8886, -- Molten Plate
			8887, -- Frozen Plate
			8888, -- MAA
			8889, -- Skullcracker Armor
			8890, -- Underworld Robe
			8821, -- Witch Hunter's Armor
			9776, -- Yalahari Armor
			12607,-- Elite Draken Armor
			12642,-- Royal Draken Mail
			12643,-- Royal Scale Mail
			15406,-- Ornate Chestplate
			15407,-- Depth Lorica
			21692,-- Albino Plate
			21706,-- Goo Shell
			2495, -- Demon Legs
			2470, -- Golen Legs
			7730, -- Blue Legs
			9777, -- Yalahari Leg Piece
			11240,-- Guardian Boots
			11118,-- Dragon Scale Boots
			15410,-- Depth Calcei
			2646, -- Golden Boots
			2522, -- Great Shield
			2539, -- Phoenix Shield
			12644,-- Shield of Corruption
			2139, -- Ancient Tiara
			2539  -- Phoenix Shield
		}
	},
	[33] = { -- Max Mana (+100)
		attribute = {
			name = 'Max Mana',
			rare = {30, 60},
			epic = {80, 120},
			legendary = {150, 250},
		},
		value = "Static",
		items = {
			2662, -- Magician Hat
			2663, -- Mystic Turban
			2323, -- Hat of the Mad
			2472, -- MPA
			3972, -- Beholder Helmet
			7458, -- Fur Cap
			7459, -- Earmuffs
			7903, -- Elemental Hats
			7901,
			7902,
			7900,
			9778, -- Yalahari Mask
			10016,-- Batwing Hat
			13756,-- Mage's Hat
			18398,-- Gill Hat
			2508, -- Native Armor
			2505, -- Elven Armor
			2656, -- Blue Robe
			7884, -- Elemental Coats
			7897,
			7898,
			7899,
			8892, -- Ethno Coat
			8819, -- Magician's Robe
			15489,-- Calopteryx Cape
			11356,-- Zaoan Robe
			18399,-- Gill Coat
			7885, -- Elemental Legs
			7894,
			7895,
			7896,
			7730, -- Blue Legs
			15490,-- Grasshopper Legs
			18400,-- Gill Legs
			7886, -- Elemental Boots
			7891,
			7892,
			7893,
			8900, -- Spellbooks
			8901,
			8902,
			8903,
			8904,
			8918,
			12647,
			18401,
			16112,
			22424,
			21725,-- Furious Frock
			8870, -- Spirit Cloak
			8871, -- Focus Cape
			2139, -- Ancient Tiara
			2195  -- Boots of Haste
		}
	},
	[34] = { -- Max Health % (+10%)
		attribute = {
			name = 'Max Health',
			rare = {1, 3},
			epic = {4, 6},
			legendary = {7, 10},
		},
		value = "Percent"
	},
	[35] = { -- Max Mana % (+10%)
		attribute = {
			name = 'Max Mana',
			rare = {1, 3},
			epic = {4, 6},
			legendary = {7, 10},
		},
		value = "Percent",
	},
	[36] = { -- Life Leech Chance (Currently Unused)
		attribute = {
			name = 'Mana Leech Chance',
			rare = {3, 5},
			epic = {6, 10},
			legendary = {11, 20},
		},
		value = "Percent"
		-- Items targeted by this attribute are defined in a different way below.
	},
	[37] = { -- Life Leech Amount
		attribute = {
			name = 'Life Leech',
			rare = {3, 5},
			epic = {6, 10},
			legendary = {11, 20},
		},
		value = "Percent"
		-- Items targeted by this attribute are defined in a different way below.
	},
	[38] = { -- Mana Leech Chance (Currently Unused)
		attribute = {
			name = 'Mana Leech Chance',
			rare = {3, 5},
			epic = {6, 10},
			legendary = {11, 20},
		},
		value = "Percent"
		-- Items targeted by this attribute are defined in a different way below.
	},
	[39] = { -- Mana Leech Amount
		attribute = {
			name = 'Mana Leech',
			rare = {3, 5},
			epic = {6, 10},
			legendary = {11, 20},
		},
		value = "Percent"
		-- Items targeted by this attribute are defined in a different way below.
	}
}
local cannotroll = { -- These items are special and cannot be rolled rare/epic/legendary
	8905, -- Rainbow shield
	8906,
	8907,
	8908,
	8909,
	7744, -- These are the standard and transformed Dawnbreaker weapons
	7763,
	7751,
	7770,
	7756,
	7775
}

-- Check if item can be rolled (this is for use outside of this lib, actions, quests etc)
function rollCheck(item)
	local itemtype = ItemType(item:getId())
	local itemid = itemtype:getId()
	if table.contains(cannotroll, itemid) then
		return false
	end
	for k,v in pairs(stats) do
		if v.items ~= nil then
			if table.contains(v.items, itemid) then
				return true
			end
		end
	end
	local weapontype = itemtype:getWeaponType()
	if weapontype > 0 then
		if itemtype:isStackable() then
			return false
		else
			return true
		end
	elseif itemtype:getArmor() > 0 then
		return true
	end
	return false
end

-- Get duration literally
function rollBaseDuration(item)
	local it_id = item:getId()
	local tid = ItemType(it_id):getTransformEquipId()
	if tid > 0 then
		item:transform(tid)
		local vx = item:getAttribute(ITEM_ATTRIBUTE_DURATION)
		item:transform(it_id)
		--item:removeAttribute(ITEM_ATTRIBUTE_DURATION)
		return vx
	end
	return 0
end

-- Get base/stock stat
function rollBase(item, attr)
	local id = ItemType(item:getId())
	local v = {
		[ITEM_ATTRIBUTE_ATTACK] = id:getAttack(),
		[ITEM_ATTRIBUTE_DEFENSE] = id:getDefense(),
		[ITEM_ATTRIBUTE_EXTRADEFENSE] = id:getExtraDefense(),
		[ITEM_ATTRIBUTE_ARMOR] = id:getArmor(),
		[ITEM_ATTRIBUTE_HITCHANCE] = id:getHitChance(),
		[ITEM_ATTRIBUTE_SHOOTRANGE] = id:getShootRange(),
		[ITEM_ATTRIBUTE_CHARGES] = id:getCharges(),
		[ITEM_ATTRIBUTE_DURATION] = rollBaseDuration(item)
	}
	return v[attr]
end

-- Roll a container or item
function rollRarity(container, forced)
	-- Tiers
	local tiers = {
		[1] = {
			prefix = 'rare',
			chance = {
				[1] = 750, -- 7.5% chance to roll
				[2] = 2000 -- 20% chance for second stat
			}
		},

		[2] = {
			prefix = 'epic',
			chance = {
				[1] = 375, -- 3.75% chance to roll
				[2] = 5000 -- 50% chance for second stat
			}
		},

		[3] = {
			prefix = 'legendary',
			chance = {
				[1] = 200, -- 2% chance to roll
				[2] = 10000 -- 100% chance for second stat
			}
		},
	}
	local rares = 0
	local available_stats = {}
	local it_u = container
	local it_id = ItemType(it_u:getId())
	if it_u:isContainer() then
		local h = it_u:getItemHoldingCount()
		if h > 0 then
			local i = 1 
			while i <= h do
				local bagitem = it_u:getItem(i - 1)
				if bagitem:isContainer() then
					h = h - bagitem:getItemHoldingCount()
				end
				local manualroll = forced or false
				local crares = rollRarity(bagitem, manualroll)
				rares = rares + crares
				i = i + 1
			end
		end
	else
		if not it_id:isStackable() then
			local wp = it_id:getWeaponType()
			if wp > 0 then
				-- Shields
				if wp == WEAPON_SHIELD then
					table.insert(available_stats, stats[2]) -- Defense
					table.insert(available_stats, stats[30]) -- Skill Shield
				
				-- Distance Items
				elseif wp == WEAPON_DISTANCE then
					table.insert(available_stats, stats[1]) -- Attack
					table.insert(available_stats, stats[6]) -- Range
					table.insert(available_stats, stats[10]) -- Critical Chance
					table.insert(available_stats, stats[29]) -- Skill Distance
					--table.insert(available_stats, stats[5]) -- Accuracy
					if it_u:getId() ~= 5907 then -- Slingshot
						table.insert(available_stats, stats[22]) -- Multi Shot
					end
				
				-- Wands and Rods
				elseif wp == WEAPON_WAND then -- type wand
					table.insert(available_stats, stats[21]) -- Spell Damage
					table.insert(available_stats, stats[33]) -- Max Mana
					table.insert(available_stats, stats[31]) -- Magic Level
					table.insert(available_stats, stats[39]) -- Mana Leech Amount
				
				-- Sword, Clubs and Axes
				elseif table.contains({WEAPON_SWORD, WEAPON_CLUB, WEAPON_AXE}, wp) then -- Melee Weapon
					
					if wp == WEAPON_SWORD then
						table.insert(available_stats, stats[25]) -- Sword Skill
					elseif wp == WEAPON_AXE then
						table.insert(available_stats, stats[26]) -- Axe Skill
					elseif wp == WEAPON_CLUB then
						table.insert(available_stats, stats[27]) -- Club Skill
					end
					
					if it_id:getAttack() > 0 then
						table.insert(available_stats, stats[1]) -- Attack
					end
		
					if it_id:getSlotPosition() == 2096 then -- Two-handed Weapon
						table.insert(available_stats, stats[2]) -- Defense
					end
		
					if it_id:getSlotPosition() == 48 then -- One-handed Weapon
						table.insert(available_stats, stats[3]) -- Extra Defense
					end
					table.insert(available_stats, stats[10]) -- Critical Chance
					table.insert(available_stats, stats[37]) -- Life Leech
				end
			else -- Armors, Amulets, Runes and Rings
				if it_id:getArmor() > 0 then -- Ignore clothing/things with no armor stat
					table.insert(available_stats, stats[4]) -- Armor
				end

				-- Duration
				local eq_id = it_id:getTransformEquipId()
				if eq_id > 0 then
					table.insert(available_stats, stats[9]) -- Time
				end
				
				-- Charges
				local chargecount = it_id:getCharges()
				if chargecount > 0  and it_u.itemid ~= 2173 then -- Ignore AOL
					if chargecount >= 50 then -- If its base charge is greater than 50
						table.insert(available_stats, stats[8]) -- High Charges
					else -- Its base charge is less than 50
						table.insert(available_stats, stats[7]) -- Low Charges
					end
				end
			end
			
			-- Specifically Targeted Items
			for k,v in pairs(stats) do
				if v.items ~= nil then
					if table.contains(v.items, it_u.itemid) then
						table.insert(available_stats, stats[k])
					end
				end
			end
		end
	end
	if #available_stats > 0 then -- Skips it all if it's empty
		local tier = 0 -- Normal item
		local rarity = math.random(1, 10000)
		-- Manual trigger
		if type(forced) == "string" then -- rollRarity(item, "rare")  OR  /roll legendary
			for i = 1, #tiers do
				if forced == tiers[i].prefix then
					tier = i
				end
			end
		elseif forced == true then -- rollRarity(item, true)  OR  /roll
			tier = math.random(1,#tiers)
		-- Natural rolls
		else
			for i = 1, #tiers do -- Get best roll
				if rarity <= tiers[i].chance[1] then
					tier = i -- Rolled a rare/epic/legendary
				end
			end
		end
		if tier > 0 then -- Item has rolled rare or higher
			local stats_used = {}
			for stat = 1, #tiers[tier].chance do
				if #available_stats > 0 then
					local roll = math.random(1, 10000)
					if stat == 1 then -- First stat is guaranteed
						roll = tiers[tier].chance[stat]
					end
					if roll <= tiers[tier].chance[stat] then -- All other stats are rolled by chance
						local selected_stat = math.random(1, #available_stats)
						table.insert(stats_used, available_stats[selected_stat])
						table.remove(available_stats, selected_stat)
					end
				end
			end
			if #stats_used > 0 then
				rares = rares + 1
				local stat_desc = {}
				for stat = 1, #stats_used do
					local statmin = 0
					local statmax = 0
					if tiers[tier].prefix == tiers[3].prefix then
						statmin = stats_used[stat].attribute.legendary[1]
						statmax = stats_used[stat].attribute.legendary[2]
					elseif tiers[tier].prefix == tiers[2].prefix then
						statmin = stats_used[stat].attribute.epic[1]
						statmax = stats_used[stat].attribute.epic[2]
					else
						statmin = stats_used[stat].attribute.rare[1]
						statmax = stats_used[stat].attribute.rare[2]
					end
					-- Apply Attribute
					local critv = math.random(statmin, statmax) -- The actual roll amount
					if stats_used[stat].value ~= nil then -- Is the value type defined?
						local basestat = 0
						-- Fill basestat
						if stats_used[stat].base ~= nil then
							basestat = rollBase(it_u, stats_used[stat].base) -- This is the base/stock value of the stat
						end
						-- Static
						if stats_used[stat].value == "Static" then
							table.insert(stat_desc, '[' .. stats_used[stat].attribute.name .. ': +' .. critv .. ']') -- Standard value "+10"
						-- Percentage
						elseif stats_used[stat].value == "Percent" then
							table.insert(stat_desc, '[' .. stats_used[stat].attribute.name ..': +' .. critv .. '%]') -- Percent value "+10%"
						-- Damage
						elseif stats_used[stat].value == "Damage" then
							local minimumDmg = 1 -- the item roll is the MAX damage, how do we get a minimim damage?
							if stats_used[stat].attribute.name == "Enhanced Fire Damage" then
								minimumDmg = (critv - math.random(3,5)) -- 3-5 less than the maximum damage
							elseif stats_used[stat].attribute.name == "Enhanced Ice Damage" then
								minimumDmg = (critv - math.random(5,9)) -- 5-9 less than the maxximum damage
							end -- "Enhanced Energy Damage" always has 1 as minimumDmg
							table.insert(stat_desc, '[' .. stats_used[stat].attribute.name ..': '.. minimumDmg .. '-' .. critv .. ']') -- Damage value "13-35"
						-- Duration
						elseif stats_used[stat].value == "Duration" then
							local timeconvert = critv / 60000
							table.insert(stat_desc, '[' .. stats_used[stat].attribute.name .. ': +' .. timeconvert .. ' minutes]') -- Duration value "+15 minutes"
						end
						-- If this is a vanilla attribute, overwrite it with new roll
						if basestat > 0 then
							it_u:setAttribute(stats_used[stat].base, basestat + critv)
						end
						
					end
				end
				-- Rarity prefix
				if it_id:getArticle() ~= "" then -- Replace article if exists
					if tiers[tier].prefix == "epic" then
						it_u:setAttribute(ITEM_ATTRIBUTE_ARTICLE, "an " .. tiers[tier].prefix)
					else
						it_u:setAttribute(ITEM_ATTRIBUTE_ARTICLE, "a " .. tiers[tier].prefix)
					end
				else -- Add rarity prefix to item article (this allows for easy identification of rolled items in scripts outside of this lib 'item:getArticle():find("rare")')
					it_u:setAttribute(ITEM_ATTRIBUTE_ARTICLE, tiers[tier].prefix)
				end
				-- If item has a description, retain it instead of over-writing it
				if it_id:getDescription() == "" then
					it_u:setAttribute(ITEM_ATTRIBUTE_DESCRIPTION, table.concat(stat_desc, "\n"))
				else
					it_u:setAttribute(ITEM_ATTRIBUTE_DESCRIPTION, table.concat(stat_desc, "\n")  .. "\n" .. it_id:getDescription()) -- This is how I like it - before flavour text, no double spacing
				end
				-- Capitalize tier.prefix to be used for the animated text above corpses
				rare_text = (tiers[tier].prefix:gsub("^%l", string.upper) .. "!")
			end
		end
	end
	return rares
end

-- Apply condition
function rollCondition(player, item, slot)
	local attributes = {
		 [1] = {"%[" .. stats[25].attribute.name .. ": ", CONDITION_PARAM_SKILL_SWORD}, -- "[Sword Skill: "
		 [2] = {"%[" .. stats[26].attribute.name .. ": ", CONDITION_PARAM_SKILL_AXE},
		 [3] = {"%[" .. stats[27].attribute.name .. ": ", CONDITION_PARAM_SKILL_CLUB},
		 [4] = {"%[" .. stats[28].attribute.name .. ": ", CONDITION_PARAM_SKILL_MELEE},
		 [5] = {"%[" .. stats[29].attribute.name .. ": ", CONDITION_PARAM_SKILL_DISTANCE},
		 [6] = {"%[" .. stats[30].attribute.name .. ": ", CONDITION_PARAM_SKILL_SHIELD},
		 [7] = {"%[" .. stats[31].attribute.name .. ": ", CONDITION_PARAM_STAT_MAGICPOINTS},
		 [8] = {"%[" .. stats[32].attribute.name .. ": ", CONDITION_PARAM_STAT_MAXHITPOINTS},
		 [9] = {"%[" .. stats[33].attribute.name .. ": ", CONDITION_PARAM_STAT_MAXMANAPOINTS},
		[10] = {"%[" .. stats[34].attribute.name .. ": ", CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT, percent = true, absolute = true},
		[11] = {"%[" .. stats[35].attribute.name .. ": ", CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT, percent = true, absolute = true},
		[12] = {"%[" .. stats[11].attribute.name .. ": ", CONDITION_PARAM_SPECIALSKILL_CRITICALHITAMOUNT, percent = true, absolute = true},
		[13] = {"%[" .. stats[10].attribute.name .. ": ", CONDITION_PARAM_SPECIALSKILL_CRITICALHITCHANCE, percent = true},
		[14] = {"%[" .. stats[36].attribute.name .. ": ", CONDITION_PARAM_SPECIALSKILL_LIFELEECHCHANCE, percent = true},
		[15] = {"%[" .. stats[37].attribute.name .. ": ", CONDITION_PARAM_SPECIALSKILL_LIFELEECHAMOUNT, percent = true},
		[16] = {"%[" .. stats[38].attribute.name .. ": ", CONDITION_PARAM_SPECIALSKILL_MANALEECHCHANCE, percent = true},
		[17] = {"%[" .. stats[39].attribute.name .. ": ", CONDITION_PARAM_SPECIALSKILL_MANALEECHAMOUNT, percent = true},
	}
	local itemDesc = item:getAttribute(ITEM_ATTRIBUTE_DESCRIPTION)
	for k = 1,#attributes do
		local skillBonus = 0 -- reset
		local attributeSearchValue = "%+(%d+)%]" -- "+10]"
		if attributes[k].percent ~= nil then
			attributeSearchValue = "%+(%d+)%%%]" -- "+10%]"
			if attributes[k].absolute ~= nil then
				skillBonus = 100 -- These conditions require absolutes (108%, 145% etc.)
			end
		end
		local attributeString = attributes[k][1] .. attributeSearchValue -- "%[Attack: %+(%d+)%]"
		if string.match(itemDesc, attributeString) ~= nil then -- "[Attack: +10]"
			local offset = (10 * k) + slot -- ((CONST_SLOT_LAST) * k) + slot
			local skillBonus = skillBonus + tonumber(string.match(itemDesc, attributeString)) -- Raw (%d+) value
			
			-- (!) Comment this code out if you want to use CHANCE/AMOUNT rolls (they will need to be rolled together)
			-- Leech amount is not used, set to 100%
			-- Crit amount is not used, set to 100%
			if k == 13 or k == 15 or k == 17 then
				local j = k - 1
				if player:getCondition(CONDITION_ATTRIBUTES, CONDITIONID_COMBAT, (10 * j) + slot) == nil then
					local condition = Condition(CONDITION_ATTRIBUTES)
					condition:setParameter(CONDITION_PARAM_SUBID, (10 * j) + slot)
					condition:setParameter(CONDITION_PARAM_TICKS, -1)
					condition:setParameter(attributes[j][2], 100) -- 100%
					player:addCondition(condition)
				else
					player:removeCondition(CONDITION_ATTRIBUTES, CONDITIONID_COMBAT, (10 * j) + slot)
				end
			end
			
			if player:getCondition(CONDITION_ATTRIBUTES, CONDITIONID_COMBAT, offset) == nil then
				local condition = Condition(CONDITION_ATTRIBUTES)
				condition:setParameter(CONDITION_PARAM_SUBID, offset)
				condition:setParameter(CONDITION_PARAM_TICKS, -1)
				condition:setParameter(attributes[k][2], skillBonus)
				player:addCondition(condition)
			else
				player:removeCondition(CONDITION_ATTRIBUTES, CONDITIONID_COMBAT, offset)
			end
		end
	end
end
			
-- Get item attributes
function itemAttributes(player, item, slot, equip)
	-- Check if item is rolled
	if item:getArticle() ~= "" then
		if item:getArticle():find("rare") or item:getArticle():find("epic") or item:getArticle():find("legendary") then
			local appropriateSlot = false
			local slotType = ItemType(item.itemid):getSlotPosition()
			-- What slots do we want to check? this ignores CONST_SLOT_AMMO and CONST_SLOT_BACKPACK
			local raritySlots = {
				[CONST_SLOT_LEFT] = {validPositions = {[1] = SLOTP_LEFT,[2] = SLOTP_RIGHT,[3] = SLOTP_TWO_HAND}},
				[CONST_SLOT_RIGHT] = {validPositions = {[1] = SLOTP_LEFT,[2] = SLOTP_RIGHT,[3] = SLOTP_TWO_HAND}},
				[CONST_SLOT_HEAD] = {validPositions = {[1] = SLOTP_HEAD}},
				[CONST_SLOT_NECKLACE] = {validPositions = {[1] = SLOTP_NECKLACE}},
				[CONST_SLOT_ARMOR] = {validPositions = {[1] = SLOTP_ARMOR}},
				[CONST_SLOT_LEGS] = {validPositions = {[1] = SLOTP_LEGS}},
				[CONST_SLOT_FEET] = {validPositions = {[1] = SLOTP_FEET}},
				[CONST_SLOT_RING] = {validPositions = {[1] = SLOTP_RING}}
			}
			-- If slot is one that we check
			if raritySlots[slot] ~= nil then
				-- Validate that item is being equipped to the right slot
				if slot == CONST_SLOT_LEFT or slot == CONST_SLOT_RIGHT then
					local weapon = ItemType(item.itemid):getWeaponType()
					if weapon ~= WEAPON_NONE then
						if weapon ~= WEAPON_AMMO then
							appropriateSlot = true
						end
					end
				else
					for i = 1,#raritySlots[slot].validPositions do
						if bit.band(slotType, raritySlots[slot].validPositions[i]) ~= 0 then
							appropriateSlot = true
							break
						end
					end
				end
				if appropriateSlot then -- Item is in the wrong slotType
					-- Checks have all passed, run apply/remove attribute
					rollCondition(player, item, slot)
				end
			end
		end
	end
end