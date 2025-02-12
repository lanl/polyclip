if [ -f "CMakeCache.txt" ]; then
  rm CMakeCache.txt
fi

cmake \
  -DKokkos_ROOT="/vast/home/jmathurin/kokkos-gpu/install" \
  -DUSE_KOKKOS_GPU=ON \
  ..

make