# Polyclip

Multi-material cell clipping on the GPU

## Build instructions

First get a node on Darwin, load modules and define `KOKKOS_PATH`

```sh
salloc -p shared-gpu --qos=debug --time=03:00:00
module load cmake cuda/12.0.0
```

Then build the code with CUDA backend:

```sh
git clone --recursive git@gitlab.lanl.gov:summerprojects2024/polyclip.git
cd polyclip
cmake -B build \
  -DKokkos_ENABLE_TESTS=OFF \
  -DKokkos_ENABLE_CUDA=ON \
  -DKokkos_ENABLE_OPENMP=ON
cmake --build build -j $(nproc) 
ctest --test-dir build --output-on-failure 
```

