cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(packet_handler VERSION ${PROJECT_VESRION})

#configure_file(version.h.in version.h)

add_executable(packet_handler main.cpp)

set_target_properties(packet_handler PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(packet_handler
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if (MSVC)
    target_compile_options(packet_handler PRIVATE
        /W4
    )
else ()
    target_compile_options(packet_handler PRIVATE
        #-Wall -Wextra -pedantic -Werror
        -Wall -Wextra
    )
endif()

install(TARGETS packet_handler RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT e.krotov@mail.ru)
include(CPack)

enable_testing()
include(CTest)
