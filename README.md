# polyclip

2D multi-material cell clipping on GPU.

-----
### Build instructions

#### Darwin cluster at LANL

First get a node then load modules:

```sh
salloc -p volta-x86 --qos=debug --time=02:00:00
module load cmake/3.29.2 cuda/12.3.1 gcc/12.2.0
```

Then build the code with CUDA backend:

```sh
git clone --recursive git@github.com:lanl/polyclip.git
cd polyclip
cmake -B build \
  -DKokkos_ENABLE_TESTS=OFF \
  -DKokkos_ENABLE_SERIAL=ON \
  -DKokkos_ENABLE_OPENMP=ON \
  -DKokkos_ENABLE_CUDA=ON \
  -DKokkos_ENABLE_CUDA_UVM=OFF   # turn on to use UVM 
cmake --build build --parallel
ctest --test-dir build --output-on-failure 
```

-----
### License and copyright
It is open source under the [BSD-3](./LICENSE) license (O4915).<br>
Main contributors:
- Melanie Walsmith
- Jonathan Mathurin
- Hoby Rakotoarivelo

© 2025. Triad National Security, LLC. All rights reserved.
This program was produced under U.S. Government contract 89233218CNA000001 for Los Alamos National Laboratory (LANL), which is operated by Triad National Security, LLC for the U.S. Department of Energy/National Nuclear Security Administration. All rights in the program are reserved by Triad National Security, LLC, and the U.S. Department of Energy/National Nuclear Security Administration. The Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this material to reproduce, prepare. derivative works, distribute copies to the public, perform publicly and display publicly, and to permit others to do so.
