# Polyclip

Multi-material cell clipping on the GPU

## Build instructions

First get a node on Darwin, load modules and define `KOKKOS_PATH`

```sh
salloc -p shared-gpu --qos=debug --time=03:00:00
module load cmake cuda/12.0.0
KOKKOS_PATH="/path/to/kokkos/install"
```

Then install Kokkos with CUDA backend:

```sh
git clone https://github.com/kokkos/kokkos.git
cd kokkos
cmake -B build -DKokkos_ENABLE_TESTS=OFF -DKokkos_ENABLE_CUDA=ON
cmake --build build -j $(nproc)
cmake --install build --prefix "${KOKKOS_PATH}"
```

Finally build and test the code:

```sh
git clone git@gitlab.lanl.gov:summerprojects2024/polyclip.git
cd polyclip
cmake -B build -DKokkos_ROOT="${KOKKOS_PATH}"
cmake --build build -j $(nproc) 
ctest --test-dir build --output-on-failure 
```

