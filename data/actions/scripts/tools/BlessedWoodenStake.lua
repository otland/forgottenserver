function onUse(cid, item, fromPosition, itemEx, toPosition)
	local SKINNING = { 
						--[[ ARRAY NUMBER OF CORPSES TO HELP
							
							Vampire					= 2956
							Vampire Viscount		= 21275
							Vampire Bride           = 9654
							Vampire Lords			= 8938
							Demons		 			= 2916
							
						]]	
						
						LORD_OF_ELEMENTS_STORAGE 	= {10011},
						CORPSES 					= {2956, 21275, 9654, 8938, 2916},
						CORPSES_AFTER 				= {2957, 21276, 9658, 8939, 2917},
						SKINNED_ITEM				= {5905,  5905, 5905, 5905, 5906},	
						CHANCE_PERCENTE 			= {	 100,    100,   100,   100,   100},
						
					}

		if isInArray(SKINNING.CORPSES, itemEx.itemid) then 
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
			return false
		end
			
	return true
end