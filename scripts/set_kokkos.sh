#!/bin/bash
if [ -z "$1" ]; then
    echo "Usage: $0 <KOKKOS_INSTALL_PATH>"
    exit 1
fi


echo "$1" > "kokkos_config.txt"
echo "Kokkos_ROOT path saved to kokkos_config.txt"