#!/bin/bash

target="build"

echo "*******************************************************************************"
echo "start to build project ..."

cmake -H. -B$target -DMCL_ENABLE_TEST=on
cmake --build $target

if [ $? -ne 0 ]; then
    echo "FAILED!"
    cd ..
    exit 1
fi

echo "*******************************************************************************"
echo "start to run tests..."

./$target/test/mcl_test

if [ $? -ne 0 ]; then
    echo "FAILED!"
    cd ..
    exit 1
fi

echo "*******************************************************************************"
echo "SUCCESS!"
exit 0
