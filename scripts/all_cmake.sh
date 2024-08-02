# Plan:
#   1) need to call for all the clipping algorithms
#   2) after every call delete cache
#   3) once all have all the necessary data 
#   4) run python code

# Darwin omp
cmake \
  -DCMAKE_CXX_FLAGS="-Wno-deprecated-declarations" \
  -DUSE_NATIVE_OMP=ON \
  ..

# This will give issues
./scaling.sh

# need to remove cache
rm CMakeCache.txt
rm -rf CMakeFiles
rm cmake_install.cmake


# Darwin kokkos omp
cmake \
  -DCMAKE_CXX_FLAGS="-Wno-deprecated-declarations" \
  -DUSE_KOKKOS_OMP=ON \
  -DKokkos_ROOT=""/vast/home/mcwalsmith/dev/kokkos/install"" \
  ..

./scaling.sh

# need to remove cache
rm CMakeCache.txt
rm -rf CMakeFiles
rm cmake_install.cmak

# Darwin kokkos GPU
cmake \
  -DCMAKE_CXX_FLAGS="-Wno-deprecated-declarations" \
  -DUSE_KOKKOS_GPU=ON \
  -DKokkos_ROOT="/vast/home/mcwalsmith/dev/kokkos-gpu/install" \
  ..

./scaling.sh
./table.py



