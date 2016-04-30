--- CONFIGURATION ---
potions = {
	removePots = true,
	addEmptyPots = true, -- Only works if removepots isn't false or nil
	words = "Aaaah...",
	[8473] = { -- ultimate health potion
		vocations = {4, 8},
		minLevel = 130,
		health = { min = 650, max = 850},
		empty = 7635
	},
	[7591] = { -- great health potion
		vocations = {4, 8},
		minLevel = 80,
		health = { min = 425, max = 575},
		empty = 7635
	},
	[7590] = { -- great mana potion
		vocations = {1, 2, 5, 6},
		minLevel = 80,
		mana = { min = 150, max = 250},
		empty = 7635
	},
	[8472] = { -- great spitit potion
		vocations = {3, 7},
		minLevel = 80,
		health = { min = 250, max = 350},
		mana = { min = 100, max = 200},
		empty = 7635
	},
	[7588] = { -- strong health potion
		vocations = {3, 4, 7, 8},
		minLevel = 50,
		health = { min = 250, max = 350},
		empty = 7634
	},
	[7589] = { -- strong mana potion
		vocations = {1, 2, 3, 5, 6, 7},
		minLevel = 50,
		mana = { min = 115, max = 185},
		empty = 7634
	},
	[7620] = { -- mana potion
		mana = { min = 75, max = 125},
		empty = 7636
	},
	[7618] = { -- health potion
		health = { min = 125, max = 175},
		empty = 7636
	},
	[8704] = { -- small health potion
		health = { min = 60, max = 90},
		empty = 7636
	}
}
-- antidote
local antidote = Combat()
antidote:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
antidote:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
antidote:setParameter(COMBAT_PARAM_TARGETCASTERORTOPMOST, true)
antidote:setParameter(COMBAT_PARAM_AGGRESSIVE, false)
antidote:setParameter(COMBAT_PARAM_DISPEL, CONDITION_POISON)
potions[8474] = {combat = antidote, empty = 7636}

--- FUNCTIONS ---

local exhaust = Condition(CONDITION_EXHAUST_HEAL)
exhaust:setParameter(CONDITION_PARAM_TICKS, (configManager.getNumber(configKeys.EX_ACTIONS_DELAY_INTERVAL) - 100))
-- 1000 - 100 due to exact condition timing. -100 doesn't hurt us, and players don't have reminding ~50ms exhaustion.

function onUse(player, potion, fromPosition, target, toPosition, isHotkey)
	if target == nil or not target:isPlayer() then
		return true
	end
	
	if player:getCondition(CONDITION_EXHAUST_HEAL) then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_YOUAREEXHAUSTED))
		return true
	end

	local stats = potions[potion:getId()]
	if stats then
		if (stats.vocations or stats.minLevel) and not player:getGroup():getAccess() then
			if not stats.minLevel then
				stats.minLevel = 1
			end
			if not stats.vocations then
				stats.vocations = {}
			end
			
			if not inArray(states.vocations, player:getVocation():getId()) or player:getLevel() < stats.minLevel then
				local str, vocations = "", {}
				-- We only show unpromoted vocations in description.
				for k, v in ipairs(stats.vocations) do
					if Vocation(v):getDemotion() then
						table.insert(vocations, v)
					end
				end
				if #vocations > 0 then
					for i = 1, #vocations do
						local voc = Vocation(vocations[i])
						if not voc:getDemotion() then -- Elder druid won't be showed;
							if i == #vocations then
								str = str .. (#vocations ~= 1 and " and " or "") .. voc:getName():lower() .. "s"
							else
								str = str .. (i ~= 1 and ", " or "") .. voc:getName():lower() .. "s"
							end
						end
					end
				end
				vocations = nil
				if str == "" then
					str = "players"
				end
				player:say("This potion can be consumed by ".. str .. " of level " .. stats.minLevel .. " or higher.", TALKTYPE_MONSTER_SAY)
				return false
			end
		end
		if stats.health then
			if not doTargetCombatHealth(0, target, COMBAT_HEALING, stats.health.min, stats.health.max, CONST_ME_MAGIC_BLUE) then
				return false
			end
		end
		if stats.mana then
			if not doTargetCombatMana(0, target, stats.mana.min, stats.mana.max, CONST_ME_MAGIC_BLUE) then
				return false
			end
		end
		
		if stats.combat then
			if not stats.combat:execute(target, numberToVariant(target:getId())) then
				return false
			end
		end
		
		player:addCondition(exhaust)
		if potions.words then
			target:say(potions.words, TALKTYPE_MONSTER_SAY)
		end
		if potions.removePots then
			item:remove(1)
			if potions.addEmptyPots and stats.empty then
				player:addItem(stats.empty, 1)
			end
		end
	end
	
	return true
end
