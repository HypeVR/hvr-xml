#! /bin/bash
# Created by Caoyang Jiang
# Date: 05/10/2019

# Prequisite
# * Current directory is the project build directory
REPO_NAME=hvr-xml

# Set directory variables
build_dir=$(pwd)
lib_install_dir=$build_dir/install
installtest_dir=$build_dir/installtest
installtest_build_dir=$installtest_dir/build/

# Create source directory
mkdir -p $installtest_dir
cd    $installtest_dir

# Create dummy main file
touch main.cpp

# Create CMakeLists
> CMakeLists.txt
echo 'cmake_minimum_required(VERSION 3.10)' >> CMakeLists.txt
echo 'project(Example)' >> CMakeLists.txt
echo 'set(PROJECT_SRCS ${PROJECT_SOURCE_DIR}/main.cpp)' >> CMakeLists.txt
echo "find_package(${REPO_NAME} REQUIRED)" >> CMakeLists.txt
echo 'add_executable(${PROJECT_NAME} ${PROJECT_SRCS} ${PROJECT_INCS})' >> CMakeLists.txt
echo "target_link_libraries(\${PROJECT_NAME} hvr::${REPO_NAME})" >> CMakeLists.txt

# Create build
mkdir -p $installtest_build_dir
cd    $installtest_build_dir
cmake -DCMAKE_PREFIX_PATH="${lib_install_dir}" ..