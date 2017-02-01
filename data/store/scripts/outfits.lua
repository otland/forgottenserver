-- Load outfits
local outfits = {}

for line in io.lines('data/XML/outfits.xml') do
    local sex = tonumber(line:match('type.-=[\'"](%d-)[\'"]'))
    local lookType = tonumber(line:match('looktype.-=[\'"](.-)[\'"]'))
    local name = line:match('name.-=[\'"](.-)[\'"]')
    local enabled = line:match('enabled.-=[\'"](.-)[\'"]') == 'yes'
    local unlocked = line:match('unlocked.-=[\'"](.-)[\'"]') == 'yes'

	if enabled then
		local outfit = outfits[name:lower()]
		if not outfit then
			outfit = {}
			outfits[name:lower()] = outfit
		end
		table.insert(outfit, lookType)
		outfit.unlocked = unlocked
	end
end

local function getAffectedOutfit(offerName)
    local outfitName
    local addons
    if offerName:sub(1, 5) == 'full ' then
        outfitName = offerName:match('^full (.-) outfit$')
        addons = {1, 2}
    else
        outfitName = offerName:match('^(.+) outfit')
        addons = {tonumber(offerName:match('outfit addon (.-)$'))}
    end
 
    return outfits[outfitName], addons
end

function onRender(player, offer)
    local lookTypes, addons = getAffectedOutfit(offer:getName():lower())

    if not lookTypes then
        return false
    end

    for _, lookType in ipairs(lookTypes) do
        if #addons == 0 then
            if player:hasOutfit(lookType, 0) or lookTypes.unlocked then
                return false
            end
        elseif #addons == 1 then
            return (player:hasOutfit(lookType, 0) or lookTypes.unlocked) and not player:hasOutfit(lookType, addons[1])
        else
            for _, addon in ipairs(addons) do
                if player:hasOutfit(lookType, addon) then
                    return false
                end
            end
        end
    end

    return true
end

function onBuy(player, offer)
    local lookTypes, addons = getAffectedOutfit(offer:getName():lower())

    if not lookTypes then
        player:sendStoreError('Outfit "' .. offer:getName() .. '" was not found. Please contact the administrator.')
        return false
    end

    for _, lookType in ipairs(lookTypes) do
        player:addOutfit(lookType)
 
        for _, addon in ipairs(addons) do
            player:addOutfitAddon(lookType, addon)
        end
    end

    return true
end
