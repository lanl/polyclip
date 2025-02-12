if [ -f "CMakeCache.txt" ]; then
  rm CMakeCache.txt
fi

cmake \
  -DUSE_NATIVE_OMP=ON \
  ..

make