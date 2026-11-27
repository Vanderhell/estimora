cmake_minimum_required(VERSION 3.16)
project(Estimora VERSION 0.2.0 LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(ESTIMORA_BUILD_TESTS "Build Estimora validation tests" OFF)
option(ESTIMORA_BUILD_EXAMPLES "Build Estimora examples" OFF)
option(ESTIMORA_STRICT_WARNINGS "Treat supported compiler warnings as errors" ON)

function(estimora_apply_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
        if(ESTIMORA_STRICT_WARNINGS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        if(ESTIMORA_STRICT_WARNINGS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

add_library(estimora STATIC
    src/estimora_histogram_total_u32.c
    src/estimora_hist_cdf_u32.c
    src/estimora_hist_clip_redistribute_u32.c
    src/estimora_hist_dynamic_range_u32.c
    src/estimora_hist_entropy_norm_q.c
    src/estimora_hist_equalize_u8.c
    src/estimora_hist_high_percentile_u32.c
    src/estimora_hist_low_percentile_u32.c
    src/estimora_hist_valley_count_u32.c
    src/estimora_image_hist_bin_u32.c
    src/estimora_mean3_i32.c
    src/estimora_mean4_i32.c
    src/estimora_mean5_i32.c
    src/estimora_median_network5_i32.c
    src/estimora_median_network7_i32.c
    src/estimora_running_median3_i32.c
    src/estimora_sensor_triple_median_i32.c
    src/estimora_trimmed_mean5_i32.c
    src/estimora_weighted_sensor_fuse_i32.c
)
add_library(Estimora::Estimora ALIAS estimora)
target_compile_features(estimora PUBLIC c_std_99)
set_target_properties(estimora PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO EXPORT_NAME Estimora)
target_include_directories(estimora PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
estimora_apply_warnings(estimora)

if(ESTIMORA_BUILD_EXAMPLES)
    add_executable(estimora_example_minimal examples/minimal.c)
    target_link_libraries(estimora_example_minimal PRIVATE Estimora::Estimora)
    target_compile_features(estimora_example_minimal PRIVATE c_std_99)
    set_target_properties(estimora_example_minimal PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    estimora_apply_warnings(estimora_example_minimal)
endif()

if(ESTIMORA_BUILD_TESTS)
    enable_testing()
    enable_language(CXX)
    set(ESTIMORA_TESTS
        test
        test_c
        test_dynamic_range
        test_edge_grid
        test_entropy
        test_equalize
        test_hist_cdf
        test_high_percentile
        test_percentile
        test_redistribute
        test_image_hist_bin_u32
        test_median5
        test_median7
        test_trimmed_mean
        test_valleys
        test_weighted_sensor_fuse
        test_audit
    )
    foreach(test_name IN LISTS ESTIMORA_TESTS)
        add_executable(estimora_test_${test_name} tests/${test_name}.c)
        target_link_libraries(estimora_test_${test_name} PRIVATE Estimora::Estimora)
        target_compile_features(estimora_test_${test_name} PRIVATE c_std_99)
        set_target_properties(estimora_test_${test_name} PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        estimora_apply_warnings(estimora_test_${test_name})
        if((test_name STREQUAL "test_audit" OR test_name STREQUAL "test_entropy") AND UNIX AND NOT APPLE)
            target_link_libraries(estimora_test_${test_name} PRIVATE m)
        endif()
        add_test(NAME estimora.${test_name} COMMAND estimora_test_${test_name})
    endforeach()

    set(ESTIMORA_PUBLIC_HEADERS
        statistics/histogram_total_u32.h
        statistics/hist_cdf_u32.h
        statistics/hist_clip_redistribute_u32.h
        statistics/hist_dynamic_range_u32.h
        statistics/hist_entropy_norm_q.h
        statistics/hist_equalize_u8.h
        statistics/hist_high_percentile_u32.h
        statistics/hist_low_percentile_u32.h
        statistics/hist_valley_count_u32.h
        statistics/image_hist_bin_u32.h
        statistics/mean3_i32.h
        statistics/mean4_i32.h
        statistics/mean5_i32.h
        statistics/median_network5_i32.h
        statistics/median_network7_i32.h
        statistics/running_median3_i32.h
        statistics/sensor_triple_median_i32.h
        statistics/trimmed_mean5_i32.h
        statistics/weighted_sensor_fuse_i32.h
    )
    foreach(header IN LISTS ESTIMORA_PUBLIC_HEADERS)
        string(REPLACE "/" "_" header_test_name "${header}")
        string(REPLACE ".h" "" header_test_name "${header_test_name}")
        set(header_smoke_source "${CMAKE_CURRENT_BINARY_DIR}/header_${header_test_name}.c")
        file(WRITE "${header_smoke_source}" "#include <estimora/${header}>\nint main(void) { return 0; }\n")
        add_executable(estimora_header_${header_test_name} "${header_smoke_source}")
        target_link_libraries(estimora_header_${header_test_name} PRIVATE Estimora::Estimora)
        target_compile_features(estimora_header_${header_test_name} PRIVATE c_std_99)
        set_target_properties(estimora_header_${header_test_name} PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        estimora_apply_warnings(estimora_header_${header_test_name})
        add_test(NAME estimora.header.${header_test_name} COMMAND estimora_header_${header_test_name})
    endforeach()

    set(ESTIMORA_CPP_SMOKE "")
    foreach(header IN LISTS ESTIMORA_PUBLIC_HEADERS)
        string(APPEND ESTIMORA_CPP_SMOKE "#include <estimora/${header}>\n")
    endforeach()
    string(APPEND ESTIMORA_CPP_SMOKE
        "int main() { uint32_t total = 0; return estimora_histogram_total_u32(0, 0, &total) && total == 0u ? 0 : 1; }\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/headers_cpp.cpp" "${ESTIMORA_CPP_SMOKE}")
    add_executable(estimora_headers_cpp "${CMAKE_CURRENT_BINARY_DIR}/headers_cpp.cpp")
    target_link_libraries(estimora_headers_cpp PRIVATE Estimora::Estimora)
    estimora_apply_warnings(estimora_headers_cpp)
    add_test(NAME estimora.headers_cpp COMMAND estimora_headers_cpp)
endif()

install(TARGETS estimora EXPORT EstimoraTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT EstimoraTargets FILE EstimoraTargets.cmake NAMESPACE Estimora::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Estimora)
configure_package_config_file(cmake/EstimoraConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/EstimoraConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Estimora)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/EstimoraConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EstimoraConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/EstimoraConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Estimora)
