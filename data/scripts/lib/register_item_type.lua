local abilitieMap = {
	["elementice"] = true,
	["elementearth"] = true,
	["elementfire"] = true,
	["elementenergy"] = true,
	["elementdeath"] = true,
	["elementholy"] = true,
	["invisible"] = true,
	["speed"] = true,
	["healthgain"] = true,
	["healthticks"] = true,
	["managain"] = true,
	["manaticks"] = true,
	["manashield"] = true,
	["skillsword"] = true,
	["skillaxe"] = true,
	["skillclub"] = true,
	["skilldist"] = true,
	["skillfish"] = true,
	["skillshield"] = true,
	["skillfist"] = true,
	["maxhitpoints"] = true,
	["maxhitpointspercent"] = true,
	["maxmanapoints"] = true,
	["maxmanapointspercent"] = true,
	["magicpoints"] = true,
	["magiclevelpoints"] = true,
	["magicpointspercent"] = true,
	["criticalhitchance"] = true,
	["criticalhitamount"] = true,
	["lifeleechchance"] = true,
	["lifeleechamount"] = true,
	["manaleechchance"] = true,
	["manaleechamount"] = true,
	["fieldabsorbpercentenergy"] = true,
	["fieldabsorbpercentfire"] = true,
	["fieldabsorbpercentpoison"] = true,
	["fieldabsorbpercentearth"] = true,
	["absorbpercentall"] = true,
	["absorbpercentallelements"] = true,
	["absorbpercentelements"] = true,
	["absorbpercentmagic"] = true,
	["absorbpercentenergy"] = true,
	["absorbpercentfire"] = true,
	["absorbpercentpoison"] = true,
	["absorbpercentearth"] = true,
	["absorbpercentice"] = true,
	["absorbpercentholy"] = true,
	["absorbpercentdeath"] = true,
	["absorbpercentlifedrain"] = true,
	["absorbpercentmanadrain"] = true,
	["absorbpercentdrown"] = true,
	["absorbpercentphysical"] = true,
	["absorbpercenthealing"] = true,
	["absorbpercentundefined"] = true,
	["suppressdrunk"] = true,
	["suppressenergy"] = true,
	["suppressfire"] = true,
	["suppresspoison"] = true,
	["suppressdrown"] = true,
	["suppressphysical"] = true,
	["suppressfreeze"] = true,
	["suppressdazzle"] = true,
	["suppresscurse"] = true
}

registerItemType = {}
setmetatable(registerItemType,
{
	__call =
	function(self, itype, mask)
		for f, v in pairs(mask) do
			local parse = self[f] or self[f:lower()]
			if parse then
				parse(itype, v)
			else
				local name = f:lower()
				if abilitieMap[name] then
					itype:abilities(name, v)
				else
					print("[Warning - registerItemType] Unknow parse name: " .. f .. " [ "..name.." ]")
				end
			end
		end
	end
})

if not ItemType.reg then
	ItemType.reg = ItemType.register
end

ItemType.register = function(self, mask)
	registerItemType(self, mask)
	return self:reg()
end

registerItemType.corpse = function(itype, corpse)
	if corpse then
		itype:corpse(corpse)
	end
end

registerItemType.door = function(itype, door)
	if door then
		itype:door(door)
	end
end

registerItemType.container = function(itype, container)
	if container then
		itype:container(container)
	end
end

registerItemType.fluidcontainer = function(itype, fluidContainer)
	if fluidContainer then
		itype:fluidContainer(fluidContainer)
	end
end

registerItemType.movable = function(itype, movable)
	if movable then
		itype:movable(movable)
	end
end
registerItemType.moveable = registerItemType.movable

registerItemType.rune = function(itype, rune)
	if rune then
		itype:rune(rune)
	end
end

registerItemType.stackable = function(itype, stackable)
	if stackable then
		itype:stackable(stackable)
	end
end

registerItemType.readable = function(itype, readable)
	if readable then
		itype:readable(readable)
	end
end

registerItemType.writeable = function(itype, writeable)
	if writeable then
		itype:writeable(writeable)
	end
end

registerItemType.blockprojectile = function(itype, blockProjectile)
	if blockProjectile then
		itype:blockProjectile(blockProjectile)
	end
end

registerItemType.blocking = function(itype, blocking)
	if blocking then
		itype:blocking(blocking)
	end
end

