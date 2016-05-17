--[[
	-- Don't forget to put new potions in actions.xml in addition to adding here!
	Values of potions are copied from tibia wiki!
	-- Explanations!
	effect : effect to send to player after using potion! Default : CONST_ME_MAGIC_BLUE (required)
	removePotions : removing the potion after use. Default : true (optional)
	addEmptyFlasks : add the empty flask after using! ( only works if removePotions ) Default : true (optional)
	words : to say after using! Default : "Aaaah..." (optional)
	healthRate : affects the formula of health/mana added (min * healthRate), (max * healthRate) -- Default: 1 (required)
	manaRate : same as healthRate but, for mana -- Default: 1 (required)
	-- For every potion!
	[potionId] = {
		vocations : the vocations that are able to use the potion! (optional)
		minLevel : minimum level to use the potion (optional)
		health/mana : (required)
			min, max : the Formula of adding health (i.e , the potion addes from 150 ~ 200 health) (required)
		emptyFlask : the empty flask to add to player (optional)
	}
]]
local potions = {
	-- These values are default values in tibia, copied from tibiawiki.
	effect = CONST_ME_MAGIC_GREEN,
	removePotions = true,
	addEmptyFlasks = true,
	words = "Aaaah...",
	healthRate = 1,
	manaRate = 1,
	-- string to say if player can't use the potion!
	-- First skip is for vocations(string), second skip is for level(int)!
	stringOfVocation = "This potion can only be consumed by %s of level %d or higher", -- (Required*)
	-- Health & Spirit Potions
	[8473] = { -- ultimate health potion
		vocations = {4, 8},
		minLevel = 130,
		healthToAdd = { min = 650, max = 850 },
		emptyFlask = 7635
	},
	[7591] = { -- great health potion
		vocations = {4, 8},
		minLevel = 80,
		healthToAdd = { min = 425, max = 575},
		emptyFlask = 7635
	},
	[8472] = { -- great spitit potion
		vocations = {3, 7},
		minLevel = 80,
		healthToAdd = { min = 250, max = 350},
		manaToAdd = { min = 100, max = 200},
		emptyFlask = 7635
	},
	[7588] = { -- strong health potion
		vocations = {3, 4, 7, 8},
		minLevel = 50,
		healthToAdd = { min = 250, max = 350},
		emptyFlask = 7634
	},
	[7618] = { -- health potion
		healthToAdd = { min = 125, max = 175},
		emptyFlask = 7636
	},
	-- Mana Potions
	[7590] = { -- great mana potion
		vocations = {1, 2, 5, 6},
		minLevel = 80,
		manaToAdd = { min = 150, max = 250},
		emptyFlask = 7635
	},
	[7589] = { -- strong mana potion
		vocations = {1, 2, 3, 5, 6, 7},
		minLevel = 50,
		manaToAdd = { min = 115, max = 185},
		emptyFlask = 7634
	},
	[7620] = { -- mana potion
		manaToAdd = { min = 75, max = 125},
		emptyFlask = 7636
	},
	[8704] = { -- small health potion
		healthToAdd = { min = 60, max = 90},
		emptyFlask = 7636
	}
}
--[[ available custom methods!
	combat : MetatableCombat
	condition : MetatableCondition (not used)
]]
-- antidote
local antidote = Combat()
antidote:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
antidote:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
antidote:setParameter(COMBAT_PARAM_TARGETCASTERORTOPMOST, true)
antidote:setParameter(COMBAT_PARAM_AGGRESSIVE, false)
antidote:setParameter(COMBAT_PARAM_DISPEL, CONDITION_POISON)
potions[8474] = {combat = antidote, emptyFlask = 7636}

--- FUNCTIONS ---

