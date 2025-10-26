// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TILE_H
#define FS_TILE_H

#include "item.h"
#include "tools.h"

class BedItem;
class Creature;
class MagicField;
class Mailbox;
class SpectatorVec;
class Teleport;
class TrashHolder;

using CreatureVector = std::vector<std::shared_ptr<Creature>>;
using ItemVector = std::vector<std::shared_ptr<Item>>;

enum tileflags_t : uint32_t
{
	TILESTATE_NONE = 0,

	TILESTATE_FLOORCHANGE_DOWN = 1 << 0,
	TILESTATE_FLOORCHANGE_NORTH = 1 << 1,
	TILESTATE_FLOORCHANGE_SOUTH = 1 << 2,
	TILESTATE_FLOORCHANGE_EAST = 1 << 3,
	TILESTATE_FLOORCHANGE_WEST = 1 << 4,
	TILESTATE_FLOORCHANGE_SOUTH_ALT = 1 << 5,
	TILESTATE_FLOORCHANGE_EAST_ALT = 1 << 6,
	TILESTATE_PROTECTIONZONE = 1 << 7,
	TILESTATE_NOPVPZONE = 1 << 8,
	TILESTATE_NOLOGOUT = 1 << 9,
	TILESTATE_PVPZONE = 1 << 10,
	TILESTATE_TELEPORT = 1 << 11,
	TILESTATE_MAGICFIELD = 1 << 12,
	TILESTATE_MAILBOX = 1 << 13,
	TILESTATE_TRASHHOLDER = 1 << 14,
	TILESTATE_BED = 1 << 15,
	TILESTATE_DEPOT = 1 << 16,
	TILESTATE_BLOCKSOLID = 1 << 17,
	TILESTATE_BLOCKPATH = 1 << 18,
	TILESTATE_IMMOVABLEBLOCKSOLID = 1 << 19,
	TILESTATE_IMMOVABLEBLOCKPATH = 1 << 20,
	TILESTATE_IMMOVABLENOFIELDBLOCKPATH = 1 << 21,
	TILESTATE_NOFIELDBLOCKPATH = 1 << 22,
	TILESTATE_SUPPORTS_HANGABLE = 1 << 23,

	TILESTATE_FLOORCHANGE = TILESTATE_FLOORCHANGE_DOWN | TILESTATE_FLOORCHANGE_NORTH | TILESTATE_FLOORCHANGE_SOUTH |
	                        TILESTATE_FLOORCHANGE_EAST | TILESTATE_FLOORCHANGE_WEST | TILESTATE_FLOORCHANGE_SOUTH_ALT |
	                        TILESTATE_FLOORCHANGE_EAST_ALT,
};

enum ZoneType_t
{
	ZONE_PROTECTION,
	ZONE_NOPVP,
	ZONE_PVP,
	ZONE_NOLOGOUT,
	ZONE_NORMAL,
};

class TileItemVector : private ItemVector
{
public:
	using ItemVector::at;
	using ItemVector::begin;
	using ItemVector::clear;
	using ItemVector::const_iterator;
	using ItemVector::const_reverse_iterator;
	using ItemVector::empty;
	using ItemVector::end;
	using ItemVector::erase;
	using ItemVector::insert;
	using ItemVector::iterator;
	using ItemVector::push_back;
	using ItemVector::rbegin;
	using ItemVector::rend;
	using ItemVector::reverse_iterator;
	using ItemVector::size;
	using ItemVector::value_type;

	iterator getBeginDownItem() { return begin(); }
	const_iterator getBeginDownItem() const { return begin(); }
	iterator getEndDownItem() { return begin() + downItemCount; }
	const_iterator getEndDownItem() const { return begin() + downItemCount; }
	iterator getBeginTopItem() { return getEndDownItem(); }
	const_iterator getBeginTopItem() const { return getEndDownItem(); }
	iterator getEndTopItem() { return end(); }
	const_iterator getEndTopItem() const { return end(); }

	uint32_t getTopItemCount() const { return size() - downItemCount; }
	uint32_t getDownItemCount() const { return downItemCount; }
	inline std::shared_ptr<Item> getTopTopItem() const
	{
		if (getTopItemCount() == 0) {
			return nullptr;
		}
		return *(getEndTopItem() - 1);
	}
	inline std::shared_ptr<Item> getTopDownItem() const
	{
		if (downItemCount == 0) {
			return nullptr;
		}
		return *getBeginDownItem();
	}
	void addDownItemCount(int16_t count) { downItemCount += count; }

private:
	uint16_t downItemCount = 0;
};

