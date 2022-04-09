## Note: on OS X you should install XCode and the associated command-line tools

## cmake flags
cmake_minimum_required(VERSION 3.5)

## project name
project("generator-subscriber" 
    VERSION "1.2.0"
    LANGUAGES CXX
)

## --- Build options ---

if(WIN32)
  option(PAHO_BUILD_STATIC "Build static library" TRUE)
  option(PAHO_BUILD_SHARED "Build shared library (DLL)" FALSE)
  option(PAHO_WITH_SSL "Build SSL-enabled library" FALSE)
else()
  option(PAHO_BUILD_STATIC "Build static library" FALSE)
  option(PAHO_BUILD_SHARED "Build shared library" TRUE)
  option(PAHO_WITH_SSL "Build SSL-enabled library" TRUE)
  option(PAHO_BUILD_DEB_PACKAGE "Build debian package" FALSE)
endif()

option(PAHO_BUILD_SAMPLES "Build sample programs" FALSE)
option(PAHO_BUILD_TESTS "Build tests" FALSE)
option(PAHO_BUILD_DOCUMENTATION "Create and install the API documentation (requires Doxygen)" FALSE)

## --- C++11 build flags ---

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate position-independent code (-fPIC on UNIX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --- System Libraries ---

include(GNUInstallDirs)

if(WIN32)
	  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(LIBS_SYSTEM ws2_32)
elseif(UNIX)
    set(LIBS_SYSTEM c stdc++)
endif()

## --- Build directories ---

# For the paho_mqtt_c module
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
add_subdirectory(src)

# --- Default library for samples and unit tests ---

if(PAHO_BUILD_SHARED)
    set(PAHO_CPP_LIB paho-mqttpp3)
else()
    set(PAHO_CPP_LIB paho-mqttpp3-static)
endif()

## --- Packaging settings ---

include(CPack)


