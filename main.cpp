#include "clippings.h"
#include "mesh.h"
#include "intersect.h"

int main(int argc, const char * argv[]){
    using namespace polyintersect;
    bool horizontal = true;
    int n_cells = 4;
    Mesh mesh(n_cells);

    std::vector<std::array<Point, 2>> line(4); 
    line[0] = {{{2.0, 0.10}, {-1.0, 0.10}}}; // ccw
    line[1] = {{{2.0, 0.4}, {-1.0, 0.4}}}; // ccw
    line[2] = {{{2.0, 0.6}, {-1.0, 0.6}}}; // ccw
    line[3] = {{{2.0, 0.85}, {-1.0, 0.85}}}; // ccw 

    for(int c = 0; c < (n_cells * n_cells); c++){
        int const k = static_cast<int>(c / n_cells);
        auto const interface1 = intersect_cell_with_line(mesh, c, line[k]);
        auto const belowLine3 = clip_below_3(c, mesh, interface1, true);
    }

    return EXIT_SUCCESS;
}

int main2(int argc, const char * argv[]){

    using namespace polyintersect;
    bool horizontal = true;
    bool four = false;

    std::map<std::pair<int, int>, int> mapping;
    std::array<Point, 2> line1, line2, line3, line4, line5, line6, line7, line8;
    std::array<Point, 2> line9, line10, line11, line12, line13, line14, line15, line16;

    // 4 by 4 Case
    if(four){
    int n_cells = 4;
    Mesh mesh(n_cells);
        
        // Horizontal Interface /////////////////
        if(horizontal){
            std::cout << std::endl << "////////////////// Horizontal ///////////////////" << std::endl;

            // Interface
            line1 = {{{2.0, 0.10}, {-1.0, 0.10}}}; // ccw
            line2 = {{{2.0, 0.4}, {-1.0, 0.4}}}; // ccw
            line3 = {{{2.0, 0.6}, {-1.0, 0.6}}}; // ccw
            line4 = {{{2.0, 0.85}, {-1.0, 0.85}}}; // ccw

            for(int i = 0; i < 4; i++){
                auto const interface1 = intersect_cell_with_line(mesh, i, line1);
                auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
            }
            for(int i = 4; i < 8; i++){
                auto const interface2 = intersect_cell_with_line(mesh, i, line2);
                auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
            }
            for(int i = 8; i < 12; i++){
                auto const interface3 = intersect_cell_with_line(mesh, i, line3);
                auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
            }
            for(int i = 12; i < 16; i++){
                auto const interface4 = intersect_cell_with_line(mesh, i, line4);
                auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
            }
        }
        
        // Vertical Interface ///////////////////
        else{
            std::cout << std::endl << "////////////////// Vertical ///////////////////" << std::endl;

            // Interface
            line1 = {{{.1, -1}, {.1, 2}}}; // ccw
            line2 = {{{.4, -1}, {.4, 2}}}; // ccw
            line3 = {{{.6, -1}, {.6, 2}}}; // ccw
            line4 = {{{.8, -1}, {.8, 2}}}; // ccw

            // Finding the interface that passes through each cell
            for(int i = 0; i < 16; i++){
                if(i % n_cells == 0){
                    auto const interface1 = intersect_cell_with_line(mesh, i, line1);
                    auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
                }
                else if(i % n_cells == 1){
                    auto const interface2 = intersect_cell_with_line(mesh, i, line2);
                    auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
                }
                else if(i % n_cells == 2){
                    auto const interface3 = intersect_cell_with_line(mesh, i, line3);
                    auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
                }
                else if(i % n_cells == 3){
                    auto const interface4 = intersect_cell_with_line(mesh, i, line4);
                    auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
                }
            }
        }
    }

    // 16 by 16 Case ////////////////////////////
    else{
        int n_cells = 16;
        Mesh mesh(n_cells);
        
        // Horizontal Interface /////////////////
        if(horizontal){
            std::cout << std::endl << "////////////////// Horizontal ///////////////////" << std::endl;

            // Interface
            line1 = {{{0.03, -1.0}, {0.03, 2.0}}};  // ccw
            line2 = {{{0.09, -1}, {0.09}}};  // ccw
            line3 = {{{0.14, -1}, {0.14}}};  // ccw
            line4 = {{{0.2}, {0.2}}};    // ccw
            line5 = {{{0.29}, {0.29}}};  // ccw
            line6 = {{{0.35}, {0.35}}};  // ccw
            line7 = {{{0.4}, {0.4}}};    // ccw
            line8 = {{{0.45}, {0.45}}};  // ccw

            line9 = {{{0.53}, {0.53}}};    // ccw
            line10 = {{{0.6}, {0.6}}};   // ccw
            line11 = {{{0.65}, {0.65}}}; // ccw
            line12 = {{{0.72}, {0.72}}}; // ccw
            line13 = {{{0.79}, {0.79}}}; // ccw
            line14 = {{{0.85}, {0.85}}}; // ccw
            line15 = {{{0.9}, {0.9}}};   // ccw
            line16 = {{{0.95}, {0.95}}}; // ccw

            for(int i = 0; i < 16; i++){
                auto const interface1 = intersect_cell_with_line(mesh, i, line1);
                auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
            }
            for(int i = 16; i < 32; i++){
                auto const interface2 = intersect_cell_with_line(mesh, i, line2);
                auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
            }
            for(int i = 32; i < 48; i++){
                auto const interface3 = intersect_cell_with_line(mesh, i, line3);
                auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
            }
            for(int i = 48; i < 64; i++){
                auto const interface4 = intersect_cell_with_line(mesh, i, line4);
                auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
            }

            for(int i = 64; i < 80; i++){
                auto const interface1 = intersect_cell_with_line(mesh, i, line5);
                auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
            }
            for(int i = 80; i < 96; i++){
                auto const interface2 = intersect_cell_with_line(mesh, i, line6);
                auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
            }
            for(int i = 96; i < 112; i++){
                auto const interface3 = intersect_cell_with_line(mesh, i, line7);
                auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
            }
            for(int i = 112; i < 128; i++){
                auto const interface4 = intersect_cell_with_line(mesh, i, line8);
                auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
            }

            for(int i = 128; i < 144; i++){
                auto const interface1 = intersect_cell_with_line(mesh, i, line9);
                auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
            }
            for(int i = 144; i < 160; i++){
                auto const interface2 = intersect_cell_with_line(mesh, i, line10);
                auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
            }
            for(int i = 160; i < 176; i++){
                auto const interface3 = intersect_cell_with_line(mesh, i, line11);
                auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
            }
            for(int i = 176; i < 192; i++){
                auto const interface1 = intersect_cell_with_line(mesh, i, line12);
                auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
            }

            for(int i = 192; i < 208; i++){
                auto const interface2 = intersect_cell_with_line(mesh, i, line13);
                auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
            }
            for(int i = 208; i < 224; i++){
                auto const interface3 = intersect_cell_with_line(mesh, i, line14);
                auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
            }
            for(int i = 224; i < 240; i++){
                auto const interface4 = intersect_cell_with_line(mesh, i, line15);
                auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
            }
            for(int i = 240; i < 256; i++){
                auto const interface4 = intersect_cell_with_line(mesh, i, line16);
                auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
            }
        }

        // Vertical Interface ///////////////////
        else{
            std::cout << std::endl << "////////////////// Vertical ///////////////////" << std::endl;

            // Interface
            line1 = {{{2.0, 0.03}, {-1.0, 0.03}}};  // ccw
            line2 = {{{2.0, 0.09}, {-1.0, 0.09}}};  // ccw
            line3 = {{{2.0, 0.14}, {-1.0, 0.14}}};  // ccw
            line4 = {{{2.0, 0.2}, {-1.0, 0.2}}};    // ccw
            line5 = {{{2.0, 0.29}, {-1.0, 0.29}}};  // ccw
            line6 = {{{2.0, 0.35}, {-1.0, 0.35}}};  // ccw
            line7 = {{{2.0, 0.4}, {-1.0, 0.4}}};    // ccw
            line8 = {{{2.0, 0.45}, {-1.0, 0.45}}};  // ccw

            line9 = {{{2.0, 0.53}, {-1.0, 0.53}}};    // ccw
            line10 = {{{2.0, 0.6}, {-1.0, 0.6}}};   // ccw
            line11 = {{{2.0, 0.65}, {-1.0, 0.65}}}; // ccw
            line12 = {{{2.0, 0.72}, {-1.0, 0.72}}}; // ccw
            line13 = {{{2.0, 0.79}, {-1.0, 0.79}}}; // ccw
            line14 = {{{2.0, 0.85}, {-1.0, 0.85}}}; // ccw
            line15 = {{{2.0, 0.9}, {-1.0, 0.9}}};   // ccw
            line16 = {{{2.0, 0.95}, {-1.0, 0.95}}}; // ccw

            // Finding the interface that passes through each cell
            for(int i = 0; i < (n_cells * n_cells); i++){
                if(i % n_cells == 0){
                    auto const interface1 = intersect_cell_with_line(mesh, i, line1);
                    auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
                }
                else if(i % n_cells == 1){
                    auto const interface2 = intersect_cell_with_line(mesh, i, line2);
                    auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
                }
                else if(i % n_cells == 2){
                    auto const interface3 = intersect_cell_with_line(mesh, i, line3);
                    auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
                }
                else if(i % n_cells == 3){
                    auto const interface4 = intersect_cell_with_line(mesh, i, line4);
                    auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
                }
                else if(i % n_cells == 4){
                    auto const interface2 = intersect_cell_with_line(mesh, i, line5);
                    auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
                }
                else if(i % n_cells == 5){
                    auto const interface3 = intersect_cell_with_line(mesh, i, line6);
                    auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
                }
                else if(i % n_cells == 6){
                    auto const interface4 = intersect_cell_with_line(mesh, i, line7);
                    auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
                }
                else if(i % n_cells == 7){
                    auto const interface4 = intersect_cell_with_line(mesh, i, line8);
                    auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
                }
                else if(i % n_cells == 8){
                    auto const interface1 = intersect_cell_with_line(mesh, i, line8);
                    auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
                }
                else if(i % n_cells == 9){
                    auto const interface2 = intersect_cell_with_line(mesh, i, line10);
                    auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
                }
                else if(i % n_cells == 10){
                    auto const interface3 = intersect_cell_with_line(mesh, i, line11);
                    auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
                }
                else if(i % n_cells == 11){
                    auto const interface4 = intersect_cell_with_line(mesh, i, line12);
                    auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
                }
                else if(i % n_cells == 12){
                    auto const interface2 = intersect_cell_with_line(mesh, i, line13);
                    auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
                }
                else if(i % n_cells == 13){
                    auto const interface3 = intersect_cell_with_line(mesh, i, line14);
                    auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
                }
                else if(i % n_cells == 14){
                    auto const interface4 = intersect_cell_with_line(mesh, i, line15);
                    auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
                }
                else if(i % n_cells == 15){
                    auto const interface4 = intersect_cell_with_line(mesh, i, line16);
                    auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
                }
            }
        }
    }
    



    return EXIT_SUCCESS;
}