class Tile : public Thing
{
public:
	static std::shared_ptr<Tile> nullptr_tile;
	Tile(uint16_t x, uint16_t y, uint8_t z) : tilePos(x, y, z) {}
	virtual ~Tile() = default;

	// non-copyable
	Tile(const Tile&) = delete;
	Tile& operator=(const Tile&) = delete;

	std::shared_ptr<Tile> getTile() override final { return std::static_pointer_cast<Tile>(shared_from_this()); }
	std::shared_ptr<const Tile> getTile() const override final
	{
		return std::static_pointer_cast<const Tile>(shared_from_this());
	}

	virtual TileItemVector* getItemList() = 0;
	virtual const TileItemVector* getItemList() const = 0;
	virtual TileItemVector* makeItemList() = 0;

	virtual CreatureVector* getCreatures() = 0;
	virtual const CreatureVector* getCreatures() const = 0;
	virtual CreatureVector* makeCreatures() = 0;

	int32_t getThrowRange() const override final { return 0; }
	bool isPushable() const override final { return false; }

	std::shared_ptr<MagicField> getFieldItem() const;
	std::shared_ptr<Teleport> getTeleportItem() const;
	std::shared_ptr<TrashHolder> getTrashHolder() const;
	std::shared_ptr<Mailbox> getMailbox() const;
	std::shared_ptr<BedItem> getBedItem() const;

	std::shared_ptr<Creature> getTopCreature() const;
	std::shared_ptr<const Creature> getBottomCreature() const;
	std::shared_ptr<Creature> getTopVisibleCreature(std::shared_ptr<const Creature> creature) const;
	std::shared_ptr<const Creature> getBottomVisibleCreature(std::shared_ptr<const Creature> creature) const;
	std::shared_ptr<Item> getTopTopItem() const;
	std::shared_ptr<Item> getTopDownItem() const;
	bool isMoveableBlocking() const;
	std::shared_ptr<Thing> getTopVisibleThing(std::shared_ptr<const Creature> creature);
	std::shared_ptr<Item> getItemByTopOrder(int32_t topOrder);

	size_t getThingCount() const
	{
		size_t thingCount = getCreatureCount() + getItemCount();
		if (ground) {
			thingCount++;
		}
		return thingCount;
	}
	// If these return != 0 the associated vectors are guaranteed to exists
	size_t getCreatureCount() const;
	size_t getItemCount() const;
	uint32_t getTopItemCount() const;
	uint32_t getDownItemCount() const;

	bool hasProperty(ITEMPROPERTY prop) const;
	bool hasProperty(std::shared_ptr<const Item> exclude, ITEMPROPERTY prop) const;

	bool hasFlag(uint32_t flag) const { return hasBitSet(flag, this->flags); }
	void setFlag(uint32_t flag) { this->flags |= flag; }
	void resetFlag(uint32_t flag) { this->flags &= ~flag; }

	ZoneType_t getZone() const
	{
		if (hasFlag(TILESTATE_PROTECTIONZONE)) {
			return ZONE_PROTECTION;
		} else if (hasFlag(TILESTATE_NOPVPZONE)) {
			return ZONE_NOPVP;
		} else if (hasFlag(TILESTATE_PVPZONE)) {
			return ZONE_PVP;
		} else if (hasFlag(TILESTATE_NOLOGOUT)) {
			return ZONE_NOLOGOUT;
		}
		return ZONE_NORMAL;
	}

	bool hasHeight(uint32_t n) const;

	int32_t getClientIndexOfCreature(std::shared_ptr<const Player> player,
	                                 std::shared_ptr<const Creature> creature) const;
	int32_t getStackposOfItem(std::shared_ptr<const Player> player, std::shared_ptr<const Item> item) const;

	ReturnValue queryAdd(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                     std::shared_ptr<Creature> actor = nullptr) const override;
	ReturnValue queryMaxCount(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count,
	                          uint32_t& maxQueryCount, uint32_t flags) const override final;
	ReturnValue queryRemove(std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                        std::shared_ptr<Creature> actor = nullptr) const override;
	std::shared_ptr<Thing> queryDestination(int32_t& index, std::shared_ptr<const Thing> thing,
	                                        std::shared_ptr<Item>& destItem, uint32_t& flags) override;

