#pragma once

#include <cstdint>
#include <string>
#include <sstream>

#include "position.h"

class WalkMatrix {
public:

    void updatePosition(const Position& newPos)
    {
        int32_t xdiff = pos.x - newPos.x;
        int32_t ydiff = pos.y - newPos.y;
        if (pos.z != newPos.z || std::abs(xdiff) > 6 || std::abs(ydiff) > 6) {
            pos = newPos;
            return clear();
        }
        pos = newPos;

        if (xdiff < 0) {
            for (int x = 0; x < 7; ++x) {
                for (int y = 0; y < 7; ++y) {
                    if (x - xdiff > 6) {
                        matrix[x][y] = 0;
                    } else {
                        matrix[x][y] = matrix[x - xdiff][y];
                    }
                }
            }
        }
        if (xdiff > 0) {
            for (int x = 6; x >= 0; --x) {
                for (int y = 6; y >= 0; --y) {
                    if (x - xdiff < 0) {
                        matrix[x][y] = 0;
                    } else {
                        matrix[x][y] = matrix[x - xdiff][y];
                    }
                }
            }
        }
        if (ydiff < 0) {
            for (int x = 0; x < 7; ++x) {
                for (int y = 0; y < 7; ++y) {
                    if (y - ydiff > 6) {
                        matrix[x][y] = 0;
                    } else {
                        matrix[x][y] = matrix[x][y - ydiff];
                    }
                }
            }
        }
        if (ydiff > 0) {
            for (int x = 6; x >= 0; --x) {
                for (int y = 6; y >= 0; --y) {
                    if (y - ydiff < 0) {
                        matrix[x][y] = 0;
                    } else {
                        matrix[x][y] = matrix[x][y - ydiff];
                    }
                }
            }
        }
    }

    bool inRange(const Position& pos2)
    {
        if (pos.z != pos2.z) return false;
        int32_t xdiff = pos.x - pos2.x;
        int32_t ydiff = pos.y - pos2.y;
        if (std::abs(xdiff) > 3 || std::abs(ydiff) > 3) return false;
        return true;
    }

    int32_t update(const Position& pos2, int32_t value = 0)
    {
        if (pos.z != pos2.z) return 0;
        int32_t xdiff = pos.x - pos2.x;
        int32_t ydiff = pos.y - pos2.y;
        if (std::abs(xdiff) > 3 || std::abs(ydiff) > 3) return 0;
        if (value != 0) {
            matrix[3 - xdiff][3 - ydiff] = value;
            return value;
        }
        counter += 1;
        matrix[3 - xdiff][3 - ydiff] = counter;
        return counter;
    }

    int32_t get(const Position& pos2)
    {
        if (pos.z != pos2.z) return 0;
        int32_t xdiff = pos.x - pos2.x;
        int32_t ydiff = pos.y - pos2.y;
        if (std::abs(xdiff) > 3 || std::abs(ydiff) > 3) return 0;
        return matrix[3 - xdiff][3 - ydiff];
    }

    void clear()
    {
        for (auto& x : matrix) {
            for (auto& y : x) {
                y = 0;
            }
        }
    }

    uint32_t reset(uint32_t value = 0)
    {
        if (value == 0) {
            counter += 1;
            value = counter;
        }
        for (auto& x : matrix) {
            for (auto& y : x) {
                y = value;
            }
        }
        return value;
    }

    std::string dump()
    {
        std::stringstream ss;
        for (int y = 0; y < 7; ++y) {
            for (int x = 0; x < 7; ++x) {
                ss << matrix[x][y] << "\t";
            }
            ss << "\n";
        }
        return ss.str();
    }

private:
    Position pos;
    int32_t matrix[7][7] = { { 0 } };
    int32_t counter = 0;
};