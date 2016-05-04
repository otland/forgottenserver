--[[
	-- Don't forget to put new potions in actions.xml before adding here!
	Values of potions are copied from tibia wiki!
	-- Explanations!
	removePotions : removing the potion after use. (optional)
	addEmptyPotions : add the empty flask after using! ( only works if removePotions ) (optional)
	words : to say after using!
	healthRate : affects the formula of health/mana added (min * healthRate), (max * healthRate) - Default: 1 (required)
	manaRate : same as healthRate but, for mana - Default: 1 (required)
	-- For every potion!
	[potionId] = {
		vocations : the vocations that are able to use the potion! (optional)
		minLevel : minimum level to use the potion (optional)
		health/mana : (required)
			min, max : the Formula of adding health (i.e , the potion addes from 150 ~ 200 health)
		emptyPotion : the empty flask to add to player (optional)
	}
]]
potions = {
	-- The values used in every config is copied from tibia wiki!
	removePotions = true,
	addEmptyPotions = true,
	words = "Aaaah...",
	healthRate = 1,
	manaRate = 1,	
	-- string to say if player can't use the potion!
	-- First skip is for vocations, second skip is for level!
	stringOfVocation = "This potion can only be consumed by %s of level %s or higher", -- (Required*)
	-- Health & Spirit Potions
	[8473] = { -- ultimate health potion
		vocations = {4, 8},
		minLevel = 130,
		health = { min = 650, max = 850 },
		emptyPotion = 7635
	},
	[7591] = { -- great health potion
		vocations = {4, 8},
		minLevel = 80,
		health = { min = 425, max = 575},
		emptyPotion = 7635
	},
	[8472] = { -- great spitit potion
		vocations = {3, 7},
		minLevel = 80,
		health = { min = 250, max = 350},
		mana = { min = 100, max = 200},
		emptyPotion = 7635
	},
	[7588] = { -- strong health potion
		vocations = {3, 4, 7, 8},
		minLevel = 50,
		health = { min = 250, max = 350},
		emptyPotion = 7634
	},
	[7618] = { -- health potion
		health = { min = 125, max = 175},
		emptyPotion = 7636
	},
	-- Mana Potions
	[7590] = { -- great mana potion
		vocations = {1, 2, 5, 6},
		minLevel = 80,
		mana = { min = 150, max = 250},
		emptyPotion = 7635
	},
	[7589] = { -- strong mana potion
		vocations = {1, 2, 3, 5, 6, 7},
		minLevel = 50,
		mana = { min = 115, max = 185},
		emptyPotion = 7634
	},
	[7620] = { -- mana potion
		mana = { min = 75, max = 125},
		emptyPotion = 7636
	},
	[8704] = { -- small health potion
		health = { min = 60, max = 90},
		emptyPotion = 7636
	}
}
--[[ available custom methods!
	combat : MetatableCombat
]]
-- antidote
local antidote = Combat()
antidote:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
antidote:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
antidote:setParameter(COMBAT_PARAM_TARGETCASTERORTOPMOST, true)
antidote:setParameter(COMBAT_PARAM_AGGRESSIVE, false)
antidote:setParameter(COMBAT_PARAM_DISPEL, CONDITION_POISON)
potions[8474] = {combat = antidote, emptyPotion = 7636}

--- FUNCTIONS ---

potions.exhaust = Condition(CONDITION_EXHAUST_HEAL)
potions.exhaust:setParameter(CONDITION_PARAM_TICKS, (configManager.getNumber(configKeys.EX_ACTIONS_DELAY_INTERVAL) - 100))
-- 1000 - 100 due to exact condition timing. -100 doesn't hurt us, and players don't have reminding ~50ms exhaustion.

function onUse(player, potion, fromPosition, target, toPosition, isHotkey)
	if target == nil or not target:isPlayer() then
		return true
	end
	local stats = potions[potion:getId()]
	if stats then
		if (stats.vocations or stats.minLevel) and not player:getGroup():getAccess() then
			if not stats.minLevel then
				stats.minLevel = 1
			end
			if not stats.vocations then
				stats.vocations = { [1] = player:getVocation():getId() }
			end
			if not isInArray(stats.vocations, player:getVocation():getId()) or player:getLevel() < stats.minLevel then
				local str, vocations = "", {}
				-- We only show unpromoted vocations in description.
				for k, v in ipairs(stats.vocations) do
					if not Vocation(v):getDemotion() then
						table.insert(vocations, v)
					end
				end
				-- If we prefered to only put promoted vocations, we only show those who are lowered!
				-- Instead of showing (elite knigts, epic knights, ..), only shows (elite knights)
				-- It don't get affected by adding different vocations!
				if #vocations <= 0 then
					local function tablefind(t, value)
						for k, v in ipairs(t) do
							if v == value then
								return true
							end
						end
						return false
					end
					for k, v in ipairs(stats.vocations) do
						if not tablefind(stats.vocations, Vocation(v):getDemotion():getId()) then
							table.insert(vocations, v)
						end
					end
				end
				for i = 1, #vocations do
					if i == #vocations then
						str = str .. (#vocations ~= 1 and " and " or "") .. Vocation(vocations[i]):getName():lower() .. "s"
					else
						str = str .. (i ~= 1 and ", " or "") .. Vocation(vocations[i]):getName():lower() .. "s"
					end
				end
				vocations = nil
				if str == "" then
					str = "players"
				end
				player:say(string.format(potions.stringOfVocation, str, stats.minLevel), TALKTYPE_MONSTER_SAY)
				return false
			end
		end
		-- Condition exists after the check!
		if player:getCondition(CONDITION_EXHAUST_HEAL) then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_YOUAREEXHAUSTED))
			return true
		end
		if stats.health then
			if not doTargetCombatHealth((target.uid == player.uid and player.uid or 0), target, COMBAT_HEALING, stats.health.min * (potions.healthRate and potions.healthRate or 1),
					stats.health.max * (potions.healthRate and potions.healthRate or 1), CONST_ME_MAGIC_BLUE) then
				return false
			end
		end
		if stats.mana then
			if not doTargetCombatMana((target.uid == player.uid and player.uid or 0), target, stats.mana.min * (potions.manaRate and potions.manaRate or 1),
					stats.mana.max * (potions.manaRate and potions.manaRate or 1), CONST_ME_MAGIC_BLUE) then
				return false
			end
		end
		if stats.combat then
			if not stats.combat:execute(target, numberToVariant(target:getId())) then
				return false
			end
		end
		if potions.words then
			target:say(potions.words, TALKTYPE_MONSTER_SAY)
		end

		if potions.removePotions then
			if potions.addEmptyPotions and stats.emptyPotion then
				-- potion's empty potion should be in player's backpack only if the potion was in his backpack!
				if potion:getTopParent().uid == player.uid then
					player:addItem(stats.emptyPotion)
				else
					Game.createItem(stats.emptyPotion, 1, potion:getPosition())
				end
			end
			-- Removing should be last part not to cause errors if only 1 potion (nil value).
			potion:remove(1)
		end
		-- Access members aren't affected by the exaust!
		if not player:getGroup():getAccess() then
			player:addCondition(potions.exhaust) -- Last thing to do is adding condition!
		end
	end
	return true
end
