function canJoin(cid)
	return getPlayerAccountType(cid) >= ACCOUNT_TYPE_GAMEMASTER
end
