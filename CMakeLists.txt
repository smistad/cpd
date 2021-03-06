project(cpd CXX)
cmake_minimum_required(VERSION 2.8.12)

if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()
if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW) # Quoted variables in IF statements
endif()

set(CPD_VERSION 0.3.3)
configure_file(
    src/version.cpp.in
    ${PROJECT_BINARY_DIR}/src/version.cpp
    )
set(CPD_SOVERSION 0)
message(STATUS "[cpd] Version: ${CPD_VERSION}")

set(GTEST_SOURCE_DIR "${PROJECT_SOURCE_DIR}/vendor/gtest-1.7.0")

option(WITH_TESTS "Build unit tests" ON)

set(FGT_MINIMUM_VERSION 0.4)
find_package(Fgt ${FGT_MINIMUM_VERSION} REQUIRED)
message(STATUS "[cpd] Fgt version: ${Fgt_VERSION}")

add_library(Library-C++
    src/affine.cpp
    src/matrix.cpp
    src/nonrigid.cpp
    src/normalization.cpp
    src/rigid.cpp
    src/utils.cpp
    ${PROJECT_BINARY_DIR}/src/version.cpp
    )
target_include_directories(Library-C++
    INTERFACE
    ${CMAKE_INSTALL_PREFIX}/include
    PRIVATE
    include
    )
target_link_libraries(Library-C++
    PUBLIC
    Fgt::Library-C++
    )
target_compile_definitions(Library-C++
    PRIVATE
    _USE_MATH_DEFINES
    )
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    target_compile_options(Library-C++
        PRIVATE
        -pedantic -Wall
        )
endif()
set_target_properties(Library-C++ PROPERTIES
    OUTPUT_NAME cpd
    VERSION ${CPD_VERSION}
    SOVERSION ${CPD_SOVERSION}
    )
install(DIRECTORY include/cpd DESTINATION include)
install(TARGETS Library-C++
    DESTINATION lib
    EXPORT cpd-targets
    )

configure_file(Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile)
add_custom_target(docs
    COMMAND doxygen ${PROJECT_BINARY_DIR}/Doxyfile
    )

if(WITH_TESTS)
    enable_testing()
    add_subdirectory(${GTEST_SOURCE_DIR})
    set_target_properties(gtest PROPERTIES MACOSX_RPATH ON)
    set_target_properties(gtest_main PROPERTIES MACOSX_RPATH ON)
    add_subdirectory(test)
endif()

include(CMakePackageConfigHelpers)
configure_file(cmake/cpd-config.cmake
    ${PROJECT_BINARY_DIR}/cpd-config.cmake
    @ONLY
    )
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/cpd-config-version.cmake
    VERSION ${CPD_VERSION}
    COMPATIBILITY AnyNewerVersion
    )
install(FILES
    ${PROJECT_BINARY_DIR}/cpd-config.cmake
    ${PROJECT_BINARY_DIR}/cpd-config-version.cmake
    DESTINATION lib/cmake/cpd
    )
install(EXPORT cpd-targets
    DESTINATION lib/cmake/cpd
    NAMESPACE Cpd::
    )
