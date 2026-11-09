cmake_minimum_required(VERSION 3.20)
project(estimora VERSION 0.1.0 LANGUAGES C)
option(ESTIMORA_BUILD_TESTS 'Build migrated tests' ON)
file(GLOB ESTIMORA_SOURCES CONFIGURE_DEPENDS src/*.c)
add_library(estimora STATIC ${ESTIMORA_SOURCES})
target_include_directories(estimora PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/estimora/statistics)
target_compile_features(
estimora
 PUBLIC c_std_99)
if(ESTIMORA_BUILD_TESTS)
 enable_testing()
 file(GLOB ESTIMORA_TESTS CONFIGURE_DEPENDS tests/*.c)
 foreach(test_source IN LISTS 
ESTIMORA
_TESTS)
  get_filename_component(test_name "${test_source}" NAME_WE)
  add_executable(estimora_${test_name} ${test_source})
  target_include_directories(estimora_${test_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/estimora/statistics)
  target_link_libraries(
estimora
_${test_name} PRIVATE 
estimora
)
  add_test(NAME "${test_name}" COMMAND "estimora_${test_name}")
 endforeach()
endif()
