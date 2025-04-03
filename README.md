# PolyIntersection

Multi-material cell clipping on the GPU

## Build instructions

First get a node on Darwin then load modules

```sh
salloc -p shared-gpu --qos=debug --time=03:00:00
module load cmake cuda/12.0.0
```

### Install Kokkos

Then install Kokkos with CUDA backend:

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
> Set `Kokkos_ENABLE_CUDA=OFF` for local builds on your laptop.<br>
> If so, the code will still compile using the `Serial` backend.

### Build the code
Finally build and test the code:

```sh
git clone git@gitlab.lanl.gov:summerprojects2024/polyintersection.git
cd polyintersection
cmake -B build -DKokkos_ROOT="/path/to/kokkos/install"
cmake --build build -j $(nproc) 
ctest --test-dir build --output-on-failure 
```

