# PolyIntersection

Multi-material cell clipping on the GPU

## Build instructions

First get a node on Darwin then load modules

```sh
salloc -p shared-gpu --qos=debug --time=03:00:00
module load cmake cuda/12.0.0
```
Then install Kokkos:

```sh
git clone git@github.com:kokkos/kokkos.git
cd kokkos
cmake -B build \
  -DKokkos_ENABLE_TESTS=OFF \
  -DKokkos_ENABLE_CUDA=ON \
  -DCMAKE_CXX_EXTENSIONS=OFF
cmake --build build -j $(nproc)
cmake --install build --prefix "/path/to/kokkos/install"
```
Finally build the code:

```sh
git clone git@gitlab.lanl.gov:summerprojects2024/polyintersection.git
cd polyintersection
cmake -B build \
  -DUSE_KOKKOS_GPU=ON \
  -DKokkos_ROOT="/path/to/kokkos/install"
cmake --build build -j $(nproc)  
```

