#!/bin/bash
#
# template generated by genshtest

build_dir=~/bisectav1/build
cd $build_dir

testcase=AV1/ErrorResilienceTestLarge.ParseAbilityTest/1


rm -r -f *

cmake ../aom -DSANITIZE=integer -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_FLAGS=-O1 -DCMAKE_CXX_FLAGS=-O1 -DCMAKE_BUILD_TYPE=Debug '-DCMAKE_EXE_LINKER_FLAGS=-Wl,--start-group ' -DENABLE_CCACHE=1

make -j12 && make testdata

./test_libaom --gtest_filter=$testcase

