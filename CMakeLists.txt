cmake_minimum_required(VERSION 3.20)
project(estimora VERSION 0.1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
option(ESTIMORA_BUILD_TESTS "Build unit tests" ON)
option(ESTIMORA_STRICT_WARNINGS "Enable strict compiler warnings" ON)
file(GLOB ESTIMORA_SOURCES CONFIGURE_DEPENDS src/*.c)
add_library(estimora STATIC ${ESTIMORA_SOURCES})
target_include_directories(estimora PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/estimora/statistics)
target_compile_features(estimora PUBLIC c_std_99)
if(ESTIMORA_STRICT_WARNINGS)
 if(MSVC)
  target_compile_options(estimora PRIVATE /W4 /WX)
 else()
  target_compile_options(estimora PRIVATE -Wall -Wextra -Wpedantic -Werror)
 endif()
endif()
if(ESTIMORA_BUILD_TESTS)
 enable_testing()
 set(ESTIMORA_TESTS
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_c.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_dynamic_range.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_edge_grid.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_entropy.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_equalize.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_hist_cdf.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_high_percentile.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_percentile.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_redistribute.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_image_hist_bin_u32.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_valleys.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_weighted_sensor_fuse.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_audit.c)
 foreach(test_source IN LISTS ESTIMORA_TESTS)
  get_filename_component(test_name "${test_source}" NAME_WE)
  add_executable(estimora_${test_name} ${test_source})
  target_include_directories(estimora_${test_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/estimora/statistics)
  target_link_libraries(estimora_${test_name} PRIVATE estimora)
  if(ESTIMORA_STRICT_WARNINGS)
   if(MSVC)
    target_compile_options(estimora_${test_name} PRIVATE /W4 /WX)
   else()
    target_compile_options(estimora_${test_name} PRIVATE -Wall -Wextra -Wpedantic -Werror)
   endif()
  endif()
  if(UNIX AND NOT APPLE)
   target_link_libraries(estimora_${test_name} PRIVATE m)
  endif()
  add_test(NAME "${test_name}" COMMAND "estimora_${test_name}")
 endforeach()
endif()
