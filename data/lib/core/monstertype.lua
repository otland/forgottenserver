MonsterType.getName = function(self) return self:name() end
MonsterType.setName = function(self, name) return self:name(name) end

MonsterType.getNameDescription = function(self) return self:nameDescription() end
MonsterType.setNameDescription = function(self, desc) return self:nameDescription(desc) end

MonsterType.getExperience = function(self) return self:experience() end
MonsterType.setExperience = function(self, exp) return self:experience(exp) end

MonsterType.getOutfit = function(self) return self:outfit() end
MonsterType.setOutfit = function(self, outfit) return self:outfit(outfit) end

MonsterType.getMaxHealth = function(self) return self:maxHealth() end
MonsterType.setMaxHealth = function(self, health) return self:maxHealth(health) end

MonsterType.getHealth = function(self) return self:health() end
MonsterType.setHealth = function(self, health) return self:health(health) end

MonsterType.getRunHealth = function(self) return self:runHealth() end
MonsterType.setRunHealth = function(self, health) return self:runHealth(health) end

MonsterType.getMaxSummons = function(self) return self:maxSummons() end
MonsterType.setMaxSummons = function(self, max) return self:maxSummons(max) end

MonsterType.getRace = function(self) return self:race() end
MonsterType.setRace = function(self, race) return self:race(race) end

MonsterType.getManaCost = function(self) return self:manaCost() end
MonsterType.setManaCost = function(self, mana) return self:manaCost(mana) end

MonsterType.getBaseSpeed = function(self) return self:baseSpeed() end
MonsterType.setBaseSpeed = function(self, speed) return self:baseSpeed(speed) end

MonsterType.getCorpseId = function(self) return self:corpseId() end
MonsterType.setCorpseId = function(self, id) return self:corpseId(id) end

MonsterType.getLight = function(self) return self:light() end
MonsterType.setLight = function(self, color, level) return self:light(color, level) end

MonsterType.getChangeTargetChance = function(self) return self:changeTargetChance() end
MonsterType.setChangeTargetChance = function(self, chance) return self:changeTargetChance(chance) end

MonsterType.getChangeTargetSpeed = function(self) return self:changeTargetSpeed() end
MonsterType.setChangeTargetSpeed = function(self, interval) return self:changeTargetSpeed(interval) end

MonsterType.getArmor = function(self) return self:armor() end
MonsterType.setArmor = function(self, armor) return self:armor(armor) end

MonsterType.getDefense = function(self) return self:defense() end
MonsterType.setDefense = function(self, def) return self:defense(def) end

MonsterType.getStaticAttackChance = function(self) return self:staticAttackChance() end
MonsterType.setStaticAttackChance = function(self, chance) return self:staticAttackChance(chance) end

MonsterType.getTargetDistance = function(self) return self:targetDistance() end
MonsterType.setTargetDistance = function(self, dist) return self:targetDistance(dist) end

MonsterType.getYellChance = function(self) return self:yellChance() end
MonsterType.setYellChance = function(self, chance) return self:yellChance(chance) end

MonsterType.getYellSpeedTicks = function(self) return self:yellSpeedTicks() end
MonsterType.setYellSpeedTicks = function(self, ticks) return self:yellSpeedTicks(ticks) end
