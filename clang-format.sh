#!/usr/bin/env bash

RELATIVE_FCAR_SOURCE_PATH=`dirname ${BASH_SOURCE[@]}`
FCAR_SOURCE_PATH=`realpath ${RELATIVE_FCAR_SOURCE_PATH}`

clang-format -i -style=file \
    `find ${FCAR_SOURCE_PATH}/bus_stop -type f -name '*.ino'` \
    `find ${FCAR_SOURCE_PATH}/bus_stop -type f -name '*.cpp'` \
    `find ${FCAR_SOURCE_PATH}/bus_stop -type f -name '*.h'`\
    `find ${FCAR_SOURCE_PATH}/depo -type f -name '*.ino'` \
    `find ${FCAR_SOURCE_PATH}/depo -type f -name '*.cpp'` \
    `find ${FCAR_SOURCE_PATH}/depo -type f -name '*.h'`
