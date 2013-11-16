local text = "Information on Offline Training:\n 1. You need to have a Premium account in order to train here.\n 2. Choose a skill you'd like to train. Shielding is ALWAYS included.\n 3. If you're not sure which statue trains what, read the inscriptions.\n 4. Use a statue to be logged out of the game and train the skills associated with that statue.\n 5. When you log back into the game, your skills will have improved depending on how long you trained.\n 6. You have to be logged out of the game for at least 10 minutes in order for the training to take effect.\n 7. After 12 hours of constant offline training, your skills will not improve any further. Similar to stamina, your training bar will regenerate if you are not training offline."

function onUse(cid, item, fromPosition, itemEx, toPosition)
	doShowTextDialog(cid, item.itemid, text)
	return true
end
