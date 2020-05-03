# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindSPIRV-Headers
# ----------
#
# Try to find SPIRV-Headers
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines :prop_tgt:`IMPORTED` target ``SPIRV-Headers::SPIRV-Headers``, if
# SPIRV-Headers has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables::
#
#   SPIRV-Headers_FOUND          - True if SPIRV-Headers was found
#   SPIRV-Headers_INCLUDE_DIRS   - include directories for SPIRV-Headers
#
# The module will also define two cache variables::
#
#   SPIRV-Headers_INCLUDE_DIR    - the SPIRV-Headers include directory
#

find_path(SPIRV-Headers_INCLUDE_DIR
  NAMES spirv-headers/spirv.hpp11
  PATHS
  "$ENV{VULKAN_SDK}/include")

set(SPIRV-Headers_INCLUDE_DIRS ${SPIRV-Headers_INCLUDE_DIR})

#include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SPIRV-Headers
  DEFAULT_MSG
  SPIRV-Headers_INCLUDE_DIR)

mark_as_advanced(SPIRV-Headers_INCLUDE_DIR)

if(SPIRV-Headers_FOUND AND NOT TARGET SPIRV-Headers::SPIRV-Headers)
  add_library(SPIRV-Headers::SPIRV-Headers UNKNOWN IMPORTED)
  set_target_properties(SPIRV-Headers::SPIRV-Headers PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${SPIRV-Headers_INCLUDE_DIRS}")
endif()
