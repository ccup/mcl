#!/usr/bin/env bash

export CCUP_CMD=$(cd "$(dirname "$0")"; pwd)

project="mcl"
build="build"
docker_image="magicbowen/ubuntu-cc-dev:v1"
deps_cache="./deps"

function help() {
    cat<<-EOF
Usage: ./ccup.sh [OPTIONS]

Options:
    -e, Prepare environment
    -u, Update deps and cmake generation
    -b, Build project
    -r, Run executable
    -t, Execute testing
    -c, Clean the build
    -C, Clean the build and deps
    -h, The usage of ccup
EOF
}

function env() {
    docker pull $docker_image
    work_path=$(cd `dirname $0`;pwd)
    docker run -it -v $work_path:/$project --user $(id -u):$(id -g) -w /$project $docker_image /bin/bash
}

function update() {
    echo "Start to update ${project}"
    echo "============================"
    export GIT_SSL_NO_VERIFY=1
    cmake -H. -B$build -DENABLE_TEST=on -DCPM_SOURCE_CACHE=$deps_cache
    echo "============================"
    echo "SUCCESS!"
}

function build() {
    echo "Start to build ${project}"
    echo "============================"
    cmake --build $build

    if [ $? -ne 0 ]; then
        echo "FAILED!"
        exit 1
    fi
    echo "============================"
    echo "SUCCESS!"
}

function run() {
    ./$build/src/${project}_service
}

function test() {
    echo "Start to test ${project}"
    echo "============================"
    ./$build/test/${project}_test

    if [ $? -ne 0 ]; then
        echo "FAILED!"
        exit 1
    fi 
    echo "============================"
    echo "SUCCESS!" 
}

function clean() {
    echo "Start to clean ${project}"
    echo "============================"
    rm -rf build/*
    echo "SUCCESS!" 
}

function clean_all() {
    echo "Start to clean all ${project}"
    echo "============================"
    rm -rf build/*
    rm -rf deps/*
    echo "SUCCESS!" 
}

function parse_args() {
    while getopts ':cCbrteuh' OPTS; do
        case $OPTS in
            e) env ;;
            u) update; ;;
            b) build; ;;
            r) run; ;;
            t) test; ;;
            c) clean; ;;
            C) clean_all; ;;
            h) help; ;;
            *) help;;
        esac
    done
}

function main() {
    parse_args "$@"
}

main "$@"


