#pragma once
#include "geometry.h"

namespace polyintersect {
/*
    Code Description:
        - Calculates the normal vector of the interface.
        - Calculates the direction vector with respect to (WRT) the normal vector.
        - Calculates the dot product of the normal and direction vector.
            - This will indicate the sign of the node. 
        - This code contains 3 different clipping methods.
            1) Method 1: Clips using edges, compares the sign of every node,
               and stores as edges
                * But skips the the interface edge and uses maps.

            2) Method 2: Clips by pointing to the next node based on the sign 
               comparison.
                * But it uses a map and a reverse map.

            3) Method 3: Finds the center point of all the nodes, calculates 
               the degree of every node WTR to the center point, and sorts based 
               on smallest to biggest degree. Uses the sorted value of all points to 
               compare the signs.

*/


// Below the Line Using Edges ///////////////////////////////////////////////////////////
std::vector<std::pair<int, int>> clip_below_1(std::vector<point> const& nodes, 
                                              std::array<point, 2> const& interface,
                                              bool print);
// Below the Line Pointing to the Next Node /////////////////////////////////////////////
std::vector<int> clip_below_2(std::vector<point> const& nodes, 
                              std::array<point, 2> const& interface, 
                              bool print);
/////////////////////////////////////////////////////////////////////////////////////////
std::vector<int> clip_below_3(std::vector<point> const& nodes, 
                              std::array<point, 2> const& interface, 
                              bool print);

}