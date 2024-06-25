
#include "SingleCell.h"
//using namespace std;  //forget this

// Global Variables
//double dx, dy;
std::vector <point> nodes = {{0,0}, {1,0}, {1,1}, {0,1}};
//vector <point> nodes = {{1,0}, {2,0}, {2,1}, {1,1}};
std::array <std::array<double,2>, 2> interface = {{{1, 0.5}, {0, 0.5}}};  // Note: when i switch points the normal vector points downward...


// Finding the normal vector between 2 points ///////////////////////////////////////////
std::array <double, 2> normV(std::array <std::array<double, 2>,2> &inter){
   // Direction vec
   double dx = inter[1][0] - inter[0][0];     // x2 - x1
   double dy = inter[1][1] - inter[0][1];     // y2 - y1
   
   // Normal vec and normalized
   std::array <double, 2> normal = {dy, -dx};
   return normal;
}

// Finding the dot product of the array and vector //////////////////////////////////////
double dotProduct(std::array <double, 2> v , std::array<double,2> &n){
   double product = (v[0] * n[0]) + (v[1] * n[1]);
   return product;
}

// Trying to figure out the loop to eventually store Cell points ////////////////////////
void arrange(int cols, int rows){
   int cell = 0, loop = 0;
   for (int i = 0; i < (cols - 1); ++i){               // 1) Total amount of cells in columns
       for(int j = 0; j < (rows - 1) ; ++j){           // 2) Total amount of cells in rows
           printf("Cell %d: ", cell);
           for(int y = i; y < (i + 2); ++y){           // 3) going through the points (x,y)
               for(int x = j + loop; x < (j + 2) + loop; ++x){
                   printf("(%d, %d) ", x, y);
                   if(loop == 1){                      // Note: This will help keep track of loop
                       x -= 2;
                       loop = -1;
                   }
               }
              loop++;
           }
           printf("\n");
           cell++;
       }
   }
}

// Print the normal vector //////////////////////////////////////////////////////////////
void printN(std::array <double, 2> normal){
   printf("Normal: ");
   for(int i = 0; i < 2; ++i){
       printf("%.1lf ", normal[i]);
   }
   printf("\n");
}


// Point Vector /////////////////////////////////////////////////////////////////////////
std::array <double, 2> pointVec(const point &p, std::array <std::array<double,2>,2> inter){
   double dx = inter[0][0] - p.x;
   double dy = inter[0][1] - p.y;
   
   std::array <double, 2> point = {dx, dy};
   return point;
}

// Will use maybe later??
char compare(const point &p, std::array<std::array<double, 2>,2> &delta){
   char result = 'T';
   if(delta[0][1] > p.y && delta[0][0] <= p.x){
       result = 'B';
   }
   return result;
}


int main(int argc, const char * argv[]){
   int helper = 0, help = 0;
   std::pair <double,double> Top[100], Bottom[100];
   std::array <double, 2> V;
   
   // 1) Find the coordinates in a clockwise manner ////////////////
   auto start = std::chrono::high_resolution_clock::now();
   arrange(2,2);   // Ignore for now
   auto end = std::chrono::high_resolution_clock::now();
   printf("\n");
   std::chrono::duration<double> time = end - start;
   printf("Time: %lf\n", time.count());
   
   // 2) Deduce the normal vector of the cutting line //////////////
   std::array <double, 2> normal = normV(interface);
   printN(normal);
   printf("\n");
   
   // 3) Deduce the sign of every node using the dot product ///////
   for(const auto& point : nodes){
       // Vector of Node
       V = pointVec(point, interface);
       
       // Dot Product of normal and node vector
       double dp = dotProduct(V , normal);
       printf("Dot Product of (%.1lf, %.1lf) and (%.1lf, %.1lf): %.2lf\n", V[0], V[1], normal[0], normal[1], dp);
       
       // Convection of placement
       if(dp < 0){                             // Over the Line for ths case
           Top[helper] = {point.x, point.y};
           helper++;
       }
       else{                                   // Under the line for this case
           Bottom[help] = {point.x, point.y};
           help++;
       }
   }
   
   printf("\n");
   for(int i = 0; i < helper; ++i){
       printf("Top Coordinate: (%.2lf, %.2lf)\n", Top[i].first, Top[i].second);
   }
   printf("\n");
   for(int i = 0; i < help; ++i){
       printf("Bottom Coordinate: (%.2lf, %.2lf)\n", Bottom[i].first, Bottom[i].second);
   }
   printf("\n");
   
}
