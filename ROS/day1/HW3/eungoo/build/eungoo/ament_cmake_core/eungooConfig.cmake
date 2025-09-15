# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_eungoo_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED eungoo_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(eungoo_FOUND FALSE)
  elseif(NOT eungoo_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(eungoo_FOUND FALSE)
  endif()
  return()
endif()
set(_eungoo_CONFIG_INCLUDED TRUE)

# output package information
if(NOT eungoo_FIND_QUIETLY)
  message(STATUS "Found eungoo: 0.0.0 (${eungoo_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'eungoo' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${eungoo_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(eungoo_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${eungoo_DIR}/${_extra}")
endforeach()
