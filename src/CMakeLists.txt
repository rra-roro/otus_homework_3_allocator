cmake_minimum_required(VERSION 3.2)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.12")
    message(STATUS "CMake version ${CMAKE_VERSION}")
    cmake_policy(SET CMP0074 NEW)
endif ()

#set (Boost_USE_STATIC_RUNTIME ON)
#set (Boost_USE_STATIC_LIBS ON)

if(STATIC_LINK_LIBS)
        message(STATUS "CMake STATIC_LINK_LIBS = ${STATIC_LINK_LIBS}")        
        set (Boost_USE_STATIC_LIBS ON)
        set (Boost_USE_STATIC_RUNTIME ON)        
        if (MSVC)
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS_MINSIZEREL})
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
            string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
        endif ()
endif ()

set (Boost_NO_BOOST_CMAKE ON)

find_package(Boost COMPONENTS math_c99f REQUIRED)

if (Boost_FOUND)
        message(STATUS "Setting up BOOST")
        message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
        message(STATUS " Library  - ${Boost_LIBRARY_DIRS}")
endif (Boost_FOUND)

find_path(GMP_INCLUDE_DIRS NAMES gmp.h)
find_library(GMP_LIBRARY NAMES gmp libgmp mpir)

add_executable(allocator main.cpp)
target_include_directories(allocator PUBLIC "../include/" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.." "${Boost_INCLUDE_DIR}" "${GMP_INCLUDE_DIRS}")

if (MSVC)

    message(STATUS "MSVC")

	set_target_properties(allocator PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      COMPILE_OPTIONS "/permissive-;/Zc:wchar_t"
    )

else()

	set_target_properties(allocator PROPERTIES
		  CXX_STANDARD 17
		  CXX_STANDARD_REQUIRED ON
		  COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
	)

endif ()


#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}" " /VERBOSE:LIB")

target_link_libraries(allocator my_lib ${Boost_LIBRARIES} ${GMP_LIBRARY})