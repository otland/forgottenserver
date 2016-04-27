--- CONFIGURATION ---
potions = {
	removePots = true,
	addEmptyPots = true, -- Only works if removepots isn't false or nil
	words = "Aaaah...",
	[8473] = { -- ultimate health potion
		vocations = {4},
		minLevel = 130,
		health = {650, 850},  -- min, max
		empty = 7635
	},
	[7591] = { -- great health potion
		vocations = {4},
		minLevel = 80,
		health = {425, 575},
		empty = 7635
	},
	[7590] = { -- great mana potion
		vocations = {1, 2},
		minLevel = 80,
		mana = {150, 250},
		empty = 7635
	},
	[8472] = { -- great spitit potion
		vocations = {3},
		minLevel = 80,
		health = {250, 350},
		mana = {100, 200},
		empty = 7635
	},
	[7588] = { -- strong health potion
		vocations = {3, 4},
		minLevel = 50,
		health = {250, 350},
		empty = 7634
	},
	[7589] = { -- strong mana potion
		vocations = {1, 2, 3},
		minLevel = 50,
		mana = {115, 185},
		empty = 7634
	},
	[7618] = { -- health potion
		health = {75, 125},
		empty = 7636
	},
	[7620] = { -- mana potion
		mana = {125, 175},
		empty = 7636
	},
	[8704] = { -- small health potion
		health = {60, 90},
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

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target == nil or not target:isPlayer() then
		return true
	end
	
	if player:getCondition(CONDITION_EXHAUST_HEAL) then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_YOUAREEXHAUSTED))
		return true
	end

	local stats = potions[item:getId()]
	if stats then
		if (stats.vocations or stats.minLevel) and not player:getGroup():getAccess() then
			if not stats.minLevel then
				stats.minLevel = 1
			end
			if not stats.vocations then
				stats.vocations = {}
			end
			
			local vocationList, mvoc = {}, target:getVocation()
			while mvoc do
				table.insert(vocationList, mvoc:getId())
				mvoc = mvoc:getDemotion()
			end
			-- Checking vocation, level
			local isDifferentVocation = true
			if #stats.vocations > 0 then
				for k, v in ipairs(stats.vocations) do
					if isInArray(vocationList, v) then
						isDifferentVocation = false
						break
					end
				end
			else
				isDifferentVocation = false
			end
			vocationList = nil
			if isDifferentVocation or target:getLevel() < stats.minLevel then
				local str, vocations = "", stats.vocations
				if #vocations > 0 then
					for i = 1, #vocations do
						local voc = Vocation(vocations[i])
						if i == #vocations then
							str = str .. (#vocations ~= 1 and " and " or "") .. voc:getName():lower() .. "s"
						else
							str = str .. (i ~= 1 and ", " or "") .. voc:getName():lower() .. "s"
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
			local hmin, hmax = stats.health[1], stats.health[2]
			if not doTargetCombatHealth(0, target, COMBAT_HEALING, hmin, hmax, CONST_ME_MAGIC_BLUE) then
				return false
			end
		end
		if stats.mana then
			local mmin, mmax = stats.mana[1], stats.mana[2]
			if not doTargetCombatMana(0, target, mmin, mmax, CONST_ME_MAGIC_BLUE) then
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
