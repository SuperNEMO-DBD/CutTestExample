# - Basic CMake setup
# Check version meets ou requirements
# Declare project, which will configure compiler for us
cmake_minimum_required(VERSION 3.3)
project(CutTest VERSION 0.1.0)

# Modules use Bayeux, so we need to locate this or fail
find_package(Bayeux REQUIRED)

# Ensure our code can see the Bayeux headers
include_directories(${Bayeux_INCLUDE_DIRS})

# Build a dynamic library from our sources
add_library(CutTest SHARED CutTest.h CutTest.cpp)

# - Don't link directly as Bayeux/Falaise will export the
#   needed symbols.
# Ensure dynamic lookup on OS X
if(APPLE)
  set_target_properties(CutTest
    PROPERTIES LINK_FLAGS "-undefined dynamic_lookup"
    )
endif()

# Configure config files for use from build directory
configure_file(CutDefinitions.conf.in CutDefinitions.conf @ONLY)
configure_file(CutTest.conf.in CutTest.conf @ONLY)
configure_file(CutTestExamplePipeline.conf.in CutTestExamplePipeline.conf @ONLY)

