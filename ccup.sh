#!/usr/bin/env bash

# project config
project_path=$(cd "$(dirname "$0")"; pwd)
project_name="${project_path##*/}"
build="build"
cpm_cache="./deps"
doc_gitbook="./docs/gitbook"
docker_image="magicbowen/ubuntu-cc-dev:v2"

if [ "$(uname)" == "Darwin" ]; then
    #running on Mac OS
    docker_cmd=docker
    docker_src_path="$project_path"
    docker_work_dir="/$project_name"
    cmake_generate_type=""
    cmake_extra_definations=    
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ] || [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    #running on Windows
    docker_cmd="winpty docker"
    docker_src_path="/$project_path"
    docker_work_dir="//$project_name"
    cmake_generate_type='-GMinGW Makefiles'
    cmake_extra_definations="-DCMAKE_SH=CMAKE_SH-NOTFOUND"    
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    #running on Linux
    docker_cmd=docker
    docker_src_path="$project_path"
    docker_work_dir="/$project_name"
    cmake_generate_type=""
    cmake_extra_definations=
fi

# output utils
command_exists() {
	command -v "$@" >/dev/null 2>&1
}

info() {
	echo ${BOLD}"$@"${RESET} >&2
}

error() {
	echo ${RED}"ERROE: $@"${RESET} >&2
}

success() {
	echo ${GREEN}"SUCCESS: $@"${RESET} >&2
}

underline() {
	echo "$(printf '\033[4m')$@$(printf '\033[24m')"
}

splider() {
    info "==============================================="
}

start_exec() {
    info "START: $@ ${project_name}"
    splider
}

success_exec() {
    splider
    success "$@ ${project_name}!"
}

failed_exec() {
    splider
    error "$@ ${project_name}!"
}

setup_color() {
	# Only use colors if connected to a terminal
	if [ -t 1 ]; then
		RED=$(printf '\033[31m')
		GREEN=$(printf '\033[32m')
		YELLOW=$(printf '\033[33m')
		BLUE=$(printf '\033[34m')
		BOLD=$(printf '\033[1m')
		RESET=$(printf '\033[m')
	else
		RED=""
		GREEN=""
		YELLOW=""
		BLUE=""
		BOLD=""
		RESET=""
	fi
}

# ccup commands
function help() {
    cat<<-EOF
Usage: ./ccup.sh [OPTIONS]

Options:
    -e, Launch project in docker environment
    -u, Update dependent and generate makefile
    -b, Build project
    -t, Execute testing
    -i, Install project
    -r, Run executable
    -d, Document generation
    -c, Clean the build
    -C, Clean the build and dependent codes
    -h, The usage of ccup
EOF
}

function env() {
    start_exec "setup docker environment of"
	command_exists docker || {
		error "docker is not installed"
		exit 1
	}  

    $docker_cmd run -it -v $docker_src_path:/$project_name -w $docker_work_dir $docker_image /bin/bash
    if [ $? -ne 0 ]; then
        failed_exec "setup docker environment"
        exit 1
    fi    
}

function update() {
    start_exec "update dependent and generate makefile of"
	command_exists cmake || {
		error "cmake is not installed"
		exit 1
	} 
    export GIT_SSL_NO_VERIFY=1
    cmake -H. "$cmake_generate_type" -B$build -DPROJECT_NAME=${project_name} -DENABLE_TEST=on -DCPM_SOURCE_CACHE="$cpm_cache" "$cmake_extra_definations"
    if [ $? -ne 0 ]; then
        failed_exec "update"
        exit 1
    fi
    success_exec "update"
}

function build() {
    start_exec "build"
	command_exists cmake || {
		error "cmake is not installed"
		exit 1
	} 
    
    cmake --build $build

    if [ $? -ne 0 ]; then
        failed_exec "build"
        exit 1
    fi
    success_exec "build"
}

function test() {
    start_exec "test"
    find ./$build -perm -111 -type f -name "*_test"  -exec {} \;

    if [ $? -ne 0 ]; then
        failed_exec "test"
        exit 1
    fi 
    success_exec "test"
}

function install() {
    start_exec "install"
    
    cmake --install $build

    if [ $? -ne 0 ]; then
        failed_exec "install"
        exit 1
    fi
    success_exec "install"
}

function run() {
    start_exec "run"
    ./$build/entry/${project_name}
}

function doc() {
    start_exec "generate documents of gitbook"
	command_exists gitbook || {
		error "gitbook is not installed, reference : ./docs/README-gitbook.md"
		exit 1
	}      
    gitbook init $doc_gitbook
    gitbook build $doc_gitbook

    if [ $? -ne 0 ]; then
        failed_exec "document generation"
        exit 1
    fi
    success_exec "document generation"
}

function clean() {
    start_exec "clean"
    if [ -d "build" ]; then 
        cd $build
        ls | grep -v _deps | xargs rm -rf
        info "deleting the build targets of ${project_name}"
        cd -
    fi
    success_exec "clean"
}

function clean_all() {
    start_exec "clean all"
    info "deleting the build targets of ${project_name}"
    rm -rf build
    info "deleting the dependent codes of ${project_name}"
    rm -rf ${cpm_cache}
    success_exec "clean all"
}

function parse_args() {
    while getopts ':eubtirdcCh' OPTS; do
        case $OPTS in
            e) env ;;
            u) update; ;;
            b) build; ;;
            t) test; ;;
            i) install; ;;
            r) run; ;;
            d) doc; ;;
            c) clean; ;;
            C) clean_all; ;;
            h) help; ;;
            *) help;;
        esac
    done
}

function main() {
    setup_color
    parse_args "$@"
}

main "$@"
