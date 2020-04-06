local config = {
	-- obsidian knife
	[5908] = {
		-- minotaurs
		[4272] = {chance = 7000, newItem = 5878, after = 4012}, -- minotaur
		[5969] = {chance = 7000, newItem = 5878, after = 4012}, -- minotaur, after being killed
		[4052] = {chance = 7000, newItem = 5878, after = 4053}, -- minotaur archer
		[5982] = {chance = 7000, newItem = 5878, after = 4053}, -- minotaur archer, after being killed
		[4047] = {chance = 7000, newItem = 5878, after = 4048}, -- minotaur mage
		[5981] = {chance = 7000, newItem = 5878, after = 4048}, -- minotaur mage, after being killed
		[4057] = {chance = 7000, newItem = 5878, after = 4058}, -- minotaur guard
		[5983] = {chance = 7000, newItem = 5878, after = 4058}, -- minotaur guard, after being killed
		[21092] = {chance = 7000, newItem = 5878, after = 21093}, -- mooh'tah warrior
		[21091] = {chance = 7000, newItem = 5878, after = 21093}, -- mooh'tah warrior, after being killed
		[21096] = {chance = 7000, newItem = 5878, after = 21097}, -- minotaur hunter
		[21095] = {chance = 7000, newItem = 5878, after = 21097}, -- minotaur hunter, after being killed
		[21100] = {chance = 7000, newItem = 5878, after = 21101}, -- worm priestess
		[21099] = {chance = 7000, newItem = 5878, after = 21101}, -- worm priestess, after being killed
		[21000] = {chance = 7000, newItem = 5878, after = 21002}, -- minotaur amazon
		[21001] = {chance = 7000, newItem = 5878, after = 21002}, -- minotaur amazon, after being killed
		[21004] = {chance = 7000, newItem = 5878, after = 21006}, -- execowtioner
		[21005] = {chance = 7000, newItem = 5878, after = 21006}, -- execowtioner, after being killed
		[20996] = {chance = 7000, newItem = 5878, after = 20998}, -- moohtant
		[20997] = {chance = 7000, newItem = 5878, after = 20998}, -- moohtant, after being killed
		-- low class lizards
		[4324] = {chance = 6000, newItem = 5876, after = 4325}, -- lizard sentinel
		[6040] = {chance = 6000, newItem = 5876, after = 4325}, -- lizard sentinel, after being killed
		[4327] = {chance = 6000, newItem = 5876, after = 4328}, -- lizard snakecharmer
		[6041] = {chance = 6000, newItem = 5876, after = 4328}, -- lizard snakecharmer, after being killed
		[4321] = {chance = 6000, newItem = 5876, after = 4322}, -- lizard templar
		[4239] = {chance = 6000, newItem = 5876, after = 4322}, -- lizard templar, after being killed
		-- high class lizards
		[10368] = {chance = 10000, newItem = 5876, after = 10369}, -- lizard chosen,
		[10371] = {chance = 10000, newItem = 5876, after = 10369}, -- lizard chosen, after being killed
		[10360] = {chance = 10000, newItem = 5876, after = 10361}, -- lizard dragon priest
		[10363] = {chance = 10000, newItem = 5876, after = 10361}, -- lizard dragon priest, after being killed
		[10352] = {chance = 10000, newItem = 5876, after = 10353}, -- lizard high guard
		[10355] = {chance = 10000, newItem = 5876, after = 10353}, -- lizard high guard, after being killed
		[10364] = {chance = 10000, newItem = 5876, after = 10365}, -- lizard zaogun
		[10367] = {chance = 10000, newItem = 5876, after = 10365}, -- lizard zaogun, after being killed
		-- dragon
		[4286] = {chance = 5000, newItem = 5877, after = 4287},
		[5973] = {chance = 5000, newItem = 5877, after = 4287}, -- after being killed
		-- dragon lord
		[4062] = {chance = 5000, newItem = 5948, after = 4063},
		[5984] = {chance = 5000, newItem = 5948, after = 4063}, -- after being killed
		-- behemoth
		[4112] = {chance = 10000, newItem = 5893, after = 4113},
		[5999] = {chance = 10000, newItem = 5893, after = 4113}, -- after being killed
		-- bone beast
		[4212] = {chance = 6000, newItem = 5925, after = 4213},
		[6030] = {chance = 6000, newItem = 5925, after = 4213}, -- after being killed
		-- clomp
		[22743] = {chance = 50000, newItem = 24842, after = 22744},
		[22742] = {chance = 50000, newItem = 24842, after = 22744}, -- after being killed
		-- piece of marble rock
		[10426] = {
			{chance = 530, newItem = 10429, desc = "This little figurine of a goddess was masterfully sculpted by |PLAYERNAME|."},
			{chance = 9600, newItem = 10428, desc = "This little figurine made by |PLAYERNAME| has some room for improvement."},
			{chance = 24000, newItem = 10427, desc = "This shoddy work was made by |PLAYERNAME|."}
		},
		-- ice cube
		[7441] = {chance = 22000, newItem = 7442},
		[7442] = {chance = 4800, newItem = 7444},
		[7444] = {chance = 900, newItem = 7445},
		[7445] = {chance = 40, newItem = 7446}
	},
	-- blessed wooden stake
	[5942] = {
		-- demon
		[4097] = {chance = 3000, newItem = 5906, after = 4098},
		[5995] = {chance = 3000, newItem = 5906, after = 4098}, -- after being killed
		-- vampires
		[4137] = {chance = 6000, newItem = 5905, after = 4138}, -- vampire
		[6006] = {chance = 6000, newItem = 5905, after = 4138}, -- vampire, after being killed
		[8738] = {chance = 6000, newItem = 5905, after = 8742}, -- vampire bride
		[8744] = {chance = 6000, newItem = 5905, after = 8742}, -- vampire bride, after being killed
		[18958] = {chance = 6000, newItem = 5905, after = 18959}, -- vampire viscount
		[18961] = {chance = 6000, newItem = 5905, after = 18959} -- vampire viscount, after being killed
	}
}

