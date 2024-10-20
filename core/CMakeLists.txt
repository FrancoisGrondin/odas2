project(odas3)
cmake_minimum_required(VERSION 2.4.6)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

find_package(PkgConfig REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-march=native -Wall -ffast-math")

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
    src/systems/steering.c
    src/systems/stft.c

    src/types/cplx.c
    src/types/xyz.c
    
    src/utils/fft.c
    src/utils/linalg.c
    src/utils/mics.c
    src/utils/points.c

)

add_library(odas2 SHARED
    ${SRC}
)

target_link_libraries(odas2
    m
)

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

add_executable(demo_rec demo/demo_rec.c)
target_link_libraries(demo_rec odas2)

add_executable(example_ssl examples/example_ssl.c)
target_link_libraries(example_ssl odas2)

add_executable(buffer tools/buffer.c)
target_link_libraries(buffer pthread)

install(TARGETS odas2 DESTINATION lib)