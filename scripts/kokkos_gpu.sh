if [ -f "CMakeCache.txt" ]; then
  rm CMakeCache.txt
fi


if [ ! -f "kokkos_config.txt" ]; then
  echo "kokkos_config.txt file not found! Run set_kokkos_root.sh [KOKKOS_INSTALL_PATH] first."
  exit 1
fi

KOKKOS_ROOT=$(cat "kokkos_config.txt")
cmake \
  -DKokkos_ROOT="$KOKKOS_ROOT" \
  -DUSE_KOKKOS_GPU=ON \
  ..

make