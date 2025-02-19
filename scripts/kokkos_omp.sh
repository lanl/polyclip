if [ -f "CMakeCache.txt" ]; then
  rm CMakeCache.txt
fi

base_directory=$(basename "$PWD")

if [[ "$base_directory" != "build" && "$base_directory" != "cmake-build-debug" ]]; then
    echo "This script can only be called from the build folder!"
    exit 1
fi

if [ ! -f "../scripts/kokkos_config.txt" ]; then
  echo "kokkos_config.txt file not found! Run set_kokkos_root.sh [KOKKOS_INSTALL_PATH] first."
  exit 1
fi

KOKKOS_ROOT=$(cat "../scripts/kokkos_config.txt")
cmake \
  -DKokkos_ROOT="$KOKKOS_ROOT" \
  -DUSE_KOKKOS_OMP=ON \
  ..

make


#/vast/home/jmathurin/kokkos-gpu/install"
