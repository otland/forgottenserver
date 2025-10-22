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

	virtual bool hasParent() const { return false; }
	virtual std::shared_ptr<Cylinder> getParent() const { return nullptr; }
	virtual std::shared_ptr<Cylinder> getRealParent() const { return getParent(); }

	virtual void setParent(std::shared_ptr<Cylinder>) { throw std::runtime_error("Not implemented"); };

	virtual std::shared_ptr<Cylinder> getCylinder() { return nullptr; }
	virtual std::shared_ptr<const Cylinder> getCylinder() const { return nullptr; }
	virtual std::shared_ptr<Tile> getTile() { return nullptr; }
	virtual std::shared_ptr<const Tile> getTile() const { return nullptr; }

	virtual const Position& getPosition() const;
	virtual int32_t getThrowRange() const { throw std::runtime_error("Not implemented"); };
	virtual bool isPushable() const { throw std::runtime_error("Not implemented"); };

	virtual std::shared_ptr<Container> getContainer() { return nullptr; }
	virtual std::shared_ptr<const Container> getContainer() const { return nullptr; }
	virtual std::shared_ptr<Item> getItem() { return nullptr; }
	virtual std::shared_ptr<const Item> getItem() const { return nullptr; }
	virtual std::shared_ptr<Creature> getCreature() { return nullptr; }
	virtual std::shared_ptr<const Creature> getCreature() const { return nullptr; }

	virtual bool isRemoved() const { return true; }
};

#endif // FS_THING_H