registerItemType.allowdistread = function(itype, allowDistRead)
	if allowDistRead then
		itype:allowDistRead(allowDistRead)
	end
end

registerItemType.rotatable = function(itype, rotatable)
	if rotatable then
		itype:rotatable(rotatable)
	end
end

registerItemType.groundtile = function(itype, groundTile)
	if groundTile then
		itype:groundTile(groundTile)
	end
end

registerItemType.magicfield = function(itype, magicField)
	if magicField then
		itype:magicField(magicField)
	end
end

registerItemType.useable = function(itype, useable)
	if useable then
		itype:useable(useable)
	end
end

registerItemType.pickupable = function(itype, pickupable)
	if pickupable then
		itype:pickupable(pickupable)
	end
end

registerItemType.allowpickupable = function(itype, allowPickupable)
	if allowPickupable then
		itype:allowPickupable(allowPickupable)
	end
end

registerItemType.ammotype = function(itype, ammoType)
	if ammoType then
		itype:ammoType(ammoType)
	end
end

registerItemType.corpsetype = function(itype, corpseType)
	if corpseType then
		itype:corpseType(corpseType)
	end
end

registerItemType.type = function(itype, type)
	if type then
		itype:type(type)
	end
end

registerItemType.id = function(itype, id)
	if id then
		itype:id(id)
	end
end

registerItemType.clientid = function(itype, clientId)
	if clientId then
		itype:clientId(clientId)
	end
end

registerItemType.name = function(itype, name)
	if name then
		itype:name(name)
	end
end

registerItemType.pluralname = function(itype, pluralName)
	if pluralName then
		itype:pluralName(pluralName)
	end
end
registerItemType.plural = registerItemType.pluralname

registerItemType.article = function(itype, article)
	if article then
		itype:article(article)
	end
end

registerItemType.description = function(itype, description)
	if description then
		itype:description(description)
	end
end

registerItemType.slotposition = function(itype, slotPosition)
	if slotPosition then
		itype:slotPosition(slotPosition)
	end
end

registerItemType.slottype = registerItemType.slotposition

registerItemType.charges = function(itype, charges)
	if charges then
		itype:charges(charges)
	end
end

registerItemType.fluidsource = function(itype, fluidSource)
	if fluidSource then
		itype:fluidSource(fluidSource)
	end
end

registerItemType.capacity = function(itype, capacity)
	if capacity then
		itype:capacity(capacity)
	end
end

registerItemType.weight = function(itype, weight)
	if weight then
		itype:weight(weight, true)
	end
end

registerItemType.hitchance = function(itype, hitChance)
	if hitChance then
		itype:hitChance(hitChance)
	end
end

registerItemType.shootrange = function(itype, shootRange)
	if shootRange then
		itype:shootRange(shootRange)
	end
end

registerItemType.range = function(itype, range)
	if range then
		itype:shootRange(range)
	end
end

registerItemType.shoottype = function(itype, shootType)
	if shootType then
		itype:shootType(shootType)
	end
end

registerItemType.attack = function(itype, attack)
	if attack then
		itype:attack(attack)
	end
end

registerItemType.defense = function(itype, defense)
	if defense then
		itype:defense(defense)
	end
end

registerItemType.extradefense = function(itype, extraDefense)
	if extraDefense then
		itype:extraDefense(extraDefense)
	end
end

registerItemType.extradef = registerItemType.extradefense

registerItemType.armor = function(itype, armor)
	if armor then
		itype:armor(armor)
	end
end

registerItemType.weapontype = function(itype, weaponType)
	if weaponType then
		itype:weaponType(weaponType)
	end
end

registerItemType.elementtype = function(itype, elementType)
	if elementType then
		itype:elementType(elementType)
	end
end

registerItemType.elementdamage = function(itype, elementDamage)
	if elementDamage then
		itype:elementDamage(elementDamage)
	end
end

registerItemType.transformequipid = function(itype, transformEquipId)
	if transformEquipId then
		itype:transformEquipId(transformEquipId)
	end
end

registerItemType.transformdeequipid = function(itype, transformDeEquipId)
	if transformDeEquipId then
		itype:transformDeEquipId(transformDeEquipId)
	end
end

registerItemType.destroyid = function(itype, destroyId)
	if destroyId then
		itype:destroyId(destroyId)
	end
end

registerItemType.decayid = function(itype, decayId)
	if decayId then
		itype:decayId(decayId)
	end
