#pragma once

#include "Elements.hpp"

namespace maps {

std::vector<std::vector<Tile>> TestRoom() {
    return {
        {Tile(0, 0), Tile(0, 1), Tile(0, 1), Tile(0, 1), Tile(0, 1), Tile(0, 1), Tile(0, 1), Tile(0, 1), Tile(0, 1), Tile(0, 2)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(0, 5), Tile(2, 1), Tile(2, 1), Tile(2, 1), Tile(2, 1), Tile(2, 2)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2), Tile(-1, -1), Tile(-1, -1),  Tile(-1, -1),  Tile(-1, -1),  Tile(-1, -1)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2), Tile(-1, -1), Tile(0, 0),  Tile(0, 1),  Tile(0, 2),  Tile(-1, -1)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(6, 3), Tile(3, 1), Tile(6, 5),  Tile(1, 1),  Tile(1, 2),  Tile(-1, -1)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2), Tile(-1, -1), Tile(2, 0),  Tile(2, 1),  Tile(2, 2),  Tile(-1, -1)},
        {Tile(2, 0), Tile(2, 1), Tile(2, 1), Tile(2, 1), Tile(2, 2), Tile(-1, -1), Tile(-1, -1),  Tile(-1, -1),  Tile(-1, -1),  Tile(-1, -1)}
    };
}

std::vector<std::vector<Tile>> TestRoom2() {
    return {
        {Tile(4, 4), Tile(4, 5), Tile(4, 5), Tile(4, 5), Tile(4, 5), Tile(4, 5), Tile(4, 5), Tile(4, 5), Tile(4, 5), Tile(4, 6)},
        {Tile(5, 4), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 6)},
        {Tile(5, 4), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 6)},
        {Tile(5, 4), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 6)},
        {Tile(5, 4), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(2, 8), Tile(6, 5), Tile(6, 5), Tile(6, 5), Tile(6, 5), Tile(6, 6)},
        {Tile(5, 4), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 6), Tile(-1, -1), Tile(-1, -1),  Tile(-1, -1),  Tile(-1, -1),  Tile(-1, -1)},
        {Tile(5, 4), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 6), Tile(-1, -1), Tile(4, 4),  Tile(4, 5),  Tile(4, 6),  Tile(-1, -1)},
        {Tile(5, 4), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(9, 13), Tile(7, 5), Tile(6, 5),  Tile(5, 5),  Tile(5, 7),  Tile(-1, -1)},
        {Tile(5, 4), Tile(5, 5), Tile(5, 5), Tile(5, 5), Tile(5, 6), Tile(-1, -1), Tile(6, 4),  Tile(6, 5),  Tile(6, 6),  Tile(-1, -1)},
        {Tile(6, 4), Tile(6, 5), Tile(6, 5), Tile(6, 5), Tile(6, 6), Tile(-1, -1), Tile(-1, -1),  Tile(-1, -1),  Tile(-1, -1),  Tile(-1, -1)}
    };
}

};