local skinning = Action()

function skinning.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local skin = config[item.itemid][target.itemid]
	if not skin then
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return true
	end

	local randomChance = math.random(1, 100000)
	local effect = CONST_ME_MAGIC_GREEN
	local transform = true
	if type(skin[1]) == "table" then
		local added = false
		for _, skinChild in ipairs(skin) do
			if randomChance <= skinChild.chance then
				if target.itemid == 10426 then
					local marble = player:addItem(skinChild.newItem, skinChild.amount or 1)
					if marble then
						marble:setAttribute(ITEM_ATTRIBUTE_DESCRIPTION, skinChild.desc:gsub("|PLAYERNAME|", player:getName()))
					end
					effect = CONST_ME_HITAREA
					target:remove()
					added = true
				else
					target:transform(skinChild.newItem, skinChild.amount or 1)
					effect = CONST_ME_HITAREA
					added = true
				end
				break
			end
		end
		if not added and target.itemid == 10426 then
			effect = CONST_ME_HITAREA
			player:say("Your attempt at shaping that marble rock failed miserably.", TALKTYPE_MONSTER_SAY)
			transform = false
			target:remove()
		end
	elseif randomChance <= skin.chance then
		if table.contains({7441, 7442, 7444, 7445}, target.itemid) then
			target:transform(skin.newItem, 1)
			effect = CONST_ME_HITAREA
		else
			player:addItem(skin.newItem, skin.amount or 1)
		end
	else
		if table.contains({7441, 7442, 7444, 7445}, target.itemid) then
			player:say("The attempt of sculpting failed miserably.", TALKTYPE_MONSTER_SAY)
			effect = CONST_ME_HITAREA
			target:remove()
		else
			effect = CONST_ME_BLOCKHIT
		end
	end
	if toPosition.x == CONTAINER_POSITION then
		toPosition = player:getPosition()
	end
	toPosition:sendMagicEffect(effect)
	if transform then
		target:transform(skin.after or target:getType():getDecayId() or target.itemid + 1)
	else
		target:remove()
	end
	return true
end

skinning:id(5908, 5942)
skinning:register()
