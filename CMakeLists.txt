cmake_minimum_required (VERSION 2.6)
project (OutSpacer)
set (OutSpacer_VERSION_MAJOR 1)
set (OutSpacer_VERSION_MINOR_0)

configure_file (
    "${PROJECT_SOURCE_DIR}/OutSpacerConfig.h.in"
    "${PROJECT_BINARY_DIR}/OutSpacerConfig.h"
    )

include_directories("${PROJECT_BINARY_DIR}")

add_executable (OutSpacer OutSpacer.cpp)
