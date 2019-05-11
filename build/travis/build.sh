#!/bin/bash

if [[ $DOCKER_IMAGE ]]; then 
    WORKSPACE=/workspace
    SOURCE_DIR=${WORKSPACE}/source
    OUTPUT_DIR=${WORKSPACE}/output
    BUILD_DIR=${WORKSPACE}/build
    DEPLOY_DIR=${WORKSPACE}/source/_deploy

else
    WORKSPACE=$TRAVIS_BUILD_DIR
    SOURCE_DIR=${WORKSPACE}
    OUTPUT_DIR=${WORKSPACE}/output
    BUILD_DIR=${WORKSPACE}/build
    DEPLOY_DIR=${WORKSPACE}/deploy
fi

BUILD_DATE=`date +"%Y%m%d"`
WORKRAVE_LONG_GIT_VERSION=`( cd ${SOURCES_DIR} ; git describe --tags --abbrev=10 --dirty 2>/dev/null )`
WORKRAVE_GIT_VERSION=`( cd ${SOURCES_DIR} ; git describe --tags --abbrev=10 --dirty 2>/dev/null | sed -e 's/-g.*//' )`
WORKRAVE_VERSION=`cat ${SOURCES_DIR}/configure.ac | grep AM_INIT_AUTOMAKE | cut -d ','  -f2 | cut -d' ' -f2 | cut -d')' -f1`

CMAKE_FLAGS=()
CMAKE_FLAGS64=()
MAKE_FLAGS=()

build()
{
  config=$1
  cmake_args=("${!2}")

  mkdir -p ${BUILD_DIR}/${config}
  mkdir -p ${OUTPUT_DIR}/${config}

  cd ${BUILD_DIR}/${config}

  cmake ${SOURCE_DIR} -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=${OUTPUT_DIR}/${config} ${cmake_args[@]}

  make ${MAKE_FLAGS[@]} VERBOSE=1
  make ${MAKE_FLAGS[@]} install VERBOSE=1
}

if [[ ${CONF_ENABLE} ]]; then
    for i in ${CONF_ENABLE//,/ }
    do
        CMAKE_FLAGS+=("-DWITH_$i=ON")
        CMAKE_FLAGS64+=("-DWITH_$i=ON")
    done
fi

if [[ ${CONF_DISABLE} ]]; then
    for i in ${CONF_DISABLE//,/ }
    do
        CMAKE_FLAGS+=("-DWITH_$i=OFF")
        CMAKE_FLAGS64+=("-DWITH_$i=OFF")
    done
fi

CMAKE_FLAGS+=("-DWITH_UI=${WITH_UI}")

if [[ $COMPILER = 'gcc' ]] ; then
    CMAKE_FLAGS+=("-DCMAKE_CXX_COMPILER=g++")
    CMAKE_FLAGS64+=("-DCMAKE_CXX_COMPILER=g++")
    CMAKE_FLAGS+=("-DCMAKE_C_COMPILER=gcc")
    CMAKE_FLAGS64+=("-DCMAKE_C_COMPILER=gcc")
elif [[ $COMPILER = 'clang' ]] ; then
    CMAKE_FLAGS+=("-DCMAKE_CXX_COMPILER=clang++-6.0")
    CMAKE_FLAGS64+=("-DCMAKE_CXX_COMPILER=clang++-6.0")
    CMAKE_FLAGS+=("-DCMAKE_C_COMPILER=clang-6.0")
    CMAKE_FLAGS64+=("-DCMAKE_C_COMPILER=clang-6.0")
fi

if [[ $TRAVIS_OS_NAME = 'osx' ]]; then
    CMAKE_FLAGS+=("-DCMAKE_PREFIX_PATH=$(brew --prefix qt5)")
fi

case "$DOCKER_IMAGE" in
    mingw-qt5)
        CMAKE_FLAGS+=("-DCMAKE_TOOLCHAIN_FILE=${SOURCE_DIR}/build/cmake/mingw64.cmake")
        CMAKE_FLAGS+=("-DPREBUILT_PATH=${WORKSPACE}/prebuilt")
        ;;

    mingw-gtk)

        case "$CONFIG" in
            vs)
                CMAKE_FLAGS+=("-DCMAKE_TOOLCHAIN_FILE=${SOURCE_DIR}/build/cmake/mingw32.cmake")
                CMAKE_FLAGS+=("-DPREBUILT_PATH=${WORKSPACE}/prebuilt")
                ;;

            *)
                CMAKE_FLAGS+=("-DCMAKE_TOOLCHAIN_FILE=${SOURCE_DIR}/build/cmake/mingw32.cmake")
                CMAKE_FLAGS+=("-DPREBUILT_PATH=${OUTPUT_DIR}/.64")

                CMAKE_FLAGS64+=("-DCMAKE_TOOLCHAIN_FILE=${SOURCE_DIR}/build/cmake/mingw64.cmake")
                CMAKE_FLAGS64+=("-DWITH_UI=None")
                CMAKE_FLAGS64+=("-DCMAKE_BUILD_TYPE=Release")

                build ".64" CMAKE_FLAGS64[@]
                ;;
        esac
        ;;
esac

build "" CMAKE_FLAGS[@]

mkdir ${DEPLOY_DIR}

EXTRA=
#if [ $CONFIGURATION == "Debug" ]; then
#    EXTRA="-Debug"
#fi

if [[ -e ${OUTPUT_DIR}/mysetup.exe ]]; then
  if [[ -z "$TRAVIS_TAG" ]]; then
      echo "No tag build."
      cp ${OUTPUT_DIR}/mysetup.exe ${DEPLOY_DIR}/workrave-win32-${WORKRAVE_LONG_GIT_VERSION}-${BUILD_DATE}${EXTRA}.exe
  else
      echo "Tag build : $TRAVIS_TAG"
      cp ${OUTPUT_DIR}/mysetup.exe ${DEPLOY_DIR}/workrave-win32-${TRAVIS_TAG}${EXTRA}.exe
  fi
fi
