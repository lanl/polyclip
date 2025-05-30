export KOKKOS_TOOLS_LIBS=${HOME}/kokkos-tools/install/lib64/libkp_nvtx_connector.so
if [ "$#" -lt 2 ]
  then
    echo "USAGE: $0 [MESH_FILE] [LINE FILE]"
    exit 1
fi

if ! command -v nsys &> /dev/null
  then
    echo "Error: Nsys is not currently installed on the node!"
    exit 1
fi

mesh_file=$1
file_name="$(basename "$mesh_file")"
rootname="${file_name%.*}"
for i in {1..5}
  do
    output_name="${rootname}_$i"
    nsys profile -t cuda,nvtx --output="output/$output_name" ./test_mesh "$1" "$2"
    nsys export --type sqlite --output="output/${output_name}.sqlite" "${output_name}.nsys-rep"
done

for i in {1..5}
  do
    output_name="${rootname}_$i"
    python3 ../../scripts/parse_profiling_data.py "output/$rootname"
done
