registerMonsterType = {}
setmetatable(registerMonsterType,
{
	__call =
	function(self, mtype, mask)
		for _,parse in pairs(self) do
			parse(mtype, mask)
		end
	end
})

MonsterType.register = function(self, mask)
	return registerMonsterType(self, mask)
end

registerMonsterType.name = function(mtype, mask)
	if mask.name then
		mtype:name(mask.name)
	end
end
registerMonsterType.description = function(mtype, mask)
	if mask.description then
		mtype:nameDescription(mask.description)
	end
end
registerMonsterType.experience = function(mtype, mask)
	if mask.experience then
		mtype:experience(mask.experience)
	end
end
registerMonsterType.skull = function(mtype, mask)
	if mask.skull then
		mtype:skull(mask.skull)
	end
end
registerMonsterType.outfit = function(mtype, mask)
	if mask.outfit then
		mtype:outfit(mask.outfit)
	end
end
registerMonsterType.maxHealth = function(mtype, mask)
	if mask.maxHealth then
		mtype:maxHealth(mask.maxHealth)
		mtype:health(math.min(mtype:health(), mask.maxHealth))
	end
end
registerMonsterType.health = function(mtype, mask)
	if mask.health then
		mtype:health(mask.health)
		mtype:maxHealth(math.max(mask.health, mtype:maxHealth()))
	end
end
registerMonsterType.runHealth = function(mtype, mask)
	if mask.runHealth then
		mtype:runHealth(mask.runHealth)
	end
end
registerMonsterType.maxSummons = function(mtype, mask)
	if mask.maxSummons then
		mtype:maxSummons(mask.maxSummons)
	end
end
registerMonsterType.race = function(mtype, mask)
	if mask.race then
		mtype:race(mask.race)
	end
end
registerMonsterType.manaCost = function(mtype, mask)
	if mask.manaCost then
		mtype:manaCost(mask.manaCost)
	end
end
registerMonsterType.speed = function(mtype, mask)
	if mask.speed then
		mtype:baseSpeed(mask.speed)
	end
end
registerMonsterType.corpse = function(mtype, mask)
	if mask.corpse then
		mtype:corpseId(mask.corpse)
	end
end
registerMonsterType.flags = function(mtype, mask)
	if mask.flags then
		if mask.flags.attackable ~= nil then
			mtype:isAttackable(mask.flags.attackable)
		end
		if mask.flags.healthHidden ~= nil then
			mtype:isHealthHidden(mask.flags.healthHidden)
		end
		if mask.flags.boss ~= nil then
			mtype:isBoss(mask.flags.boss)
		end
		if mask.flags.challengeable ~= nil then
			mtype:isChallengeable(mask.flags.challengeable)
		end
		if mask.flags.convinceable ~= nil then
			mtype:isConvinceable(mask.flags.convinceable)
		end
		if mask.flags.summonable ~= nil then
			mtype:isSummonable(mask.flags.summonable)
		end
		if mask.flags.ignoreSpawnBlock ~= nil then
			mtype:isIgnoringSpawnBlock(mask.flags.ignoreSpawnBlock)
		end
		if mask.flags.illusionable ~= nil then
			mtype:isIllusionable(mask.flags.illusionable)
		end
		if mask.flags.hostile ~= nil then
			mtype:isHostile(mask.flags.hostile)
		end
		if mask.flags.pushable ~= nil then
			mtype:isPushable(mask.flags.pushable)
		end
		if mask.flags.canPushItems ~= nil then
			mtype:canPushItems(mask.flags.canPushItems)
		end
		if mask.flags.canPushCreatures ~= nil then
			mtype:canPushCreatures(mask.flags.canPushCreatures)
		end
		-- if a monster can push creatures,
		-- it should not be pushable
		if mask.flags.canPushCreatures then
			mtype:isPushable(false)
		end
		if mask.flags.targetDistance then
			mtype:targetDistance(mask.flags.targetDistance)
		end
		if mask.flags.staticAttackChance then
			mtype:staticAttackChance(mask.flags.staticAttackChance)
		end
		if mask.flags.canWalkOnEnergy ~= nil then
			mtype:canWalkOnEnergy(mask.flags.canWalkOnEnergy)
		end
		if mask.flags.canWalkOnFire ~= nil then
			mtype:canWalkOnFire(mask.flags.canWalkOnFire)
		end
		if mask.flags.canWalkOnPoison ~= nil then
			mtype:canWalkOnPoison(mask.flags.canWalkOnPoison)
		end
	end
end
registerMonsterType.light = function(mtype, mask)
	if mask.light then
		mtype:light(mask.light.color or 0, mask.light.level or 0)
	end
end
registerMonsterType.changeTarget = function(mtype, mask)
	if mask.changeTarget then
		if mask.changeTarget.chance then
			mtype:changeTargetChance(mask.changeTarget.chance)
		end
		if mask.changeTarget.interval then
			mtype:changeTargetSpeed(mask.changeTarget.interval)
		end
	end
end
registerMonsterType.voices = function(mtype, mask)
	if type(mask.voices) == "table" then
		local interval, chance
		if mask.voices.interval then
			interval = mask.voices.interval
		end
		if mask.voices.chance then
			chance = mask.voices.chance
		end
		for k, v in pairs(mask.voices) do
			if type(v) == "table" then
				mtype:addVoice(v.text, interval, chance, v.yell)
			end
		end
	end
end
registerMonsterType.summons = function(mtype, mask)
	if type(mask.summons) == "table" then
		for k, v in pairs(mask.summons) do
			mtype:addSummon(v.name, v.interval, v.chance)
		end
	end
