#!/bin/sh

export BUILD_DIR=$1
export GMOCK_ROOT=$2
export GMOCK_BUILD_ROOT=$3
export CUKE_ROOT=".."

cmake -E make_directory $BUILD_DIR

cmake -E chdir $BUILD_DIR cmake $CUKE_ROOT \
-DCMAKE_BUILD_TYPE=Debug \
-DGTEST_LIBRARY=$GMOCK_BUILD_ROOT/gtest/libgtest.a \
-DGTEST_INCLUDE_DIR=$GMOCK_ROOT/gtest/include \
-DGTEST_MAIN_LIBRARY=$GMOCK_BUILD_ROOT/gtest/libgtest_main.a \
-DGMOCK_LIBRARY=$GMOCK_BUILD_ROOT/libgmock.a \
-DGMOCK_INCLUDE_DIR=$GMOCK_ROOT/include \
-DGMOCK_MAIN_LIBRARY=$GMOCK_BUILD_ROOT/libgmock_main.a \
-DCUKE_DISABLE_CPPSPEC=True \
-DCUKE_ENABLE_EXAMPLES=on

cmake --build $BUILD_DIR
cmake --build $BUILD_DIR --target test
cmake --build $BUILD_DIR --target features
