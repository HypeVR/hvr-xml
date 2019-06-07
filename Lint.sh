#! /bin/bash

ALL_PATH=($(find modules apps -name *.cpp -or -name *.h -or -name *.cu -or -name *.cuh))
 
echo "Running CppLint"
cpplint --counting=detailed  "${ALL_PATH[@]}"

echo "Running CMakeLint"
cmakelint --filter=-readability/mixedcase,-whitespace/extra,-whitespace/indent $(find modules/ apps/ -name \CMakeLists.txt -or -name \*.cmake) CMakeLists.txt