end
registerMonsterType.events = function(mtype, mask)
	if type(mask.events) == "table" then
		for k, v in pairs(mask.events) do
			mtype:registerEvent(v)
		end
	end
end
registerMonsterType.loot = function(mtype, mask)
	if type(mask.loot) == "table" then
		local lootError = false
		for _, loot in pairs(mask.loot) do
			local parent = Loot()
			if not parent:setId(loot.id) then
				lootError = true
			end
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
				for _, children in pairs(loot.child) do
					local child = Loot()
					if not child:setId(children.id) then
						lootError = true
					end
					if children.chance then
						child:setChance(children.chance)
					end
					if children.maxCount then
						child:setMaxCount(children.maxCount)
					end
					if children.aid or children.actionId then
						child:setActionId(children.aid or children.actionId)
					end
					if children.subType or children.charges then
						child:setSubType(children.subType or children.charges)
					end
					if children.text or children.description then
						child:setDescription(children.text or children.description)
					end
					parent:addChildLoot(child)
				end
			end
			mtype:addLoot(parent)
		end
		if lootError then
			print("[Warning - end] Monster: \"".. mtype:name() .. "\" loot could not correctly be load.")
		end
	end
end
registerMonsterType.elements = function(mtype, mask)
	if type(mask.elements) == "table" then
		for _, element in pairs(mask.elements) do
			if element.type and element.percent then
				mtype:addElement(element.type, element.percent)
			end
		end
	end
end
registerMonsterType.immunities = function(mtype, mask)
	if type(mask.immunities) == "table" then
		for _, immunity in pairs(mask.immunities) do
			if immunity.type and immunity.combat then
				mtype:combatImmunities(immunity.type)
			end
			if immunity.type and immunity.condition then
				mtype:conditionImmunities(immunity.type)
			end
		end
	end
end
local function AbilityTableToSpell(ability)
	local spell = MonsterSpell()
	if ability.name then
		if ability.name == "melee" then
			spell:setType("melee")
			if ability.attack and ability.skill then
				spell:setAttackValue(ability.attack, ability.skill)
			end
			if ability.minDamage and ability.maxDamage then
				spell:setCombatValue(ability.minDamage, ability.maxDamage)
			end
			if ability.interval then
				spell:setInterval(ability.interval)
			end
			if ability.effect then
				spell:setCombatEffect(ability.effect)
			end
		else
			spell:setType(ability.name)
			if ability.type then
				spell:setCombatType(ability.type)
			end
			if ability.interval then
				spell:setInterval(ability.interval)
			end
			if ability.chance then
				spell:setChance(ability.chance)
			end
			if ability.range then
				spell:setRange(ability.range)
			end
			if ability.duration then
				spell:setConditionDuration(ability.duration)
			end
			if ability.speed then
				if type(ability.speed) ~= "table" then
					spell:setConditionSpeedChange(ability.speed)
				elseif type(ability.speed) == "table" then
					if ability.speed.min and ability.speed.max then
						spell:setConditionSpeedChange(ability.speed.min, ability.speed.max)
					end
				end
			end
			if ability.target then
				spell:setNeedTarget(ability.target)
			end
			if ability.length then
				spell:setCombatLength(ability.length)
			end
			if ability.spread then
				spell:setCombatSpread(ability.spread)
			end
			if ability.radius then
				spell:setCombatRadius(ability.radius)
			end
			if ability.ring then
				spell:setCombatRing(ability.ring)
			end
			if ability.minDamage and ability.maxDamage then
				spell:setCombatValue(ability.minDamage, ability.maxDamage)
			end
			if ability.effect then
				spell:setCombatEffect(ability.effect)
			end
			if ability.shootEffect then
				spell:setCombatShootEffect(ability.shootEffect)
			end
			if ability.name == "drunk" then
				spell:setConditionType(CONDITION_DRUNK)
				if ability.drunkenness then
					spell:setConditionDrunkenness(ability.drunkenness)
				end
			end
		end
		if ability.condition then
			if ability.condition.type then
				spell:setConditionType(ability.condition.type)
			end
			local startDamage = 0
			if ability.condition.startDamage then
				startDamage = ability.condition.startDamage
			end
			if ability.condition.minDamage and ability.condition.maxDamage then
				spell:setConditionDamage(ability.condition.minDamage, ability.condition.maxDamage, startDamage)
			end
			if ability.condition.duration then
				spell:setConditionDuration(ability.condition.duration)
			end
			if ability.condition.interval then
				spell:setConditionTickInterval(ability.condition.interval)
			end
		end
	elseif ability.script then
		spell:setScriptName(ability.script)
		if ability.interval then
			spell:setInterval(ability.interval)
		end
		if ability.chance then
			spell:setChance(ability.chance)
		end
		if ability.minDamage and ability.maxDamage then
			spell:setCombatValue(ability.minDamage, ability.maxDamage)
		end
		if ability.target then
			spell:setNeedTarget(ability.target)
		end
		if ability.direction then
			spell:setNeedDirection(ability.direction)
		end
	end
	return spell
end
registerMonsterType.attacks = function(mtype, mask)
	if type(mask.attacks) == "table" then
		for _, attack in pairs(mask.attacks) do
			local spell = AbilityTableToSpell(attack)
			mtype:addAttack(spell)
		end
	end
end
registerMonsterType.defenses = function(mtype, mask)
	if type(mask.defenses) == "table" then
		if mask.defenses.defense then
			mtype:defense(mask.defenses.defense)
		end
		if mask.defenses.armor then
			mtype:armor(mask.defenses.armor)
		end
		for _, defense in pairs(mask.defenses) do
			if type(defense) == "table" then
				local spell = AbilityTableToSpell(defense)
				mtype:addDefense(spell)
			end
		end
	end
end
