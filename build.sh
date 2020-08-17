#!/bin/bash

target="build"

if [ ! -d $target ]; then
    mkdir -p $target
fi

cd $target

echo "*******************************************************************************"
echo "start to build project ..."


cmake -DENABLE_TEST=1 ..
cmake --build .

if [ $? -ne 0 ]; then
    echo "FAILED!"
    cd ..
    exit 1
fi

echo "*******************************************************************************"
echo "start to run tests..."

./test/test_mcl

if [ $? -ne 0 ]; then
    echo "FAILED!"
    cd ..
    exit 1
fi

cd ..

echo "*******************************************************************************"
echo "SUCCESS!"
exit 0
