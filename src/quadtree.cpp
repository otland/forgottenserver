#include "quadtree.h"

namespace {
std::array<QuadTree*, 4> nodes = {};

uint8_t create_index(uint16_t x, uint16_t y) { return ((x & 0x8000) >> 15) | ((y & 0x8000) >> 14); }

QuadTree* find_leaf(QuadTree* node, uint16_t x, uint16_t y)
{
	if (node->is_leaf()) {
		return node;
	}

	auto index = create_index(x, y);
	if (auto node_child = node->get_child(index)) {
		return find_leaf(node_child, x << 1, y << 1);
	}
	return nullptr;
}

Leaf* find_leaf_in_root(uint16_t x, uint16_t y)
{
	auto index = create_index(x, y);
	if (auto node = nodes[index]) {
		if (auto leaf = find_leaf(node, x, y)) {
			return static_cast<Leaf*>(leaf);
		}
	}
	return nullptr;
}

/**
 * @brief Establishes relationships with neighboring Leaf nodes.
 *
 * This section checks for neighboring leaf nodes (north, south, east, and west) and establishes two-way relationships
 * if those neighboring leaves are found.
 *
 * @param {x} The x-coordinate of the leaf node in the quadtree.
 * @param {y} The y-coordinate of the leaf node in the quadtree.
 */
void update_leaf_neighbors(uint16_t x, uint16_t y)
{
	/*
	 * The following relationships are updated:
	 * - The north neighbor's south_leaf pointer is updated to point to this leaf.
	 * - The west neighbor's east_leaf pointer is updated to point to this leaf.
	 * - This leaf's south_leaf pointer is updated to point to the south neighbor, if found.
	 * - This leaf's east_leaf pointer is updated to point to the east neighbor, if found.
	 */
	if (auto leaf = find_leaf_in_root(x, y)) {
		// update north
		if (auto north_leaf = find_leaf_in_root(x, y - TILE_GRID_SIZE)) {
			north_leaf->south_leaf = leaf;
		}

		// update west
		if (auto west_leaf = find_leaf_in_root(x - TILE_GRID_SIZE, y)) {
			west_leaf->east_leaf = leaf;
		}

		// update south
		if (auto south_leaf = find_leaf_in_root(x, y + TILE_GRID_SIZE)) {
			leaf->south_leaf = south_leaf;
		}

		// update east
		if (auto east_leaf = find_leaf_in_root(x + TILE_GRID_SIZE, y)) {
			leaf->east_leaf = east_leaf;
		}
	}
}

void create_leaf_node(QuadTree* node, uint16_t x, uint16_t y, uint8_t z)
{
	if (node->is_leaf()) {
		return;
	}

	auto index = create_index(x, y);
	auto node_child = node->get_child(index);
	if (!node_child) {
		if (z == TILE_GRID_BITS) {
			node_child = new Leaf();
		} else {
			node_child = new Node();
		}

		node->set_child(index, node_child);
	}

	create_leaf_node(node_child, x * 2, y * 2, z - 1);
}

void create_leaf_in_root(uint16_t x, uint16_t y)
{
	auto index = create_index(x, y);
	if (!nodes[index]) {
		nodes[index] = new Node();
	}

	create_leaf_node(nodes[index], x, y, (MAP_MAX_LAYERS - 1));
	update_leaf_neighbors(x, y);
}

} // namespace

SpectatorVec tfs::map::quadtree::find_in_range(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
	SpectatorVec spectators;

	int32_t start_x_aligned = start_x - (start_x % TILE_GRID_SIZE);
	int32_t start_y_aligned = start_y - (start_y % TILE_GRID_SIZE);
	int32_t end_x_aligned = end_x - (end_x % TILE_GRID_SIZE);
	int32_t end_y_aligned = end_y - (end_y % TILE_GRID_SIZE);

	if (auto start_leaf = find_leaf_in_root(start_x_aligned, start_y_aligned)) {
		auto south_leaf = start_leaf;

		for (int32_t ny = start_y_aligned; ny <= end_y_aligned; ny += TILE_GRID_SIZE) {
			auto east_leaf = south_leaf;

			for (int32_t nx = start_x_aligned; nx <= end_x_aligned; nx += TILE_GRID_SIZE) {
				if (east_leaf) {
					for (auto creature : east_leaf->creatures) {
						spectators.emplace_back(creature);
					}

					east_leaf = east_leaf->east_leaf;
				} else {
					east_leaf = find_leaf_in_root(nx + TILE_GRID_SIZE, ny);
				}
			}

			if (south_leaf) {
				south_leaf = south_leaf->south_leaf;
			} else {
				south_leaf = find_leaf_in_root(start_x_aligned, ny + TILE_GRID_SIZE);
			}
		}
	}

	return spectators;
}

Tile* tfs::map::quadtree::find_tile(uint16_t x, uint16_t y, uint8_t z)
{
	if (auto leaf = find_leaf_in_root(x, y)) {
		// Find the tile at layer z, using TILE_INDEX_MASK to ensure that the x and y coordinates
		// are within the bounds of the leaf (only the least significant bits are used).
		return leaf->tiles[z][x & TILE_INDEX_MASK][y & TILE_INDEX_MASK];
	}
	return nullptr;
}

void tfs::map::quadtree::create_tile(uint16_t x, uint16_t y, uint8_t z, Tile* tile)
{
	create_leaf_in_root(x, y);

	if (auto leaf = find_leaf_in_root(x, y)) {
		// Store the tile in the correct position in the tile array.
		// Here, we also use TILE_INDEX_MASK to index correctly, ensuring that only
		// the relevant bits of the x and y coordinates are used.
		leaf->tiles[z][x & TILE_INDEX_MASK][y & TILE_INDEX_MASK] = tile;
	}
}

void tfs::map::quadtree::move_creature(uint16_t old_x, uint16_t old_y, uint16_t x, uint16_t y, Creature* creature)
{
	if (auto old_leaf = find_leaf_in_root(old_x, old_y)) {
		if (auto leaf = find_leaf_in_root(x, y)) {
			if (old_leaf != leaf) {
				old_leaf->remove_creature(creature);
				leaf->push_creature(creature);
			}
		}
	}
}

void tfs::map::quadtree::push_creature(uint16_t x, uint16_t y, Creature* creature)
{
	if (auto leaf = find_leaf_in_root(x, y)) {
		leaf->push_creature(creature);
	}
}

void tfs::map::quadtree::remove_creature(uint16_t x, uint16_t y, Creature* creature)
{
	if (auto leaf = find_leaf_in_root(x, y)) {
		leaf->remove_creature(creature);
	}
}

Node::~Node()
{
	for (auto node_ptr : nodes) {
		delete node_ptr;
	}
}

Leaf::~Leaf()
{
	for (auto& layer : tiles) {
		for (auto& row : layer) {
			for (auto tile : row) {
				delete tile;
			}
		}
	}
}

void Leaf::push_creature(Creature* creature) { creatures.insert(creature); }

void Leaf::remove_creature(Creature* creature) { creatures.erase(creature); }
