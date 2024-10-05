#include "otpch.h"

#include "creature.h"
#include "spectators.h"

namespace tfs::quadtree {
void find(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, SpectatorVec& spectators,
          std::function<bool(Creature*)> comparasion);
Tile* find_tile(uint16_t x, uint16_t y, uint8_t z);
void create_tile(uint16_t x, uint16_t y, uint8_t z, Tile* tile);
void move_creature(uint16_t old_x, uint16_t old_y, uint8_t old_z, uint16_t x, uint16_t y, uint8_t z,
                   Creature* creature);
void insert_creature(uint16_t x, uint16_t y, uint8_t z, Creature* creature);
void remove_creature(uint16_t x, uint16_t y, uint8_t z, Creature* creature);
} // namespace tfs::quadtree

class QuadTree
{
public:
	explicit QuadTree() = default;

	virtual bool is_leaf() = 0;

	std::array<std::unique_ptr<QuadTree>, 4> nodes = {};
};

inline constexpr int32_t FLOOR_BITS = 3;
inline constexpr int32_t FLOOR_SIZE = (1 << FLOOR_BITS);
inline constexpr int32_t FLOOR_MASK = (FLOOR_SIZE - 1);

class Node final : public QuadTree
{
public:
	explicit Node() = default;

	// non-copyable
	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;

	virtual bool is_leaf() { return false; }
};

class Leaf final : public QuadTree
{
public:
	explicit Leaf(uint16_t x, uint16_t y);

	// non-copyable
	Leaf(const Leaf&) = delete;
	Leaf& operator=(const Leaf&) = delete;

	virtual bool is_leaf() { return true; }

	void push_creature(Creature* creature);
	void remove_creature(Creature* creature);

	Tile* tiles[MAP_MAX_LAYERS][FLOOR_SIZE][FLOOR_SIZE] = {};
	std::set<Creature*> creatures;
	std::set<Monster*> monsters;
	std::set<Npc*> npcs;
	std::set<Player*> players;

	Leaf* south_leaf = nullptr;
	Leaf* east_leaf = nullptr;
};
