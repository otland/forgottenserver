local minLookType = 0
local maxLookType = 699
local blockLookType = {161, 162, 163, 164, 165, 166, 167, 168, 
                          169, 170, 171, 172, 173, 174, 175, 176, 
                          177, 178, 179, 180, 181, 182, 183, 184, 
                          185, 186, 187, 188, 189, 190, 191, 474, 
                          475, 476, 477, 478, 479, 480, 481, 482, 
                          483, 484, 485, 638, 639, 640, 641, 642, 
                          643, 645, 646, 652, 653, 654, 655, 656, 
                          657, 658, 659, 660, 661, 662, 663, 135, 
                          411, 415, 424, 439, 440, 468, 469, 518, 
                          519, 520, 524, 525, 536, 543, 549, 576, 
                          581, 582, 597, 616, 623, 635, 1}


function onSay(player, words, param)
    if not player:getGroup():getAccess() then
        return true
    end

    local lookType = tonumber(param)

    if not lookType then
        player:sendCancelMessage("Param incorrect.")
        return false 
    end

    if lookType >= minLookType and lookType <= maxLookType and not isInArray(blockLookType, lookType) then
        local playerOutfit = player:getOutfit()
        playerOutfit.lookType = lookType
        player:setOutfit(playerOutfit)
    else
        player:sendCancelMessage("A look type with that id does not exist.")
    end
    return false
end
