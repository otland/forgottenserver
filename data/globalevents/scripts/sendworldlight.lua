local intensity, color = getWorldLight()
function onThink(interval)
	local newIntensity, newColor = getWorldLight()
	if newIntensity ~= intensity or newColor ~= color then
		for k,v in pairs(Game.getPlayers()) do
			v:sendWorldLight(intensity, color)
		end
		intensity,color = getWorldLight()
	end
	return true
end