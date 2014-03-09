function onUse(cid, item, fromPosition, itemEx, toPosition)
    local i = Item(itemEx.uid)
    if(itemEx.uid <= 65535 or itemEx.actionid > 0 and itemEx.itemid == 354 or itemEx.itemid == 355) then
        i:transform(392)
        i:decay()
        toPosition:sendMagicEffect(CONST_ME_POFF)
        return true
    elseif(itemEx.itemid == 7200) then -- pick fragile ice --
        i:transform(7236)
        i:decay()
        toPosition:sendMagicEffect(CONST_ME_HITAREA)
        return true
    end
    return true
end
