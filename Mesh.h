#pragma once      //prevent headers being used twice

#include <iostream>
#include <cstdio>
#include <vector>
#include <array>
#include <cmath>
#include <chrono>
#include <map>
#include <utility>
#include <algorithm>
#include <cmath>


// Need to set up a single cell now knowing that i will be recieving x (rows) and y (columns)
struct point{
   double x = 0.;
   double y = 0.;
};  // Point