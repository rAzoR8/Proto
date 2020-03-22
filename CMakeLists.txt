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

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(thirdparty/glfw)

add_subdirectory(SpvGenTwo)

#proto sources
add_sources("gui/source/*.cpp" "proto_sources")
add_sources("gui/include/proto/*.h" "proto_sources")
add_include_folder("gui/include" "proto_includes")

#imgui sources
add_sources("thirdparty/imgui/imgui.cpp" "imgui_sources")
add_sources("thirdparty/imgui/imgui_draw.cpp" "imgui_sources")
add_sources("thirdparty/imgui/imgui_widgets.cpp" "imgui_sources")
add_sources("thirdparty/imgui/imgui_demo.cpp" "imgui_sources")
add_sources("thirdparty/imgui/examples/imgui_impl_vulkan.cpp" "imgui_sources")
add_sources("thirdparty/imgui/examples/imgui_impl_glfw.cpp" "imgui_sources")
add_include_folder("thirdparty/imgui" "imgui_includes")
add_include_folder("thirdparty/imgui/examples" "imgui_includes")

#imnodes sources
add_sources("thirdparty/ImNodes/ImNodes.cpp" "imgui_sources")
add_sources("thirdparty/ImNodes/ImNodesEz.cpp" "imgui_sources")
add_include_folder("thirdparty/ImNodes" "imgui_includes")

#lib project
add_executable(proto "${proto_sources};${imgui_sources}")
target_include_directories(proto PUBLIC "${proto_includes};${imgui_includes}")

#link SpvGenTwo
target_link_libraries(proto PUBLIC SpvGenTwoLib SpvGenTwoCommon)

#link vulkan
target_include_directories(proto PRIVATE "${Vulkan_INCLUDE_DIR}")
target_link_libraries(proto PRIVATE "${Vulkan_LIBRARIES}")

#link glfw
target_include_directories(proto PRIVATE "${glfw_INCLUDE_DIRS}")
target_link_libraries(proto PRIVATE glfw)

message(STATUS "")