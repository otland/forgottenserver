local destroyableItems = {
	2376, 2377, 2378, 2379, 2380, 2381, 2382, 2383, 2384, 2385, 2386, 2387, 2388, 2389, 2390, 2391, 2392, 2393, 2394,
	2395, 2396, 2397, 2398, 2399, 2400, 2401, 2402, 2403, 2404, 2406, 2407, 2408, 2409, 2410, 2411, 2412, 2413, 2414,
	2415, 2416, 2417, 2418, 2419, 2421, 2422, 2423, 2424, 2425, 2426, 2427, 2428, 2429, 2430, 2431, 2432, 2433, 2434,
	2435, 2436, 2437, 2438, 2439, 2440, 2441, 2442, 2443, 2444, 2445, 2446, 2447, 2448, 2449, 2450, 2451, 2452, 2453
}

local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	return destroyItem(player, target, toPosition)
end

action:id(table.unpack(destroyableItems))
action:register()
