function onUse(cid, item, fromPosition, itemEx, toPosition)
	local SKINNING = { 
						--[[ ARRAY NUMBER OF CORPSES TO HELP
							
							Dragon 					= 3104
							Dragon Lord 			= 2881
							Bonebeast 				= 3031
							Behemoth 				= 2931
							Lizard Sentinel 		= 4259
							Lizard Templar 			= 4256
							Lizard Snakecharmer 	= 4262
							Lizard Dragon Priest 	= 11277
							Lizard High Guard 		= 11269
							Lizard legionnaire 		= 11273
							Lizard Zaogun 			= 11281
							Minotaur 				= 3090
							Minotaur Archer 		= 2871
							Minotaur Mage			= 2866
							Minotaur Guard			= 2876
							Lord of Elements 		= 9010
						]]	
						
						LORD_OF_ELEMENTS_STORAGE 	= {10011},
						CORPSES 					= {3104, 2881, 3031, 2931, 4259, 4256, 4262, 11277, 11269, 11273, 11281, 3090, 2871, 2866, 2876, 9010},
						CORPSES_AFTER 				= {3105, 2882, 3032, 2932, 4260, 4257, 4263, 11278, 11270, 11274, 11282, 3091, 2872, 2867, 2877},
						SKINNED_ITEM				= {5877, 5948, 5925, 5893, 5876, 5876, 5876,  5876,  5876,  5876,  5876, 5878, 5878, 5878, 5878},	
						CHANCE_PERCENTE 			= {	 30,   25,   30,   30,   40,   40,   30,    30,    30,    25,    25,   45,   45,   45,   45},
						
						--[[ ARRAY NUMBER OF ESPECIAL ITEMS TO HELP
							
							Pupikin					= 2683
							Ice Cube				= 7441

						]]							
						
						ESPECIAL_ITEMS              = {2683, 7441, 7442, 7444, 7445, 11343, 11344, 11345},
						ESPECIAL_ITEMS_AFTER        = {2096, 7442, 7444, 7445, 7446, 11344, 11345, 11346},
						CHANCE_ESPECIAL_PERCENTE	= { 100,   70,   60,   40,   20,   50,     40, 	  20},
				
						--[[ ARRAY NUMBER OF ESPECIAL BOSSES
							The Mutated Pupink		= 8961
						]]		

						ESPECIAL_BOSSES_CORPSES 		= 	{8961},
						ESPECIAL_BOSSES_RANDOM_ITEMS 	=	{ 
																[8961] = {
																																					-- Segundos
																			[1] 	= {itemid = 9006, CountMax = 1,  WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																			[2] 	= {itemid = 8860, CountMax = 1,  WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																			[3]		= {itemid = 2688, CountMax = 50, WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																			[4]		= {itemid = 6571, CountMax = 1,  WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																			[5] 	= {itemid = 6492, CountMax = 1,  WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																			[6] 	= {itemid = 6574, CountMax = 1,  WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																			[7] 	= {itemid = 6526, CountMax = 1,  WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																			[8] 	= {itemid = 2096, CountMax = 1,  WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																			[9] 	= {itemid = 9005, CountMax = 20, WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																			[10] 	= {itemid = 6570, CountMax = 1,  WaitStorage = 23000, TimeWaitValue = 60 * 60 },
																		
																			
																		}, 
															},
					 }

		if isInArray(SKINNING.CORPSES, itemEx.itemid) then 
			if (itemEx.itemid ~= 9010) then
				for i = 1, #SKINNING.CORPSES do	
					if SKINNING.CORPSES[i] ==  itemEx.itemid then
						local PERCENT = math.random(1, 100)
							if (PERCENT) <= (SKINNING.CHANCE_PERCENTE[i]) then	
								doTransformItem(itemEx.uid,SKINNING.CORPSES_AFTER[i])
								doSendMagicEffect(toPosition, 14)
								doPlayerAddItem(cid,SKINNING.SKINNED_ITEM[i],1)
							else
								doSendMagicEffect(toPosition, CONST_ME_BLOCKHIT)
								doTransformItem(itemEx.uid,SKINNING.CORPSES_AFTER[i])	
							end
						break
					end
				end
			else 
				if (getPlayerStorageValue(cid,SKINNING.LORD_OF_ELEMENTS_STORAGE) == -1) then
					setPlayerStorageValue(cid,SKINNING.LORD_OF_ELEMENTS_STORAGE)
					doSendMagicEffect(toPosition, 14)
					doPlayerAddItem(cid,8310,1)								
				else
					return false
				end
			end
		elseif 	isInArray(SKINNING.ESPECIAL_ITEMS, itemEx.itemid) then
				for i = 1, #SKINNING.ESPECIAL_ITEMS do	
					if SKINNING.ESPECIAL_ITEMS[i] ==  itemEx.itemid then
						local PERCENT = math.random(1, 100)
							if (PERCENT) <= (SKINNING.CHANCE_ESPECIAL_PERCENTE[i]) then	
								doTransformItem(itemEx.uid,SKINNING.ESPECIAL_ITEMS_AFTER[i])
								doSendMagicEffect(toPosition, 14)
							else
								doSendMagicEffect(toPosition, CONST_ME_BLOCKHIT)
								doRemoveItem(itemEx.uid,1)
							end
						break
					end
				end		
		elseif 	isInArray(SKINNING.ESPECIAL_BOSSES_CORPSES, itemEx.itemid) then				
				for i = 1, #SKINNING.ESPECIAL_BOSSES_CORPSES do	
					if SKINNING.ESPECIAL_BOSSES_CORPSES[i] ==  itemEx.itemid then
							local TempCorpse =  SKINNING.ESPECIAL_BOSSES_CORPSES[i]
							local TotalItems =  table.maxn(SKINNING.ESPECIAL_BOSSES_RANDOM_ITEMS[TempCorpse])
							local RandonItem  = math.random(1, TotalItems)
							local TempItem 	  = SKINNING.ESPECIAL_BOSSES_RANDOM_ITEMS[TempCorpse][RandonItem]
							
							if (getPlayerStorageValue(cid, TempItem.WaitStorage) - os.time() <= 0) then
								setPlayerStorageValue(cid,SKINNING.LORD_OF_ELEMENTS_STORAGE)
								time = os.time() + ( TempItem.TimeWaitValue )
								setPlayerStorageValue(cid, TempItem.WaitStorage, time)								
								doPlayerAddItem(cid,TempItem.itemid,math.random(1, TempItem.CountMax))	
							else
								return false
							end											
						break
					end
				end			
		else
			return false
		end
			
	return true
end