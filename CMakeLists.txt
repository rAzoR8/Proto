cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

macro(add_sources expression sources)
	file(GLOB source_files RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "${expression}")
	set(${sources} "${${sources}};${source_files}")
endmacro()

macro(add_include_folder include_folder includes)
	set(${includes} "${${includes}};${CMAKE_CURRENT_LIST_DIR}/${include_folder}")
endmacro()

function(cmake_option option description default)
	option(${option} description ${default})
	if(${option})
		message(STATUS "'${option}' is TRUE")
		add_definitions(-D"${option}"=1)
	else()
		message(STATUS "'${option}' is FALSE")
	endif()
endfunction()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#this project
project(Proto)

#dependencies
find_package(Vulkan REQUIRED)

add_subdirectory(SpvGenTwo)

#gui sources
add_sources("gui/source/*.cpp" "gui_sources")
add_sources("gui/include/proto/*.h" "gui_sources")
add_include_folder("gui/include" "gui_includes")

#lib project
add_executable(proto "${gui_sources}")
target_include_directories(proto PUBLIC "${gui_includes}")
#target_include_directories(proto PUBLIC "${SPIRV_INCLUDE_DIR}")

target_link_libraries(proto PUBLIC lib)

message(STATUS "")