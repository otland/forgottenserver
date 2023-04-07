#include "otpch.h"

#include "matrixarea.h"

MatrixArea MatrixArea::rotate90() const
{
	Container newArr(arr.size());
	for (uint32_t i = 0; i < rows; ++i) {
		// assign rows, top to bottom, to the current cols, right to left
		newArr[std::slice(i, cols, rows)] = arr[std::slice((rows - i - 1) * cols, cols, 1)];
	}
	auto &&[centerX, centerY] = center;
	return {{rows - centerY - 1, centerX}, cols, rows, std::move(newArr)};
}

MatrixArea MatrixArea::rotate180() const
{
	Container newArr(arr.size());
	std::reverse_copy(std::begin(arr), std::end(arr), std::begin(newArr));
	auto &&[centerX, centerY] = center;
	return {{cols - centerX - 1, rows - centerY - 1}, rows, cols, std::move(newArr)};
}

MatrixArea MatrixArea::rotate270() const
{
	Container newArr(arr.size());
	for (uint32_t i = 0; i < cols; ++i) {
		// assign cols, left to right, to the current rows, bottom to top
		newArr[std::slice(i * rows, rows, 1)] = arr[std::slice(cols - i - 1, rows, cols)];
	}
	auto &&[centerX, centerY] = center;
	return {{centerY, cols - centerX - 1}, cols, rows, std::move(newArr)};
}

MatrixArea createArea(const std::vector<uint32_t> &vec, uint32_t rows)
{
	uint32_t cols;
	if (rows == 0) {
		cols = 0;
	} else {
		cols = vec.size() / rows;
	}

	MatrixArea area{rows, cols};

	uint32_t x = 0;
	uint32_t y = 0;

	for (uint32_t value : vec) {
		if (value == 1 || value == 3) {
			area(y, x) = true;
		}

		if (value == 2 || value == 3) {
			area.setCenter(y, x);
		}

		++x;

		if (cols == x) {
			x = 0;
			++y;
		}
	}
	return area;
}
