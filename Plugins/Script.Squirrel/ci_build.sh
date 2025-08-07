#!/usr/bin/env bash

# build.sh <action> ...
# ci_platform:       windows
# ci_action:         dependencies|generate|build|install|test
# ci_source_dir:     source code directory
# ci_build_dir:      cmake cache directory
# ci_native_sdk_dir: native SDK (linux) installation directory
# ci_target_sdk_dir: target SDK (web) installation directory

ci_action=$1; shift;
ci_source_dir=${ci_source_dir%/};   # remove trailing slash if any

echo "ci_platform=$ci_platform"
echo "ci_action=$ci_action"
echo "ci_source_dir=$ci_source_dir"
echo "ci_build_dir=$ci_build_dir"
echo "ci_native_sdk_dir=$ci_native_sdk_dir"
echo "ci_target_sdk_dir=$ci_target_sdk_dir"

declare -A build_config=(
    [web]='Release'
    [windows]='RelWithDebInfo'
)

function action-dependencies() {
    :
}

function action-generate() {
    local params=(
        "-B"
        "$ci_build_dir"
        "-S"
        "$ci_source_dir"
        "-DREBELFORK_SDK=$ci_target_sdk_dir"
    )

    if [[ "$ci_platform" == "windows" ]]; then
        params+=(
            "-G"
            "Visual Studio 17 2022"
            "-A"
            "x64"
            "-DCMAKE_BUILD_TYPE=${build_config[$ci_platform]}"
        )
    fi

    cmake "${params[@]}"
}

function action-build() {
    cmake --build $ci_build_dir --parallel $(nproc) --config ${build_config[$ci_platform]}
}

function action-prepare() {
    if [[ "$ci_platform" == "windows" ]]; then
        cd $ci_build_dir/${build_config[$ci_platform]}
        mkdir $ci_build_dir/upload/
        cp *.dll *.lib *.exp *.pdb $ci_build_dir/upload/
    fi
}

action-$ci_action
