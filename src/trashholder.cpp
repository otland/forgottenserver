//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include "otpch.h"

#include "trashholder.h"
#include "game.h"

extern Game g_game;

TrashHolder::TrashHolder(uint16_t _type, MagicEffectClasses _effect /*= NM_ME_NONE*/) : Item(_type)
{
	effect = _effect;
}

TrashHolder::~TrashHolder()
{
	//
}

ReturnValue TrashHolder::__queryAdd(int32_t index, const Thing* thing, uint32_t count,
                                    uint32_t flags, Creature* actor/* = NULL*/) const
{
	return RET_NOERROR;
}

ReturnValue TrashHolder::__queryMaxCount(int32_t index, const Thing* thing, uint32_t count,
        uint32_t& maxQueryCount, uint32_t flags) const
{
	maxQueryCount = std::max<uint32_t>(1, count);
	return RET_NOERROR;
}

ReturnValue TrashHolder::__queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const
{
	return RET_NOTPOSSIBLE;
}

Cylinder* TrashHolder::__queryDestination(int32_t& index, const Thing* thing, Item** destItem,
        uint32_t& flags)
{
	return this;
}

void TrashHolder::__addThing(Thing* thing)
{
	return __addThing(0, thing);
}

void TrashHolder::__addThing(int32_t index, Thing* thing)
{
	Item* item = thing->getItem();

	if (!item) {
		return;
	}

	if (item == this || !item->hasProperty(MOVEABLE)) {
		return;
	}

	if (item->isHangable() && isGroundTile()) {
		Cylinder* parent = getParent();

		if (parent) {
			Tile* tile = dynamic_cast<Tile*>(parent);

			if (tile) {
				if (const TileItemVector* items = tile->getItemList()) {
					for (ItemVector::const_iterator it = items->begin(); it != items->end(); ++it) {
						const ItemType& iiType = Item::items[(*it)->getID()];

						if (iiType.isHorizontal || iiType.isVertical) {
							return;
						}
					}
				}
			}
		}
	}

	g_game.internalRemoveItem(item);

	if (effect != NM_ME_NONE) {
		g_game.addMagicEffect(getPosition(), effect);
	}
}

void TrashHolder::__updateThing(Thing* thing, uint16_t itemId, uint32_t count)
{
	//
}

void TrashHolder::__replaceThing(uint32_t index, Thing* thing)
{
	//
}

void TrashHolder::__removeThing(Thing* thing, uint32_t count)
{
	//
}

void TrashHolder::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link /*= LINK_OWNER*/)
{
	getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void TrashHolder::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link /*= LINK_OWNER*/)
{
	getParent()->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_PARENT);
}
