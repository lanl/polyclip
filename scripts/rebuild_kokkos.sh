compute_mode=$(nvidia-smi --query-gpu=compute_cap --format=csv,noheader | head -n 1)
compute_options=(12.0 10.0 9.0 8.9 8.6 8.0 7.5 7.2 7.0 6.1 6.0 5.3 5.2 5.0 3.7 3.5 3.2 3.0)
architecture_options=("Kokkos_ARCH_BLACKWELL120" "Kokkos_ARCH_BLACKWELL100" "Kokkos_ARCH_HOPPER90"
"Kokkos_ARCH_ADA89"
"Kokkos_ARCH_AMPERE86"
"Kokkos_ARCH_AMPERE80"
"Kokkos_ARCH_TURING75"
"Kokkos_ARCH_VOLTA72"
"Kokkos_ARCH_VOLTA70"
"Kokkos_ARCH_PASCAL61"
"Kokkos_ARCH_PASCAL60"
"Kokkos_ARCH_MAXWELL53"
"Kokkos_ARCH_MAXWELL52"
"Kokkos_ARCH_MAXWELL50"
"Kokkos_ARCH_KEPLER37"
"Kokkos_ARCH_KEPLER35"
"Kokkos_ARCH_KEPLER32"
"Kokkos_ARCH_KEPLER30")

located_index=-1

for (( i=0; i < 18; i++ ))
  do
    result=$(echo "${compute_options[i]} > $compute_mode" | bc -l)

    if [[ $result -eq 1 ]]
    then
      located_index=$i
    fi
  done



KOKKOS_ARCHITECTURE=${architecture_options[$located_index]}


INSTALL_DIR=${HOME}/kokkos/install

cmake \
  -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
  -DKokkos_ENABLE_TESTS=On \
  -DKokkos_ENABLE_CUDA=On \
  -DCMAKE_CXX_EXTENSIONS=On \
  -D$KOKKOS_ARCHITECTURE=On \
..

make -j 10
make install
