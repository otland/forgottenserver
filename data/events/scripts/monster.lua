-- Rarity Animations
local rare_popup = true
local rare_effect = true
local rare_effect_id = CONST_ME_STUN

-- Custom special item drops
local jewelDrops = false -- turned off by default
local jewelChance = 2 -- 2%
local jewelCorpse = {
    [5995] = { -- Demon
        2493, -- Demon Helmet
        2494, -- Demon Armor
        2495, -- Demon Legs
        2400, -- Magic Sword
        2431, -- SCA
        2421, -- Thunder Hammer
        16112,-- Spellbook of Arcana
        8852, -- The Devileye
        8905,  -- Rainbow Shield
        2131  -- Star Amulet
    },
    [5999] = { -- Behemoth
        2503,  -- Dwarven Armor
        2504,  -- Dwarven Legs
        2502   -- Dwarven Helmet
    },
    [6332] = { -- Hellhound
        15406, -- Ornate Chest
        15414, -- Ornate Mace
        15412, -- Ornate Legs
        15644, -- Ornate Crossbow
        15407, -- Depth Lorica
        8905,  -- Rainbow Shield
        22424, -- Master Umbral Spellbook
        12644, -- Shield of Corruption
        12649, -- Blade of Corruption
        7390,  -- Justice Seeker
        7435   -- Impaler
    },
    [8955] = { -- Grim Reaper
        9778,  -- Yalahari Mask
        9776,  -- Yalahari Armor
        7429,  -- Blessed Sceptre
        14327, -- Bronze Ring
        12645  -- Elite Draken Helmet
    },
    [5984] = { -- Dragon Lord
        11118, -- Dragon Scale Boots
        2506,  -- Dragon Scale Helmet
        8886,  -- Molten Plate
    },
    [19963] = {-- Lost Basher
        2503,  -- Dwarven Armor
        2504,  -- Dwarven Legs
        2502,  -- Dwarven Helmet
    },
    [19998] = {-- Lost Thrower
        2503,  -- Dwarven Armor
        2504,  -- Dwarven Legs
        2502,  -- Dwarven Helmet
    },
    [17408] = {-- Enslaved Dwarf
        2503,  -- Dwarven Armor
        2504,  -- Dwarven Legs
        2502,  -- Dwarven Helmet
    },
    [17416] = {-- Lost Berserker
        2503,  -- Dwarven Armor
        2504,  -- Dwarven Legs
        2502,  -- Dwarven Helmet
    },
    [6320] = { -- Destroyer
        2503,  -- Dwarven Armor
        2504,  -- Dwarven Legs
        2502,  -- Dwarven Helmet
        7455,  -- Mythril Axe
        7390,  -- Justice Seeker
        8888   -- MAA
    },
    [6324] = { -- Hellfire Fighter
        18409, -- Wand of Everblazing
        8925,  -- Solar Axe
        7417,  -- Runed Sword
        8927,  -- Dark Trinity Mace
        8905,  -- Rainbow Shield
        12647, -- Snakegod's Wristguard
        8881   -- Fireborn
    },
    [6336] = { -- Juggernaut
        2522,  -- Great Shield
        21693, -- Horn
        8882,  -- Earthborn
        8930,  -- Emerald Sword
        8929,  -- Stomper
        8932,  -- The Calamity
        7405,  -- Havoc
        2415   -- Great Axe
    },
    [10524] = { -- Medusa
        8869,   -- Greenwood Coat
        2537,   -- Amazon Shield
        5803    -- Arbalest
    },
    [8951] = { -- Bog Raider
        21706, -- Goo Shell
        23528  -- Glooth Cape
    },
    [6532] = { -- Defiler
        21706  -- Goo Shell
    },
    [6061] = { -- Serpent Spawn
        12642, -- Royal Draken Mail
        12643, -- Royal Scale Robe
        8851   -- Royal Crossbow
    },
    [6012] = { -- Elf Scout
        2505   -- Elven Mail
    },
    [6011] = { -- Elf Arcanist
        2505   -- Elven Mail
    },
    [6028] = { -- Lich
        7429,  -- Blessed Sceptre
        7730   -- Blue Legs

    },
    [15354] = { -- Kollos
        8883,   -- Windborn
        16111,  -- Thorn Spitter
        15413   -- Ornate Shield
    },
    [15296] = { -- Spidris
        11374,  -- Beetle Necklace
        16111   -- Thorn Spitter
    },
    [20455] = { -- Necromancer
        6433,   -- Necromancer Shield
        7429    -- Blessed Sceptre
    },
    [9923] = { -- Hellspawn
        12607, -- Elite Draken Mail
        7390,  -- Justice Seeker
        21693, -- Horn
        7433   -- Ravenwing
    },
    [6328] = { -- Dark Torturer
        12607, -- Elite Draken Mail
        12644, -- Shield of Corruption
        12649, -- Blade of Corruption
        8924,  -- Hellforged Axe
        8927,  -- Dark Trinity Mace
        21693, -- Horn
        7453,  -- Executioner
        6391,  -- Nightmare Shield
        2415   -- Great Axe
    },
    [6048] = { -- Hydra
        2424,  -- Silver Mace
        14327, -- Bronze Ring
        11305, -- Drakinata
        7411,  -- Ornamented Axe
        7384,  -- Mystic Blade
        2508   -- Native Armor
    },
    [20527] = { -- Warlock
        2542,   -- Tempest Shield
        7410,   -- Queen's Sceptre
        2662    -- Magician Hat
    },
    [20415] = { -- Hero
        8885,   -- Divine Plate
        11302,  -- Zaoan Helmet
        7384,   -- Mystic Blade
        2424,   -- Silver Mace
        14327,  -- Bronze Ring
        2508    -- Native Armor
    }
}

