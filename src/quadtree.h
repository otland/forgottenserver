#include "otpch.h"

#include "creature.h"
#include "map.h"
#include "spectators.h"

namespace tfs::map::quadtree {

void find(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, SpectatorVec& spectators,
          std::function<bool(Creature*)> comparasion);

/// @brief Finds the tile at the specified coordinates and layer.
Tile* find_tile(uint16_t x, uint16_t y, uint8_t z);

/// @brief Creates a tile at the specified coordinates and layer.
void create_tile(uint16_t x, uint16_t y, uint8_t z, Tile* tile);

/// @brief Moves a creature from one location to another within the quadtree.
void move_creature(uint16_t old_x, uint16_t old_y, uint8_t old_z, uint16_t x, uint16_t y, uint8_t z,
                   Creature* creature);

/// @brief Adds a creature into the quadtree at the specified coordinates.
void push_creature(uint16_t x, uint16_t y, uint8_t z, Creature* creature);

/// @brief Removes a creature from the quadtree at the specified coordinates.
void remove_creature(uint16_t x, uint16_t y, uint8_t z, Creature* creature);

} // namespace tfs::map::quadtree

/**
 * @brief Base class representing a QuadTree.
 *
 * This class defines the interface for a QuadTree node.
 * The class is non-copyable to prevent accidental copying of nodes.
 */
class QuadTree
{
public:
	/// @brief Default constructor for QuadTree.
	explicit QuadTree() = default;

	/**
	 * @brief Virtual destructor for QuadTree.
	 *
	 * The virtual destructor allows for proper cleanup
	 * of derived classes
	 * that may be allocated dynamically.
	 */
	virtual ~QuadTree() = default;

	/// Deleted copy constructor to ensure QuadTree is non-copyable.
	QuadTree(const QuadTree&) = delete;
	/// Deleted assignment operator to ensure QuadTree is non-copyable.
	QuadTree& operator=(const QuadTree&) = delete;

	/**
	 * @brief Check if the node is a leaf node.
	 * @return true if the node is a leaf, false otherwise.
	 */
	virtual bool is_leaf() const = 0;

	/**
	 * @brief Set a child node at a specified index.
	 *
	 * This method assigns a child node to the
	 * current node at the given
	 * index. The index must be within valid bounds for child nodes (0 to 3).
	 * @param {index} The index at which to set the child node.
	 * @param {node} A pointer to the child node to be
	 * set.
	 */
	virtual void set_child(uint8_t index, QuadTree* node) = 0;

	/**
	 * @brief Get a child node at a specified index.
	 * @param {index} The index of the child node to
	 * retrieve (0 to 3).
	 * @return A pointer to the child node, or nullptr if no child exists.
	 */
	virtual QuadTree* get_child(uint8_t index) const = 0;
};

/// The number of bits used to represent the dimensions of a tile grid.
inline constexpr int32_t TILE_GRID_BITS = 3;
/// The size of the tile grid, calculated as 2 ^ TILE_GRID_BITS.
/// This value indicates the number of tiles that can fit along one dimension of the grid.
inline constexpr int32_t TILE_GRID_SIZE = (1 << TILE_GRID_BITS);
/// A mask to isolate the tile index within the bounds of the grid size.
/// This mask is used to ensure tile indices remain valid and wrap around correctly.
inline constexpr int32_t TILE_INDEX_MASK = (TILE_GRID_SIZE - 1);

/**
 * @class Node
 * @brief Represents a node in a QuadTree.
 *
 * This class extends the QuadTree interface and
 * implements a specific type of node that can have up to four child nodes.
 */
class Node final : public QuadTree
{
public:
	/// @brief Default constructor for QuadTree.
	explicit Node() = default;

	/// Deleted copy constructor to ensure Node is non-copyable.
	Node(const Node&) = delete;
	/// Deleted assignment operator to ensure Node is non-copyable.
	Node& operator=(const Node&) = delete;

