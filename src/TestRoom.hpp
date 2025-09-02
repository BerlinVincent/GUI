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
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2)},
        {Tile(1, 0), Tile(1, 1), Tile(1, 1), Tile(1, 1), Tile(1, 2)},
        {Tile(2, 0), Tile(2, 1), Tile(2, 1), Tile(2, 1), Tile(2, 2)}
    };
}

};