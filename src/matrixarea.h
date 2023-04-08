#ifndef TFS_MATRIXAREA
#define TFS_MATRIXAREA

class MatrixArea
{
	using Center = std::pair<uint32_t, uint32_t>;
	using Container = std::valarray<bool>;

public:
	MatrixArea() = default;
	MatrixArea(uint32_t rows, uint32_t cols) : arr(rows * cols), rows{rows}, cols{cols} {}

	bool operator()(uint32_t row, uint32_t col) const { return arr[row * cols + col]; }
	bool& operator()(uint32_t row, uint32_t col) { return arr[row * cols + col]; }

	void setCenter(uint32_t y, uint32_t x) { center = std::make_pair(x, y); }
	const Center& getCenter() const { return center; }

	uint32_t getRows() const { return rows; }
	uint32_t getCols() const { return cols; }

	[[nodiscard]] MatrixArea rotate90() const;
	[[nodiscard]] MatrixArea rotate180() const;
	[[nodiscard]] MatrixArea rotate270() const;

	operator bool() const { return rows == 0 || cols == 0; }

private:
	MatrixArea(Center center, uint32_t rows, uint32_t cols, Container&& arr) :
	    arr{std::move(arr)}, center{std::move(center)}, rows{rows}, cols{cols}
	{}

	Container arr = {};
	Center center = {};
	uint32_t rows = 0, cols = 0;
};

MatrixArea createArea(const std::vector<uint32_t>& vec, uint32_t rows);

#endif
