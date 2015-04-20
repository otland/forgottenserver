print(">> Loading lua monsters")
function MonsterType.register(self, mask)
	
	if mask.description then
		self:setNameDescription(mask.description)
	end
	
	if mask.experience then
		self:setExperience(mask.experience)
	end
	
	if mask.outfit then
		self:setOutfit(mask.outfit)
	end
	
	if mask.maxHealth then
		self:setMaxHealth(mask.maxHealth)
	end
	
	if mask.health then
		self:setHealth(mask.health)
	end
	
	if mask.runHealth then
		self:setRunHealth(mask.runHealth)
	end
	
	if mask.maxSummons then
		self:setMaxSummons(mask.maxSummons)
	end
	
	if mask.race then
		self:setRace(mask.race)
	end
	
	if mask.manaCost then
		self:setManaCost(mask.manaCost)
	end
	
	if mask.speed then
		self:setBaseSpeed(mask.speed)
	end
	
	if mask.corpse then
		self:setCorpseId(mask.corpse)
	end
	
	if mask.flags then
		if mask.flags.attackable then
			self:setIsAttackable(mask.flags.attackable)
		end
		if mask.flags.healthHidden then
			self:setIsHealthHidden(mask.flags.healthHidden)
		end
		if mask.flags.convinceable then
			self:setIsConvinceable(mask.flags.convinceable)
		end
		if mask.flags.illusionable then
			self:setIsIllusionable(mask.flags.illusionable)
		end
		if mask.flags.hostile then
			self:setIsHostile(mask.flags.hostile)
		end
		if mask.flags.pushable then
			self:setIsPushable(mask.flags.pushable)
		end
	end
	
	if mask.canPushItems then
		self:setCanPushItems(mask.canPushItems)
	end
	
	if mask.canPushCreatures then
		self:setCanPushCreatures(mask.canPushCreatures)
	end
	
	if mask.skull then
		self:setSkull(mask.skull)
	end
	
	if mask.light then
		if mask.light.color then
			local color = mask.light.color
		end
		if mask.light.level then
			self:setLight(color, mask.light.level)
		end
	end
	
	if mask.changeTarget then
		if mask.changeTarget.chance then
			local chance = mask.changeTarget.chance
		end
		if mask.changeTarget.speed then
			self:setChangeTarget(chance, mask.changeTarget.speed)
		end
	end
	
	if type(mask.voices) == "table" then
		for k, v in pairs(mask.voices) do
			self:addVoice(v.text, v.interval, v.chance, v.yell)
		end
	end
	
	if type(mask.summons) == "table" then
		for k, v in pairs(mask.summonss) do
			self:addSummon(v.name, v.interval, v.chance)
		end
	end
	
	if type(mask.events) == "table" then
		for k, v in pairs(mask.events) do
			self:registerEvent(v)
		end
	end
	
	if type(mask.loot) == "table" then
		for _, loot in pairs(mask.loot) do
			local parent = Loot()
			parent:setId(loot.id)
			if loot.chance then
				parent:setChance(loot.chance)
			end
			if loot.maxCount then
				parent:setMaxCount(loot.maxCount)
			end
			if loot.aid or loot.actionId then
				parent:setActionId(loot.aid or loot.actionId)
			end
			if loot.subType or loot.charges then
				parent:setSubType(loot.subType or loot.charges)
			end
			if loot.text or loot.description then
				parent:setDescription(loot.text or loot.description)
			end
			if loot.child then
				for _, childs in pairs(loot.child) do
					local child = Loot()
					child:setId(childs.id)
					if childs.chance then
						child:setChance(childs.chance)
					end
					if childs.maxCount then
						child:setMaxCount(childs.maxCount)
					end
					if childs.aid or childs.actionId then
						child:setActionId(childs.aid or childs.actionId)
					end
					if childs.subType or childs.charges then
						child:setSubType(childs.subType or childs.charges)
					end
					if childs.text or childs.description then
						child:setDescription(childs.text or childs.description)
					end
					parent:addChildLoot(child)
				end
			end
			self:addLoot(parent)
		end
	end	
end