potions.exhaust = Condition(CONDITION_EXHAUST_HEAL)
potions.exhaust:setParameter(CONDITION_PARAM_TICKS, (configManager.getNumber(configKeys.EX_ACTIONS_DELAY_INTERVAL) - 100))
-- 1000 - 100 due to exact condition timing. -100 doesn't hurt us, and players don't have reminding ~50ms exhaustion.

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target == nil or not target:isPlayer() then
		return false
	end

	local potion = potions[item:getId()]
	if potion then
		if (potion.vocations or potion.minLevel) and not getPlayerFlagValue(player, PlayerFlag_IgnoreSpellCheck) then
			potion.minLevel = math.max(potion.minLevel, 1)

			if not potion.vocations then
				potion.vocations = { [1] = player:getVocation():getId() }
			end

			if not isInArray(potion.vocations, player:getVocation():getId()) or player:getLevel() < potion.minLevel then
				local vocationString, vocations = "", {}
				-- We only show unpromoted vocations in description.
				for key, vocationId in ipairs(potion.vocations) do
					if not Vocation(vocationId):getDemotion() then
						table.insert(vocations, vocationId)
					end
				end

				-- If we prefered to only put promoted vocations, we only show those who are lowered!
				-- Instead of showing (elite knigts, epic knights, ..), only shows (elite knights)
				-- It don't get affected by adding different vocations!
				if #vocations == 0 then
					table.find = function(table, toFind)
						for key, value in ipairs(table) do
							if value == toFind then
								return true
							end
						end
						return false
					end

					for key, vocationId in ipairs(potion.vocations) do
						if not table.find(potion.vocations, Vocation(vocationId):getDemotion():getId()) then
							table.insert(vocations, vocationId)
						end
					end
				end

				for i = 1, #vocations do
					local stringPtr
					if i == #vocations then
						stringPtr = #vocations ~= 1 and " and " or ""
					else
						stringPtr = i ~= 1 and ", " or ""
					end

					vocationString = vocationString .. stringPtr .. Vocation(vocations[i]):getName():lower() .. "s"
				end

				if vocationString:len() == 0 then
					vocationString = "players"
				end

				if not potion.stringOfVocation then
					potion.stringOfVocation = "This potion can only be consumed by %s of level %d or higher"
				end

				return not player:say(string.format(potions.stringOfVocation, vocationString, potion.minLevel), TALKTYPE_MONSTER_SAY)
			end
		end

		-- Condition exists after the check!
		if player:getCondition(CONDITION_EXHAUST_HEAL) then
			return player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_YOUAREEXHAUSTED))
		end

		local effect = potions.effect or CONST_ME_MAGIC_BLUE

		if potion.healthToAdd then
			local min, max = potion.healthToAdd.min * (potions.healthRate or 1), potion.healthToAdd.max * (potions.healthRate or 1)
			if not doTargetCombatHealth(0, target, COMBAT_HEALING, min, max, effect) then
				return false
			end
		end

		if potion.manaToAdd then
			local min, max = potion.manaToAdd.min * (potions.manaRate or 1), potion.manaToAdd.max * (potions.manaRate or 1)
			if not doTargetCombatMana(0, target, min, max, effect) then
				return false
			end
		end

		if potion.combat and not potion.combat:execute(target, numberToVariant(target:getId())) then
			return false
		end

		if potion.condition and potion.condition:getType() ~= CONDITION_EXHAUST_HEAL and not player:addCondition(potion.condition) then
			return false
		end

		if not potions.words then
			potions.words = "Aaaah..."
		end

		target:say(potions.words, TALKTYPE_MONSTER_SAY)

		-- The default behavior is removing even if nil.
		if potions.removePotions ~= false then
			if potions.addEmptyFlasks ~= false and potion.emptyFlask then
				-- potion's empty potion should be in player's backpack only if the potion was in his backpack!
				if item:getTopParent().uid == player.uid then
					player:addItem(potion.emptyFlask)
				else
					Game.createItem(potion.emptyFlask, 1, item:getPosition())
				end
			end

			-- Removing potion at the end to avoid <nil> error.
			item:remove(1)
		end

		if not getPlayerFlagValue(player, PlayerFlag_HasNoExhaustion) then
			player:addCondition(potions.exhaust) -- Last thing to do is adding condition!
		end
		return true
	end
	return false -- you can't use this object (potion isn't added to the table.)
end
