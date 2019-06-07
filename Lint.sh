#! /bin/bash


EXCLUDED_PATHS=()
EXCLUDED_PATHS+=("^modules/core/include/hvr/core/cuda/")
EXCLUDED_PATHS+=("^modules/core/include/hvr/core/utils/network/asio")
EXCLUDED_PATHS+=("^modules/ts/")
EXCLUDED_PATHS+=("^modules/core/include/hvr/core/utils/loguru")
EXCLUDED_PATHS+=("^modules/core/src/utils/loguru")

ALL_PATH=($(find modules apps -name *.cpp -or -name *.h -or -name *.cu -or -name *.cuh))
OUT_PATH=()

for exclude in ${EXCLUDED_PATHS[@]}
do
    for path in "${ALL_PATH[@]}"; do
        value=$(echo $path | grep -vE ${exclude})
        if [ value != "" ]
        then
           OUT_PATH+=($value)
        fi
     done

     ALL_PATH=("${OUT_PATH[@]}")
     OUT_PATH=()
done

echo "Running CppLint"
cpplint --counting=detailed  "${ALL_PATH[@]}"

echo "Running CMakeLint"
cmakelint --filter=-readability/mixedcase,-whitespace/extra,-whitespace/indent $(find modules/ apps/ cmake/ -name \CMakeLists.txt -or -name \*.cmake) CMakeLists.txt
