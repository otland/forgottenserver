local foods = {
	--[itemid] = {feed value, say text, client version}
	[2695] = {6, "Gulp.", 0}, -- egg
	[2362] = {5, "Crunch.", 0}, -- carrot
	[2666] = {15, "Munch.", 0}, -- meat
	[2667] = {12, "Munch.", 0}, -- fish
	[2668] = {10, "Mmmm.", 0}, -- salmon
	[2669] = {17, "Munch.", 0}, -- northern pike
	[2670] = {4, "Gulp.", 0}, -- shrimp
	[2671] = {30, "Chomp.", 0}, -- ham
	[2672] = {60, "Chomp.", 0}, -- dragon ham
	[2673] = {5, "Yum.", 0}, -- pear
	[2674] = {6, "Yum.", 0}, -- red apple
	[2675] = {13, "Yum.", 0}, -- orange
	[2676] = {8, "Yum.", 0}, -- banana
	[2677] = {1, "Yum.", 0}, -- blueberry
	[2678] = {18, "Slurp.", 0}, -- coconut
	[2679] = {1, "Yum.", 0}, -- cherry
	[2680] = {2, "Yum.", 0}, -- strawberry
	[2681] = {9, "Yum.", 0}, -- grapes
	[2682] = {20, "Yum.", 0}, -- melon
	[2683] = {17, "Munch.", 0}, -- pumpkin
	[2684] = {5, "Crunch.", 0}, -- carrot
	[2685] = {6, "Munch.", 0}, -- tomato
	[2686] = {9, "Crunch.", 0}, -- corncob
	[2687] = {2, "Crunch.", 0}, -- cookie
	[2688] = {2, "Munch.", 0}, -- candy cane
	[2689] = {10, "Crunch.", 0}, -- bread
	[2690] = {3, "Crunch.", 0}, -- roll
	[2691] = {8, "Crunch.", 0}, -- brown bread
	[2696] = {9, "Smack.", 0}, -- cheese
	[2787] = {9, "Munch.", 0}, -- white mushroom
	[2788] = {4, "Munch.", 0}, -- red mushroom
	[2789] = {22, "Munch.", 0}, -- brown mushroom
	[2790] = {30, "Munch.", 0}, -- orange mushroom
	[2791] = {9, "Munch.", 0}, -- wood mushroom
	[2792] = {6, "Munch.", 0}, -- dark mushroom
	[2793] = {12, "Munch.", 0}, -- some mushrooms
	[2794] = {3, "Munch.", 0}, -- some mushrooms
	[2795] = {36, "Munch.", 0}, -- fire mushroom
	[2796] = {5, "Munch.", 0}, -- green mushroom
	[5097] = {4, "Yum.", 780}, -- mango
	[6125] = {8, "Gulp.", 780}, -- tortoise egg
	[6278] = {10, "Mmmm.", 790}, -- cake
	[6279] = {15, "Mmmm.", 790}, -- decorated cake
	[6393] = {12, "Mmmm.", 790}, -- valentine's cake
	[6394] = {15, "Mmmm.", 790}, -- cream cake
	[6501] = {20, "Mmmm.", 790}, -- gingerbread man
	[6541] = {6, "Gulp.", 790}, -- coloured egg (yellow)
	[6542] = {6, "Gulp.", 790}, -- coloured egg (red)
	[6543] = {6, "Gulp.", 790}, -- coloured egg (blue)
	[6544] = {6, "Gulp.", 790}, -- coloured egg (green)
	[6545] = {6, "Gulp.", 790}, -- coloured egg (purple)
	[6569] = {1, "Mmmm.", 792}, -- candy
	[6574] = {5, "Mmmm.", 792}, -- bar of chocolate
	[7158] = {15, "Munch.", 800}, -- rainbow trout
	[7159] = {13, "Munch.", 800}, -- green perch
	[7372] = {2, "Yum.", 800}, -- ice cream cone (crispy chocolate chips)
	[7373] = {2, "Yum.", 800}, -- ice cream cone (velvet vanilla)
	[7374] = {2, "Yum.", 800}, -- ice cream cone (sweet strawberry)
	[7375] = {2, "Yum.", 800}, -- ice cream cone (chilly cherry)
	[7376] = {2, "Yum.", 800}, -- ice cream cone (mellow melon)
	[7377] = {2, "Yum.", 800}, -- ice cream cone (blue-barian)
	[7909] = {4, "Crunch.", 810}, -- walnut
	[7910] = {4, "Crunch.", 810}, -- peanut
	[7963] = {60, "Munch.", 810}, -- marlin
	[7966] = {9, "Hum.", 790}, -- cream cake
	[8112] = {9, "Urgh.", 810}, -- scarab cheese
	[8838] = {10, "Gulp.", 820}, -- potato
	[8839] = {5, "Yum.", 820}, -- plum
	[8840] = {1, "Yum.", 820}, -- raspberry
	[8841] = {1, "Urgh.", 820}, -- lemon
	[8842] = {7, "Munch.", 820}, -- cucumber
	[8843] = {5, "Crunch.", 820}, -- onion
	[8844] = {1, "Gulp.", 820}, -- jalapeno pepper
	[8845] = {5, "Munch.", 820}, -- beetroot
	[8847] = {11, "Yum.", 820}, -- chocolate cake
	[9005] = {7, "Slurp.", 820}, -- yummy gummy worm
	[9114] = {5, "Crunch.", 840}, -- bulb of garlic
	[10454] = {0, "Your head begins to feel better.", 850}, -- headache pill
	[11246] = {15, "Yum.", 854}, -- rice ball
	[11370] = {3, "Urgh.", 854}, -- terramite eggs
	[11429] = {10, "Mmmm.", 854}, -- crocodile steak
	[12415] = {20, "Yum.", 860}, -- pineapple
	[12416] = {10, "Munch.", 860}, -- aubergine
	[12417] = {8, "Crunch.", 860}, -- broccoli
	[12418] = {9, "Crunch.", 860}, -- cauliflower
	[12637] = {55, "Gulp.", 860}, -- ectoplasmic sushi
	[12638] = {18, "Yum.", 860}, -- dragonfruit
	[12639] = {2, "Munch.", 860}, -- peas
	[13297] = {20, "Crunch.", 870}, -- haunch of boar
	[15405] = {55, "Munch.", 940}, -- sandfish
	[15487] = {14, "Urgh.", 940}, -- larvae
	[15488] = {15, "Munch.", 940}, -- deepling filet
	[16014] = {60, "Mmmm.", 944}, -- anniversary cake
	[18306] = {0, "Phew!", 960}, -- stale mushroom beer
	[18397] = {33, "Munch.", 960}, -- mushroom pie
	[19737] = {10, "Urgh.", 980}, -- insectoid eggs
	[20100] = {15, "Smack.", 980}, -- soft cheese
	[20101] = {12, "Smack.", 980}, -- rat cheese
	[22644] = {4, "Mmmm.", 1030}, -- christmas cookie tray
	[23514] = {15, "Munch.", 1050}, -- glooth sandwich
	[23515] = {7, "Slurp.", 1050}, -- bowl of glooth soup
	[23516] = {6, "Burp.", 1050}, -- bottle of glooth wine
	[23517] = {25, "Chomp.", 1050}, -- glooth steak
	[24841] = {12, "Yum.", 1090}, -- pickle pear
	[24843] = {60, "Chomp.", 1090}, -- roasted meat
	[26191] = {25, "Mmmm.", 1094}, -- energy bar
	[26201] = {15, "Mmmm.", 1094}, -- energy drink
}

local food = Action()

function food.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local food = foods[item.itemid]
	if not food then
		return false
	end

	local condition = player:getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT)
	if condition and math.floor(condition:getTicks() / 1000 + (food[1] * 12)) >= 1200 then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You are full.")
	else
		player:feed(food[1] * 12)
		player:say(food[2], TALKTYPE_MONSTER_SAY)
		item:remove(1)
	end
	return true
end

for i, f in pairs(foods) do
	if Game.getClientVersion().min >= f[3] then
		food:id(i)
	end
end
food:register()
