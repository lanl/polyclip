#!/bin/bash

base_directory=$(basename "$PWD")

if [[ "$base_directory" != "build" && "$base_directory" != "cmake-build-debug" ]]; then
    echo "This script can only be called from the build folder!"
    exit 1
fi

if [ -z "$1" ]; then
    echo "Usage: $0 <KOKKOS_INSTALL_PATH>"
    exit 1
fi


echo "$1" > "../scripts/kokkos_config.txt"
echo "Kokkos_ROOT path saved to kokkos_config.txt"