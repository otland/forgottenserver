#include "quadtree.h"

namespace {
std::array<QuadTree*, 4> nodes = {};

uint8_t create_index(uint16_t x, uint16_t y) { return ((x & 0x8000) >> 15) | ((y & 0x8000) >> 14); }

Leaf* find_leaf_in_root(uint16_t x, uint16_t y)
{
	auto index = create_index(x, y);
	if (auto node = nodes[index]; auto leaf = find_leaf(x, y, node)) {
		return static_cast<Leaf*>(leaf);
	}
	return nullptr;
}

QuadTree* find_leaf(uint16_t x, uint16_t y, QuadTree* node)
{
	if (node->is_leaf()) {
		return node;
	}

	auto index = create_index(x, y);
	if (auto node_child = node->get_child(index)) {
		return find_leaf(x << 1, y << 1, node_child);
	}
	return nullptr;
}

void create_leaf_in_root(uint16_t x, uint16_t y, uint8_t z)
{
	auto index = create_index(x, y);
	if (!nodes[index]) {
		nodes[index] = new Node();
	}

	create_leaf(x, y, (MAP_MAX_LAYERS - 1), nodes[index]);
}

void create_leaf(uint16_t x, uint16_t y, uint8_t z, QuadTree* node)
{
	if (node->is_leaf()) {
		return;
	}

	auto index = create_index(x, y);
	auto node_child = node->get_child(index);
	if (!node_child) {
		if (z == FLOOR_BITS) {
			node_child = new Leaf(x, y);
		} else {
			node_child = new Node();
		}

		node->set_child(index, node_child);
	}

	create_leaf(x * 2, y * 2, z - 1, node_child);
}

} // namespace

void tfs::map::quadtree::find(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y,
                              SpectatorVec& spectators, std::function<bool(Creature*)> comparison)
{
	int32_t start_x_aligned = start_x - (start_x % FLOOR_SIZE);
	int32_t start_y_aligned = start_y - (start_y % FLOOR_SIZE);
	int32_t end_x_aligned = end_x - (end_x % FLOOR_SIZE);
	int32_t end_y_aligned = end_y - (end_y % FLOOR_SIZE);

	if (auto start_leaf = find_leaf_in_root(start_x_aligned, start_y_aligned)) {
		auto south_leaf = start_leaf;

		for (int32_t ny = start_y_aligned; ny <= end_y_aligned; ny += FLOOR_SIZE) {
			auto east_leaf = south_leaf;

			for (int32_t nx = start_x_aligned; nx <= end_x_aligned; nx += FLOOR_SIZE) {
				if (east_leaf) {
					for (auto creature : east_leaf->creatures) {
						if (comparison(creature)) {
							spectators.emplace_back(creature);
						}
					}

					east_leaf = east_leaf->east_leaf;
				} else {
					east_leaf = find_leaf_in_root(nx + FLOOR_SIZE, ny);
				}
			}

			if (south_leaf) {
				south_leaf = south_leaf->south_leaf;
			} else {
				south_leaf = find_leaf_in_root(start_x_aligned, ny + FLOOR_SIZE);
			}
		}
	}
}

Tile* tfs::map::quadtree::find_tile(uint16_t x, uint16_t y, uint8_t z)
{
	if (auto leaf = find_leaf_in_root(x, y)) {
		return leaf->tiles[z][x & FLOOR_MASK][y & FLOOR_MASK];
	}
	return nullptr;
}

void tfs::map::quadtree::create_tile(uint16_t x, uint16_t y, uint8_t z, Tile* tile)
{
	create_leaf_in_root(x, y, z);

	if (auto leaf = find_leaf_in_root(x, y)) {
		leaf->tiles[z][x & FLOOR_MASK][y & FLOOR_MASK] = tile;
	}
}

void tfs::map::quadtree::move_creature(uint16_t old_x, uint16_t old_y, uint8_t old_z, uint16_t x, uint16_t y, uint8_t z,
                                       Creature* creature)
{
	if (auto old_leaf = find_leaf_in_root(old_x, old_y); auto leaf = find_leaf_in_root(x, y)) {
		if (old_leaf != leaf) {
			old_leaf->remove_creature(creature);
			leaf->push_creature(creature);
		}
	}
}

void tfs::map::quadtree::insert_creature(uint16_t x, uint16_t y, uint8_t z, Creature* creature)
{
	if (auto leaf = find_leaf_in_root(x, y)) {
		leaf->push_creature(creature);
	}
}

void tfs::map::quadtree::remove_creature(uint16_t x, uint16_t y, uint8_t z, Creature* creature)
{
	if (auto leaf = find_leaf_in_root(x, y)) {
		leaf->remove_creature(creature);
	}
}

Leaf::Leaf(uint16_t x, uint16_t y)
{
	// update north
	if (auto north_leaf = find_leaf_in_root(x, y - FLOOR_SIZE)) {
		north_leaf->south_leaf = this;
	}

	// update west
	if (auto west_leaf = find_leaf_in_root(x - FLOOR_SIZE, y)) {
		west_leaf->east_leaf = this;
	}

	// update south
	if (auto south_leaf = find_leaf_in_root(x, y + FLOOR_SIZE)) {
		this->south_leaf = south_leaf;
	}

	// update east
	if (auto east_leaf = find_leaf_in_root(x + FLOOR_SIZE, y)) {
		this->east_leaf = east_leaf;
	}
}

void Leaf::push_creature(Creature* creature)
{
	creatures.insert(creature);

	if (auto monster = creature->getMonster()) {
		monsters.insert(monster);
	} else if (auto npc = creature->getNpc()) {
		npcs.insert(npc);
	} else if (auto player = creature->getPlayer()) {
		players.insert(player);
	}
}

void Leaf::remove_creature(Creature* creature)
{
	creatures.erase(creature);

	if (auto monster = creature->getMonster()) {
		monsters.erase(monster);
	} else if (auto npc = creature->getNpc()) {
		npcs.erase(npc);
	} else if (auto player = creature->getPlayer()) {
		players.erase(player);
	}
}
