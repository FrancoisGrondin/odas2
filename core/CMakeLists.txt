cmake_minimum_required(VERSION 3.15...3.27)

project(odas2)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories("${PROJECT_SOURCE_DIR}/include")

if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

if(WIN32)
    set(DEFAULT_BUILD_SHARED_LIBS OFF)
else()
    set(DEFAULT_BUILD_SHARED_LIBS ON)
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" ${DEFAULT_BUILD_SHARED_LIBS})

if(NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -Wall")
endif(NOT MSVC)

if(NOT MSVC AND NOT APPLE AND NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffast-math")
endif()

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

#Add base directory for includes (global)
include_directories(include/odas2)

add_subdirectory(include)

set(SRC

    src/dnns/functionals.c
    src/dnns/layers.c
    src/dnns/tensors.c

    src/ios/msg.c
    src/ios/wav.c
    src/ios/raw.c

    src/signals/covs.c
    src/signals/doas.c
    src/signals/dsf.c
    src/signals/freqs.c
    src/signals/hops.c
    src/signals/masks.c
    src/signals/tdoas.c
    src/signals/weights.c

    src/systems/beamformer.c
    src/systems/delaysum.c
    src/systems/enhancement.c
    src/systems/fcc.c
    src/systems/gcc.c
    src/systems/mixer.c
    src/systems/mvdr.c
    src/systems/phat.c
    src/systems/postfilter.c
    src/systems/scm.c
    src/systems/ssl.c
    src/systems/sst.c
    src/systems/steering.c
    src/systems/stft.c

    src/utils/fft.c
    src/utils/linalg.c
    src/utils/mics.c
    src/utils/points.c
)

add_library(odas2
    ${SRC}
)

if(NOT MSVC)
    target_link_libraries(odas2 m)
endif()

add_executable(tests
    test/tests.c
    test/units/test_beamformer.c
    test/units/test_cplx.c
    test/units/test_delaysum.c
    test/units/test_fft.c
    test/units/test_gcc.c
    test/units/test_linalg.c
    test/units/test_mics.c
    test/units/test_mixer.c
    test/units/test_mvdr.c
    test/units/test_phat.c
    test/units/test_points.c
    test/units/test_scm.c
    test/units/test_ssl.c
    test/units/test_sst.c
    test/units/test_steering.c
    test/units/test_stft.c
    test/units/test_xyz.c
)

target_link_libraries(tests odas2)

add_executable(demo_wave demo/demo_wave.c)
target_link_libraries(demo_wave odas2)

add_executable(demo_spex demo/demo_spex.c)
target_link_libraries(demo_spex odas2)

add_executable(demo_tdoas demo/demo_tdoas.c)
target_link_libraries(demo_tdoas odas2)

add_executable(demo_doas demo/demo_doas.c)
target_link_libraries(demo_doas odas2)

add_executable(demo_steer demo/demo_steer.c)
target_link_libraries(demo_steer odas2)

add_executable(demo_delaysum demo/demo_delaysum.c)
target_link_libraries(demo_delaysum odas2)

add_executable(demo_mvdr demo/demo_mvdr.c)
target_link_libraries(demo_mvdr odas2)

add_executable(demo_enhance demo/demo_enhance.c)
target_link_libraries(demo_enhance odas2)

add_executable(demo_mix demo/demo_mix.c)
target_link_libraries(demo_mix odas2)

add_executable(example_introlab_ssl examples/example_introlab_ssl.c)
target_link_libraries(example_introlab_ssl odas2)

add_executable(example_vibeus_ssl examples/example_vibeus_ssl.c)
target_link_libraries(example_vibeus_ssl odas2)

add_executable(example_soundskrit_sst examples/example_soundskrit_sst.c)
target_link_libraries(example_soundskrit_sst odas2)

add_executable(example_sc16_sst examples/example_sc16_sst.c)
target_link_libraries(example_sc16_sst odas2)

if (UNIX)
    add_executable(buffer tools/buffer.c)
    target_link_libraries(buffer pthread)
endif ()

install(TARGETS odas2 DESTINATION lib)