end

registerItemType.requiredlevel = function(itype, requiredLevel)
	if requiredLevel then
		itype:requiredLevel(requiredLevel)
	end
end

registerItemType.subtype = function(itype, subType)
	if subType then
		itype:subType(subType)
	end
end

registerItemType.abilities = function(itype, abilities)
	if abilities then
		for name, value in pairs(abilities) do
			itype:abilities(name, value)
		end
	end
end

registerItemType.storeitem = function(itype, storeItem)
	if storeItem then
		itype:storeItem(storeItem)
	end
end

registerItemType.showcount = function(itype, showCount)
	if showCount then
		itype:showCount(showCount)
	end
end

registerItemType.replaceable = function(itype, replaceable)
	if replaceable then
		itype:replaceable(replaceable)
	end
end

registerItemType.showcharges = function(itype, showCharges)
	if showCharges then
		itype:showCharges(showCharges)
	end
end

registerItemType.showattributes = function(itype, showAttributes)
	if showAttributes then
		itype:showAttributes(showAttributes)
	end
end

registerItemType.showduration = function(itype, showDuration)
	if showDuration then
		itype:showDuration(showDuration)
	end
end

registerItemType.floorchange = function(itype, floorChange)
	if floorChange then
		itype:floorChange(floorChange)
	end
end

registerItemType.leveldoor = function(itype, levelDoor)
	if levelDoor then
		itype:levelDoor(levelDoor)
	end
end

registerItemType.effect = function(itype, effect)
	if effect then
		itype:effect(effect)
	end
end

registerItemType.containersize = function(itype, containerSize)
	if containerSize then
		itype:containerSize(containerSize)
	end
end

registerItemType.rotateto = function(itype, rotateTo)
	if rotateTo then
		itype:rotateTo(rotateTo)
	end
end

registerItemType.partnerdirection = function(itype, partnerDirection)
	if partnerDirection then
		itype:partnerDirection(partnerDirection)
	end
end

registerItemType.femalesleeper = function(itype, femaleSleeper)
	if femaleSleeper then
		itype:femaleSleeper(femaleSleeper)
	end
end

registerItemType.malesleeper = function(itype, maleSleeper)
	if maleSleeper then
		itype:maleSleeper(maleSleeper)
	end
end

registerItemType.maxtextlen = function(itype, maxTextLen)
	if maxTextLen then
		itype:maxTextLen(maxTextLen)
	end
end

registerItemType.writeonceitemid = function(itype, writeOnceItemId)
	if writeOnceItemId then
		itype:writeOnceItemId(writeOnceItemId)
	end
end

registerItemType.runespellname = function(itype, runeSpellName)
	if runeSpellName then
		itype:runeSpellName(runeSpellName)
	end
end

registerItemType.maxhitchance = function(itype, maxHitChance)
	if maxHitChance then
		itype:maxHitChance(maxHitChance)
	end
end

registerItemType.duration = function(itype, duration)
	if duration then
		itype:duration(duration)
	end
end

registerItemType.decayto = function(itype, decayTo)
	if decayTo then
		itype:decayTo(decayTo)
	end
end

registerItemType.transformequipid = function(itype, transformEquipId)
	if transformEquipId then
		itype:transformEquipId(transformEquipId)
	end
end
registerItemType.transformequipto = registerItemType.transformequipid

registerItemType.transformdeequipid = function(itype, transformDeEquipId)
	if transformDeEquipId then
		itype:transformDeEquipId(transformDeEquipId)
	end
end
registerItemType.transformdeequipto = registerItemType.transformdeequipid

registerItemType.stopduration = function(itype, stopDuration)
	if stopDuration then
		itype:stopDuration(stopDuration)
	end
end

registerItemType.walkstack = function(itype, walkStack)
	if walkStack then
		itype:walkStack(walkStack)
	end
end

registerItemType.field = function(itype, field)
	if field then
		itype:field(field)
	end
end

registerItemType.vocationstring = function(itype, vocationString)
	if vocationString then
		itype:vocationString(vocationString)
	end
end

registerItemType.minreqlevel = function(itype, minReqLevel)
	if minReqLevel then
		itype:minReqLevel(minReqLevel)
	end
end

registerItemType.minreqmagiclevel = function(itype, minReqMagicLevel)
	if minReqMagicLevel then
		itype:minReqMagicLevel(minReqMagicLevel)
	end
end

