local food =
{
	[2362] = {8, "Crunch."},
	[2666] = {15, "Munch."},
	[2667] = {12, "Munch."},
	[2668] = {10, "Mmmm."},
	[2669] = {17, "Munch."},
	[2670] = {4, "Gulp."},
	[2671] = {30, "Chomp."},
	[2672] = {60, "Chomp."},
	[2673] = {5, "Yum."},
	[2674] = {6, "Yum."},
	[2675] = {13, "Yum."},
	[2676] = {8, "Yum."},
	[2677] = {1, "Yum."},
	[2678] = {18, "Slurp."},
	[2679] = {1, "Yum."},
	[2680] = {2, "Yum."},
	[2681] = {9, "Yum."},
	[2682] = {20, "Yum."},
	[2683] = {17, "Munch."},
	[2684] = {8, "Crunch."},
	[2685] = {6, "Munch."},
	[2686] = {9, "Crunch."},
	[2687] = {2, "Crunch."},
	[2688] = {9, "Munch."},
	[2689] = {10, "Crunch."},
	[2690] = {3, "Crunch."},
	[2691] = {8, "Crunch."},
	[2792] = {6, "Munch."},
	[2793] = {9, "Munch."},
	[2695] = {6, "Gulp."},
	[2696] = {9, "Smack."},
	[2787] = {9, "Munch."},
	[2788] = {4, "Munch."},
	[2789] = {22, "Munch."},
	[2790] = {30, "Munch."},
	[2791] = {30, "Munch."},
	[2792] = {6, "Munch."},
	[2794] = {3, "Munch."},
	[2795] = {36, "Munch."},
	[2796] = {5, "Munch."},
	[2793] = {9, "Munch."},
	[5097] = {4, "Yum."},
	[6125] = {8, "Gulp."},
	[6278] = {10, "Mmmm."},
	[6279] = {15, "Mmmm."},
	[6393] = {12, "Mmmm."},
	[6394] = {15, "Mmmm."},
	[6501] = {20, "Mmmm."},
	[6541] = {6, "Gulp."},
	[6542] = {6, "Gulp."},
	[6543] = {6, "Gulp."},
	[6544] = {6, "Gulp."},
	[6545] = {6, "Gulp."},
	[6569] = {1, "Mmmm."},
	[6574] = {4, "Mmmm."},
	[7158] = {15, "Munch."},
	[7159] = {13, "Munch."},
	[7372] = {7, "Yum."},
	[7373] = {7, "Yum."},
	[7374] = {7, "Yum."},
	[7375] = {7, "Yum."},
	[7376] = {7, "Yum."},
	[7377] = {7, "Yum."}
}
function onUse(cid, item, frompos, item2, topos)
	if(food[item.itemid] ~= nil) then
		if (getPlayerFood(cid) + food[item.itemid][1]) >= 400 then
			doPlayerSendCancel(cid, "You are full.")
		else
			doPlayerFeed(cid, food[item.itemid][1] * 4)
			doCreatureSay(cid, food[item.itemid][2], TALKTYPE_ORANGE_1)
			doRemoveItem(item.uid, 1)
		end
		return TRUE
	end
	return FALSE
end