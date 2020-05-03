# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindSPIRV-Tools
# ----------
#
# Try to find SPIRV-Tools
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines :prop_tgt:`IMPORTED` target ``SPIRV-Tools::SPIRV-Tools``, if
# SPIRV-Tools has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables::
#
#   SPIRV-Tools_FOUND          - True if SPIRV-Tools was found
#   SPIRV-Tools_INCLUDE_DIRS   - include directories for SPIRV-Tools
#   SPIRV-Tools_LIBRARIES      - link against this library to use SPIRV-Tools
#
# The module will also define two cache variables::
#
#   SPIRV-Tools_INCLUDE_DIR    - the SPIRV-Tools include directory
#   SPIRV-Tools_LIBRARY        - the path to the SPIRV-Tools library
#

if(WIN32)
  find_path(SPIRV-Tools_INCLUDE_DIR
    NAMES spirv-tools/libspirv.hpp
    PATHS
      "$ENV{VULKAN_SDK}/Include"
    )

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_library(SPIRV-Tools_LIBRARY
      NAMES SPIRV-Tools SPIRV-Tools-shared
      PATHS
        "$ENV{VULKAN_SDK}/Lib"
        "$ENV{VULKAN_SDK}/Bin"
        )
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    find_library(SPIRV-Tools_LIBRARY
      NAMES SPIRV-Tools SPIRV-Tools-shared
      PATHS
        "$ENV{VULKAN_SDK}/Lib32"
        "$ENV{VULKAN_SDK}/Bin32"
        NO_SYSTEM_ENVIRONMENT_PATH
        )
  endif()
else()
    find_path(SPIRV-Tools_INCLUDE_DIR
      NAMES spirv-tools/libspirv.hpp
      PATHS
        "$ENV{VULKAN_SDK}/include")
    find_library(SPIRV-Tools_LIBRARY
      NAMES SPIRV-Tools SPIRV-Tools-shared
      PATHS
        "$ENV{VULKAN_SDK}/lib")
endif()

set(SPIRV-Tools_LIBRARIES ${SPIRV-Tools_LIBRARY})
set(SPIRV-Tools_INCLUDE_DIRS ${SPIRV-Tools_INCLUDE_DIR})

#include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SPIRV-Tools
  DEFAULT_MSG
  SPIRV-Tools_LIBRARY SPIRV-Tools_INCLUDE_DIR)

mark_as_advanced(SPIRV-Tools_INCLUDE_DIR SPIRV-Tools_LIBRARY)

if(SPIRV-Tools_FOUND AND NOT TARGET SPIRV-Tools::SPIRV-Tools)
  add_library(SPIRV-Tools::SPIRV-Tools UNKNOWN IMPORTED)
  set_target_properties(SPIRV-Tools::SPIRV-Tools PROPERTIES
    IMPORTED_LOCATION "${SPIRV-Tools_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${SPIRV-Tools_INCLUDE_DIRS}")
endif()
