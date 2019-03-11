cmake_minimum_required(VERSION 3.2)

project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

configure_file("./include/version.h.in" version.h)

add_subdirectory(src_lib)   
add_subdirectory(src)   
add_subdirectory(src_test)   

install(PROGRAMS src/allocator src_test/test_allocator DESTINATION bin)

SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA ${CMAKE_CURRENT_SOURCE_DIR}/cpack_script/postinst)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT rra.roro@gmail.com)

include(CPack)

enable_testing()
add_test(test_allocator src_test/test_allocator)