VOCATION_SORCERER = 1
VOCATION_DRUID = 2
VOCATION_PALADIN = 3
VOCATION_KNIGHT = 4
VOCATION_MASTER_SORCERER = 5
VOCATION_ELDER_DRUID = 6
VOCATION_ROYAL_PALADIN = 7
VOCATION_ELITE_KNIGHT = 8

function Vocation.getBase(self)
	local base = self
	while base:getDemotion() do
		base = base:getDemotion()
	end
	return base
end
