// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_THING_H
#define FS_THING_H

class Container;
class Creature;
class Cylinder;
class Item;
class Tile;
struct Position;

class Thing
{
public:
	constexpr Thing() = default;
	virtual ~Thing() = default;

	// non-copyable
	Thing(const Thing&) = delete;
	Thing& operator=(const Thing&) = delete;

	bool hasParent() const { return getParent(); }
	virtual Cylinder* getParent() const { return parent; }
	Cylinder* getRealParent() const { return getParent(); }
	virtual void setParent(Cylinder* cylinder) { parent = cylinder; }

	virtual Tile* getTile();
	virtual const Tile* getTile() const;

	virtual const Position& getPosition() const;
	virtual int32_t getThrowRange() const = 0;
	virtual bool isPushable() const = 0;

	virtual Item* getItem() { return nullptr; }
	virtual const Item* getItem() const { return nullptr; }
	virtual Creature* getCreature() { return nullptr; }
	virtual const Creature* getCreature() const { return nullptr; }

	virtual bool isRemoved() const { return true; }

private:
	Cylinder* parent = nullptr;
};

#endif // FS_THING_H
