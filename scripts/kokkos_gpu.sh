
#if [ -z "$1" ]; then
#  if [ -f "CMakeCache.txt" ]; then
#    rm CMakeCache.txt
#  fi
#fi
#base_directory=$(basename "$PWD")
#
#if [[ "$base_directory" != "build" && "$base_directory" != "cmake-build-debug" ]]; then
#    echo "This script can only be called from the build folder!"
#    exit 1
#fi
#
#if [ ! -f "../scripts/kokkos_config.txt" ]; then
#  echo "kokkos_config.txt file not found! Run set_kokkos_root.sh [KOKKOS_INSTALL_PATH] first."
#  exit 1
#fi

#KOKKOS_DIR=${HOME}/kokkos/install/lib64/cmake/Kokkos
rm CMakeCache.txt
KOKKOS_ROOT=${HOME}/dev/kokkos/install #$(cat "../scripts/kokkos_config.txt")
cmake \
  -DKokkos_ROOT="$KOKKOS_ROOT" \
  -DUSE_KOKKOS_GPU=ON \
  -DKokkos_ENABLE_TESTS=On \
  -DKokkos_ENABLE_CUDA=On \
  ..

make