	void addThing(std::shared_ptr<Thing> thing) override final;
	void addThing(int32_t index, std::shared_ptr<Thing> thing) override;

	void updateThing(std::shared_ptr<Thing> thing, uint16_t itemId, uint32_t count) override final;
	void replaceThing(uint32_t index, std::shared_ptr<Thing> thing) override final;

	void removeThing(std::shared_ptr<Thing> thing, uint32_t count) override final;

	bool hasCreature(std::shared_ptr<Creature> creature) const;
	void removeCreature(std::shared_ptr<Creature> creature);

	int32_t getThingIndex(std::shared_ptr<const Thing> thing) const override final;
	size_t getFirstIndex() const override final;
	size_t getLastIndex() const override final;
	uint32_t getItemTypeCount(uint16_t itemId, int32_t subType = -1) const override final;
	std::shared_ptr<Thing> getThing(size_t index) const override final;

	void postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override final;
	void postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override final;

	void internalAddThing(std::shared_ptr<Thing> thing) override final;
	void internalAddThing(uint32_t index, std::shared_ptr<Thing> thing) override;

	const Position& getPosition() const { return tilePos; }

	bool isRemoved() const override final { return false; }

	std::shared_ptr<Item> getUseItem(int32_t index) const;

	std::shared_ptr<Item> getGround() const { return ground; }
	void setGround(std::shared_ptr<Item> item) { ground = std::move(item); }

private:
	void onAddTileItem(std::shared_ptr<Item> item);
	void onUpdateTileItem(std::shared_ptr<Item> oldItem, const ItemType& oldType, std::shared_ptr<Item> newItem,
	                      const ItemType& newType);
	void onRemoveTileItem(const SpectatorVec& spectators, const std::vector<int32_t>& oldStackPosVector,
	                      std::shared_ptr<Item> item);
	void onUpdateTile(const SpectatorVec& spectators);

	void setTileFlags(std::shared_ptr<const Item> item);
	void resetTileFlags(std::shared_ptr<const Item> item);

	std::shared_ptr<Item> ground = nullptr;
	Position tilePos;
	uint32_t flags = 0;
};

// Used for walkable tiles, where there is high likeliness of
// items being added/removed
class DynamicTile : public Tile
{
	// By allocating the vectors in-house, we avoid some memory fragmentation
	TileItemVector items;
	CreatureVector creatures;

public:
	DynamicTile(uint16_t x, uint16_t y, uint8_t z) : Tile{x, y, z} {}
	~DynamicTile() = default;

	using Tile::internalAddThing;

	// non-copyable
	DynamicTile(const DynamicTile&) = delete;
	DynamicTile& operator=(const DynamicTile&) = delete;

	TileItemVector* getItemList() override { return &items; }
	const TileItemVector* getItemList() const override { return &items; }
	TileItemVector* makeItemList() override { return &items; }

	CreatureVector* getCreatures() override { return &creatures; }
	const CreatureVector* getCreatures() const override { return &creatures; }
	CreatureVector* makeCreatures() override { return &creatures; }
};

// For blocking tiles, where we very rarely actually have items
class StaticTile final : public Tile
{
	// We very rarely even need the vectors, so don't keep them in memory
	std::unique_ptr<TileItemVector> items;
	std::unique_ptr<CreatureVector> creatures;

public:
	StaticTile(uint16_t x, uint16_t y, uint8_t z) : Tile{x, y, z} {}
	~StaticTile() = default;

	using Tile::internalAddThing;

	// non-copyable
	StaticTile(const StaticTile&) = delete;
	StaticTile& operator=(const StaticTile&) = delete;

	TileItemVector* getItemList() override { return items.get(); }
	const TileItemVector* getItemList() const override { return items.get(); }
	TileItemVector* makeItemList() override
	{
		if (!items) {
			items.reset(new TileItemVector);
		}
		return items.get();
	}

	CreatureVector* getCreatures() override { return creatures.get(); }
	const CreatureVector* getCreatures() const override { return creatures.get(); }
	CreatureVector* makeCreatures() override
	{
		if (!creatures) {
			creatures.reset(new CreatureVector);
		}
		return creatures.get();
	}
};

#endif // FS_TILE_H
