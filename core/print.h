#pragma once      //prevent headers being used twice

#include <iostream>
#include <cstdio>
#include "geometry.h"

namespace polyintersect {

    void print_results(int end, int end_including_copy, int max_threads, int total_cells, int total_points, 
                       Kokkos::View<Point*>::HostMirror mirror_points_, Kokkos::View<int***>::HostMirror mirror_cells_, 
		       Kokkos::View<Segment*>::HostMirror mirror_intersect_points_, Kokkos::View<Line*>::HostMirror mirror_line_, 
		       Kokkos::View<int*>::HostMirror mirror_num_verts_per_cell_, Kokkos::View<Point**>::HostMirror mirror_allPoints_, 
                       Kokkos::View<int**>::HostMirror mirror_size_output_, Kokkos::View<int***>::HostMirror mirror_output_, 
		       Kokkos::View<int**>::HostMirror mirror_signs_){

        // Print elapsed time 
        std::cout << "Duration: " << end << " µs" << std::endl;
        std::cout << "Deep copy: " << end_including_copy << " µs" << std::endl;
        std::cout << "Max Threads: " << max_threads << std::endl << std::endl; 

        // Print Cells 
        std::cout << std::endl;
        std::cout << "---------------- GPU Results ----------------" << std::endl;
	    std::cout << std::endl;

	    std::cout << "------ Cell + Edges ------" << std::endl;
            for(int j = 0; j < total_cells; j++){   // Cell
                std::cout << "Cell " << j << ":" << std::endl;
                int t = mirror_num_verts_per_cell_(j);
                for (int i = 0; i < t; i++) {      // Edge       
                    std::cout << "Edge " << i << " (" << mirror_cells_(j , i, 0) << ", ";
                    std::cout << mirror_cells_(j, i, 1) << ") ";
            }
                std::cout << std::endl;
            }
        std::cout << std::endl;

        // Print Point Coordinates
	    std::cout << "------ Cell Vertices ------" << std::endl;
            for (int j = 0; j < total_points; j++) {             // All Points
                std::cout << "Point " << j << ": (" << mirror_points_(j).x << ", " << mirror_points_(j).y << ")" << std::endl;
        }

	     // Print Line
        std::cout << std::endl;
        std::cout << "------ Line ------" << std::endl;
        for (int j = 0; j < total_cells; ++j) {
           	 auto const pa = mirror_line_(j).n;
           	 auto const dist = mirror_line_(j).d;
        	 std::cout << "Line at Cell  "<< j << ": normal = ("<< pa.x << ", "<< pa.y << ") and distance = " << dist << std::endl;
	}
       
	    // Print Intersect Points
        std::cout << std::endl;
        std::cout << "------ Intersect Points ------" << std::endl;
        for (int j = 0; j < total_cells; ++j) {
	    int below = mirror_size_output_(j, 0);
            if(below > 0){
               auto const pa = mirror_intersect_points_(j).a;
               auto const pb = mirror_intersect_points_(j).b;
               std::cout << "Intersection Points at Cell  "<< j << ": ("<< pa.x << ", "<< pa.y << "), ("<< pb.x << ", "<< pb.y << ")" << std::endl;
	    }
	 }

	    // Print all Points (Vertices + Intersect Points)
        std::cout << std::endl;
	    std::cout << "------ All Points ------" << std::endl;
        for(int c = 0; c < total_cells; c++){
		    int t = mirror_num_verts_per_cell_(c) + 2;
            int below = mirror_size_output_(c, 0);
	    if(below > 0){
            	for(int i = 0; i < t; i++){
                    auto const p = mirror_allPoints_(c, i);
               	    std::cout << "Points at Cell  " << c << ": (" << p.x << ", "<< p.y << ") "<< std::endl;
            	}
            	std::cout << std::endl;
	    }
        }

        // Output Results
        std::cout << std::endl;
	    std::cout << "------ Output ------" << std::endl;
        for(int c = 0; c < total_cells; c++){
	        int below = mirror_size_output_(c, 0);
	        int above = mirror_size_output_(c, 1);
		if(below > 0){
	        	std::cout << "Cell " << c << " Output: " << std::endl;
	        	for(int i = 0; i < below; i++){
                    	    int const j = mirror_output_(c, 0, i);
                    	    auto const p = mirror_allPoints_(c, j);
	            	    std::cout << "Below: (" << p.x << ", "<< p.y << ") "<< std::endl;
            		}
	        	for(int i = 0; i < above; i++){
                    	    int const j = mirror_output_(c, 1, i);
                    	    auto const p = mirror_allPoints_(c, j);
                    	    std::cout << "Above: (" << p.x << ", "<< p.y << ") "<< std::endl;
            		}
			std::cout << std::endl;	
		}
        }

	    // Print signs
	    std::cout << "------ Signs ------" << std::endl;
        for(int i = 0; i < total_cells; i++){
            int t = mirror_num_verts_per_cell_(i) + 2;
            for(int j = 0; j < t; j++){
                std::cout << "Sign at cell "<< i << ": " << mirror_signs_(i, j) << std::endl;
            }
            std::cout << std::endl;
        }
    }


}