function Monster:onDropLoot(corpse)
	if configManager.getNumber(configKeys.RATE_LOOT) == 0 then
		return
	end

	local player = Player(corpse:getCorpseOwner())
	local mType = self:getType()
	if not player or player:getStamina() > 840 then
		local monsterLoot = mType:getLoot()
		for i = 1, #monsterLoot do
			local item = corpse:createLootItem(monsterLoot[i])
			if not item then
				print('[Warning] DropLoot:', 'Could not add loot item to corpse.')
			end
		end

		-- Apply rarity chance to corpse contents and apply animation
		if rollRarity(corpse) > 0 then -- If a rare item was rolled, play animation
				if rare_popup then
					local spectators = Game.getSpectators(corpse:getPosition(), false, true, 7, 7, 5, 5)
					for i = 1, #spectators do
						spectators[i]:say(rare_text, TALKTYPE_MONSTER_SAY, false, spectators[i], corpse:getPosition())
					end
				end
			if rare_effect then
				corpse:getPosition():sendMagicEffect(rare_effect_id)
			end
		end

		if jewelDrops then
			if jewelCorpse[corpse:getId()] then
				if math.random(1,100) <= jewelChance then
					local jewelCase = corpse:addItem(6104, 1)
					if jewelCase then
						local jewelItem = jewelCase:addItem(jewelCorpse[corpse:getId()][math.random(1,#jewelCorpse[corpse:getId()])], 1)
						if rollCheck(jewelItem) then
							rollRarity(jewelItem, true) -- Force roll a rarity
						end
					end
				end
			end
		end

		if player then
			local text = ("Loot of %s: %s"):format(mType:getNameDescription(), corpse:getContentDescription())
			local party = player:getParty()
			if party then
				party:broadcastPartyLoot(text)
			else
				player:sendTextMessage(MESSAGE_LOOT, text)
			end
		end
	else
		local text = ("Loot of %s: nothing (due to low stamina)"):format(mType:getNameDescription())
		local party = player:getParty()
		if party then
			party:broadcastPartyLoot(text)
		else
			player:sendTextMessage(MESSAGE_LOOT, text)
		end
	end
end

function Monster:onSpawn(position, startup, artificial)
    self:registerEvent("rollHealth")
    self:registerEvent("rollMana")
   return true
end