	/**
	 * @brief Check if the node is a leaf node.
	 *
	 * This implementation always returns false, as Node
	 * objects are designed to have children.
	 * @return false, indicating that the node is not a leaf.
	 */
	bool is_leaf() const override { return false; }

	/**
	 * @brief Set a child node at a specified index.
	 *
	 * This method assigns a child node to the
	 * current node at the given index. The index must be in the range of 0 to 3.
	 *
	 * @param {index} The index
	 * at which to set the child node (0 to 3).
	 * @param {node} A pointer to the child QuadTree node to be set.
	 */
	void set_child(uint8_t index, QuadTree* node) override { nodes[index] = node; }

	/**
	 * @brief Get a child node at a specified index.
	 *
	 * This method retrieves the child node at the
	 * specified index.
	 *
	 * @param {index} The index of the child node to retrieve (0 to 3).
	 * @return A
	 * pointer to the child QuadTree node, or nullptr if no child exists.
	 */
	QuadTree* get_child(uint8_t index) const override { return nodes[index]; };

private:
	/// An array storing pointers to the child nodes of the QuadTree node.
	std::array<QuadTree*, 4> nodes = {};
};

/**
 * @class Leaf
 * @brief Represents a leaf node in a QuadTree.
 *
 * This class extends the QuadTree interface and
 * is designed to hold data specific to a leaf node, such as creatures and tile information.
 */
class Leaf final : public QuadTree
{
public:
	/**
	 * @brief Constructor for Leaf.
	 *
	 * This constructor initializes the leaf node with the specified
	 * coordinates.
	 */
	explicit Leaf(uint16_t x, uint16_t y);

	/// Deleted copy constructor to ensure Leaf is non-copyable.
	Leaf(const Leaf&) = delete;
	/// Deleted assignment operator to ensure Leaf is non-copyable.
	Leaf& operator=(const Leaf&) = delete;

	/**
	 * @brief Check if the node is a leaf node.
	 *
	 * This implementation always returns true,
	 * indicating that this object is a leaf.
	 * @return true, indicating that the node is a Leaf.
	 */
	bool is_leaf() const override { return true; }

	/**
	 * @brief Set a child node at a specified index.
	 *
	 * This method does nothing, as leaf nodes
	 * cannot have children.
	 * @param {index} The index at which to set the child node (not used).
	 * @param
	 * {node} A pointer to the child node to be set (not used).
	 */
	void set_child(uint8_t, QuadTree*) override {}

	/**
	 * @brief Get a child node at a specified index.
	 *
	 * This method always returns nullptr, as leaf
	 * nodes do not have children.
	 * @param {index} The index of the child node to retrieve (not used).
	 * @return Always returns nullptr.
	 */
	QuadTree* get_child(uint8_t) const override { return nullptr; };

	/**
	 * @brief Add a creature to the leaf.
	 *
	 * This method adds the specified creature to the Leaf
	 * node.
	 * @param {creature} A pointer to the creature to be added.
	 */
	void push_creature(Creature* creature);

	/**
	 * @brief Remove a creature from the leaf.
	 *
	 * This method removes the specified creature from
	 * the leaf node.
	 * @param {creature} A pointer to the creature to be removed.
	 */
	void remove_creature(Creature* creature);

	/**
	 * @brief A 3D array of pointers to tiles for the leaf across multiple layers.
	 *
	 *  This array holds pointers to `Tile` objects, organized in a three-dimensional
	 *  structure. The first dimension represents different layers, while the second
	 *  and third dimensions represent the x and y coordinates of the tiles within
	 *  each layer. Each layer can contain a grid of tiles.
	 */
	std::array<std::array<std::array<Tile*, TILE_GRID_SIZE>, TILE_GRID_SIZE>, MAP_MAX_LAYERS> tiles = {};

	/// @brief A set of creatures (monsters, NPCs and players) present in this leaf node.
	std::set<Creature*> creatures;

	Leaf* south_leaf = nullptr;
	Leaf* east_leaf = nullptr;
};
