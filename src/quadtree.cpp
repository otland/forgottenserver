#include "quadtree.h"

namespace {
std::array<QuadTree*, 4> nodes = {};

uint8_t create_index(uint16_t x, uint16_t y) { return ((x & 0x8000) >> 15) | ((y & 0x8000) >> 14); }

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

Leaf* find_leaf_in_root(uint16_t x, uint16_t y)
{
	auto index = create_index(x, y);
	if (auto node = nodes[index]; auto leaf = find_leaf(x, y, node)) {
		return static_cast<Leaf*>(leaf);
	}
	return nullptr;
}

void create_leaf(uint16_t x, uint16_t y, uint8_t z, QuadTree* node)
{
	if (node->is_leaf()) {
		return;
	}

	auto index = create_index(x, y);
	auto node_child = node->get_child(index);
	if (!node_child) {
		if (z == TILE_GRID_BITS) {
			node_child = new Leaf(x, y);
		} else {
			node_child = new Node();
		}

		node->set_child(index, node_child);
	}

	create_leaf(x * 2, y * 2, z - 1, node_child);
}

void create_leaf_in_root(uint16_t x, uint16_t y)
{
	auto index = create_index(x, y);
	if (!nodes[index]) {
		nodes[index] = new Node();
	}

	create_leaf(x, y, (MAP_MAX_LAYERS - 1), nodes[index]);
}

} // namespace

std::experimental::generator<Creature*> tfs::map::quadtree::find(uint16_t start_x, uint16_t start_y, uint16_t end_x,
                                                                 uint16_t end_y)
{
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
						co_yield creature;
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

	co_return;
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

/**
 * @brief Constructs a Leaf node in the quadtree and establishes relationships with neighboring Leaf nodes.
 *
 * The constructor takes the coordinates of the leaf and attempts to find neighboring leaf nodes (north, south, east,
 * and west)
 * within the quadtree. If neighboring leaf nodes are found, it establishes two-way relationships between
 * the leaf nodes by updating their respective pointers.
 *
 * @param {x} The x-coordinate of the leaf node in the
 * quadtree.
 * @param {y} The y-coordinate of the leaf node in the quadtree.
 *
 * The following relationships are
 * established:
 * - North neighbor's south_leaf points to this leaf.
 * - West neighbor's east_leaf points to this
 * leaf.
 * - This leaf south_leaf points to the south neighbor, if found.
 * - This leaf east_leaf points to the
 * east neighbor, if found.
 */
Leaf::Leaf(uint16_t x, uint16_t y)
{
	// update north
	if (auto north_leaf = find_leaf_in_root(x, y - TILE_GRID_SIZE)) {
		north_leaf->south_leaf = this;
	}

	// update west
	if (auto west_leaf = find_leaf_in_root(x - TILE_GRID_SIZE, y)) {
		west_leaf->east_leaf = this;
	}

	// update south
	if (auto south_leaf = find_leaf_in_root(x, y + TILE_GRID_SIZE)) {
		this->south_leaf = south_leaf;
	}

	// update east
	if (auto east_leaf = find_leaf_in_root(x + TILE_GRID_SIZE, y)) {
		this->east_leaf = east_leaf;
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
