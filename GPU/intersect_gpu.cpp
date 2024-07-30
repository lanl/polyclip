#include "intersect_gpu.h"
#include <Kokkos_Core.hpp>
#include <Kokkos_Vector.hpp>
#include <cfloat>


namespace polyintersect {
    Line intersect_cell_with_line(Mesh_Kokkos const& mesh,
                                  int c,
                                  Line const& line) {

        int const n = 4;
        Point points[2];

        // deduce bounds on coordinates
        double x_min = DBL_MAX; // DBL_MAX
        double y_min = x_min; 
        double x_max = -x_min;
        double y_max = -y_min;

        for (int i = 0; i < n; ++i) {
            int const a = mesh.cells_(c, i);
            if(mesh.points_(a).x < x_min){      // x min
                x_min = mesh.points_(a).x;
            }
            if(mesh.points_(a).y < y_min){      // y min
                y_min = mesh.points_(a).y;
            }
            if(mesh.points_(a).x > x_max){      // x max
                x_min = mesh.points_(a).x;
            }
            if(mesh.points_(a).y > y_max){      // y max 
                y_min = mesh.points_(a).y;
            }
        }

        int k = 0;
        for (int i = 0; i < n; ++i) {
            int const j = (i + 1) % n;
            int const a = mesh.cells_(c, i);//[c][i];
            int const b = mesh.cells_(c, j);//[c][j];
            
            double const& xa = mesh.points_(a).x;
            double const& ya = mesh.points_(a).y;
            double const& xb = mesh.points_(b).x;
            double const& yb = mesh.points_(b).y;

            double const& xp = line.a.x;
            double const& yp = line.a.y;
            double const& xq = line.b.x;
            double const& yq = line.b.y;

            // (ab) is represented as: a1x + b1y = c1
            // (pq) is represented as: a2x + b2y = c2
            double const a1 = yb - ya;
            double const b1 = xa - xb;
            double const a2 = yq - yp;
            double const b2 = xp - xq;
            double const det = a1 * b2 - a2 * b1;
            
            // fabs
            if (fabs(det) < 1.e-15) {
                continue;
            } else {
                double const c1 = a1 * xa + b1 * ya;
                double const c2 = a2 * xp + b2 * yp;
                double const x = (b2 * c1 - b1 * c2) / det;
                double const y = (a1 * c2 - a2 * c1) / det;
                
                if (x < x_min or x > x_max or y < y_min or y > y_max) {
                    continue;
                }
                points[k] = {x, y};
                k++;
            }
        }
        return {points[0], points[1]};
    }
}