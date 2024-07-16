#pragma once
#include "geometry.h"
#include "Mesh.h"

namespace polyintersect {
/*
    Clippings.h Description:
        - Contains the 3 Different Clipping Methods.
            1) Sorts using edges
            2) Sorts by pointing to the next node
            3) Sorts using degrees
*/


// Below the Line Using Edges ///////////////////////////////////////////////////////////
std::vector<std::pair<int, int>> clip_below_1(int cell, Mesh const &mesh, 
                                              std::array<Point, 2> const& interface, 
                                              bool print,
                                              std::map<std::pair<int, int>, int> const& intersectPoints);

// Below the Line Pointing to the Next Node /////////////////////////////////////////////
std::vector<int> clip_below_2(int cell, Mesh const &mesh, 
                              std::array<Point, 2> const& interface, 
                              bool print,
                              std::map<std::pair<int, int>, int> const& intersectPoints);

// Below the line using Degrees /////////////////////////////////////////////////////////
std::vector<int> clip_below_3(int cell, Mesh const &mesh, 
                              std::array<Point, 2> const& interface, 
                              bool print);

}