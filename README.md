# polyclip

Multi-material cell clipping on the GPU.

### Build instructions

First get a node on LANL's Darwin cluster then load modules:

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

### License and acknowledgements
`polyclip` is open source under the [BSD-3 licence](./LICENSE).<br>
The main contributors are:
- Melanie Walsmith
- Jonathan Mathurin
- Hoby Rakotoarivelo

We gratefully acknowledge the NNSA-MSIPP program whose funding made this project possible.

### Copyright

© 2025. Triad National Security, LLC. All rights reserved.
This program was produced under U.S. Government contract 89233218CNA000001 for Los Alamos National Laboratory (LANL), which is operated by Triad National Security, LLC for the U.S. Department of Energy/National Nuclear Security Administration. All rights in the program are reserved by Triad National Security, LLC, and the U.S. Department of Energy/National Nuclear Security Administration. The Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this material to reproduce, prepare. derivative works, distribute copies to the public, perform publicly and display publicly, and to permit others to do so.