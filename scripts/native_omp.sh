if [-f "CMakeCache.txt"]
  rm CMakeCache.txt
fi

cmake \
  -DKokkos_ROOT="/vast/home/jmathurin/kokkos-gpu/install" \
  -DUSE_NATIVE_OMP=ON \
  ..

make